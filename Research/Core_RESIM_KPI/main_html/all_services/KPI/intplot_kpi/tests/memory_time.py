import psutil
import time
import pandas as pd
import plotly.graph_objs as go
from plotly.subplots import make_subplots
import os
import sys
import argparse
import platform


def get_process_by_name(name):
    for proc in psutil.process_iter(["name", "pid"]):
        if proc.info["name"].lower() == name.lower():
            return proc
    return None


def monitor_process(name=None, pid=None, interval=1):
    cpu_count = psutil.cpu_count(logical=True)
    system_memory = psutil.virtual_memory().total / (1024 * 1024)  # Total RAM in MB

    # Get process object
    proc = None
    if pid:
        try:
            proc = psutil.Process(pid)
            name = proc.name()
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            print(f"Process with PID {pid} not found or access denied.")
            return None
    elif name:
        proc = get_process_by_name(name)
        if proc is None:
            print(f"Process '{name}' not found.")
            return None
    else:
        print("Either process name or PID must be provided.")
        return None

    # Start monitoring
    data = {
        "timestamp": [],
        "time_str": [],
        "cpu_percent": [],
        "cpu_cores_used": [],
        "memory_percent": [],
        "memory_mb": [],
    }

    print(f"Monitoring process '{name}' (PID: {proc.pid}) until completion...")
    print(f"System info: {cpu_count} CPU cores, {system_memory:.0f} MB RAM")
    print("Press Ctrl+C to stop monitoring manually.")

    start_time = time.time()
    try:
        # Initialize CPU monitoring for this process
        proc.cpu_percent()

        while proc.is_running():
            try:
                # Get process info
                cpu_percent = proc.cpu_percent()
                cpu_cores_used = cpu_percent / 100.0 * cpu_count
                mem_percent = proc.memory_percent()
                try:
                    mem_info = proc.memory_info()
                    mem_mb = mem_info.rss / (1024 * 1024)  # Convert to MB
                except (AttributeError, TypeError):
                    # Fallback if detailed memory info not available
                    mem_mb = (mem_percent / 100.0) * system_memory

                # Record current time
                current_time = time.time()
                time_str = time.strftime(
                    "%Y-%m-%d %H:%M:%S", time.localtime(current_time)
                )

                # Store data
                data["timestamp"].append(current_time)
                data["time_str"].append(time_str)
                data["cpu_percent"].append(cpu_percent)
                data["cpu_cores_used"].append(cpu_cores_used)
                data["memory_percent"].append(mem_percent)
                data["memory_mb"].append(mem_mb)

                # Print progress
                elapsed = current_time - start_time
                print(
                    f"\rMonitoring for {elapsed:.1f}s | CPU: {cpu_percent:.1f}% ({cpu_cores_used:.2f} cores) | RAM: {mem_mb:.1f} MB ({mem_percent:.2f}%)",
                    end="",
                )

                time.sleep(interval)
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                print("\nProcess ended or access denied.")
                break
    except KeyboardInterrupt:
        print("\nMonitoring stopped by user.")

    print("\nMonitoring complete.")
    return pd.DataFrame(data)


def save_to_csv(df, name, save_dir=None):
    if df is None or df.empty:
        print("No data to save.")
        return None

    timestamp = time.strftime("%Y%m%d-%H%M%S")
    filename = f"{name}_usage_{timestamp}.csv"

    if save_dir:
        os.makedirs(save_dir, exist_ok=True)
        filepath = os.path.join(save_dir, filename)
    else:
        filepath = filename

    df.to_csv(filepath, index=False)
    print(f"Data saved to {filepath}")
    return filepath


def plot_from_csv(csv_file, save_dir=None):
    if not os.path.exists(csv_file):
        print(f"CSV file '{csv_file}' not found.")
        return

    try:
        df = pd.read_csv(csv_file)
        if df.empty:
            print("CSV file contains no data.")
            return

        process_name = os.path.basename(csv_file).split("_usage_")[0]

        x_values = df["time_str"] if "time_str" in df.columns else df["timestamp"]

        fig = make_subplots(
            rows=4,
            cols=1,
            shared_xaxes=True,
            vertical_spacing=0.05,
            subplot_titles=(
                f"CPU Usage (%) of {process_name}",
                f"CPU Cores Used by {process_name}",
                f"RAM Usage (%) of {process_name}",
                f"RAM Usage (MB) of {process_name}",
            ),
        )

        fig.add_trace(
            go.Scatter(x=x_values, y=df["cpu_percent"], mode="lines", name="CPU %"),
            row=1,
            col=1,
        )

        if "cpu_cores_used" in df.columns:
            fig.add_trace(
                go.Scatter(
                    x=x_values, y=df["cpu_cores_used"], mode="lines", name="CPU Cores"
                ),
                row=2,
                col=1,
            )

        # Add RAM percentage trace
        fig.add_trace(
            go.Scatter(x=x_values, y=df["memory_percent"], mode="lines", name="RAM %"),
            row=3,
            col=1,
        )

        # Add RAM MB trace if available
        if "memory_mb" in df.columns:
            fig.add_trace(
                go.Scatter(x=x_values, y=df["memory_mb"], mode="lines", name="RAM MB"),
                row=4,
                col=1,
            )

        # Get system info if available
        system_info = f"Monitoring on {platform.system()} {platform.release()}"

        # Update layout
        fig.update_layout(
            height=900,
            width=1000,
            title_text=f"Resource Usage for Process '{process_name}' - {system_info}",
            margin=dict(l=50, r=50, b=50, t=100),
        )

        # Update axes labels
        fig.update_xaxes(title_text="Time", row=4, col=1)
        fig.update_yaxes(title_text="CPU %", row=1, col=1)
        fig.update_yaxes(title_text="Cores", row=2, col=1)
        fig.update_yaxes(title_text="RAM %", row=3, col=1)
        fig.update_yaxes(title_text="RAM MB", row=4, col=1)

        # Save the figure as HTML
        timestamp = time.strftime("%Y%m%d-%H%M%S")
        html_filename = f"{process_name}_plot_{timestamp}.html"

        # Handle save directory
        if save_dir:
            # Create directory if it doesn't exist
            os.makedirs(save_dir, exist_ok=True)
            html_filepath = os.path.join(save_dir, html_filename)
        else:
            html_filepath = html_filename

        fig.write_html(html_filepath)
        print(f"Plot saved as HTML to {html_filepath}")

        # Show interactive plot
        fig.show()
        print(f"Plot displayed for data from {csv_file}")

        return html_filepath
    except Exception as e:
        print(f"Error plotting data: {e}")
        return None


def main():
    parser = argparse.ArgumentParser(description="Process Resource Monitor")
    parser.add_argument("--monitor", action="store_true", help="Run in monitoring mode")
    parser.add_argument("--plot", action="store_true", help="Run in plotting mode")
    parser.add_argument("--name", type=str, help="Process name to monitor")
    parser.add_argument("--pid", type=int, help="Process ID to monitor")
    parser.add_argument(
        "--interval", type=float, default=1.0, help="Sampling interval in seconds"
    )
    parser.add_argument("--csv", type=str, help="CSV file to plot (for plotting mode)")
    parser.add_argument(
        "--save-dir", type=str, help="Directory to save CSV and HTML files"
    )

    args = parser.parse_args()

    # If no arguments provided, run in interactive mode
    if len(sys.argv) == 1:
        mode = input("Select mode (1: Monitor process, 2: Plot existing data): ")

        if mode == "1":
            name_input = input(
                "Enter process name to monitor (or leave blank to use PID): "
            )
            pid_input = None
            if not name_input:
                try:
                    pid_input = int(input("Enter process ID (PID) to monitor: "))
                except ValueError:
                    print("Invalid PID. Exiting.")
                    return

            interval = 1.0
            try:
                interval = float(
                    input("Enter sampling interval in seconds (default: 1.0): ")
                    or "1.0"
                )
            except ValueError:
                print("Invalid interval. Using default of 1.0 seconds.")
                interval = 1.0

            save_dir = input(
                "Enter directory to save files (leave blank for current directory): "
            ).strip()
            save_dir = save_dir if save_dir else None

            df = monitor_process(name=name_input, pid=pid_input, interval=interval)
            if df is not None and not df.empty:
                process_name = name_input if name_input else f"pid_{pid_input}"
                csv_file = save_to_csv(df, process_name, save_dir=save_dir)

                if csv_file:
                    plot_choice = input("Do you want to plot the data now? (y/n): ")
                    if plot_choice.lower() == "y":
                        plot_from_csv(csv_file, save_dir=save_dir)
                    else:
                        print(
                            f"You can plot the data later by running: python {sys.argv[0]} --plot --csv {csv_file}"
                        )

        elif mode == "2":
            csv_file = input("Enter path to CSV file: ")
            save_dir = input(
                "Enter directory to save HTML plot (leave blank for current directory): "
            ).strip()
            save_dir = save_dir if save_dir else None
            plot_from_csv(csv_file, save_dir=save_dir)

        else:
            print("Invalid mode selection.")

    # If arguments provided, run in command line mode
    else:
        if args.monitor:
            df = monitor_process(name=args.name, pid=args.pid, interval=args.interval)
            if df is not None and not df.empty:
                process_name = args.name if args.name else f"pid_{args.pid}"
                csv_file = save_to_csv(df, process_name, save_dir=args.save_dir)
                if csv_file and args.plot:
                    plot_from_csv(csv_file, save_dir=args.save_dir)

        elif args.plot:
            plot_from_csv(args.csv, save_dir=args.save_dir)

        else:
            print("Please specify either --monitor or --plot mode.")
            parser.print_help()


if __name__ == "__main__":
    main()
