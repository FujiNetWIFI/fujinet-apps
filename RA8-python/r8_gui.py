#!/usr/bin/env python3
# Feb 3, 2021 11:30
# atari_gui.py

import os
import sys

# add our own custom modules to the lib path
# you'll also need an empty __init__.py file
# in the same directory

my_modules_path = os.getcwd()
if sys.path[0] != my_modules_path:
    sys.path.insert(0, os.getcwd())

import tkinter as tk
from tkinter import ttk
import threading
import time
import clipboard

import ra8_comm
import fuji80_comm
import atari_chars

from debug_output import *

def donothing():
    pass

class AtariGui():

    def __init__(self, debug_status=False):
        self.version = "2021-01-29 10:13"
        self.txt_host_display  = None
        self.txt_host_commands = None
        self.send_upper        = None
        self.default_host      ='Not set'
        self.set_desired_connect_state = None
        self.btn_connect       = None
        self.display_thread_id = None
        self.connect_state     = False
        self.desired_connect_state = False
        self.combo_host        = None
        self.hostname          = ""
        self.data              = ''
        self.display_running   = True
        self.gui               = None
        self.comm              = None
        self.get_connect_state = None
        self.send_data_event   = None
        self.comm_module       = "Unknown"
        self.atari_screen      = "READY"
        self.hostnames         = []
        self.connect_string    = "Connect"
        self.connected_string  = "Connected.\n"
        self.disconnect_string = "Disconnect"
        self.disconnected_string = "Disconnected.\n"
        self.ac                = atari_chars.AtariChars()
        self.next_comm_package = False
        self.backend_thread_id = None
        self.frm_commands      = None
        self.ra8               = "RA8"
        self.fuji80            = "FUJI80"
        self.last_char         = ' '
        
        self.atascii_clear_screen = ''
        self.atascii_blank_row    = ''
        
        max_x = 80
        max_y = 24
        for y in range(max_y):
            for x in range(max_x):
                self.atascii_clear_screen += atari_chars.ATASCII_UNICODE_SPACE
                
                
        for x in range(max_x):
            self.atascii_blank_row += atari_chars.ATASCII_UNICODE_SPACE        
        
        self.atascii_screen       = list(self.atascii_clear_screen)
        self.cursor = [0, 0]
              
        if (debug_status):
            print("Full debug")
            self.debug = debug_output
        else:
            self.debug = debug_none
        
        self.gui = None
        

    def paste(self):
        text = clipboard.paste()
        print("in clipboard:")
        print(text)
        text = text.replace('\n', '\r')
        comm.send_data_event(text, True)
        
        
    def switch_comm_package(self):
        self.next_comm_package = True
        self.gui.destroy()
        
    def set_hostnames(self, hostnames_in):
        self.hostnames = hostnames_in
        
    def get_hostnames(self):
        self.debug(f"hostnames: {self.hostnames}")
        return self.hostnames

    def set_default_host(self, hostname_in):
        self.debug(f"hostname_in: {hostname_in}")
        self.hostname = hostname_in
        self.default_host = hostname_in
        
    def get_hostname(self):
        return self.hostname
    
    def set_hostname(self, hostname_in):
        self.debug(f"newhostname: {hostname_in}")
        self.hostname = hostname_in
    
    # -------------------------------------------------------
    # -------------------------------------------------------
    # -------------------------------------------------------
    # UPDATE DISPLAY
    # -------------------------------------------------------
    # -------------------------------------------------------
    # -------------------------------------------------------
     
    def update_remote_display(self, display, data_type, erase):
        #self.debug(f"update_remote_display")
        
        #print(display)
        raw = bytes(display)
        chars_per_line = 0
        no_esc = False
        atascii_unicode = ''
        
        max_y = self.desired_height.get()
        max_x = self.desired_width.get()

        #------------------------------------------
        #------------------DATA_ATASCII------------
        #------------------------------------------
        if data_type   == atari_chars.DATA_ATASCII:
           
            atascii_data = ''            
            hexbig = raw.hex()
            #print(hexbig)

            for i in range(0, len(hexbig),2):
                hexshort = hexbig[i:i+2]
                #print (hexshort + " ", end='')
                try:
                    atascii = int(hexshort, 16)
                except Exception as e:
                    atascii = '?'
                    print(str(e))
                    
                if type(atascii) == int:
                    atascii_data += chr(atascii)
                else:
                    atascii_data += atascii
                
            atascii_unicode = self.ac.convert_atascii_to_unicode(atascii_data)
            #print(f"return = {ord(atari_chars.ATASCII_UNICODE_RETURN)}")            

        #------------------------------------------
        #------------------DATA_SCREEN------------
        #------------------------------------------
        elif data_type == atari_chars.DATA_SCREEN:
            
            screen_data =''
            hexbig = raw.hex()                      
            for i in range(0, len(hexbig),2):
                hexshort = hexbig[i:i+2]
                #print (hexshort + " ", end='')
                screen_byte = int(hexshort, 16)
                screen_data += chr(screen_byte)
                
            self.cursor = [0, 0]        
            atascii_unicode = self.ac.convert_screen_to_unicode(screen_data)
            
            
        #------------------------------------------
        #------------------DATA_ASCII--------------
        #------------------------------------------    
        elif data_type == atari_chars.DATA_ASCII:
              
            atascii_unicode = self.ac.convert_ascii_to_unicode( display.decode() )
            
        else:
            pass
            
            print("unknown data screen")

        # ************************************************************
        # ************************************************************
        # ************************************************************
        # Format incoming data
        # ************************************************************
        # ************************************************************
        # ************************************************************


        if self.comm_module == self.ra8:
            self.atascii_screen = list(atascii_unicode)
                    
        elif self.comm_module == self.fuji80:
            #print("fuji80")
            x = self.cursor[0]
            y = self.cursor[1]
            

            esc_next_char = False
            for i in range(len(atascii_unicode)):

                last_char_was_esc = (self.last_char == atari_chars.ATASCII_UNICODE_ESC)
                #print(f"last_char_was_esc: {last_char_was_esc}")

                char = atascii_unicode[i]
                #print(f"char: {char}")
                
                if self.comm_module == self.ra8:
                    esc_next_char = True
                else:
                    if not last_char_was_esc:
                        if char == atari_chars.ATASCII_UNICODE_ESC:
                            #print("ESC!")
                            self.last_char = char
                            continue
                        else:
                            esc_next_char = False
                    else:
                        esc_next_char = True

                    
                    if not esc_next_char:
                        #print("not esc before")
                        last_char_was_esc = False
                        esc_next_char = False
                        
                        # special keys here
                        
                        if char == atari_chars.ATASCII_UNICODE_CLR_SCR:
                            self.cursor = [0, 0]
                            x = 0
                            y = 0
                            self.atascii_screen = list(self.atascii_clear_screen)
                            continue
                        
                        if char == atari_chars.ATASCII_UNICODE_UP:
                            #print("up")
                            y -= 1
                            if y < 0:
                                y = max_y - 1
                            cusor = [x,y]
                            continue

                        elif char == atari_chars.ATASCII_UNICODE_DOWN:
                            #print("down")

                            y += 1
                            if y >= max_y:
                                y = 0
                            cusor = [x,y]
                            continue

                        elif char == atari_chars.ATASCII_UNICODE_LEFT:
                            #print("left")

                            x -= 1
                            if x < 0:
                                x = max_x - 1
                            cusor = [x,y]
                            continue
                        
                        elif char == atari_chars.ATASCII_UNICODE_RIGHT:
                            #print("right")
                            x += 1
                            if x >=  max_x:
                                x = 0
                            cusor = [x,y]
                            continue
                        
                        elif char == atari_chars.ATASCII_UNICODE_RETURN:
                            #print("return")
                            x  = 0
                            y += 1
                                    
                            if y >= max_y:
                                y = max_y - 1

                                # convert list to string
                                self.atascii_screen = "".join(self.atascii_screen)
                                # delete top row
                                self.atascii_screen = self.atascii_screen[max_x-1:]
                                # add blank line
                                self.atascii_screen += self.atascii_blank_row
                                #convert back to list
                                self.atascii_screen  = list(self.atascii_screen)
                            cusor = (x,y)
                            continue
                    
                    last_char = char
                    
                    self.atascii_screen[x+y*max_x] = atascii_unicode[i]
                    x += 1
                            
                    if x >= max_x:
                        x  = 0
                        y += 1
                                           
                        if y >= max_y:
                            y = max_y - 1
                            # convert list to string
                            self.atascii_screen = "".join(self.atascii_screen)
                            # delete top row
                            self.atascii_screen = self.atascii_screen[max_x-1:]
                            # add blank line
                            self.atascii_screen += self.atascii_blank_row
                            #convert back to list
                            self.atascii_screen  = list(self.atascii_screen)

                            
                    self.cursor = [x, y]
        
        
        
        if self.txt_host_display != None:

            screen = "".join(self.atascii_screen)
                
            self.txt_host_display.configure(state="normal")
               
            self.txt_host_display.delete('1.0',tk.END)                            
            self.txt_host_display.insert(tk.END, screen)
            
            if self.comm_module == self.ra8:
                self.txt_host_display.configure(state="disabled")
                self.ent_host_commands.focus_set()
            
        else:
            self.debug("txt_host_display is None")
            
    # -------------------------------------------------------
    # -------------------------------------------------------
    # -------------------------------------------------------
 
    def send_break(self):
        if self.send_data_event != None:
            self.send_data_event("{BREAK}", False)
 
    def set_send_data_event(self, function):
        self.debug(f"function: {function.__name__}")
        self.send_data_event = function

    def send_call_back(self):
        self.debug("")
        if self.send_data_event != None:
            self.send_data_event(self.data.get())
        
    def set_connect_state(self, desired_connect_state):
        self.debug(f"connect_state: {desired_connect_state}")

        self.desired_connect_state = desired_connect_state
        if self.get_connect_state != None:
            self.connected = self.get_connect_state()

        if self.btn_connect != None:
            if self.connected:
                if self.combo_host != None:
                    self.hostname = self.combo_host.get()
                    self.hostname = self.hostname.strip()
                else:
                    return
                if len(self.hostname) > 0:
                    if not self.hostname in self.hostnames:
                        self.hostnames.insert(0, self.hostname)
                    else:
                        # Remove all the same hostname from the list
                        while(True):
                            try:
                                self.hostnames.remove(self.hostname)
                            except:
                                break
                        self.hostnames.insert(0, self.hostname.strip())
                    
                    #self.combo_host.configure(values = self.hostnames)
                    self.all_entry('enabled')
                    self.btn_connect['text'] = self.disconnect_string
                    self.btn_connect.configure(bg = "Red")
                    self.combo_host.configure(state = "disabled")

                    if self.comm_module == self.fuji80:
                        self.txt_host_display.configure(state="normal")
                        self.ent_host_commands.grid_remove()
                        self.txt_host_display.focus_set()
                    
                    if self.comm_module == self.ra8:
                        self.txt_host_display.configure(state="disabled")
                        self.ent_host_commands.grid()
                        self.ent_host_commands.focus_set()
                
                    if self.txt_host_display != None:
                        self.txt_host_display.delete('1.0',tk.END)
                        self.atari_screen = self.connected_string
                        self.txt_host_display.insert('1.0', self.atari_screen)
                        self.txt_host_display.focus_set()
            else:
                self.all_entry('normal')
                self.btn_connect['text'] = self.connect_string
                self.btn_connect.configure(bg = "Green")
                self.combo_host.configure(state = "normal")

                if self.comm_module == self.fuji80:
                    if self.txt_host_display != None:   
                        self.txt_host_display.configure(state="normal")
                        self.txt_host_display.focus_set()

                
                if self.comm_module == self.ra8:
                    if self.txt_host_display != None:   
                        self.txt_host_display.configure(state="disabled")
                    if self.frm_commands != None:
                        self.ent_host_commands.focus_set()
                
                if self.txt_host_display != None:                    
                    self.txt_host_display.delete('1.0',tk.END)
                    self.atari_screen = self.disconnected_string
                    self.txt_host_display.insert('1.0', self.atari_screen)
                    
                    
    def connect_call_back(self):
        
        if self.get_connect_state != None:
            self.connected = self.get_connect_state()
            
            self.set_hostname(self.combo_host.get())
            
            self.desired_connect_state = not self.connected
            self.debug(f">>****new state: desired state: {self.desired_connect_state} connected {self.connected}")

    def reboot_from_basic_callback(self):
        self.debug("")
        comm.send_data_event("POKE 580,0\rX = USR(58487)\r", True)
        
    def reboot_from_dos_callback(self):
        self.debug("")
        comm.send_data_event("\rDOS\rREBOOT.COM\r", True)
    
    def running(self):
        #self.debug(f"<< display_running: {self.display_running}")
        return self.display_running

    def all_entry(self, new_state):
        self.debug(f"new_state: {new_state}")
        #self.txt_host_display.config(state=new_state)
        #self.txt_host_commands.config(state=new_state)
        pass
    
    def generic_key_handler(self, event):
        #print(f"event.char:{event.char}, event.keysym: {event.keysym}, event.keycode: {event.keycode}")
        
        comm_char = None
        
        if len(event.char) > 0:
            comm_char = event.char
            
        if str(event.keysym) == "Return":
            comm_char = chr(13)
            
        if str(event.keysym) == "BackSpace":
            comm_char = chr(8)
            
        if str(event.keysym) == "Up":
            comm_char = chr(224) + atari_chars.ASCII_TERM_UP
            
        if str(event.keysym) == "Down":
            comm_char = chr(224) + atari_chars.ASCII_TERM_DOWN
            
        if str(event.keysym) == "Right":
            comm_char = chr(224) + atari_chars.ASCII_TERM_RIGHT
            
        if str(event.keysym) == "Left":
            comm_char = chr(224) + atari_chars.ASCII_TERM_LEFT
            
        return comm_char
    


    def fuji80_key_handler(self, event):

        comm_char = self.generic_key_handler(event)
            
            
        if comm_char == chr(224) + atari_chars.ASCII_TERM_UP:
            self.update_remote_display(atari_chars.ATASCII_UNICODE_UP.encode(), atari_chars.DATA_ATASCII, False)
            
        if comm_char == chr(224) + atari_chars.ASCII_TERM_DOWN:
            self.update_remote_display(atari_chars.ATASCII_UNICODE_DOWN.encode(), atari_chars.DATA_ATASCII, False )
            
        if comm_char == chr(224) + atari_chars.ASCII_TERM_RIGHT:
            self.update_remote_display(atari_chars.ATASCII_UNICODE_RIGHT.encode(), atari_chars.DATA_ATASCII, False)
            
        if comm_char == chr(224) + atari_chars.ASCII_TERM_LEFT:
            self.update_remote_display(atari_chars.ATASCII_UNICODE_LEFT.encode(), atari_chars.DATA_ATASCII, False)
            
        data = self.txt_host_display.get(1.0, tk.END)
        
        if data == self.connected_string+"\n":
            data = ' '
            self.txt_host_display.delete('1.0',tk.END)
            self.txt_host_display.insert('1.0', data)

        # remove last character in command field
        data = data[:-1]
        self.txt_host_display.delete('1.0',tk.END)
        self.txt_host_display.insert('1.0', data)

        if comm_char != None:
            if (self.send_upper_state.get() == 1):
                comm_char = comm_char.upper()
                
            comm.send_data_event(comm_char, True)

    def ra8_key_handler(self, event):
        
        comm_char = self.generic_key_handler(event)
        
        if str(event.keysym) == "Return":
            data = self.ent_host_commands.get()
            data += '\r'
            self.ent_host_commands.delete(0,tk.END)
            if (self.send_upper_state.get() == 1):
                data = data.upper()
            comm.send_data_event(data, True)
        
    def send_host_commands(self):
        data = self.txt_host_commands.get(1.0, tk.END)
        data = data.replace('\n','\r')
        self.txt_host_commands.delete('1.0', tk.END)
        comm.send_data_event(data, True)

    def display_error(self, error):
        self.set_connect_state(False)
        tk.messagebox.showerror("Error", error)
        self.set_connect_state(False)
    
    def set_terminal_attributes(self):
        self.desired_height.set(24)
        self.bg_color            = "black"
        self.fg_color            = "white"
        self.terminalFont        = "Atari Classic"
        desired_width = self.desired_width.get()
        desired_height= self.desired_height.get()
        
        if   desired_width == 80:
            self.terminalFontSize    = 10
        elif desired_width == 160:
            self.terminalFontSize    = 8
        elif desired_width == 40:
            self.terminalFontSize    = 14
        self.terminalFontWeight  = "normal"
        
        if self.txt_host_display != None:
            self.txt_host_display.config(width  = desired_width)
            self.txt_host_display.config(height = desired_height)
            self.txt_host_display.config(font   = (self.terminalFont, self.terminalFontSize, self.terminalFontWeight))
            self.gui.update()
            
            
    def create_display(self):
        self.debug("")
        self.display_running = True
        
        self.send_upper_state = tk.IntVar()
        self.send_upper_state.set(1)
           
        self.gui.title(self.comm_module + " - Python Client " + self.version)
        
        self.desired_width  = tk.IntVar()
        self.desired_height = tk.IntVar()        
        

        self.desired_height.set(24)
        self.desired_width.set(80)
        #==================================================
             
        menubar  = tk.Menu(self.gui)
        filemenu = tk.Menu(menubar, tearoff = 0)
        filemenu.add_command(label = "Exit", command = self.gui.quit)
        menubar.add_cascade(label = "File", menu = filemenu)
        
        editmenu = tk.Menu(menubar, tearoff=0)
        editmenu.add_command(label = "Copy", command = donothing)
        editmenu.add_command(label = "Paste", command = self.paste)
        editmenu.add_command(label = "Select All", command = donothing)
        menubar.add_cascade(label  = "Edit", menu = editmenu)

        termmenu = tk.Menu(menubar, tearoff=0)
        termmenu.add_checkbutton(label = "40",  onvalue=40,  offvalue=0, variable=self.desired_width, command=self.set_terminal_attributes)
        termmenu.add_checkbutton(label = "80",  onvalue=80,  offvalue=0, variable=self.desired_width, command=self.set_terminal_attributes)
        termmenu.add_checkbutton(label = "160", onvalue=160, offvalue=0, variable=self.desired_width, command=self.set_terminal_attributes)
        termmenu.add_separator()
        termmenu.add_command(label     = "XEP-80", command = donothing)
        termmenu.add_command(label     = "ADM-3A", command = donothing)
        menubar.add_cascade(label      = "View", menu = termmenu)

        helpmenu = tk.Menu(menubar, tearoff=0)
        helpmenu.add_command(label     = "About...", command = donothing)
        menubar.add_cascade(label      = "Help", menu = helpmenu)


        self.gui.config(menu = menubar)
        #==================================================
        # FRAME 1 - BEGIN - top
        self.frm_host = tk.Frame(relief=tk.GROOVE, borderwidth=1)
        self.frm_host.grid(row=0, column=0, padx=2)

        #==================================================

        btn_switch = tk.Button(master=self.frm_host,
                               text="Switch Application",
                               command=self.switch_comm_package)
        btn_switch.grid(row=0, column=0, padx=2)
        
        lbl_host = tk.Label(master=self.frm_host, text="HOST: ")
        lbl_host.grid(row=0, column=1, padx=2)

        self.combo_host = ttk.Combobox( master=self.frm_host,
                                        values = self.hostnames)
        self.combo_host.set(self.default_host)
        
        self.combo_host.grid(row=0, column=2, padx=2)

        #==================================================

        self.btn_connect = tk.Button(master=self.frm_host,
                                     text="Connect", bg = "red",
                                     command=self.connect_call_back)
        self.btn_connect.grid(row=0, column=3, padx=2)
        
        btn_break = tk.Button(master=self.frm_host,
                              text="BREAK",
                              command=self.send_break)
        btn_break.grid(row=0, column=4, padx=2)
        #==================================================
        #==================================================

        self.connected = False # Call before will change this to diconnected
        self.set_connect_state(self.connected)

        #==================================================
        
        # FRAME 2 - BEGIN - bottom
        frm_display = tk.Frame(relief=tk.GROOVE)
        frm_display.grid(row=1,column=0, padx=2)
        #

        if self.comm.get_module_name() == self.fuji80:
            self.set_terminal_attributes()
        else:
            self.desired_width.set(40)
            self.desired_height.set(24)
            self.bg_color            = "blue"
            self.fg_color            = "white"
            self.terminalFont        = "Atari Classic"
            self.terminalFontSize    = 14
            self.terminalFontWeight  = "normal"

        
        self.txt_host_display = tk.Text(master=frm_display, bg=self.bg_color, fg=self.fg_color,
                                   font=(self.terminalFont, self.terminalFontSize, self.terminalFontWeight),
                                   width=self.desired_width.get(),
                                   height=self.desired_height.get())
        self.txt_host_display.grid(row=1, column=0, padx=2)
        
        self.atari_screen = self.ac.convert_ascii_to_unicode(self.comm_module)  
        
        self.txt_host_display.delete('1.0', tk.END)
        self.txt_host_display.insert('1.0', self.atari_screen)
        
        #==================================================

        self.frm_commands = tk.Frame(master=frm_display)
        self.frm_commands.grid(row=2, column=0, padx=2)
        self.ent_host_commands = tk.Entry(master=self.frm_commands,
                                          font=(self.terminalFont, self.terminalFontSize, self.terminalFontWeight),
                                          width=self.desired_width.get())
        self.ent_host_commands.grid(row=0, column=0, padx=2)
        #==================================================
        
       
        #==================================================
        frm_buttons = tk.Frame(master=frm_display)
        frm_buttons.grid(row=4, column=0)

        self.chk_send_upper = tk.Checkbutton(master=frm_buttons,
                                             text='Uppercase',
                                             variable=self.send_upper_state,
                                             onvalue=1,
                                             offvalue=0)
        self.send_upper_state.set(1)
        self.chk_send_upper.grid(row=0, column=0, padx=2, sticky='w')
        
        btn_reboot = tk.Button(master=frm_buttons,
                               text="Reboot From BASIC",
                               command=self.reboot_from_basic_callback)
        btn_reboot.grid(row=1, column=0, padx=2)

        btn_reboot_dos = tk.Button(master=frm_buttons,
                                   text="Reboot From DOS",
                                   command=self.reboot_from_dos_callback)
        btn_reboot_dos.grid(row=1, column=1, padx=2)

        if self.comm_module == self.fuji80:
            self.txt_host_display.bind("<Key>", self.fuji80_key_handler)
            self.frm_commands.grid_remove()
        
        if self.comm_module == self.ra8:
            self.ent_host_commands.bind("<Key>", self.ra8_key_handler)
            self.frm_commands.grid()
        
        self.gui.mainloop()
        self.debug("GUI quit")
        
        self.comm.gui_get_hostname      = None
        self.comm.gui_set_connect_state = None
        self.comm.gui_update_display    = None
        self.comm.gui_display_error     = None
        
        del(self.send_upper)
        self.send_upper = None
        
        self.display_running = False
  
    def backend_thread(self, gui, comm):
        self.debug("")

        while (gui.running()):
            
            # Get the actual state of communications
            if (gui.get_connect_state != None):
                gui.connect_state = self.get_connect_state()
                #self.debug(f"<<gui.connect.state: {gui.connect_state}")

            # If we've had an error, disconnect
            if (comm.error):
                gui.desired_connect_state = gui.connect_state
                
            # If our desired state doesn't match our current state
            if (gui.desired_connect_state != gui.connect_state):

                if (gui.set_desired_connect_state != None):
                    
                    # If we're connecting, get the hostname
                    if gui.desired_connect_state:
                        self.debug(f"<<set _get_hostname: {gui.connect_state}")

                        _hostname = gui.get_hostname()
                        comm.set_hostname(_hostname)
                
                # change our state
                self.debug(f"changing state : desired: {gui.desired_connect_state} ")
                gui.set_desired_connect_state(gui.desired_connect_state)
                time.sleep(2)
                
                # get out actual state
                if (gui.get_connect_state != None):
                    gui.connect_state = self.get_connect_state()
                
                # make it our desired state
                gui.desired_connect_state = gui.connect_state
                gui.set_connect_state(gui.desired_connect_state)

                
                
            

        self.set_desired_connect_state  = None
        self.get_connect_state          = None
        self.send_data_event            = None

        self.connect_state              = False
        self.desired_connect_state      = False
        self.display_running            = False
        
        self.debug("GUI: backend thread stopped.")
        
    def start_backend(self):
        self.debug("")
        self.comm.start()
        
        self.backend_thread_id = threading.Thread(target=self.backend_thread, args=(self, self.comm,))
        self.backend_thread_id.start()
        
    def stop_backend(self):
        self.debug("")
        
        comm.gui_get_hostname      = None
        comm.gui_set_connect_state = None
        comm.gui_update_display    = None
        
        if self.comm != None:
            self.comm.stop()
                    
        if self.backend_thread_id != None:
            while self.backend_thread_id.is_alive():
                time.sleep(1)
                
            
    def stop(self):
        self.debug("")
        self.gui.quit()
        self.debug("quit sent")
        self.stop_backend()
        

if __name__ == "__main__":

    hostnames = []
    
    try:
        with open('hostnames.txt', 'r') as fhn:
            hostnames = fhn.readlines()
    except Exception as e:
        pass
    
    print(hostnames)
    
    #hostnames=[    "192.168.2.100", "FujiNet600XL", "FujiNet800XL","FujiNet400", "ARIEL-I7", "192.168.2.204", "192.168.2.175" ]
    comm_package = 1

    if len(hostnames) > 0:
        for i in range(len(hostnames)):

            hostname = hostnames[i].strip()
            
            if hostname == "":
                continue
            
            if (i == 0):
                last_comm_package = hostname
            else:
                hostnames[i] = hostname
        
        if len(hostnames) >= 2:
            hostname = hostnames[1]
        else:
            hostname = ""
        
        if len(hostnames) >= 1:
            hostnames.remove(hostnames[0])
        else:
            hostnames = []
            
        comm_package = int(last_comm_package)
    else:
        hostname = ""
    
    
    debug_state = True
    
    next_comm_package = True
    while next_comm_package:
        
        gui  = AtariGui(debug_status = debug_state)
        gui.gui = tk.Tk()

        if (comm_package > 2):
            comm_package = 1

        if (comm_package == 1):
            comm = ra8_comm.Comm(debug_status = debug_state)
        elif (comm_package == 2):
            comm = fuji80_comm.Comm(debug_status = debug_state)
            
        gui.comm = comm

        gui.comm_module = comm.get_module_name()
        
        gui.set_hostnames(hostnames)
        
        gui.set_default_host(hostname)
        gui.set_connect_state(False)
        
        gui.set_desired_connect_state   = comm.set_connect_state_event
        gui.get_connect_state      = comm.get_connect_state_event
        
        comm.gui_get_hostname      = gui.get_hostname
        comm.gui_set_connect_state = gui.set_connect_state
        comm.gui_update_display    = gui.update_remote_display
        comm.gui_display_error     = gui.display_error
        
            
        gui.start_backend()
        gui.create_display()
        
        hostnames = gui.get_hostnames()
        try:
            last_hostname = gui.get_hostname().strip()
            fhn = open('hostnames.txt', 'w')
            print(f"write: {str(comm_package)}")
            fhn.write(str(comm_package)+"\n")
            print(f"write<current>: {last_hostname}")
            fhn.write(last_hostname+"\n")
            for hostname in hostnames:
                hostname = hostname.strip()
                if (hostname == ""):
                    continue
                
                if (hostname != last_hostname):
                    fhn.write(hostname.strip()+"\n")
                    print(f"write: {hostname}")
            fhn.close()
            hostname = last_hostname
        except Exception as e:
            print(str(e))
            fhn.close()

        gui.stop()
        
        next_comm_package = gui.next_comm_package
        if gui.next_comm_package:
            comm_package += 1
            del(gui)
            gui=None
            del(comm)
            comm=None
            

    print("done")




