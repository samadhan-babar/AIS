import serial
import datetime
import time
import os
from colorama import init, Fore, Style
from pyais import decode
import argparse


# Initialize colorama
init(autoreset=True)

# Get current timestamp for log filenames
timestamp = datetime.datetime.now().strftime('%d%m%Y%H%M%S')

serial_port = ""
baud_rate = ""

# Open log files
raw_log_file = open(f'raw{timestamp}.log', 'a')
gps_log_file = open(f'GPS{timestamp}.log', 'a')
ais_log_file = open(f'AIS{timestamp}.log', 'a')
dbg_log_file = open(f'DBG{timestamp}.log', 'a')

# Clear the terminal
def clear_terminal():
    os.system('cls' if os.name == 'nt' else 'clear')



# Function to flush logs to disk
def flush_logs():
    raw_log_file.flush()
    gps_log_file.flush()
    ais_log_file.flush()
    dbg_log_file.flush()

def main():
    # Define and parse arguments
    parser = argparse.ArgumentParser(description="Serial port data logger")
    parser.add_argument("serial_port", help="Serial port device (e.g., /dev/ttyUSB0)")
    parser.add_argument("-b", "--baud_rate", type=int, default=38400, help="Serial port baud rate (default: 38400)")
    args = parser.parse_args()

    # Configure serial port
    serial_port = args.serial_port
    baud_rate = args.baud_rate

    # Initialize last received data
    last_raw_data = ""
    last_gps_data = "No data"
    last_ais_data = "No data"
    last_dbg_data = "No data"

    try:
        # Open the serial port
        with serial.Serial(serial_port, baud_rate, timeout=1) as ser:
            last_flush_time = time.time()
            while True:
                # Get current timestamp
                timestamp1 = datetime.datetime.now().strftime('%d/%m/%Y %H:%M:%S:%f -> ')
                # Read a line from the serial port
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                # Log the raw data
                raw_log_file.write(timestamp1 + line + '\n')
                last_raw_data = line  # Update last raw data

                # Prepare to display in blocks
                clear_terminal()
                decoded_payload = "No Data Received"
                # Check the sentence type and log accordingly
                if line.startswith('$GNGLL'):
                    last_gps_data = line
                    gps_log_file.write(timestamp1 +line + '\n')
                elif line.startswith('!AIVDM'):
                    last_ais_data = line
                    #decode the AIS sentence using pyais
                    payload = "b\""+ line +"\""
                    # decoded_payload = decode(payload)

                    ais_log_file.write(timestamp1 +line + '\n')
                elif line.startswith('$P'):
                    last_dbg_data = line
                    dbg_log_file.write(timestamp1 +line + '\n')

                # Print the output in block format with colors
                print(Fore.CYAN + "RAW\n" + Style.RESET_ALL + last_raw_data)
                print(Fore.GREEN + "GPS\n" + Style.RESET_ALL +  timestamp1 + last_gps_data)
                print(Fore.BLUE + "AIS\n" + Style.RESET_ALL +  timestamp1 + Fore.RED + last_ais_data + Style.RESET_ALL)
                print(Fore.YELLOW + "DBG\n" + Style.RESET_ALL + timestamp1 + last_dbg_data)
                print(Fore.MAGENTA + "Decoded\n" + Style.RESET_ALL + timestamp1 + decoded_payload)

                # Flush logs every 5 seconds
                current_time = time.time()
                if current_time - last_flush_time >= 5:
                    flush_logs()
                    last_flush_time = current_time
            
    except KeyboardInterrupt:
        print("Logging stopped.")


if __name__ == "__main__":
    try:
        main()
    except serial.SerialException as e:
        print(f"Error opening serial port '{serial_port}': {e}")
    finally:
        # Close log files even on exceptions
        flush_logs()
        raw_log_file.close()
        gps_log_file.close()
        ais_log_file.close()
        dbg_log_file.close()



