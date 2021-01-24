#%%
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

fileName = "meritev_3" + ".npy"
convertToMinutes = 1

data = np.load(fileName)
print(data)
x = data[:, 0]
R0 = data[:, 1]

start = datetime.strptime(x[0], "%d/%m/%Y, %H:%M:%S")
dt = [datetime.strptime(x[i], "%d/%m/%Y, %H:%M:%S").timestamp() - start.timestamp() for i in range(len(x))]
if(convertToMinutes):
    dt = [i/60 for i in dt]
# %%
plt.figure()
plt.plot(dt, R0, label="R0")
plt.legend()
if(convertToMinutes):
    plt.xlabel("Time [min]")
else:
    plt.xlabel("Time [s]")
plt.ylabel("R0")
# %%
