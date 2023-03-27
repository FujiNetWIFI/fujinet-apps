#!/usr/bin/env python3
# Feb 3, 2021 11:30
# fuji80_comm.py

import os
import sys

# add our own custom modules to the lib path
# you'll also need an empty __init__.py file
# in the same directory

my_modules_path = os.getcwd()
if sys.path[0] != my_modules_path:
    sys.path.insert(0, os.getcwd())

from abstract_comm import *

import atari_chars

class Comm(AbstractComm):
           
    def get_module_name(self):
        return "FUJI80";
            
    def comm_thread(self):
        
        self.debug("started")
        self.comm_running = True
        self.desired_connect_state = False

        PORT = 6520        # The port used by the server
        while(self.comm_running):

            self.error = False
            if self.desired_connect_state:
                
                if self.gui_get_hostname == None:
                    self.debug(">>no hostname")
                    time.sleep(1)
                    continue
                
                self.hostname = self.gui_get_hostname()
                self.debug(f">>COMM: hostname: {self.hostname}:{PORT}")


                try:
                    server_ip = socket.gethostbyname(self.hostname)
                    HOST = server_ip;
                    self.debug(f">>COMM: Connecting to server at: {HOST}:{PORT}")
                except Exception as e:
                    self.debug(f">>COMM: Error {str(e)}")
                    self.errorStr = str(e)
                    self.error = True
            else:
                time.sleep(1)
                continue
            
            s = None
            connected = False
            
            while (self.desired_connect_state and not self.error and self.comm_running):
                while (not connected and not self.error and self.comm_running):
                    try:
                        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)          
                        s.connect((HOST,PORT))
                        s.setblocking(False)
                        connected = True
                        self.connect_state = True
                        self.gui_connect_state_event(True)                
                    except Exception as e:
                        err = e.args[0]
                        if not (err in [errno.EAGAIN, errno.EWOULDBLOCK]):
                            self.debug(">>COMM: not connected: " + str(e))
                            self.error = True
                            self.errorStr = str(e)

                if (self.error):
                    break
                
                firstTime = True

                data = b''
                while (self.connect_state and not self.error and self.comm_running):
                    if (firstTime):
                        firstTime = False
                        self.debug(">>COMM: Connected to Server")

                    # ***************
                    # ** RECEIVING **
                    # ***************
                    try:
                        data = b''
                        data = s.recv(1024)
                    except socket.timeout:
                        self.debug(">>COMM: timeout")
                        pass
                    except Exception as e:
                        if e.errno == 10054:
                            self.connect_state = False
                            self.error = True
                            self.errorStr = str(e)
                            break;
                        if not (e.errno in [errno.EAGAIN, errno.EWOULDBLOCK]):
                            self.debug(">>COMM: recv error: "+str(e))
                            self.error = True
                            self.errorStr = str(e)
                            break

                    # ***************
                    # *** SENDING ***
                    # ***************

                    try:
                        if self.data_to_send:
                            hexbig = '' 
                            for i in range(len(self.send_data)):
                                shorthex = hex(ord(self.send_data[i]))[2:]
                                hexbig += shorthex
                            s.send(bytearray.fromhex(hexbig))
                            self.data_to_send = False                       
                    except Exception as e:
                        self.debug(">>COMM: send error:DOS " +str(e))
                        self.error = True
                        self.errorStr = str(e)
                        break

                    if (len(data) == 0):
                        continue
                    
                    # *********************
                    # ** SEND TO DISPLAY **
                    # *********************                    
                    
                    if self.gui_update_display != None:
                        #print(data)
                        self.gui_update_display(data, atari_chars.DATA_ATASCII, False)
                        data = b''
                    else:
                        print("No display!")
                    
            try:
                if (s != None):
                    self.debug(">>COMM: Client Closed")
                    s.close()
            except:
                pass
                
            if (self.error):
                self.desired_connect_state = False
                if self.gui_set_connect_state:
                    self.gui_set_connect_state(self.desired_connect_state)
                if self.gui_display_error:
                    self.gui_display_error(self.errorStr)
                

            time.sleep(1)
        
        self.debug(">>****************************")
        self.debug(f">>FUJI80COMM: thread stopped self.comm_running: {self.comm_running}")
        self.debug(">>****************************")

if __name__ == "__main__":

    print("You're in fuji80_comm.py")
