
import socket
import time
import sys
import RPi.GPIO as GPIO
import serial
import threading

HOST_IP = "192.168.137.219"
HOST_PORT = 8888
print("Starting socket: TCP...")

socket_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("TCP server listen @ %s:%d!" %(HOST_IP, HOST_PORT) )
host_addr = (HOST_IP, HOST_PORT)

socket_tcp.bind(host_addr)

socket_tcp.listen(1)

socket_con, (client_ip, client_port) = socket_tcp.accept()
print("Connection accepted from %s." %client_ip)
#socket_con.send("Welcome to RPi TCP server!".encode())

ser = serial.Serial('/dev/ttyUSB0',115200,timeout = 0.1)

close_flag = 0

def ser_read():
    while True:
        try:
            if ser.isOpen() == False:
                ser.open()
            msg = ser.read(26)
            if len(msg)>0:
                socket_con.send(msg)
                print(msg)
        except Exception:
            print('thread close')
            thread_read.stop()
            socket_tcp.close()
            sys.exit()
        if close_flag:
            thread_read.stop()

thread_read = threading.Thread(target=ser_read)
thread_read.start()


#GPIO.setwarnings(Flase)
#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(11,GPIO.OUT)
print("Receiving package...")
while True:
    try:
        data=socket_con.recv(512)
        print(type(data))
        print(len(data))
        if len(data) > 0:
            print("Received:%s" % data)
            #if data == '1':
                #GPIO.output(11, GPIO.HIGH)
            #elif data == '0':
                #GPIO.output(11, GPIO.LOW)
            #socket_con.send(data)
            print(data)
            ser.write(data)
           # time.sleep(1)
            continue
    except Exception:
        print('I close')
        close_flag = 1
        socket_tcp.close()
        sys.exit(1)
