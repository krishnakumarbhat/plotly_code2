#!/usr/bin/env python3
"""UART Loopback Test Script.

Receives data on COM5 and echoes it back automatically for UART testing.
"""

import serial
import sys
import time
from datetime import datetime

# Configuration
SERIAL_PORT = "COM5"
BAUD_RATE = 115200  # Adjust this to match your UART configuration
TIMEOUT = None  # No timeout - keep listening indefinitely


def main():
    """Main function to run UART loopback."""
    start_time = datetime.now()
    start_time_str = start_time.strftime("%Y-%m-%d %H:%M:%S")
    print("=== UART Loopback Test ===")
    print(f"Started: {start_time_str}")
    print(f"Port: {SERIAL_PORT} @ {BAUD_RATE} baud")
    print("Press Ctrl+C to exit\n")

    try:
        # Open serial port
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=TIMEOUT,
            write_timeout=TIMEOUT,
        )

        print(f"✓ Serial port {SERIAL_PORT} opened successfully")
        print(f"  Baudrate: {BAUD_RATE}")
        print("  Mode: Continuous listening (no timeout)")
        print(f"\n{'='*50}")
        print("ACTIVE - Waiting for incoming data...")
        print(f"{'='*50}\n")

        # Clear any existing data in buffers
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        packets_processed = 0

        # Main loop - read and echo back
        while True:
            # Read data (blocks until data arrives due to timeout=None)
            # Read one byte first to detect incoming data
            data = ser.read(1)

            if data:
                # Check if more data is available and read it
                time.sleep(0.01)  # Small delay to allow full packet to arrive
                if ser.in_waiting > 0:
                    data += ser.read(ser.in_waiting)

                packets_processed += 1

                # Display received data with timestamp
                timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                print(f"[{packets_processed}] {timestamp} - Received {len(data)} bytes:")
                print(f"    Hex: {data.hex(' ').upper()}")
                print(f"    ASCII: {data.decode('ascii', errors='replace')}")

                # Echo data back
                ser.write(data)
                ser.flush()  # Ensure data is sent immediately

                print(f"    ✓ Echoed back {len(data)} bytes\n")

    except serial.SerialException as e:
        print(f"\n✗ Serial port error: {e}")
        print("\nTroubleshooting:")
        print(f"  - Verify {SERIAL_PORT} exists and is not in use by another application")
        print("  - Check Device Manager for available COM ports")
        print("  - Ensure UART hardware is properly connected")
        print("\nPress Enter to close this window...")
        input()
        sys.exit(1)

    except KeyboardInterrupt:
        end_time = datetime.now()
        duration = end_time - start_time
        print(f"\n\n{'='*50}")
        print("✓ Received Ctrl+C - Shutting down...")
        print(f"{'='*50}")
        print("Session Statistics:")
        print(f"  Total packets processed: {packets_processed}")
        print(f"  Session duration: {duration}")
        print(f"{'='*50}\n")

    except Exception as e:
        print(f"\n✗ Unexpected error: {e}")
        print("\nPress Enter to close this window...")
        input()
        sys.exit(1)

    finally:
        if "ser" in locals() and ser.is_open:
            ser.close()
            print(f"✓ Serial port {SERIAL_PORT} closed")
        print("\nScript terminated. Press Enter to close this window...")
        input()


if __name__ == "__main__":
    main()
