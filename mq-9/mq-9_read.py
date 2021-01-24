"""
Upload SimpleFirmata code to Arduino
"""
#%%
import pyfirmata
import time
from numpy import mean, save
from datetime import datetime
import matplotlib.pyplot as plt
import os.path
from os import path

def doesExist(fileName):
    return path.exists(fileName)

def checkIfSaveFileExists(fileName):
    if(fileName[-3:] != "npy"):
        fileName += ".npy"

    if fileName == "test.npy" or fileName == "Test.npy":
        pass
    else:
        if(doesExist(fileName)):
            raise SystemExit("Error saving: file exists!")
        else:
            pass

# Settings
Naverage = 1
saveName = "meritev_3.npy"
debug = 1
comPort = 3

# Main code
checkIfSaveFileExists(saveName)

board = pyfirmata.Arduino('COM%d' %comPort)
print("Communication Successfully started")
it = pyfirmata.util.Iterator(board)
it.start()

a0_pin = board.analog[0]
a0_pin.enable_reporting()
time.sleep(0.1)
data_R0 = []
data_V = []

while True:
    
    sensorValues = [a0_pin.read() for i in range(100)]

    sensorValue = mean(sensorValues)

    sensor_volt = sensorValue * 5.0

    Rs_air = (5.0 - sensor_volt) / sensor_volt
    R0 = Rs_air / 9.9
    now = datetime.now()
    date_time = now.strftime("%d/%m/%Y, %H:%M:%S")
    data_R0.append([date_time, R0])
    data_V.append(sensor_volt)

    if(debug): print("R0 = %f, U = %fV" %(R0, sensor_volt))

    save(saveName, data_R0)
    
    time.sleep(1)


# %%
