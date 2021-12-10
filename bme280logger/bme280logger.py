"""
Script for logging data from BME280
Compatible with bme280_monitor.ino
Requirements: create 
"""
from argparse import ArgumentParser
from serial import Serial
from time import sleep, time
#from numpy import array, save, load, append
from datetime import datetime, timedelta
import sqliteSave as sqldb
from os.path import exists
from os import mkdir


class arduinoBME280:

    def __init__(self, fileSave, pcom="COM4", bdrate=9600, name="COMX", displayTime=60, refreshTime=1, absDiff=[0.1]):
        self._pcom = pcom
        self._bdrate = bdrate
        self._name = name
        self._sleep = 0.2
        self._displayTime = timedelta(seconds=displayTime) 
        self._refreshTime = refreshTime # seconds
        self._absDiff = [0.1, 0.1, 0.1]

        for i in range(3):
            if i < len(absDiff):
                self._absDiff[i] = float(absDiff[i])

        fileSave = fileSave.replace('\\', '/')
        self._saveFolder = fileSave[:fileSave.rfind('/')]
        self._dbFileName = fileSave[fileSave.rfind('/')+1:]

        if self._dbFileName[-3:] != ".db":
            print("[Warning] Wrong save file extension: %s. Expected .db" %self._dbFileName[-3:])
            self._dbFileName += ".db"

        self._fileSave = self._saveFolder + '/' + self._dbFileName

        print("Display data every %d seconds" %self._displayTime.total_seconds())
        print("Save data if abs. dif >", self._absDiff)

    def init(self):
        self._ser = Serial(self._pcom, self._bdrate)
        self.buf = bytearray()
        sleep(2)
        print("BME280 initialized - %s" %self._pcom)
        if exists(self._saveFolder) != 1:
            mkdir(self._saveFolder)

        print("Output file: %s" %(self._fileSave))
        return 0

    def close(self):
        self._ser.close()
        print("%s closed" %self._pcom)
        return 0

    def getData(self):
        self.sendCommand("get()")
        self.readline()
        return self.parseMsg()

    def sendCommand(self, cmd):
        #print("Sending command %s" %cmd)
        cmd += "\r\n"
        self._ser.write(cmd.encode('utf-8'))
        self._tst = int(time())
        sleep(self._sleep)
        return 0

    def readline(self):
        #print("Reading from serial")
        terminator = ";"
        msg = self._ser.readline().decode()
        self._msg = msg[:msg.find(terminator)]
        return  self._msg

    def parseMsg(self):
        delimator = ", "
        T,p,h = self._msg.split(delimator)
        return [float(T),float(p),float(h)]

    def runMonitor(self):
        displayTst = datetime.now()
        oldData = [0, 0, 0, 0]
        while True:
            try:
                data = self.getData()
                
                data = [self._tst, data[0], data[1], data[2]]
                dt_object = datetime.fromtimestamp(data[0])
                
                #compare all three variables
                for i in range(1,len(data)):
                    if abs(data[i] - oldData[i]) >= self._absDiff[i-1]:
                        sqldb.main(self._fileSave, self._tst, data[1], data[2], data[3])
                        oldData = data
                        print(dt_object.strftime("%Y-%m-%d %H:%M:%S"), data[1], data[2], data[3], "- Data saved")
                        displayTst = datetime.now()
                        break
                
                if datetime.now() - displayTst >= self._displayTime:
                    print(dt_object.strftime("%Y-%m-%d %H:%M:%S"), data[1], data[2], data[3])
                    displayTst = datetime.now()
                sleep(self._refreshTime)

            except KeyboardInterrupt:
                break
        print("Monitor closed")

if __name__ == "__main__":
    # Define the parser
    parser = ArgumentParser(description='Logger for bme280. Use bme280_monitor.ino')
    parser.add_argument('-c', '--com', action="store", type=str, dest='com', default="COM4", help="Select COM port")
    parser.add_argument('-b', '--baudrate', action="store", type=int, dest='bdrate', default=115200, help="Select baudrate, default=115200")
    parser.add_argument('-r', '--refreshTime', action="store", type=float, dest='refreshTime', default=1, help="Set sensor refresh time, default=1 s")
    parser.add_argument('-d', '--displayTime', action="store", type=float, dest='displayTime', default=60, help="Set display time, default=60 s")
    parser.add_argument('-a', '--absDiff', nargs='+', action="store", dest='absDiff', default=[0.1],
                        help="Set minimum variable change required to save the measurements, default=0.1. Specify minimum change for variables separately")
    parser.add_argument('-f', '--fileSave', action="store", type=str, dest='fileSave', default="data/airSensor.db", help="Set save file, default='data/airSensor.db'")
    
    
    args = parser.parse_args()

    arduino = arduinoBME280(pcom=args.com,
                            bdrate=args.bdrate,
                            name="test",
                            displayTime=args.displayTime,
                            refreshTime=args.refreshTime,
                            absDiff=args.absDiff,
                            fileSave=args.fileSave)
    arduino.init()
    arduino.runMonitor()
    arduino.close()
