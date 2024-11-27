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
  float mx;
  float my;
  float mz;
  float baro;
};

"""

f = open(sys.argv[1], "rb")

data = []
N = 4 + 4*10
while True:
    b = f.read(N)
    if b is None:
        break
    if len(b) != N:
        break
    # print(b)
    data.append(struct.unpack("<Lffffffffff", b))
f = open("out.csv", "w")
f.write("T ms, P pa\n")
for i in data:
    f.write(f"{i[0]},{i[-1]}\n")
# print(data, len(data))  

D = np.array([i for i in data])
D[:, 0] /= 1000
D[:, [1, 2, 3]] *= np.pi/180
 

np.save("D.npy", D)


T = np.array([i[0] for i in data])/1000
P = np.array([i[-1] for i in data])
gx = np.array([i[1] for i in data])
gy = np.array([i[2] for i in data])
gz = np.array([i[3] for i in data])
ax = np.array([i[4] for i in data])
ay = np.array([i[5] for i in data])
az = np.array([i[6] for i in data])
mx = np.array([i[7] for i in data])
my = np.array([i[8] for i in data])
mz = np.array([i[9] for i in data])

T -= T[0]
# P /= 100;

# altitude = 44330 * (1.0 - pow(P / (101992), 0.1903));
altitude = -np.log(P) *8.31*(273.5 -6)/9.8/(28/1000)

plt.figure()
plt.scatter(T, altitude - altitude[0])
# plt.scatter(T, P)

plt.ylabel("H, m")
plt.xlabel("T, s")

plt.figure()
plt.scatter(T, gx)
plt.scatter(T, gy)
plt.scatter(T, gz)
plt.ylabel("g")
plt.xlabel("T, s")

plt.figure()
plt.scatter(T, ax)
plt.scatter(T, ay)
plt.scatter(T, az)
plt.xlabel("T, s")
plt.ylabel("a")

plt.figure()
plt.scatter(T, mx)
plt.scatter(T, my)
plt.scatter(T, mz)
plt.xlabel("T, s")
plt.ylabel("m")
plt.show()