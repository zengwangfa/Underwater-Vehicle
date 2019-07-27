
import socket
import time
import sys
import RPi.GPIO as GPIO
import serial
import threading

ser = serial.Serial('/dev/ttyUSB0',115200,timeout = 0.1)

raspberry_start_on_list =\
[0xAA,0x55,0x10,  0x00,0x00,   0x00,0x00,0x00,0x00,0x00,   0x00,0x00,0x00,0x00,    0x01,        0x00,0x00,0x00,  0x02,       0x12]  
	#包头          标志位           摇杆控制位              设备控制位      树莓派开机位        预留位        启动停止位 累加校验和
      
ser.write(raspberry_start_on_list) #向MCU发送 pi已经开机指示
print("Pi Start on!") 

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
