
import paramiko
import os

# --- Connection details ---
HOST = "10.192.224.131"       # From your screenshot
PORT = 22                     # SFTP default
USERNAME = "gjlkfw"           # From your screenshot
PASSWORD = ""    # Replace with actual password

# --- File details ---
REMOTE_FILE = "/mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/gjlkfw/TOOL-RESULTS/2025-01-03/05-28-27-823729" \
              "/html_details.txt"  # Full remote path of the file
LOCAL_FILE = r"C:\Users\gjlkfw\Downloads\html_details.txt"  # Local destination path

# --- Connect and download ---
try:
    # Create SSH transport
    transport = paramiko.Transport((HOST, PORT))
    transport.connect(username=USERNAME, password=PASSWORD)

    # Open SFTP session
    sftp = paramiko.SFTPClient.from_transport(transport)

    print(f"Downloading {REMOTE_FILE} to {LOCAL_FILE}...")
    sftp.get(REMOTE_FILE, LOCAL_FILE)  # Download the file
    print("Download complete!")

    # Close connection
    sftp.close()
    transport.close()

except Exception as e:
    print(f"Error: {e}")
