#!/usr/bin/env python3
# Feb 3, 2021 11:30
# abstract_comm.py

from abc import ABC, abstractmethod
import os
import sys

# add our own custom modules to the lib path
# you'll also need an empty __init__.py file
# in the same directory

my_modules_path = os.getcwd()
if sys.path[0] != my_modules_path:
    sys.path.insert(0, os.getcwd())

from debug_output import *
import time
import socket
import errno
import threading
import atari_chars

class AbstractComm(ABC):
    def __init__(self, debug_status = False):
        self.screen_changed = False
        self.connect_state = False
        self.desired_connect_state = self.connect_state
        self.last_screen = b''
        self.data_to_send = False
        self.send_data = b''
        self.comm_thread_id = None
        self.error = False
        self.comm_running = False
        self.gui_set_connect_state = None
        self.set_gui_connect_state_event = None
        self.gui_update_display = None
        self.gui_get_hostname = None
        self.gui_display_error = None
        self.hostname = ''
        self.errorStr = ''

        
        if (debug_status):
            self.debug = debug_output
        else:
            self.debug = debug_none
                    
        self.ac = atari_chars.AtariChars()

        super().__init__()
        

    @abstractmethod
    def get_module_name(self):
        return "FUJI80";
    
    def new_screen(self):
        self.debug("")
        return self.screen_changed
    
    def set_gui_connect_state(self, function):
        self.debug(f"function={function.__name__}")
        self.gui_set_connect_state = function
        
    def gui_connect_state_event(self, state):
        self.debug(f"comm.gui_connect_state: {state}")
        if (self.gui_set_connect_state != None):
            self.gui_set_connect_state(state)
            
    def set_hostname(self, hostname_in):
        self.debug(f"hostname: '{hostname_in}'")
        self.hostname = hostname_in
        
    @abstractmethod
    def comm_thread(self):
        pass        

    def start(self):
        self.debug("")
        self.error = False
        #x = threading.Thread(target=self.display_thread, args=(self,))
        self.comm_thread_id = threading.Thread(target=self.comm_thread)
        self.comm_thread_id.start()


    def stop(self):
        self.debug("")
        self.error = False
        self.connect = False
        self.comm_running = False
        
        if self.comm_thread_id != None:
            while self.comm_thread_id.is_alive():
                time.sleep(0.10)
            
            self.debug(">>COMM: comm thread stopped")
             
    def set_connect_state_event(self, state):
        #self.debug(f">>SETTING COMM: state: {state}")
        self.desired_connect_state = state
         
    def get_connect_state_event(self):
        #self.debug(f"<<GETTING COMM: self.desired_connect_state {self.desired_connect_state}")
        return self.desired_connect_state

    
    def set_get_hostname(self, function):
        self.debug("")
        self.gui_get_hostname = function
    
    def send_data_event(self, data, ascii_flag):
        
        if not self.connect_state:
            return
        
        timeout = 10
        while self.data_to_send and (timeout > 0):
            time.sleep(0.1)
            timeout = timeout - 1
            
        if timeout == 0:
            self.debug(">>COMM: timeout")
            return
        
        if (ascii_flag):
            self.send_data = self.ac.convert_ascii_to_atascii(data)
        else:
            self.send_data = self.data
        self.data_to_send = True

if __name__ == "__main__":

    print("You're in abstract_comm.py")
