"""
VLM (Vision Language Model) module for analyzing radar visualization videos.
Generates text descriptions for videos that don't have corresponding .txt files.
"""

import os
import threading
import importlib
from collections import Counter
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

os.environ.setdefault('HF_HUB_OFFLINE', '1')
os.environ.setdefault('TRANSFORMERS_OFFLINE', '1')
os.environ.setdefault('HF_HUB_DISABLE_TELEMETRY', '1')

REMOTE_MODEL_ID = "Qwen/Qwen3-VL-2B-Instruct"
ONNX_REMOTE_MODEL_ID = "Xenova/vit-gpt2-image-captioning"
MODEL_ENV_VARS = (
    'HYPERLINK_VLM_MODEL_DIR',
    'HPCC_HYPERLINK_VLM_MODEL_DIR',
    'QWEN_VL_MODEL_DIR',
)
ONNX_MODEL_ENV_VARS = (
    'HYPERLINK_ONNX_MODEL_DIR',
    'HPCC_HYPERLINK_ONNX_MODEL_DIR',
    'VIT_GPT2_ONNX_MODEL_DIR',
)
MODEL_WEIGHT_FILES = (
    'model.safetensors.index.json',
    'pytorch_model.bin.index.json',
)
_VLM_PROCESSOR_LOCK = threading.Lock()
_VLM_PROCESSOR: Optional['VLMProcessor'] = None


def _repo_root() -> Path:
    return Path(__file__).resolve().parents[3]


def _default_cache_dir() -> Path:
    explicit_cache_root = os.environ.get('HYPERLINK_VLM_CACHE_DIR', '').strip()
    cache_root = (
        explicit_cache_root
        or os.environ.get('CACHE_HTML_DIR', '').strip()
        or str(_repo_root() / 'llm_model' / '.cache' / 'huggingface')
    )
    cache_path = Path(cache_root)
    if cache_path.name != 'huggingface':
        cache_path = cache_path / 'huggingface' if explicit_cache_root else cache_path / 'vlm_cache' / 'huggingface'
    cache_path.mkdir(parents=True, exist_ok=True)
    return cache_path


def _preferred_local_onnx_model_dir() -> Path:
    return _repo_root() / 'llm_model' / 'vit-gpt2-image-captioning-onnx'


def _has_model_weights(model_dir: Path) -> bool:
    if not model_dir.exists() or not model_dir.is_dir():
        return False
    if any((model_dir / name).exists() for name in MODEL_WEIGHT_FILES):
        return True
    for pattern in ('*.safetensors', '*.bin', '*.pt', '*.gguf'):
        if any(model_dir.glob(pattern)):
            return True
    return False


def _candidate_local_model_dirs() -> List[Path]:
    script_dir = Path(__file__).resolve().parent
    repo_root = _repo_root()
    candidates: List[Path] = []

    for env_var in MODEL_ENV_VARS:
        env_value = os.environ.get(env_var, '').strip()
        if env_value:
            candidates.append(Path(env_value).expanduser())

    candidates.extend(
        [
            repo_root / 'rag' / 'qwn_kk_fine_model',
            script_dir / 'models' / 'Qwen3-VL-2B-Instruct',
            repo_root / 'llm_model' / 'qwn_kk_fine_model',
            repo_root / 'llm_model' / 'Qwen3-VL-2B-Instruct',
        ]
    )

    unique_candidates: List[Path] = []
    seen: set[str] = set()
    for candidate in candidates:
        try:
            resolved = str(candidate.resolve(strict=False))
        except OSError:
            resolved = str(candidate)
        if resolved in seen:
            continue
        seen.add(resolved)
        unique_candidates.append(candidate)
    return unique_candidates


def _has_onnx_model_files(model_dir: Path) -> bool:
    if not model_dir.exists() or not model_dir.is_dir():
        return False

    has_config = (model_dir / 'config.json').exists() and (model_dir / 'preprocessor_config.json').exists()
    has_tokenizer = (model_dir / 'tokenizer.json').exists() or (model_dir / 'vocab.json').exists()

    encoder_candidates = (
        model_dir / 'onnx' / 'encoder_model_quantized.onnx',
        model_dir / 'onnx' / 'encoder_model.onnx',
        model_dir / 'encoder_model_quantized.onnx',
        model_dir / 'encoder_model.onnx',
    )
    decoder_candidates = (
        model_dir / 'onnx' / 'decoder_model_merged_quantized.onnx',
        model_dir / 'onnx' / 'decoder_model_merged.onnx',
        model_dir / 'onnx' / 'decoder_model_quantized.onnx',
        model_dir / 'onnx' / 'decoder_model.onnx',
        model_dir / 'decoder_model_merged_quantized.onnx',
        model_dir / 'decoder_model_merged.onnx',
        model_dir / 'decoder_model_quantized.onnx',
        model_dir / 'decoder_model.onnx',
    )

    return has_config and has_tokenizer and any(path.exists() for path in encoder_candidates) and any(
        path.exists() for path in decoder_candidates
    )


def _onnx_model_subfolder(model_dir: Path) -> str:
    return 'onnx' if (model_dir / 'onnx').is_dir() else ''


def _candidate_local_onnx_model_dirs() -> List[Path]:
    script_dir = Path(__file__).resolve().parent
    repo_root = _repo_root()
    candidates: List[Path] = []

    for env_var in ONNX_MODEL_ENV_VARS:
        env_value = os.environ.get(env_var, '').strip()
        if env_value:
            candidates.append(Path(env_value).expanduser())

    candidates.extend(
        [
            _preferred_local_onnx_model_dir(),
            repo_root / 'llm_model' / 'Xenova-vit-gpt2-image-captioning',
            script_dir / 'models' / 'vit-gpt2-image-captioning-onnx',
        ]
    )

    unique_candidates: List[Path] = []
    seen: set[str] = set()
    for candidate in candidates:
        try:
            resolved = str(candidate.resolve(strict=False))
        except OSError:
            resolved = str(candidate)
        if resolved in seen:
            continue
        seen.add(resolved)
        unique_candidates.append(candidate)
    return unique_candidates


def resolve_onnx_model_source() -> Tuple[str, bool, str]:
    incomplete_local_dir: Optional[Path] = None
    for candidate in _candidate_local_onnx_model_dirs():
        if not candidate.exists():
            continue
        if _has_onnx_model_files(candidate):
            return str(candidate), True, _onnx_model_subfolder(candidate)
        if incomplete_local_dir is None:
            incomplete_local_dir = candidate

    if incomplete_local_dir is not None:
        print(
            f"Local ONNX model directory exists but is incomplete: {incomplete_local_dir}. Remote downloads are disabled.",
            flush=True,
        )
    return '', False, ''


def resolve_model_source() -> Tuple[str, bool]:
    incomplete_local_dir: Optional[Path] = None
    for candidate in _candidate_local_model_dirs():
        if not candidate.exists():
            continue
        if _has_model_weights(candidate):
            return str(candidate), True
        if incomplete_local_dir is None:
            incomplete_local_dir = candidate

    if incomplete_local_dir is not None:
        print(
            f"Local model directory exists but has no usable weight files: {incomplete_local_dir}. Remote downloads are disabled.",
            flush=True,
        )
    return '', False


def _local_qwen_model_source() -> Optional[str]:
    for candidate in _candidate_local_model_dirs():
        if _has_model_weights(candidate):
            return str(candidate)
    return None


def _count_phrase(count: int, noun: str) -> str:
    if count <= 0:
        return f'no {noun}'
    if count == 1:
        return f'one {noun}'
    if count <= 3:
        return f'a few {noun}'
    if count <= 8:
        return f'several {noun}'
    return f'many {noun}'


def _contour_centers(mask, min_area: float = 20.0) -> List[Tuple[float, float, float]]:
    import cv2

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    centers: List[Tuple[float, float, float]] = []
    for contour in contours:
        area = cv2.contourArea(contour)
        if area < min_area:
            continue
        moments = cv2.moments(contour)
        if not moments['m00']:
            continue
        centers.append((moments['m10'] / moments['m00'], moments['m01'] / moments['m00'], area))
    return centers


def _average_offset(points: List[Tuple[float, float, float]], origin: Tuple[float, float]) -> Optional[Tuple[float, float]]:
    if not points:
        return None
    avg_x = sum(point[0] - origin[0] for point in points) / len(points)
    avg_y = sum(point[1] - origin[1] for point in points) / len(points)
    return (avg_x, avg_y)


def _sample_video_frames(video_path: str, target_frames: int = 4) -> List[Any]:
    import cv2
    from PIL import Image

    cap = cv2.VideoCapture(video_path)
    total_frames = max(int(cap.get(cv2.CAP_PROP_FRAME_COUNT) or 0), 1)
    if target_frames <= 1:
        sample_indices = [0]
    else:
        sample_indices = sorted(
            {
                min(total_frames - 1, round(((total_frames - 1) * index) / (target_frames - 1)))
                for index in range(target_frames)
            }
        )

    images: List[Any] = []
    for index in sample_indices:
        cap.set(cv2.CAP_PROP_POS_FRAMES, index)
        ok, frame = cap.read()
        if not ok or frame is None:
            continue
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        images.append(Image.fromarray(rgb_frame))

    cap.release()
    return images


def _normalize_caption_text(text: str) -> str:
    return ' '.join((text or '').strip().strip('.').split())


def _caption_hint(captions: List[str]) -> Optional[str]:
    joined = ' '.join(captions).lower()
    if not joined:
        return None

    screen_terms = {'screen', 'display', 'monitor', 'dashboard', 'computer', 'television'}
    traffic_terms = {'car', 'cars', 'vehicle', 'vehicles', 'truck', 'road', 'street', 'traffic', 'driving', 'highway'}
    map_terms = {'map', 'graph', 'diagram'}

    if any(term in joined for term in screen_terms) and any(term in joined for term in traffic_terms | map_terms):
        return 'dashboard-style traffic monitor'
    if any(term in joined for term in screen_terms):
        return 'instrumented display'
    if any(term in joined for term in traffic_terms):
        return 'traffic scene'
    return None


def _pick_best_caption(captions: List[str]) -> Optional[str]:
    normalized = [_normalize_caption_text(caption) for caption in captions if _normalize_caption_text(caption)]
    if not normalized:
        return None
    counts = Counter(caption.lower() for caption in normalized)
    best_key, _ = counts.most_common(1)[0]
    for caption in normalized:
        if caption.lower() == best_key:
            return caption
    return normalized[0]


def _analyze_radar_scene(video_path: str) -> Optional[Dict[str, Any]]:
    import cv2

    cap = cv2.VideoCapture(video_path)
    total_frames = max(int(cap.get(cv2.CAP_PROP_FRAME_COUNT) or 0), 1)
    sample_indices = sorted({0, total_frames // 2, max(total_frames - 1, 0)})
    sampled_frames = []

    for index in sample_indices:
        cap.set(cv2.CAP_PROP_POS_FRAMES, index)
        ok, frame = cap.read()
        if ok and frame is not None:
            sampled_frames.append(frame)
    cap.release()

    if not sampled_frames:
        return None

    green_counts: List[int] = []
    red_counts: List[int] = []
    green_offsets: List[Optional[Tuple[float, float]]] = []

    for frame in sampled_frames:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        green_mask = cv2.inRange(hsv, (35, 60, 60), (95, 255, 255))
        red_mask_low = cv2.inRange(hsv, (0, 80, 70), (12, 255, 255))
        red_mask_high = cv2.inRange(hsv, (168, 80, 70), (180, 255, 255))
        red_mask = cv2.bitwise_or(red_mask_low, red_mask_high)
        white_mask = cv2.inRange(hsv, (0, 0, 180), (180, 50, 255))

        green_targets = _contour_centers(green_mask)
        red_obstacles = _contour_centers(red_mask)
        white_regions = _contour_centers(white_mask, min_area=50.0)

        frame_center = (frame.shape[1] / 2.0, frame.shape[0] / 2.0)
        ego_region = max(white_regions, key=lambda item: item[2], default=(frame_center[0], frame_center[1], 0.0))
        ego_center = (ego_region[0], ego_region[1])

        green_counts.append(len(green_targets))
        red_counts.append(len(red_obstacles))
        green_offsets.append(_average_offset(green_targets, ego_center))

    motion_type = 'steady'
    if green_offsets[0] is not None and green_offsets[-1] is not None:
        delta_x = green_offsets[-1][0] - green_offsets[0][0]
        delta_y = green_offsets[-1][1] - green_offsets[0][1]
        if abs(delta_x) > 18:
            motion_type = 'left_to_right' if delta_x > 0 else 'right_to_left'
        elif abs(delta_y) > 18:
            motion_type = 'closer' if delta_y > 0 else 'farther'
        else:
            motion_type = 'circulate'

    return {
        'green_peak': max(green_counts, default=0),
        'red_peak': max(red_counts, default=0),
        'motion_type': motion_type,
    }


def _compose_radar_summary(scene: Optional[Dict[str, Any]], caption_hint: Optional[str] = None) -> Optional[str]:
    if not scene:
        return None

    green_peak = int(scene.get('green_peak', 0) or 0)
    red_peak = int(scene.get('red_peak', 0) or 0)
    motion_type = scene.get('motion_type', 'steady')

    motion_clause = ''
    if motion_type == 'left_to_right':
        motion_clause = ' and the tracked targets drift from left to right'
    elif motion_type == 'right_to_left':
        motion_clause = ' and the tracked targets drift from right to left'
    elif motion_type == 'closer':
        motion_clause = ' and the tracked targets move closer to the ego vehicle'
    elif motion_type == 'farther':
        motion_clause = ' and the tracked targets move farther ahead of the ego vehicle'
    elif motion_type == 'circulate':
        motion_clause = ' while the tracked targets circulate around the ego vehicle'

    if caption_hint == 'dashboard-style traffic monitor':
        intro = 'A dashboard-style radar display shows'
    elif caption_hint == 'instrumented display':
        intro = 'An instrumented radar display shows'
    elif caption_hint == 'traffic scene':
        intro = 'A radar-style traffic view shows'
    else:
        intro = 'Radar visualization shows'

    if green_peak and red_peak:
        return (
            f'{intro} the white ego vehicle moving with {_count_phrase(green_peak, "green tracked targets")} '
            f'while {_count_phrase(red_peak, "red static obstacles")} remain fixed around the scene{motion_clause}.'
        )
    if green_peak:
        return (
            f'{intro} the white ego vehicle moving with {_count_phrase(green_peak, "green tracked targets")} '
            f'around it{motion_clause}.'
        )
    if red_peak:
        return (
            f'{intro} the white ego vehicle moving past {_count_phrase(red_peak, "red static obstacles")} '
            'that remain fixed in place.'
        )
    return f'{intro} the white ego vehicle moving through an otherwise clear scene.'


def _heuristic_video_summary(video_path: str) -> Optional[str]:
    return _compose_radar_summary(_analyze_radar_scene(video_path))


def get_vlm_processor() -> 'VLMProcessor':
    global _VLM_PROCESSOR
    with _VLM_PROCESSOR_LOCK:
        if _VLM_PROCESSOR is None:
            _VLM_PROCESSOR = VLMProcessor()
    return _VLM_PROCESSOR


class VLMProcessor:
    """Processes videos using Vision Language Model to generate text descriptions."""
    
    def __init__(self):
        self.model = None
        self.processor = None
        self.model_id = None
        self.local_files_only = False
        self.onnx_model = None
        self.onnx_image_processor = None
        self.onnx_tokenizer = None
        self.onnx_model_id = None
        self.onnx_local_files_only = False
        self.onnx_subfolder = 'onnx'
        self.cache_dir = str(_default_cache_dir())
        self.last_backend: Optional[str] = None
        self._loaded = False
        self._onnx_loaded = False

    def current_model_source(self) -> Optional[str]:
        return self.last_backend or self.onnx_model_id or self.model_id

    def using_local_backend(self) -> bool:
        backend = self.last_backend or ''
        if backend.startswith('onnx-vision2seq:'):
            return self.onnx_local_files_only
        if backend == 'heuristic-cv-fallback':
            return True
        if self.onnx_model_id and self.model_id is None:
            return self.onnx_local_files_only
        return self.local_files_only

    def _persist_onnx_bundle(self) -> None:
        if self.onnx_local_files_only or self.onnx_model is None:
            return

        target_dir = _preferred_local_onnx_model_dir()
        if _has_onnx_model_files(target_dir):
            return

        try:
            target_dir.mkdir(parents=True, exist_ok=True)
            self.onnx_model.save_pretrained(target_dir)
            if self.onnx_image_processor is not None:
                self.onnx_image_processor.save_pretrained(target_dir)
            if self.onnx_tokenizer is not None:
                self.onnx_tokenizer.save_pretrained(target_dir)
            generation_config = getattr(self.onnx_model, 'generation_config', None)
            if generation_config is not None:
                generation_config.save_pretrained(target_dir)
            print(f'Saved local ONNX model bundle to: {target_dir}', flush=True)
        except Exception as exc:
            print(f'Unable to persist local ONNX model bundle: {exc}', flush=True)

    def _load_onnx_model(self) -> bool:
        if self._onnx_loaded:
            return True

        try:
            ort_module = importlib.import_module('optimum.onnxruntime')
            ORTModelForVision2Seq = getattr(ort_module, 'ORTModelForVision2Seq')
            from transformers import AutoImageProcessor, AutoTokenizer
        except Exception as e:
            print(
                'ONNX dependencies are missing or too old. Install Hyperlink_tool/code/requirements-vlm.txt.',
                flush=True,
            )
            print(f'ONNX dependency error: {e}', flush=True)
            return False

        self.onnx_model_id, self.onnx_local_files_only, self.onnx_subfolder = resolve_onnx_model_source()
        if not self.onnx_model_id:
            print('No local ONNX model bundle is available. Skipping ONNX inference.', flush=True)
            return False
        if self.onnx_local_files_only:
            print(f'Using local ONNX model from: {self.onnx_model_id}', flush=True)
        else:
            print(f'Using remote ONNX model source: {self.onnx_model_id}', flush=True)

        try:
            self.onnx_model = ORTModelForVision2Seq.from_pretrained(
                self.onnx_model_id,
                subfolder=self.onnx_subfolder,
                local_files_only=self.onnx_local_files_only,
                cache_dir=self.cache_dir,
                provider='CPUExecutionProvider',
                use_merged=True,
                use_cache=False,
                use_io_binding=False,
            )
            self.onnx_image_processor = AutoImageProcessor.from_pretrained(
                self.onnx_model_id,
                local_files_only=self.onnx_local_files_only,
                cache_dir=self.cache_dir,
            )
            self.onnx_tokenizer = AutoTokenizer.from_pretrained(
                self.onnx_model_id,
                local_files_only=self.onnx_local_files_only,
                cache_dir=self.cache_dir,
            )
            self._persist_onnx_bundle()
            self._onnx_loaded = True
            print('ONNX model loaded successfully!', flush=True)
            return True
        except Exception as e:
            print(f'Error loading ONNX model: {e}', flush=True)
            import traceback
            traceback.print_exc()
            return False
    
    def _load_model(self) -> bool:
        """Lazy load the model only when needed."""
        if self._loaded:
            return True
        
        try:
            import torch
            from transformers import AutoProcessor
            try:
                from transformers import AutoModelForImageTextToText as model_loader
            except ImportError:
                from transformers import Qwen3VLForConditionalGeneration as model_loader
        except Exception as e:
            print(
                "VLM dependencies are missing or too old. Install Hyperlink_tool/code/requirements-vlm.txt.",
                flush=True,
            )
            print(f"Dependency error: {e}", flush=True)
            return False

        self.model_id, self.local_files_only = resolve_model_source()
        if not self.model_id:
            print('No offline Hyperlink VLM model is available. Falling back to heuristic summaries only.', flush=True)
            return False
        if self.local_files_only:
            print(f"Using local model from: {self.model_id}", flush=True)
        else:
            print(f"Using remote model source: {self.model_id}", flush=True)

        has_cuda = bool(getattr(torch, 'cuda', None) and torch.cuda.is_available())
        device_map = 'auto' if has_cuda else 'cpu'
        torch_dtype = 'auto' if has_cuda else torch.float32

        print(
            f"Loading model ({'GPU auto' if has_cuda else 'CPU'} mode - first load may take several minutes)...",
            flush=True,
        )
        
        try:
            self.model = model_loader.from_pretrained(
                self.model_id,
                device_map=device_map,
                trust_remote_code=True,
                low_cpu_mem_usage=True,
                local_files_only=self.local_files_only,
                cache_dir=self.cache_dir,
                torch_dtype=torch_dtype,
            )
            if not has_cuda and hasattr(self.model, 'float'):
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
                local_files_only=self.local_files_only,
                cache_dir=self.cache_dir,
            )
            print("Processor loaded successfully!", flush=True)
        except Exception as e:
            print(f"Error loading processor: {e}", flush=True)
            import traceback
            traceback.print_exc()
            return False
        
        self._loaded = True
        return True

    def _analyze_with_onnx(self, video_path: str, scene: Optional[Dict[str, Any]]) -> Optional[str]:
        if not self._load_onnx_model():
            return None

        try:
            if not hasattr(self.onnx_model.__class__, '_is_stateful'):
                self.onnx_model.__class__._is_stateful = False

            sampled_images = _sample_video_frames(video_path, target_frames=4)
            if not sampled_images:
                return None

            print(f'Generating ONNX captions from {len(sampled_images)} sampled frame(s)...', flush=True)
            inputs = self.onnx_image_processor(images=sampled_images, return_tensors='pt')
            generated_ids = self.onnx_model.generate(
                pixel_values=inputs.pixel_values,
                num_beams=4,
                max_new_tokens=24,
                no_repeat_ngram_size=2,
                use_cache=False,
            )
            captions = [
                _normalize_caption_text(caption)
                for caption in self.onnx_tokenizer.batch_decode(generated_ids, skip_special_tokens=True)
            ]
            captions = [caption for caption in captions if caption]
            if captions:
                print(f'ONNX frame captions: {captions}', flush=True)

            summary = _compose_radar_summary(scene, _caption_hint(captions))
            if summary:
                self.last_backend = f'onnx-vision2seq:{self.onnx_model_id}'
                return summary

            best_caption = _pick_best_caption(captions)
            if best_caption:
                self.last_backend = f'onnx-vision2seq:{self.onnx_model_id}'
                return best_caption + '.' if not best_caption.endswith('.') else best_caption
            return None
        except Exception as e:
            print(f'Error during ONNX inference: {e}', flush=True)
            import traceback
            traceback.print_exc()
            return None

    def _analyze_with_qwen(self, video_path: str) -> Optional[str]:
        import torch
        import cv2
        from qwen_vl_utils import process_vision_info

        if not self._load_model():
            return None

        print(f"Processing {video_path} with Qwen VLM...", flush=True)

        try:
            cap = cv2.VideoCapture(video_path)
            total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
            fps_source = cap.get(cv2.CAP_PROP_FPS)
            duration = total_frames / fps_source if fps_source > 0 else 10
            cap.release()
        except Exception as e:
            print(f"Error reading video metadata: {e}", flush=True)
            return None

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

            print("Generating description with Qwen VLM (CPU inference - please wait)...", flush=True)
            with torch.no_grad():
                generated_ids = self.model.generate(**inputs, max_new_tokens=128)

            generated_ids_trimmed = [
                out_ids[len(in_ids):] for in_ids, out_ids in zip(inputs.input_ids, generated_ids)
            ]
            output_text = self.processor.batch_decode(
                generated_ids_trimmed, skip_special_tokens=True, clean_up_tokenization_spaces=False
            )
            if output_text:
                self.last_backend = self.model_id
                return output_text[0]
            return None
        except Exception as e:
            print(f"Error during Qwen inference: {e}", flush=True)
            import traceback
            traceback.print_exc()
            return None
    
    def analyze_video(self, video_path: str) -> Optional[str]:
        """
        Analyze a video and return a text description.
        
        Args:
            video_path: Path to the video file
            
        Returns:
            Generated text description or None if failed
        """
        scene = _analyze_radar_scene(video_path)

        onnx_description = self._analyze_with_onnx(video_path, scene)
        if onnx_description:
            return onnx_description

        if _local_qwen_model_source() is not None:
            qwen_description = self._analyze_with_qwen(video_path)
            if qwen_description:
                return qwen_description

        self.last_backend = 'heuristic-cv-fallback'
        fallback = _compose_radar_summary(scene) if scene else _heuristic_video_summary(video_path)
        if fallback:
            print('Falling back to heuristic CV description.', flush=True)
        return fallback


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


def _process_single_video(video_path: str, video_root: str, force: bool, vlm: VLMProcessor) -> Dict[str, Any]:
    if not os.path.isfile(video_path):
        return {
            'success': False,
            'status': 'error',
            'error': f'Video not found: {video_path}',
            'video_path': video_path,
        }

    txt_path = get_video_text_path(video_path, video_root)
    if os.path.exists(txt_path) and not force:
        existing = ''
        try:
            with open(txt_path, 'r', encoding='utf-8') as file_handle:
                existing = file_handle.read()
        except OSError:
            existing = ''
        return {
            'success': True,
            'status': 'skipped',
            'video_path': video_path,
            'text_path': txt_path,
            'description': existing,
            'model_source': vlm.current_model_source(),
            'using_local_model': vlm.using_local_backend(),
        }

    description = vlm.analyze_video(video_path)
    if not description:
        return {
            'success': False,
            'status': 'error',
            'error': 'Failed to generate description',
            'video_path': video_path,
            'text_path': txt_path,
            'model_source': vlm.current_model_source(),
            'using_local_model': vlm.using_local_backend(),
        }

    os.makedirs(os.path.dirname(txt_path), exist_ok=True)
    with open(txt_path, 'w', encoding='utf-8') as file_handle:
        file_handle.write(description)

    return {
        'success': True,
        'status': 'processed',
        'video_path': video_path,
        'text_path': txt_path,
        'description': description,
        'model_source': vlm.current_model_source(),
        'using_local_model': vlm.using_local_backend(),
    }


def process_video_with_vlm(video_path: str, video_root: str, force: bool = False) -> Dict[str, Any]:
    """Generate a single text description for the provided video."""
    normalized_video_path = os.path.abspath(video_path)
    normalized_video_root = os.path.abspath(video_root)
    vlm = get_vlm_processor()
    return _process_single_video(normalized_video_path, normalized_video_root, force, vlm)


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
    
    vlm = get_vlm_processor()
    processed = 0
    skipped = 0
    errors = 0
    
    for i, video_path in enumerate(sorted(videos), 1):
        video_name = os.path.basename(video_path)
        print(f"\n[{i}/{len(videos)}] {video_name}")

        result = _process_single_video(video_path, video_root, force, vlm)
        if result.get('status') == 'skipped':
            print(f"  ✓ Text file exists, skipping: {os.path.basename(result.get('text_path', ''))}")
            skipped += 1
        elif result.get('success'):
            description = result.get('description', '')
            print(f"  ✓ Saved to: {os.path.basename(result.get('text_path', ''))}")
            print(f"  Description: {description[:100]}..." if len(description) > 100 else f"  Description: {description}")
            processed += 1
        else:
            print(f"  ✗ {result.get('error', 'Failed to generate description')}")
            errors += 1
    
    print("\n" + "=" * 60)
    print("VLM Processing Complete:")
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