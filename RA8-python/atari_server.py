#!/usr/bin/env python3

import socket
import time
import errno

hostname = socket.gethostname()

local_ip = socket.gethostbyname(hostname)

HOST = local_ip    # Standard loopback interface address (localhost)
PORT = 6502        # Port to listen on (non-privileged ports are > 1023)

print("Echo Atari Server Started as: "+local_ip)
screen_to_ascii = list ('?'*256)
ascii_to_screen = list ('?'*256)

screen = 0
for ascii in range(ord(' '), ord('_')+2):
    screen_to_ascii[screen] = chr(ascii)
    screen += 1

screen = 97
for ascii in range(ord('a'), ord('z')+2):
    screen_to_ascii[screen] = chr(ascii)
    screen += 1


datachar = 61


#print(data)
every30 = 0
while(True):
    try:
    
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST, PORT))
            s.listen()
            conn, addr = s.accept()
            conn.setblocking(False)
            with conn:
                print('Connected by', addr)
                #conn.timeout(2)
                sendTimer = 5
                while True:
                    print('.', end='')
                    sendTimer -= 1
                    if sendTimer <=0:
                        print()
                        sendTimer = 30
                        try:
                            data = chr(datachar)*960
                            datachar += 1
                            if (datachar > 61+26):
                                datachar = 61
                            conn.sendall(data.encode())
                            print(data)
                        except Exception as e:
                            err = e.args[0]
                            if not (err in [errno.EAGAIN, errno.EWOULDBLOCK]):
                                print("send err:" + str(e))
                        print(" "*29, end='')
                        print("|")
                    time.sleep(1)
                    
                    try:
                        data = conn.recv(1024).decode()
                    except Exception as e:
                        err = e.args[0]
                        if not (err in [errno.EAGAIN, errno.EWOULDBLOCK]):
                            print("recv error:"+str(e))
                            break

                    
                        
    except Exception as e:
        print(str(e))
        print("Connection closed")
        conn.close()
        
print("Echo Server Closed")

