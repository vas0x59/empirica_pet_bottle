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
T = np.array([i[0] for i in data])
P = np.array([i[-1] for i in data])
# P /= 100;

altitude = 44330 * (1.0 - pow(P / (101992), 0.1903));

# plt.scatter(T, altitude - altitude[0])
plt.scatter(T, P)
plt.xlabel("T, ms")
plt.ylabel("P, pa")
# plt.ylabel("H, m")
plt.show()