import serial
import serial.tools.list_ports
import datetime
from typing import List
import select
import sys
import csv


def get_available_ports() -> List[str]:
    return [port.device for port in serial.tools.list_ports.comports()]


def select_port(ports: List[str]) -> str:
    print("Available ports:")
    for i, port in enumerate(ports):
        print(f"{i + 1}. {port}")
    while True:
        try:
            choice = int(input("Select a port number: ")) - 1
            if 0 <= choice < len(ports):
                return ports[choice]
            else:
                print("Invalid choice. Please try again.")
        except ValueError:
            print("Invalid input. Please enter a number.")

def input_with_timeout(prompt: str, timeout: float = 0.1) -> str:
    print(prompt, end='', flush=True)
    rlist, _, _ = select.select([sys.stdin], [], [], timeout)
    if rlist:
        return sys.stdin.readline().strip()
    return ''

def vibecheck_init(port):
    port.write(b'sensor 0 set accel odr 6660\n')
    port.write(b'sensor 0 start accel\n')
    print("sensor 0 initialized")


def parse_line(newline):
    num_points = int (newline[1])
    data_point_arr = []
    for i in range(num_points):
        # 5 is number of elements in each datapoint, 2 is index of first piece of data
        start_index = i * 5 + 2
        data_point = newline[start_index:start_index+5]
        data_point_arr.append(data_point)
    return data_point_arr

def main():

    ports = get_available_ports()
    if not ports:
        print("No serial ports found.")
        return
    selected_port = select_port(ports)
    baud_rate = 115200  # You may need to adjust this based on your device

    try:
        ser = serial.Serial(selected_port, baud_rate)
        print(f"Connected to {selected_port}")
        vibecheck_init(ser)
        current_time = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"data_{current_time}.csv"

        with open(filename,'w', newline='') as output_file:
            print(f"Logging data to {filename}. Press Enter to stop.")
            csvwriter = csv.writer(output_file)

            lines_written = 0
            max_lines = 40000
            while lines_written <= max_lines:  
                if ser.in_waiting > 0:
                    newline = ser.readline().decode().strip().split(" ")
                    if newline[0] != "data":
                        print(newline)
                    else:
                        parsed_line = parsed_line(newline)
                        #output_file.write(parsed_line)
                        csvwriter.writerow(parsed_line)
                        lines_written += 1
                    
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    finally:
        if 'ser' in locals():
            ser.close()
            print("Serial port closed.")
    
if __name__ == "__main__":
    main()