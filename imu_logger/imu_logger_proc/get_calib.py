import numpy as np

D = np.load("D.npy")
gyro = D[:, [1, 2, 3]]

Sigma_gyro = np.cov(gyro.T)
bias_gyro = np.mean(gyro, axis=0)


print("Sigma_gyro", Sigma_gyro)
print("bias_gyro", bias_gyro)


accel = D[:, [4, 5, 6]]


Sigma_accel = np.cov(accel.T)
scale_accel = np.mean(np.linalg.norm(accel, axis=1))


print("Sigma_accel", Sigma_accel)
print("scale_accel", scale_accel)


