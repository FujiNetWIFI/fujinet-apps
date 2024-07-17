.include "atari.inc"

.export _get_key_if_available


.code

; inputs: none
; outputs: X = 0 if key pressed, 128 otherwise
;          A contains ASCII value of key just pressed
_get_key_if_available:
  lda BRKKEY
  bne @no_abort
  dec BRKKEY
  lda #$03                      ; ^C
  bne @done                     ; jump always
@no_abort:
  lda HELPFG                    ; HELP key? (to be 100% correct we should check for XL-type machine before...)
  cmp #17                       ; HELP alone
  beq @help
  cmp #81                       ; Shift-HELP
  beq @help
  cmp #145                      ; Control-HELP
  bne @no_help
@help:
  lda #0
  sta HELPFG                    ; clear HELPFG
  lda #$fc                      ; pseudo key code, handled in cater.s
  bne @done
@no_help:
  lda CH                        ; keyboard input (scan code)
  cmp #255                      ; was a key pressed?
  beq nokey                     ; 255 means "no"
  bpl @call_k                   ; Ctrl wasn't pressed, continue with ROM
  jsr handle_special_keys
  bcs @done                     ; keypress was handled
@call_k:
  lda #0
  sta INVFLG                    ; never input an inverse char
  ; ideas from cc65's cgetc.s
  lda #12
  sta ICAX1Z
  jsr getkey_k
@done:
  ldx #255                      ; if K: handler hasn't been called, "consume" the key press
  stx CH                        ; and clear the zero flag which is a return value
  ldx #0
  rts
nokey:
  ldx #128
  rts

; inputs: A - keycode (CH)
; outputs: A - char, CF=0 if regular ROM handling is needed
handle_special_keys:
  ldx #special_key_table_len-1
@lookup:
  cmp special_key_table,x
  beq @found
  dex
  bpl @lookup
  clc
  rts
@found:
  lda special_key_table2,x
  sec
  rts

; call "short-cut" handler of K:
getkey_k:
  lda KEYBDV+5
  pha
  lda KEYBDV+4
  pha
  rts


.data

; keyboard translation tables for combinations not handled or supported by the ROM

; scan codes
special_key_table:
  .byte $b5     ; Ctrl-8
  .byte $f5     ; Shift-Ctrl-8
  .byte $a0     ; Ctrl-,
  .byte $e0     ; Shift-Ctrl-,
  .byte $a6     ; Ctrl-/
  .byte $e6     ; Shift-Ctrl-/
  .byte $a2     ; Ctrl-.
  .byte $e2     ; Shift-Ctrl-.
  .byte $b3     ; Ctrl-7
  .byte $f3     ; Shift-Ctrl-7
  .byte $ce     ; Shift-Ctrl--
  .byte $a1     ; Ctrl-<Space>
  .byte $e1     ; Shift-Ctrl-<Space>
  .byte $b0     ; Ctrl-9
  .byte $f0     ; Shift-Ctrl-9
  .byte $b2     ; Ctrl-0
  .byte $f2     ; Shirt-Ctrl-0
  .byte $ed     ; Shirt-Ctrl-T
  .byte $de     ; Shirt-Ctrl-2
special_key_table_len = * - special_key_table

; return values for scan codes
special_key_table2:
  .byte $00     ; Ctrl-8  ==>  Ctrl-@
  .byte $00     ; Shift-Ctrl-8  ==>  Ctrl-@
  .byte $1b     ; Ctrl-,  ==>  Ctrl-[
  .byte $1b     ; Shift-Ctrl-,  ==>  Ctrl-[
  .byte $9c     ; Ctrl-/  ==>  Ctrl-\          ($1c already used for cursor up, ATURW)
  .byte $9c     ; Shift-Ctrl-/  ==>  Ctrl-\
  .byte $9d     ; Ctrl-.  ==>  Ctrl-]          ($1d already used for cursor down, ATDRW)
  .byte $9d     ; Shift-Ctrl-.  ==>  Ctrl-]
  .byte $9e     ; Ctrl-7  ==>  Ctrl-^          ($1e already used for cursor left, ATLRW)
  .byte $60     ; Shift-Ctrl-7  ==>  `
  .byte $9f     ; Shift-Ctrl--  ==>  Ctrl-_    ($1f already used for cursor right, ATRRW)
  .byte $00     ; Ctrl-<Space>  ==>  Ctrl-@
  .byte $00     ; Shift-Ctrl-<Space>  ==>  Ctrl-@
  .byte $7b     ; Ctrl-9  ==>  {
  .byte $7b     ; Shift-Ctrl-9  ==>  {
  .byte $fd     ; Ctrl-0  ==>  }
  .byte $fd     ; Shift-Ctrl-0  ==>  }
  .byte $9a     ; Shift-Ctrl-T  ==>  ~
  .byte $9a     ; Shift-Ctrl-2  ==>  ~
; translations $9c..$9f -> $1c..$1f, $9e -> $7e, and $fd -> $7d will be done in cater.s



;-- LICENSE FOR getkey.s --
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
