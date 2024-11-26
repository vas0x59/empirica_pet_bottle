import struct 
import matplotlib.pyplot as plt
import sys
import numpy as np
"""
struct Data {
  uint32_t time;
  float wx;
  float wy;
  float wz;
  float ax;
  float ay;
  float az;
  float baro;
};

"""

f = open(sys.argv[1], "rb")

data = []

while True:
    b = f.read(4 + 4*7)
    if b is None:
        break
    if len(b) != 32:
        break
    # print(b)
    data.append(struct.unpack("<Lfffffff", b))
f = open("out.csv", "w")
f.write("T ms, P pa\n")
for i in data:
    f.write(f"{i[0]},{i[-1]}\n")
# print(data, len(data))  
N = 10
D = 16
T = np.array([i[0] for i in data])[::N]/1000
T = T - T[0]
P_f = np.array([i[-1] for i in data])[::N]
P = [P_f[0]]
for i in range(1, len(P_f)):
    P.append(P_f[i]*D - P_f[i-1]*(D-1))
P = np.array(P)
# P = P_f
# P /= 100;

# altitude = 44330 * (1.0 - pow(P / (999.1 * 100), 0.1903));
altitude = -np.log(P) *8.31*(273.5 -4)/9.8/(28/1000)

plt.scatter(T, altitude - altitude[0])
plt.ylabel("H, m")
# plt.scatter(T, P)
# plt.ylabel("P, pa")



plt.xlabel("T, ms")
print(np.max(altitude - altitude[0]))
plt.show()