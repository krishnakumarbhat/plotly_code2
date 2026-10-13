import os

class Logger:
    def __init__(self):
        self.txt_filename = ""
        
    def init_debug_file(self, filename='debug.txt'):
        self.txt_filename = filename
        # Ensure the directory exists
        dir_name = os.path.dirname(self.txt_filename)
        if dir_name and not os.path.exists(dir_name):
            os.makedirs(dir_name, exist_ok=True)
        with open(self.txt_filename, 'w'):
            pass

    def custom_print(self, text: str):
        # Print to terminal
        print(text)
        # Append to file if initialized, otherwise skip file logging (edge case before init)
        if not self.txt_filename:
            return
        try:
            # Ensure directory exists
            dir_name = os.path.dirname(self.txt_filename)
            if dir_name and not os.path.exists(dir_name):
                os.makedirs(dir_name, exist_ok=True)
            with open(self.txt_filename, 'a', encoding='utf-8') as f:
                f.write(text + '\n')
        except Exception as e:
            print(f"[LOGGER WARNING] Failed to write to {self.txt_filename}: {e}")
            
logger = Logger()