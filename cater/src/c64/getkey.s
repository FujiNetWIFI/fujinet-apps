.export _get_key_if_available


.code

; use C64 Kernel ROM function to read a key
; inputs: none
; outputs: X = 0 if key pressed, 128 otherwise
;          A contains ASCII value of key just pressed
_get_key_if_available:
  ldy #$ff
  jsr $f142                     ; not officially documented - where F13E (GETIN) falls through to if device # is 0 (KEYBD)
  cpy #$ff                      ; Y gets modified iff there's a character available - this approach allows to read ^@ as 0
  beq no_key
  ldx #0
  rts
no_key:
  ldx #128
  rts



; -- LICENSE FOR getkey.s --
; The contents of this file are subject to the Mozilla Public License
; Version 1.1 (the "License"); you may not use this file except in
; compliance with the License. You may obtain a copy of the License at
; http://www.mozilla.org/MPL/
;
; Software distributed under the License is distributed on an "AS IS"
; basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
; License for the specific language governing rights and limitations
; under the License.
;
; The Original Code is ip65.
;
; The Initial Developer of the Original Code is Jonno Downes,
; jonno@jamtronix.com.
; Portions created by the Initial Developer are Copyright (C) 2009
; Jonno Downes. All Rights Reserved.
; -- LICENSE END --
