import serial 
import sys


def main():
    PORT="/dev/ttyACM0"
    
    if len(sys.argv) > 1:
        PORT = sys.argv[1]

    print("Running data logger at port {}.".format(PORT))

    with serial.Serial( port=PORT, baudrate=9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=2) as ser:
        try:
            ser.isOpen()
            print ("Serial port is open")
        except Exception as e:
            print ("Error open serial port: " + str(e))
        
        while True:
            try:
                data = parse(ser.readline())
                if data:
                    int_value = data
                print(int_value)
            except Exception as e:
                print ("Error while communicating with Arduino : " + str(e))

def parse(bytes):
    if len(bytes) == 4:
        return int.from_bytes(bytes[:-2], byteorder="little", signed=True)
    else:
        return None

if __name__ == "__main__":
    main()
