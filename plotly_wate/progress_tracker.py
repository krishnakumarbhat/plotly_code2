"""
Progress Tracking Utility for All Services
==========================================

This module provides unified progress tracking for KPI, InteractivePlot, and DC HTML tools.
Works both locally and on SLURM clusters.

Usage:
    from tools.progress_tracker import ProgressTracker
    
    tracker = ProgressTracker(total_items=10, task_name="Processing HDF Pairs")
    for item in items:
        tracker.update(current=i, message=f"Processing {item}")
        # do work
    tracker.complete()
"""

import sys
import time
from datetime import datetime, timedelta
from typing import Optional
import os


class ProgressTracker:
    """
    A progress tracker that works in both terminal and SLURM environments.
    Provides visual progress bar, ETA estimation, and elapsed time tracking.
    """
    
    def __init__(
        self,
        total_items: int,
        task_name: str = "Processing",
        bar_width: int = 40,
        show_eta: bool = True,
        show_elapsed: bool = True,
        output_stream=None
    ):
        """
        Initialize the progress tracker.
        
        Args:
            total_items: Total number of items to process
            task_name: Name of the task being tracked
            bar_width: Width of the progress bar in characters
            show_eta: Whether to show estimated time remaining
            show_elapsed: Whether to show elapsed time
            output_stream: Output stream (default: sys.stdout)
        """
        self.total_items = max(1, total_items)
        self.task_name = task_name
        self.bar_width = bar_width
        self.show_eta = show_eta
        self.show_elapsed = show_elapsed
        self.output = output_stream or sys.stdout
        
        self.current = 0
        self.start_time = time.time()
        self.item_times = []
        
        # Detect environment
        self.is_slurm = bool(os.environ.get('SLURM_JOB_ID'))
        self.is_tty = hasattr(self.output, 'isatty') and self.output.isatty()
        
        # Colors (only if TTY)
        if self.is_tty:
            self.CYAN = '\033[0;36m'
            self.GREEN = '\033[0;32m'
            self.YELLOW = '\033[1;33m'
            self.BLUE = '\033[0;34m'
            self.NC = '\033[0m'
        else:
            self.CYAN = self.GREEN = self.YELLOW = self.BLUE = self.NC = ''
        
        self._print_header()
    
    def _print_header(self):
        """Print the task header."""
        print(f"\n{self.CYAN}{'='*60}{self.NC}", file=self.output)
        print(f"{self.CYAN}{self.task_name}{self.NC}", file=self.output)
        print(f"{self.CYAN}{'='*60}{self.NC}", file=self.output)
        print(f"Total items: {self.total_items}", file=self.output)
        print(f"Started at:  {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}", file=self.output)
        if self.is_slurm:
            print(f"SLURM Job:   {os.environ.get('SLURM_JOB_ID', 'N/A')}", file=self.output)
        print(f"{self.CYAN}{'-'*60}{self.NC}\n", file=self.output)
        self.output.flush()
    
    def _format_time(self, seconds: float) -> str:
        """Format seconds into human-readable string."""
        if seconds < 60:
            return f"{seconds:.0f}s"
        elif seconds < 3600:
            mins = int(seconds // 60)
            secs = int(seconds % 60)
            return f"{mins}m {secs}s"
        else:
            hours = int(seconds // 3600)
            mins = int((seconds % 3600) // 60)
            return f"{hours}h {mins}m"
    
    def _calculate_eta(self) -> Optional[float]:
        """Calculate estimated time remaining."""
        if self.current == 0:
            return None
        
        elapsed = time.time() - self.start_time
        rate = self.current / elapsed
        remaining = self.total_items - self.current
        
        if rate > 0:
            return remaining / rate
        return None
    
    def _build_progress_bar(self) -> str:
        """Build the visual progress bar."""
        percent = (self.current / self.total_items) * 100
        filled = int((self.current / self.total_items) * self.bar_width)
        empty = self.bar_width - filled
        
        bar = '█' * filled + '░' * empty
        return f"[{bar}] {percent:.1f}%"
    
    def update(self, current: Optional[int] = None, message: str = ""):
        """
        Update the progress tracker.
        
        Args:
            current: Current item number (if None, increments by 1)
            message: Optional message to display
        """
        item_start = time.time()
        
        if current is not None:
            self.current = current
        else:
            self.current += 1
        
        # Build progress string
        progress_bar = self._build_progress_bar()
        elapsed = time.time() - self.start_time
        
        parts = [f"{self.GREEN}{progress_bar}{self.NC}"]
        parts.append(f"({self.current}/{self.total_items})")
        
        if self.show_elapsed:
            parts.append(f"Elapsed: {self._format_time(elapsed)}")
        
        if self.show_eta:
            eta = self._calculate_eta()
            if eta is not None:
                parts.append(f"ETA: {self._format_time(eta)}")
        
        progress_str = " | ".join(parts)
        
        # Print based on environment
        if self.is_tty and not self.is_slurm:
            # Interactive terminal: use carriage return for inline update
            print(f"\r{progress_str}", end='', file=self.output)
            if message:
                print(f"\n  {self.YELLOW}→{self.NC} {message}", file=self.output)
        else:
            # SLURM or non-TTY: print each update on new line
            print(progress_str, file=self.output)
            if message:
                print(f"  → {message}", file=self.output)
        
        self.output.flush()
        
        # Track item time
        if self.item_times:
            self.item_times.append(time.time() - item_start)
    
    def log(self, message: str, level: str = "info"):
        """
        Log a message during progress tracking.
        
        Args:
            message: Message to log
            level: Log level (info, warning, error, success)
        """
        prefix_map = {
            "info": f"{self.BLUE}[INFO]{self.NC}",
            "warning": f"{self.YELLOW}[WARN]{self.NC}",
            "error": f"\033[0;31m[ERROR]\033[0m",
            "success": f"{self.GREEN}[OK]{self.NC}"
        }
        prefix = prefix_map.get(level, "[INFO]")
        
        if self.is_tty and not self.is_slurm:
            print(f"\n{prefix} {message}", file=self.output)
        else:
            print(f"{prefix} {message}", file=self.output)
        
        self.output.flush()
    
    def complete(self, message: str = ""):
        """
        Mark the task as complete and print summary.
        
        Args:
            message: Optional completion message
        """
        self.current = self.total_items
        elapsed = time.time() - self.start_time
        
        if self.is_tty and not self.is_slurm:
            print("", file=self.output)  # New line after progress bar
        
        print(f"\n{self.CYAN}{'-'*60}{self.NC}", file=self.output)
        print(f"{self.GREEN}✓ Completed!{self.NC}", file=self.output)
        print(f"  Total items processed: {self.total_items}", file=self.output)
        print(f"  Total time: {self._format_time(elapsed)}", file=self.output)
        
        if self.total_items > 0:
            avg_time = elapsed / self.total_items
            print(f"  Average time per item: {self._format_time(avg_time)}", file=self.output)
        
        if message:
            print(f"  {message}", file=self.output)
        
        print(f"  Finished at: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}", file=self.output)
        print(f"{self.CYAN}{'='*60}{self.NC}\n", file=self.output)
        self.output.flush()
    
    def error(self, message: str):
        """
        Report an error and current progress state.
        
        Args:
            message: Error message
        """
        elapsed = time.time() - self.start_time
        
        print(f"\n\033[0;31m{'='*60}\033[0m", file=self.output)
        print(f"\033[0;31m✗ Error occurred!\033[0m", file=self.output)
        print(f"  Progress: {self.current}/{self.total_items}", file=self.output)
        print(f"  Elapsed time: {self._format_time(elapsed)}", file=self.output)
        print(f"  Error: {message}", file=self.output)
        print(f"\033[0;31m{'='*60}\033[0m\n", file=self.output)
        self.output.flush()


class BatchProgressTracker:
    """
    Tracks progress for multiple sub-tasks within a batch job.
    Useful for tracking KPI pairs, Interactive Plot files, etc.
    """
    
    def __init__(self, total_batches: int, batch_name: str = "Batch"):
        """
        Initialize batch progress tracker.
        
        Args:
            total_batches: Total number of batches/pairs to process
            batch_name: Name for each batch item
        """
        self.total_batches = total_batches
        self.batch_name = batch_name
        self.current_batch = 0
        self.batch_start_times = []
        self.batch_end_times = []
        self.overall_start = time.time()
        
        # Environment detection
        self.is_slurm = bool(os.environ.get('SLURM_JOB_ID'))
        
        print(f"\n{'='*60}")
        print(f"Batch Processing: {total_batches} {batch_name}(s)")
        print(f"{'='*60}")
        if self.is_slurm:
            print(f"SLURM Job ID: {os.environ.get('SLURM_JOB_ID')}")
        print(f"Started: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"{'-'*60}\n")
    
    def start_batch(self, batch_id: int, description: str = ""):
        """Start tracking a new batch."""
        self.current_batch = batch_id
        self.batch_start_times.append(time.time())
        
        overall_elapsed = time.time() - self.overall_start
        
        print(f"\n[{batch_id}/{self.total_batches}] Starting {self.batch_name}")
        if description:
            print(f"    {description}")
        print(f"    Overall elapsed: {self._format_time(overall_elapsed)}")
        
        # ETA calculation
        if batch_id > 1 and self.batch_end_times:
            avg_batch_time = sum(
                self.batch_end_times[i] - self.batch_start_times[i]
                for i in range(len(self.batch_end_times))
            ) / len(self.batch_end_times)
            remaining_batches = self.total_batches - batch_id + 1
            eta = avg_batch_time * remaining_batches
            print(f"    Estimated remaining: {self._format_time(eta)}")
    
    def end_batch(self, batch_id: int, success: bool = True, message: str = ""):
        """End tracking for current batch."""
        self.batch_end_times.append(time.time())
        batch_time = self.batch_end_times[-1] - self.batch_start_times[-1]
        
        status = "✓" if success else "✗"
        print(f"    {status} Completed in {self._format_time(batch_time)}")
        if message:
            print(f"    {message}")
    
    def summary(self):
        """Print final summary."""
        total_time = time.time() - self.overall_start
        
        print(f"\n{'='*60}")
        print(f"Batch Processing Complete")
        print(f"{'='*60}")
        print(f"Total {self.batch_name}s: {self.total_batches}")
        print(f"Total time: {self._format_time(total_time)}")
        
        if self.batch_end_times:
            batch_times = [
                self.batch_end_times[i] - self.batch_start_times[i]
                for i in range(len(self.batch_end_times))
            ]
            avg_time = sum(batch_times) / len(batch_times)
            print(f"Average per {self.batch_name}: {self._format_time(avg_time)}")
        
        print(f"Finished: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"{'='*60}\n")
    
    def _format_time(self, seconds: float) -> str:
        """Format seconds into human-readable string."""
        if seconds < 60:
            return f"{seconds:.1f}s"
        elif seconds < 3600:
            mins = int(seconds // 60)
            secs = int(seconds % 60)
            return f"{mins}m {secs}s"
        else:
            hours = int(seconds // 3600)
            mins = int((seconds % 3600) // 60)
            return f"{hours}h {mins}m"


# Convenience function for simple progress tracking
def track_progress(items, task_name="Processing", show_item=True):
    """
    Generator that yields items while tracking progress.
    
    Usage:
        for item in track_progress(my_list, "Processing files"):
            process(item)
    """
    total = len(items) if hasattr(items, '__len__') else None
    
    if total is None:
        items = list(items)
        total = len(items)
    
    tracker = ProgressTracker(total, task_name)
    
    for i, item in enumerate(items, 1):
        message = str(item) if show_item else ""
        tracker.update(i, message)
        yield item
    
    tracker.complete()


if __name__ == "__main__":
    # Demo/test
    print("Testing ProgressTracker...")
    
    tracker = ProgressTracker(5, "Demo Task")
    for i in range(1, 6):
        time.sleep(0.5)
        tracker.update(i, f"Processing item {i}")
    tracker.complete("All items processed successfully!")
    
    print("\nTesting BatchProgressTracker...")
    
    batch_tracker = BatchProgressTracker(3, "HDF Pair")
    for i in range(1, 4):
        batch_tracker.start_batch(i, f"input_{i}.h5 → output_{i}.h5")
        time.sleep(0.5)
        batch_tracker.end_batch(i, success=True)
    batch_tracker.summary()
