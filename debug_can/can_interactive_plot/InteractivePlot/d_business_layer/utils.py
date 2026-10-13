import time
import functools
import logging
import os
import sys

def time_taken(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        logging.info(f"Function {func.__name__} took {end_time - start_time:.4f} seconds to execute.")
        return result
    return wrapper

class LoggerSetup:
    def __init__(self, output_dir):
        self.output_dir = output_dir
        self.logger = None
        self.setup_logging()

    def setup_logging(self):
        os.makedirs(self.output_dir, exist_ok=True)
        logs_file = os.path.join(self.output_dir, "logs.txt")
        
        # Create logs directory for better organization
        logs_dir = os.path.join(self.output_dir, "logs")
        os.makedirs(logs_dir, exist_ok=True)
        
        # Rotating file handler for better log management
        from logging.handlers import RotatingFileHandler
        
        self.logger = logging.getLogger()
        self.logger.setLevel(logging.DEBUG)

        for handler in self.logger.handlers[:]:
            self.logger.removeHandler(handler)

        # Rotating file handler (10MB max, keep 5 backup files)
        file_handler = RotatingFileHandler(
            os.path.join(logs_dir, "application.log"), 
            maxBytes=10*1024*1024, 
            backupCount=5,
            mode="a"
        )
        file_handler.setLevel(logging.DEBUG)
        file_formatter = logging.Formatter(
            "%(asctime)s - %(name)s - %(levelname)s - %(funcName)s:%(lineno)d - %(message)s"
        )
        file_handler.setFormatter(file_formatter)

        # Console handler with color support
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setLevel(logging.INFO)
        console_formatter = logging.Formatter(
            "%(asctime)s - %(levelname)s - %(message)s"
        )
        console_handler.setFormatter(console_formatter)

        # Error file handler for critical errors
        error_handler = RotatingFileHandler(
            os.path.join(logs_dir, "errors.log"),
            maxBytes=5*1024*1024,
            backupCount=3,
            mode="a"
        )
        error_handler.setLevel(logging.ERROR)
        error_handler.setFormatter(file_formatter)

        self.logger.addHandler(file_handler)
        self.logger.addHandler(console_handler)
        self.logger.addHandler(error_handler)

        logging.info(f"Enhanced logging initialized. Log directory: {logs_dir}")

    def log_to_both(self, message):
        self.logger.info(message)

    def log_to_file_only(self, message):
        self.logger.debug(message)

    def log_to_console_only(self, message):
        file_handler = self.logger.handlers[0]
        self.logger.removeHandler(file_handler)
        self.logger.info(message)
        self.logger.addHandler(file_handler)