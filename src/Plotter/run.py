import serial 
import matplotlib.pyplot as plt
import sys
import numpy as np
import json
import time

PATH = ""

def main():
    PORT="/dev/ttyACM0"
    TIME=300
    SAVE = True
    datenarray = list()

    if len(sys.argv) > 1:
        PORT = sys.argv[1]

    if len(sys.argv) > 2:
        TIME = sys.argv[2]

    print("Running data logger at port {}.".format(PORT))

    with serial.Serial( port=PORT, baudrate=9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=2) as ser:
        try:
            ser.isOpen()
            print ("Serial port is open")
        except Exception as e:
            print ("Error open serial port: " + str(e))
        
        for i in range(0,TIME):
            try:
                data = parse(ser.readline())
                if data:
                    int_value = data
                    datenarray.append(int_value)
                print(int_value)
            except Exception as e:
                print ("Error while communicating with Arduino : " + str(e))
        
        if SAVE:
            save(datenarray)

        plt.plot(datenarray)
        plt.xlabel('time (s)')
        plt.ylabel('Acceleration')
        plt.title('')
        plt.grid(True)
        plt.show()

def save(data):
    with open(PATH + 'rec' + str(time.time()) + '.json', 'w') as outfile:
        json.dump(data, outfile)

def parse(bytes):
    if len(bytes) == 4:
        return int.from_bytes(bytes[:-2], byteorder="little", signed=True)
    else:
        return None

if __name__ == "__main__":
    main()
