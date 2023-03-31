#!/usr/bin/env python3
# Feb 3, 2021 11:30
# atari_chars.py


            # special above 128
ATASCII_BACKSPACE   = chr(126)
ATASCII_TAB         = chr(127)
ATASCII_RETURN      = chr(155)
ATASCII_BELL        = chr(253)
ATASCII_CURSOR      = chr(32+128)
ATASCII_SPACE       = chr(32)
ATASCII_UP          = chr(28) 
ATASCII_DOWN        = chr(29) 
ATASCII_RIGHT       = chr(30) 
ATASCII_LEFT        = chr(31)
ATASCII_ESC         = chr(27)

ATASCII_CLR_SCR     = chr(125)
ATASCII_DEL_LINE    = chr(156)
ATASCII_INS_LINE    = chr(157)
ATASCII_CLR_TAB     = chr(158)
ATASCII_SET_TAB     = chr(159)
ATASCII_DEL         = chr(254)
ATASCII_INS         = chr(255)

ASCII_BACKSPACE     = chr(8)
ASCII_TAB           = chr(9)
ASCII_RETURN        = chr(13)
ASCII_BELL          = chr(7)
ASCII_CURSOR        = chr(219)
ASCII_ESC           = chr(27)

ASCII_TERM_ESC      = chr(224)
ASCII_TERM_UP       = "[1A"
ASCII_TERM_DOWN     = "[1B"
ASCII_TERM_RIGHT    = "[1C"
ASCII_TERM_LEFT     = "[1D"
 
DATA_ASCII          = 0
DATA_ATASCII        = 1
DATA_SCREEN         = 2


character_set = 0xE000

ATASCII_UNICODE_RETURN      = chr(ord(ATASCII_RETURN)   + character_set)
ATASCII_UNICODE_SPACE       = chr(ord(ATASCII_SPACE)    + character_set)
ATASCII_UNICODE_UP          = chr(ord(ATASCII_UP)       + character_set)
ATASCII_UNICODE_DOWN        = chr(ord(ATASCII_DOWN)     + character_set)
ATASCII_UNICODE_LEFT        = chr(ord(ATASCII_LEFT)     + character_set)
ATASCII_UNICODE_RIGHT       = chr(ord(ATASCII_RIGHT)    + character_set)
ATASCII_UNICODE_BACKSPACE   = chr(ord(ASCII_BACKSPACE)  + character_set)
ATASCII_UNICODE_TAB         = chr(ord(ATASCII_TAB)      + character_set)
ATASCII_UNICODE_CURSOR      = chr(ord(ATASCII_CURSOR)   + character_set)
ATASCII_UNICODE_ESC         = chr(ord(ATASCII_ESC)      + character_set)
ATASCII_UNICODE_CLR_SCR     = chr(ord(ATASCII_CLR_SCR)  + character_set)
ATASCII_UNICODE_DEL_LINE    = chr(ord(ATASCII_DEL_LINE) + character_set)
ATASCII_UNICODE_INS_LINE    = chr(ord(ATASCII_INS_LINE) + character_set)
ATASCII_UNICODE_CLR_TAB     = chr(ord(ATASCII_CLR_TAB)  + character_set)
ATASCII_UNICODE_SET_TAB     = chr(ord(ATASCII_SET_TAB)  + character_set)
ATASCII_UNICODE_DEL         = chr(ord(ATASCII_DEL)      + character_set)
ATASCII_UNICODE_INS         = chr(ord(ATASCII_INS)      + character_set)




class AtariChars:

    def __init__(self, international=False):
        global ATASCII_UNICODE_RETURN
        global ATASCII_UNICODE_SPACE
        global ATASCII_UNICODE_UP
        global ATASCII_UNICODE_DOWN
        global ATASCII_UNICODE_RIGHT
        global ATASCII_UNICODE_LEFT
        global ATASCII_UNICODE_BACKSPACE
        global ATASCII_UNICODE_TAB
        global ATASCII_UNICODE_CURSOR
        global ATASCII_UNICODE_ESC
        global ATASCII_UNICODE_CLR_SCR   
        global ATASCII_UNICODE_DEL_LINE   
        global ATASCII_UNICODE_INS_LINE    
        global ATASCII_UNICODE_CLR_TAB    
        global ATASCII_UNICODE_SET_TAB    
        global ATASCII_UNICODE_DEL       
        global ATASCII_UNICODE_INS     
        
        self.screen_to_ascii  = []
        self.atascii_to_ascii = []
        self.ascii_to_atascii = []
        self.screen_to_atascii= []
        
        if international:
            self.character_set = 0xE100
        else:
            self.character_set = 0xE000
        
        ATASCII_UNICODE_RETURN      = chr(ord(ATASCII_RETURN)   + character_set)
        ATASCII_UNICODE_SPACE       = chr(ord(ATASCII_SPACE)    + character_set)
        ATASCII_UNICODE_UP          = chr(ord(ATASCII_UP)       + character_set)
        ATASCII_UNICODE_DOWN        = chr(ord(ATASCII_DOWN)     + character_set)
        ATASCII_UNICODE_LEFT        = chr(ord(ATASCII_LEFT)     + character_set)
        ATASCII_UNICODE_RIGHT       = chr(ord(ATASCII_RIGHT)    + character_set)
        ATASCII_UNICODE_BACKSPACE   = chr(ord(ASCII_BACKSPACE)  + character_set)
        ATASCII_UNICODE_TAB         = chr(ord(ATASCII_TAB)      + character_set)
        ATASCII_UNICODE_CURSOR      = chr(ord(ATASCII_CURSOR)   + character_set)
        ATASCII_UNICODE_ESC         = chr(ord(ATASCII_ESC)      + character_set)
        ATASCII_UNICODE_CLR_SCR     = chr(ord(ATASCII_CLR_SCR)  + character_set)
        ATASCII_UNICODE_DEL_LINE    = chr(ord(ATASCII_DEL_LINE) + character_set)
        ATASCII_UNICODE_INS_LINE    = chr(ord(ATASCII_INS_LINE) + character_set)
        ATASCII_UNICODE_CLR_TAB     = chr(ord(ATASCII_CLR_TAB)  + character_set)
        ATASCII_UNICODE_SET_TAB     = chr(ord(ATASCII_SET_TAB)  + character_set)
        ATASCII_UNICODE_DEL         = chr(ord(ATASCII_DEL)      + character_set)
        ATASCII_UNICODE_INS         = chr(ord(ATASCII_INS)      + character_set)

        
        self.translations(self.screen_to_ascii,
                          self.atascii_to_ascii,
                          self.ascii_to_atascii,
                          self.screen_to_atascii)

        
    def generic_unicode_translations(self, atascii_to_unicode):
        question = 0x003F
        atascii_to_unicode.clear()
                    
        for i in range(256):
            atascii_to_unicode.append(question)

        atascii_to_unicode[0] = 0x2665
        atascii_to_unicode[1] = 0x251C
        atascii_to_unicode[2] = 0x1FB87
        atascii_to_unicode[3] = 0x2518
        atascii_to_unicode[4] = 0x2524
        atascii_to_unicode[5] = 0x2510
        atascii_to_unicode[6] = 0x2571
        atascii_to_unicode[7] = 0x2572
        atascii_to_unicode[8] = 0x25E2
        atascii_to_unicode[9] = 0x2597
        
        atascii_to_unicode[10] = 0x25e3
        atascii_to_unicode[11] = 0x259D
        atascii_to_unicode[12] = 0x2598
        atascii_to_unicode[13] = 0x1FB82
        atascii_to_unicode[14] = 0x2582
        atascii_to_unicode[15] = 0x2596
        atascii_to_unicode[16] = 0x2663
        atascii_to_unicode[17] = 0x250C
        atascii_to_unicode[18] = 0x2500
        atascii_to_unicode[19] = 0x253C
        
        atascii_to_unicode[20] = 0x2022
        atascii_to_unicode[21] = 0x2584
        atascii_to_unicode[22] = 0x258E
        atascii_to_unicode[23] = 0x252C
        atascii_to_unicode[24] = 0x2534
        atascii_to_unicode[25] = 0x258C
        atascii_to_unicode[26] = 0x2514
        atascii_to_unicode[27] = 0x001B
        atascii_to_unicode[28] = 0x2191
        atascii_to_unicode[29] = 0x2193

        atascii_to_unicode[30] = 0x2190
        atascii_to_unicode[31] = 2192
        
        c = 0x0020
        for atascii in range(32, 123):
            atascii_to_unicode[c] = atascii
            c += 1

        atascii_to_unicode[123] = 0x2660
        atascii_to_unicode[124] = 0x007C
        atascii_to_unicode[125] = 0x1F8B0
        atascii_to_unicode[126] = 0x25C0
        atascii_to_unicode[127] = 0x25B6
        
        atascii_to_unicode[130] = 0x25BA
        atascii_to_unicode[136] = 0x25E4
        atascii_to_unicode[137] = 0x259B
        atascii_to_unicode[138] = 0x25E5
        atascii_to_unicode[139] = 0x2599
        atascii_to_unicode[140] = 0x259F
        atascii_to_unicode[141] = 0x2586
        
        atascii_to_unicode[143] = 0x259C
        
        atascii_to_unicode[148] = 0x25D8
        atascii_to_unicode[149] = 0x2580

        atascii_to_unicode[153] = 0x2590
        
        atascii_to_unicode[155] = 0x000D
        
        atascii_to_unicode[160] = 0x2588
        
        atascii_to_unicode[253] = 0x0007
        atascii_to_unicode[254] = 0x007F

        return atascii_to_unicode

    def translations(self, screen_to_ascii, atascii_to_ascii, ascii_to_atascii, screen_to_atascii):
        question = '?'
        
        screen_to_ascii.clear()
        screen_to_atascii.clear()
        atascii_to_ascii.clear()
        ascii_to_atascii.clear()

        
        # dictionary unicode_to_atascii   
            
        for i in range(256):
            screen_to_ascii.append(question)
            screen_to_atascii.append(chr(0x3F))
            atascii_to_ascii.append(question)
            ascii_to_atascii.append(question)

            
        # -------------------------
        # SCREEN TO ASCII
        # -------------------------
        atascii = 0
        for ascii in range(ord(' '), ord('_')+2):
            screen_to_ascii[atascii] = chr(ascii)
            atascii += 1

        atascii = 97
        for ascii in range(ord('a'), ord('z')+2):
            screen_to_ascii[atascii] = chr(ascii)
            atascii += 1
            
        screen_to_ascii[ord(ATASCII_CURSOR)] = ASCII_CURSOR

        # -------------------------
        # SCREEN TO ATASCII
        # -------------------------

        atascii = 32
        for screen in range(0, 63+1):
            screen_to_atascii[screen] = chr(atascii)
            atascii += 1

        atascii = 0
        for screen in range(64, 95+1):
            screen_to_atascii[screen] = chr(atascii)
            atascii += 1

        for common in range(96, 127+1):
            screen_to_atascii[common] = chr(common)
            
        for inverse in range(128, 255+1):
            screen_to_atascii[inverse] = chr( ord(screen_to_atascii[inverse-128]) + 128 )

        # -------------------------
        # ATASCII TO ASCII
        # -------------------------
        for i in range(128):
            atascii_to_ascii[i] = chr(i) 

        for i in range(128):
            atascii_to_ascii[i + 128] = chr(ord(atascii_to_ascii[i]) + 128)

        # special above 128
        atascii_to_ascii[ord(ATASCII_BACKSPACE)] = ASCII_BACKSPACE  # BACKSPACE
        atascii_to_ascii[ord(ATASCII_TAB)]       = ASCII_TAB        # TAB
        atascii_to_ascii[ord(ATASCII_RETURN)]    = ASCII_RETURN     # RETURN
        atascii_to_ascii[ord(ATASCII_BELL)]      = ASCII_BELL       # BELL
        atascii_to_ascii[ord(ATASCII_CURSOR)]    = ASCII_CURSOR  

        # -------------------------
        # ATASCII TO ASCII
        # -------------------------

        for atascii in range(256):
            _ascii = ord(atascii_to_ascii[atascii])
            ascii_to_atascii[_ascii] = chr(atascii)
  

    def convert_ascii_to_unicode(self, data):
        
        atascii = ''
        skip = 0
        for i in range(len(data)):
            if skip > 0:
                skip = skip - 1
                continue
            
            if data[i] == ASCII_TERM_ESC:
                
                movement = data[i+1:i+4]
                
                if movement == ASCII_TERM_UP: # UP
                    atascii += ATASCII_UP
                
                if movement == ASCII_TERM_DOWN: # DOWN
                    atascii += ATASCII_DOWN
                    
                if movement == ASCII_TERM_RIGHT: # RIGHT
                    atascii += ATASCII_RIGHT
                    
                if movement == self.ASCII_TERM_LEFT: # LEFT
                    atascii += ATASCII_LEFT
                skip = 3;
            else:             
         
                atascii += chr( ord(self.ascii_to_atascii[ ord(data[i]) ] ) + self.character_set )
        
        return atascii

    def convert_ascii_to_atascii(self, data):
        
        atascii = ''
        skip = 0
        for i in range(len(data)):
            if skip > 0:
                skip = skip - 1
                continue
            
            if data[i] == ASCII_TERM_ESC:
                
                movement = data[i+1:i+4]
                
                if movement == ASCII_TERM_UP: # UP
                    atascii += ATASCII_UP
                
                if movement == ASCII_TERM_DOWN: # DOWN
                    atascii += ATASCII_DOWN
                    
                if movement == ASCII_TERM_RIGHT: # RIGHT
                    atascii += ATASCII_RIGHT
                    
                if movement == self.ASCII_TERM_LEFT: # LEFT
                    atascii += ATASCII_LEFT
                skip = 3;
            else:             
         
                atascii += self.ascii_to_atascii[ ord(data[i]) ] 
        
        return atascii

    def convert_atascii_to_ascii(self, data):
        
        data = data.replace(ATASCII_UP,    ASCII_TERM_ESC + ASCII_TERM_UP)
        data = data.replace(ATASCII_DOWN,  ASCII_TERM_ESC + ASCII_TERM_DOWN)
        data = data.replace(ATASCII_RIGHT, ASCII_TERM_ESC + ASCII_TERM_RIGHT)
        data = data.replace(ATASCII_LEFT,  ASCII_TERM_ESC + ASCII_TERM_LEFT)
        
        skip = 0
        ascii = ''
        for i in range(len(data)):
            if data[i] == ASCII_TERM_ESC:
                skip = 4
                 
            if (skip > 0):
                skip -= 1
                continue
             
            ascii += self.atascii_to_ascii[ord(data[i])]

        return ascii

    def convert_screen_to_ascii(self, data):
                
        ascii = ''
        for i in range(len(data)):
            ascii += self.screen_to_ascii[ord(data[i])]

        return ascii

    def convert_screen_to_unicode(self, data):
                
        atascii = ''
        for i in range(len(data)):
            _atascii = ord(self.screen_to_atascii[ord(data[i])])          
            atascii += chr( _atascii + self.character_set )

        return atascii

    def convert_atascii_to_unicode(self, data):
                
        atascii = ''
        for i in range(len(data)):
            atascii += chr( ord(data[i]) + self.character_set )

        return atascii


#Lucida Sans Unicode

if __name__ == "__main__":

    print("You're in atari_chars.py")




