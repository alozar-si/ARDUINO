# BME280Logger

Script `bme280logger.py` is used to log data from a BME280 sensor connected to Arduino Uno over I2C communication.

## Prerequests

Upload `bme280_monitor.ino` to Arduino board located in [`bme280_arduino`](https://repo.ijs.si/andrejl/bme280logger/-/tree/main/bme280_arduino) folder.

## Running the script

Run command: `python bme280logger.py`

Flags:

-c --com        : Select COM port, default=COM4

-b --baudrate   : Select baudrate, default=115200

-r --refreshTime  : Set sensor refresh time, default=1 s

-d --displayTime    : Set display time, default=60 s

-a --absDiff    : Set minimum variable change required to save the measurements, default=0.1. Specify minimum change for individual variable: `-a 0.5 0.1 0.5`

-f --fileName   : Set save file, default='data/airSensor.db'

## Data format

Data is saved to SQLite database in `data/airSensor.db` in table `airSensor`.

Saved data: id, timestamp, temperature, pressure, humidity

## Results

To run analysis use script `analysis.py`. It can be run as a notebook.

Comparison of data collected with BME280 and downloaded from [link](https://www.meteoblue.com/en/weather/archive/export/basel_switzerland_2661604) for Bassel.

![image](data/pressure_comparison_cern-bassel.png)
