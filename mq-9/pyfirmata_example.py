#%%
import pyfirmata
import time
# %%
nano = pyfirmata.Arduino('COM3')

while True:
    nano.digital[13].write(1)
    time.sleep(1)
    nano.digital[13].write(0)
    time.sleep(1)
# %%
