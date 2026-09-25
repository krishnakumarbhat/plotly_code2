"""
VLM (Vision Language Model) module for analyzing radar visualization videos.
Generates text descriptions for videos that don't have corresponding .txt files.
"""

import os
import ssl
import warnings
from typing import Optional, Tuple, List

# Disable SSL verification BEFORE importing any other libraries
# This is needed for corporate proxies with self-signed certificates
os.environ['CURL_CA_BUNDLE'] = ''
os.environ['REQUESTS_CA_BUNDLE'] = ''
os.environ['HF_HUB_DISABLE_SSL_VERIFY'] = '1'
os.environ['TRANSFORMERS_OFFLINE'] = '0'

# Patch SSL globally
ssl._create_default_https_context = ssl._create_unverified_context

import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Patch requests to disable SSL verification
import requests
from requests.adapters import HTTPAdapter

class SSLAdapter(HTTPAdapter):
    def init_poolmanager(self, *args, **kwargs):
        kwargs['ssl_context'] = ssl._create_unverified_context()
        return super().init_poolmanager(*args, **kwargs)

# Monkey-patch the default session
old_request = requests.Session.request
def new_request(self, *args, **kwargs):
    kwargs['verify'] = False
    return old_request(self, *args, **kwargs)
requests.Session.request = new_request


class VLMProcessor:
    """Processes videos using Vision Language Model to generate text descriptions."""
    
    def __init__(self):
        self.model = None
        self.processor = None
        self.model_id = None
        self._loaded = False
    
    def _load_model(self) -> bool:
        """Lazy load the model only when needed."""
        if self._loaded:
            return True
        
        import torch
        import sys
        from transformers import AutoModelForImageTextToText, AutoProcessor
        
        script_dir = os.path.dirname(os.path.abspath(__file__))
        self.model_id = os.path.join(script_dir, "models", "Qwen3-VL-2B-Instruct")
        
        # Fallback to HuggingFace if local model not found
        if not os.path.exists(self.model_id):
            print("Local model not found, using HuggingFace (requires internet)...", flush=True)
            self.model_id = "Qwen/Qwen3-VL-2B-Instruct"
        else:
            print(f"Using local model from: {self.model_id}", flush=True)
        
        print("Loading model (CPU mode - this may take a few minutes)...", flush=True)
        
        try:
            self.model = AutoModelForImageTextToText.from_pretrained(
                self.model_id,
                device_map="cpu",
                trust_remote_code=True,
                low_cpu_mem_usage=True,
                local_files_only=os.path.exists(self.model_id)
            )
            self.model = self.model.float()
            print("Model loaded successfully!", flush=True)
        except Exception as e:
            print(f"Error loading model: {e}", flush=True)
            import traceback
            traceback.print_exc()
            return False
        
        print("Loading processor...", flush=True)
        try:
            self.processor = AutoProcessor.from_pretrained(
                self.model_id, 
                trust_remote_code=True,
                local_files_only=os.path.exists(self.model_id)
            )
            print("Processor loaded successfully!", flush=True)
        except Exception as e:
            print(f"Error loading processor: {e}", flush=True)
            import traceback
            traceback.print_exc()
            return False
        
        self._loaded = True
        return True
    
    def analyze_video(self, video_path: str) -> Optional[str]:
        """
        Analyze a video and return a text description.
        
        Args:
            video_path: Path to the video file
            
        Returns:
            Generated text description or None if failed
        """
        import torch
        import cv2
        from qwen_vl_utils import process_vision_info
        
        if not self._load_model():
            return None
        
        print(f"Processing {video_path}...", flush=True)
        
        # Calculate FPS to get approx 4 frames (reduced for CPU)
        try:
            cap = cv2.VideoCapture(video_path)
            total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
            fps_source = cap.get(cv2.CAP_PROP_FPS)
            duration = total_frames / fps_source if fps_source > 0 else 10
            cap.release()
        except Exception as e:
            print(f"Error reading video metadata: {e}", flush=True)
            return None
        
        # Reduce frames for CPU to speed up processing
        target_frames = 4
        fps_sample = target_frames / duration if duration > 0 else 1
        print(f"Video duration: {duration:.2f}s. Sampling at {fps_sample:.4f} FPS to get ~{target_frames} frames.", flush=True)
        
        messages = [
            {
                "role": "user",
                "content": [
                    {
                        "type": "video",
                        "video": video_path,
                        "max_pixels": 360 * 420,
                        "min_pixels": 128 * 128,
                        "fps": fps_sample, 
                    },
                    {
                        "type": "text", 
                        "text": "analyze this radar visualization as a perception engineer, where the white ego vehicle tracks green moving targets (cars/trucks) and red static obstacles (guardrails/pedestrians), and identify the specific traffic scenario and relative motion. just give what is happening in video in 1 sentence"
                    },
                ],
            }
        ]
        
        try:
            # Preparation for inference
            text = self.processor.apply_chat_template(
                messages, tokenize=False, add_generation_prompt=True
            )
            image_inputs, video_inputs = process_vision_info(messages)
            inputs = self.processor(
                text=[text],
                images=image_inputs,
                videos=video_inputs,
                padding=True,
                return_tensors="pt",
            )
            
            # Generate
            print("Generating description (CPU inference - please wait, this may take several minutes)...", flush=True)
            with torch.no_grad():
                generated_ids = self.model.generate(**inputs, max_new_tokens=128)
            
            generated_ids_trimmed = [
                out_ids[len(in_ids):] for in_ids, out_ids in zip(inputs.input_ids, generated_ids)
            ]
            output_text = self.processor.batch_decode(
                generated_ids_trimmed, skip_special_tokens=True, clean_up_tokenization_spaces=False
            )
            
            return output_text[0] if output_text else None
            
        except Exception as e:
            print(f"Error during inference: {e}", flush=True)
            import traceback
            traceback.print_exc()
            return None


def get_video_text_path(video_path: str, video_root: str) -> str:
    """
    Get the corresponding .txt file path for a video.
    
    Args:
        video_path: Full path to the video file
        video_root: Root directory of videos
        
    Returns:
        Path where the .txt file should be stored
    """
    video_name = os.path.basename(video_path)
    base_name = os.path.splitext(video_name)[0]
    # Remove _web suffix if present
    if base_name.endswith("_web"):
        base_name = base_name[:-4]
    text_dir = os.path.join(video_root, "log_txt")
    return os.path.join(text_dir, f"{base_name}_web.txt")


def process_videos_with_vlm(video_root: str, force: bool = False) -> Tuple[int, int, int]:
    """
    Process all videos in a folder, generating .txt files for those without one.
    
    Args:
        video_root: Root directory containing videos
        force: If True, regenerate text even if .txt file exists
        
    Returns:
        Tuple of (processed_count, skipped_count, error_count)
    """
    video_extensions = {".mp4", ".avi", ".mov", ".mkv"}
    text_dir = os.path.join(video_root, "log_txt")
    os.makedirs(text_dir, exist_ok=True)
    
    # Find all videos
    videos: List[str] = []
    for entry in os.scandir(video_root):
        if entry.is_file() and os.path.splitext(entry.name)[1].lower() in video_extensions:
            videos.append(entry.path)
    
    if not videos:
        print(f"No videos found in {video_root}")
        return (0, 0, 0)
    
    print(f"\nFound {len(videos)} video(s) in {video_root}")
    print("-" * 60)
    
    vlm = VLMProcessor()
    processed = 0
    skipped = 0
    errors = 0
    
    for i, video_path in enumerate(sorted(videos), 1):
        video_name = os.path.basename(video_path)
        txt_path = get_video_text_path(video_path, video_root)
        
        print(f"\n[{i}/{len(videos)}] {video_name}")
        
        # Check if .txt already exists
        if os.path.exists(txt_path) and not force:
            print(f"  ✓ Text file exists, skipping: {os.path.basename(txt_path)}")
            skipped += 1
            continue
        
        # Process with VLM
        print(f"  → Generating text description...")
        description = vlm.analyze_video(video_path)
        
        if description:
            # Save the description
            with open(txt_path, "w", encoding="utf-8") as f:
                f.write(description)
            print(f"  ✓ Saved to: {os.path.basename(txt_path)}")
            print(f"  Description: {description[:100]}..." if len(description) > 100 else f"  Description: {description}")
            processed += 1
        else:
            print(f"  ✗ Failed to generate description")
            errors += 1
    
    print("\n" + "=" * 60)
    print(f"VLM Processing Complete:")
    print(f"  Processed: {processed}")
    print(f"  Skipped (already had text): {skipped}")
    print(f"  Errors: {errors}")
    print("=" * 60)
    
    return (processed, skipped, errors)


# For standalone testing
if __name__ == "__main__":
    import sys
    
    if len(sys.argv) < 2:
        print("Usage: python vlm.py <video_root_folder> [--force]")
        print("  --force: Regenerate text even if .txt file exists")
        sys.exit(1)
    
    video_root = sys.argv[1]
    force = "--force" in sys.argv
    
    if not os.path.isdir(video_root):
        print(f"Error: {video_root} is not a valid directory")
        sys.exit(1)
    
    process_videos_with_vlm(video_root, force=force)