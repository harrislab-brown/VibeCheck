import csv
import matplotlib.pyplot as plt

fileName = 'data_2024-10-16_16-05-00.csv'

sensor0 = []
sensor1 = []
sensor2 = []

def parse_line(newline):
    num_points = int (newline[1])
    data_point_arr = []
    for i in range(num_points):
        # 5 is number of elements in each datapoint, 2 is index of first piece of data
        start_index = i * 5 + 2
        data_point = newline[start_index:start_index+5]
        data_point_arr.append(data_point)
    return data_point_arr


with open(fileName , 'r') as f:
    line_reader = csv.reader(f, delimiter=',')

    
    next(line_reader, None) # skip file header
    for line in line_reader:
        dataPoint = [float(line[1]),float(line[2]),float(line[3]),float(line[4])]
        if line[0] == '0':
            sensor0.append(dataPoint)
        elif line[0] == '2':
            sensor1.append(dataPoint)
        elif line[0] == '4':
            sensor2.append(dataPoint)

t = []
x = []
y = []
z = []

for dp in sensor0:
    t.append(dp[0])
    x.append(dp[1])
    y.append(dp[2])
    z.append(dp[3])
plt.plot(t, x)
plt.plot(t,y)
plt.plot(t,z)

t = []
x = []
y = []
z = []
for dp in sensor1:
    t.append(dp[0])
    x.append(dp[1])
    y.append(dp[2])
    z.append(dp[3])
plt.plot(t, x)
plt.plot(t,y)
plt.plot(t,z)

t = []
x = []
y = []
z = []

plt.show()