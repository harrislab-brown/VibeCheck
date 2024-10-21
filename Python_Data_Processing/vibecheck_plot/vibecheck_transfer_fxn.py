import serial
import serial.tools.list_ports
import csv
import datetime
import time
from typing import List
import select
import sys
import matplotlib.pyplot as plt

START_FREQ = 20
STOP_FREQ = 50
STEP_FREQ = 5
STEP_TIME = 3 # seconds 

def get_available_ports() -> List[str]:
    """
    Get a list of available serial ports.
    Returns:
        List[str]: A list of available serial port names.
    """
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
    port.write(b'wavegen set amplitude 0.5\n')
    port.write(b'sensor 0 start accel\n')

def vibecheck_set_freq(port, freq):
    port.write(f'wavegen set frequency {freq}\n'.encode('utf-8'))
    port.write(b'wavegen start\n')
    
def parse_data(newline):
    num_points = int (newline[1])
    data_point_arr = []
    for i in range(num_points):
        # 5 is number of elements in each datapoint, 2 is index of first piece of data
        start_index = i * 5 + 2
        data_point = newline[start_index:start_index+5]
        data_point_arr.append(data_point)
    return data_point_arr

def plot_data(filename):
    with open(filename,'r', newline='') as csvfile:
        t = []
        x = []
        y = []
        z = []
        for line in csvfile:
            tokenized_line = line.split(',')
            t.append(float(tokenized_line[1]))
            x.append(float(tokenized_line[2]))
            y.append(float(tokenized_line[3]))
            z.append(float(tokenized_line[4]))
    plt.scatter(t,x)
    plt.scatter(t,y)
    plt.scatter(t,z)
    plt.show()


def main():
    ports = get_available_ports()
    if not ports:
        print("No serial ports found.")
        return
    selected_port = select_port(ports)
    baud_rate = 115200  # You may need to adjust this based on your device
    
    ser = serial.Serial(selected_port, baud_rate)
    print(f"Connected to {selected_port}")
    vibecheck_init(ser)
    vibecheck_set_freq(ser, START_FREQ)

    current_time = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")

    filename = f"data_{current_time}.csv"

    with open(filename,'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        print(f"Logging data to {filename}. Press Enter to stop.")
        period_start_time = datetime.datetime.now()
        sample_duration = 1
        curr_freq =60
        freq_increment = 10
        num_rows = 0
        while True:
            if ser.in_waiting > 0:
                newline = ser.readline().decode().strip().split(" ")
                if newline[0] == "data":
                    data = parse_data(newline)
                    for dp in data:
                        csvwriter.writerow(dp)
                        num_rows += 1
                else:
                    print(newline)
                curr_time = datetime.datetime.now()
                time_diff = curr_time - period_start_time
                if  time_diff > datetime.timedelta(seconds=sample_duration):
                    new_freq = curr_freq + freq_increment
                    if new_freq > 80:
                        ser.write(b'wavegen stop\n')
                        break
                    vibecheck_set_freq(ser, new_freq)
                    curr_freq = new_freq
                    period_start_time = curr_time
        ser.write(b'wavegen stop\n')
        ser.write(b'sensor 0 stop accel\n')
        print("lines written:")
        print(num_rows)            

        ser.close()
        print("Serial port closed.")
        plot_data(filename)

        
if __name__ == "__main__":
    main()