import pyfirmata
import time

if __name__ == '__main__':
    board = pyfirmata.Arduino('COM3')
    print("Communication Successfully started")
    it = pyfirmata.util.Iterator(board)
    it.start()

    a0_pin = board.analog[0]
    a0_pin.enable_reporting()
    while True:
        print(a0_pin.read()) # values are between 0 and 1
        time.sleep(0.1)