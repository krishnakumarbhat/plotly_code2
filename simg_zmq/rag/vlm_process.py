"""
VLM video analysis script — runs inside rag.simg on a Slurm compute node.
Extracts frames from a radar video and sends them to Gemma-4 (via llama.cpp)
for scene description. Writes the result to a .txt file next to the video.

Usage (inside container):
    python /app/rag/vlm_process.py <video_path> [--force] [--model /path/to/gemma.gguf]
"""
import argparse
import base64
import json
import os
import subprocess
import sys
import time
import urllib.request


LLAMA_SERVER_PORT = 8081
LLAMA_SERVER_HOST = "127.0.0.1"
MAX_FRAMES = 4
DEFAULT_PROMPT = (
    "Analyze this radar visualization as a perception engineer. "
    "The white ego vehicle tracks green moving targets (cars/trucks) and "
    "red static obstacles (guardrails/pedestrians). "
    "Identify the specific traffic scenario and relative motion. "
    "Give what is happening in the video in 1 sentence."
)


def _find_llama_server() -> str:
    candidates = [
        os.environ.get("LLAMA_SERVER_PATH", ""),
        "/app/rag/tools/llama.cpp/llama-server",
    ]
    for c in candidates:
        if c and os.path.isfile(c):
            return c
    return candidates[-1]


def _sample_frames(video_path: str, target_frames: int = MAX_FRAMES) -> list[bytes]:
    import cv2

    cap = cv2.VideoCapture(video_path)
    total = max(int(cap.get(cv2.CAP_PROP_FRAME_COUNT)), 1)
    indices = sorted({
        min(total - 1, round(((total - 1) * i) / (target_frames - 1)))
        for i in range(target_frames)
    }) if target_frames > 1 else [0]

    frames: list[bytes] = []
    for idx in indices:
        cap.set(cv2.CAP_PROP_POS_FRAMES, idx)
        ok, frame = cap.read()
        if not ok or frame is None:
            continue
        ok, buf = cv2.imencode(".jpg", frame, [cv2.IMWRITE_JPEG_QUALITY, 70])
        if ok:
            frames.append(base64.b64encode(buf.tobytes()).decode("utf-8"))
    cap.release()
    return frames


def _start_llama_server(model_path: str) -> subprocess.Popen:
    binary = _find_llama_server()
    cmd = [
        binary,
        "--model", model_path,
        "--port", str(LLAMA_SERVER_PORT),
        "--host", LLAMA_SERVER_HOST,
        "--n-gpu-layers", os.environ.get("LLAMA_N_GPU_LAYERS", "0"),
    ]
    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    # Wait for server to be ready
    url = f"http://{LLAMA_SERVER_HOST}:{LLAMA_SERVER_PORT}/health"
    for _ in range(60):
        try:
            with urllib.request.urlopen(url, timeout=2):
                return proc
        except Exception:
            time.sleep(1)
    proc.kill()
    raise RuntimeError("llama-server did not start in time")


def _call_gemma_vlm(frames_b64: list[str], prompt: str) -> str:
    messages = []
    for fb in frames_b64:
        messages.append({
            "role": "user",
            "content": [
                {"type": "image_url", "image_url": {"url": f"data:image/jpeg;base64,{fb}"}},
                {"type": "text", "text": prompt},
            ],
        })

    payload = json.dumps({
        "messages": messages,
        "max_tokens": 256,
        "temperature": 0.3,
    }).encode("utf-8")

    url = f"http://{LLAMA_SERVER_HOST}:{LLAMA_SERVER_PORT}/v1/chat/completions"
    req = urllib.request.Request(url, data=payload, headers={"Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=120) as resp:
        data = json.loads(resp.read().decode("utf-8"))

    choices = data.get("choices", [])
    if choices:
        return choices[0].get("message", {}).get("content", "").strip()
    return ""


def _get_txt_path(video_path: str) -> str:
    base = os.path.splitext(os.path.basename(video_path))[0]
    if base.endswith("_web"):
        base = base[:-4]
    d = os.path.join(os.path.dirname(video_path), "log_txt")
    return os.path.join(d, f"{base}_web.txt")


def process_video(video_path: str, model_path: str, prompt: str, force: bool) -> dict:
    if not os.path.isfile(video_path):
        return {"success": False, "error": f"Video not found: {video_path}"}

    txt_path = _get_txt_path(video_path)
    if os.path.exists(txt_path) and not force:
        with open(txt_path, "r", encoding="utf-8") as f:
            return {"success": True, "status": "skipped", "description": f.read(), "text_path": txt_path}

    frames = _sample_frames(video_path)
    if not frames:
        return {"success": False, "error": "Could not extract frames from video"}

    server = None
    try:
        server = _start_llama_server(model_path)
        description = _call_gemma_vlm(frames, prompt)
        if not description:
            return {"success": False, "error": "Gemma-4 returned empty description"}

        os.makedirs(os.path.dirname(txt_path), exist_ok=True)
        with open(txt_path, "w", encoding="utf-8") as f:
            f.write(description)

        return {"success": True, "status": "processed", "description": description, "text_path": txt_path}
    finally:
        if server:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()


def main():
    parser = argparse.ArgumentParser(description="Analyze radar video with Gemma-4 VLM")
    parser.add_argument("video_path", help="Path to the video file")
    parser.add_argument("--force", action="store_true", help="Regenerate even if .txt exists")
    parser.add_argument("--model", default=os.environ.get("GEMMA_GGUF_PATH", ""),
                        help="Path to Gemma-4 GGUF model file")
    parser.add_argument("--prompt", default=DEFAULT_PROMPT, help="VLM prompt")
    args = parser.parse_args()

    model_path = args.model
    if not model_path:
        print("ERROR: Gemma-4 GGUF model path not specified. Set GEMMA_GGUF_PATH env var or pass --model.", file=sys.stderr)
        sys.exit(1)

    result = process_video(args.video_path, model_path, args.prompt, args.force)
    print(json.dumps(result))
    sys.exit(0 if result.get("success") else 1)


if __name__ == "__main__":
    main()
