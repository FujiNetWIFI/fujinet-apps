;
;  hires.s
;  cc65 Chess
;
;  Created by Oliver Schmidt, January 2020.
;
;

	SECTION rodata
	EXPORT  _charset
	EXPORT  _charset_end
	EXPORT  _hires_Init
	EXPORT  _hires_Done
	EXPORT  _hires_Text
	EXPORT  _hires_Draw
	EXPORT  _hires_Mask
;.export _charset
;.export _hires_Init, _hires_Done, _hires_Text, _hires_Draw,  _hires_Mask

; .include "apple2.inc"
;' .include "zeropage.inc"

; .import popa, popax

; VERSION := $FBB3

; .rodata


; BASELO:
;     .repeat $C0, I
;     .byte   I & $08 << 4 | I & $C0 >> 1 | I & $C0 >> 3
;     .endrep
; 
; BASEHI:
;     .repeat $C0, I
;     .byte   >$2000 | I & $07 << 2 | I & $30 >> 4
;     .endrep

;; _hires_CharSet:
;; .incbin "charset.bin"

;; _hires_Pieces:
;; .incbin "pieces.bin"

; .code


_hires_Init

; TODO port to 6809        bit     $C082       ; Switch in ROM
; TODO port to 6809
; TODO port to 6809        lda     VERSION     ; Needs ROM
; TODO port to 6809        cmp     #$06        ; Apple //e ?
; TODO port to 6809        bne     :+
; TODO port to 6809
; TODO port to 6809        lda     #$15        ; Turn off 80-column firmware
; TODO port to 6809        jsr     $C300       ; Needs ROM (see $CEF4)
; TODO port to 6809
; TODO port to 6809:       bit     $C080       ; Back to LC bank 2 for R/O
; TODO port to 6809
; TODO port to 6809        bit     TXTCLR
; TODO port to 6809        bit     MIXCLR
; TODO port to 6809        bit     HIRES
; TODO port to 6809
; TODO port to 6809        lda     #20
; TODO port to 6809        sta     WNDTOP      ; Prepare hires_text()
        rts

;.endproc


_hires_Done

; TODO port to 6809        bit     TXTSET
; TODO port to 6809
; TODO port to 6809        lda     #00
; TODO port to 6809        sta     WNDTOP      ; Back to full screen text
        rts

;.endproc


_hires_Text

; TODO port to 6809        tax                 ; 'flag'
; TODO port to 6809        lda     MIXCLR,x
        rts

;.endproc


;.data


_hires_Draw

; TODO port to 6809        sta     src+1       ; 'src' lo
; TODO port to 6809        stx     src+2       ; 'src' hi
; TODO port to 6809
; TODO port to 6809        jsr     popax       ; 'rop'
; TODO port to 6809        stx     rop
; TODO port to 6809        sta     rop+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'ysize'
; TODO port to 6809        sta     ymax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'xsize'
; TODO port to 6809        sta     xmax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'ypos'
; TODO port to 6809        sta     ypos+1
; TODO port to 6809        tax
; TODO port to 6809
; TODO port to 6809        clc
; TODO port to 6809        adc     ymax+1
; TODO port to 6809        sta     ymax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'xpos'
; TODO port to 6809        sta     xpos+1
; TODO port to 6809
; TODO port to 6809        clc
; TODO port to 6809        adc     xmax+1
; TODO port to 6809        sta     xmax+1
; TODO port to 6809yloop:
; TODO port to 6809        lda     BASELO,x
; TODO port to 6809        sta     dst+1
; TODO port to 6809        lda     BASEHI,x
; TODO port to 6809        sta     dst+2
; TODO port to 6809
; TODO port to 6809xpos:   ldx     #$FF        ; Patched
; TODO port to 6809xloop:
; TODO port to 6809src:    lda     $FFFF,y     ; Patched
; TODO port to 6809        iny
; TODO port to 6809rop:    nop                 ; Patched
; TODO port to 6809        nop                 ; Patched
; TODO port to 6809dst:    sta     $FFFF,x     ; Patched
; TODO port to 6809        inx
; TODO port to 6809xmax:   cpx     #$FF        ; Patched
; TODO port to 6809        bne     xloop
; TODO port to 6809
; TODO port to 6809        inc     ypos+1
; TODO port to 6809ypos:   ldx     #$FF        ; Patched
; TODO port to 6809ymax:   cpx     #$FF        ; Patched
; TODO port to 6809        bne     yloop
        rts

;.endproc


_hires_Mask

; TODO port to 6809        stx     rop         ; 'rop' hi
; TODO port to 6809        sta     rop+1       ; 'rop' lo
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'ysize'
; TODO port to 6809        sta     ymax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'xsize'
; TODO port to 6809        sta     xmax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'ypos'
; TODO port to 6809        tax
; TODO port to 6809
; TODO port to 6809        clc
; TODO port to 6809        adc     ymax+1
; TODO port to 6809        sta     ymax+1
; TODO port to 6809
; TODO port to 6809        jsr     popa        ; 'xpos'
; TODO port to 6809        sta     xpos+1
; TODO port to 6809
; TODO port to 6809        clc
; TODO port to 6809        adc     xmax+1
; TODO port to 6809        sta     xmax+1
; TODO port to 6809
; TODO port to 6809yloop:
; TODO port to 6809        lda     BASELO,x
; TODO port to 6809        sta     src+1
; TODO port to 6809        sta     dst+1
; TODO port to 6809        lda     BASEHI,x
; TODO port to 6809        sta     src+2
; TODO port to 6809        sta     dst+2
; TODO port to 6809
; TODO port to 6809xpos:   ldy     #$FF        ; Patched
; TODO port to 6809xloop:
; TODO port to 6809src:    lda     $FFFF,y     ; Patched
; TODO port to 6809rop:    nop                 ; Patched
; TODO port to 6809        nop                 ; Patched
; TODO port to 6809dst:    sta     $FFFF,y     ; Patched
; TODO port to 6809        iny
; TODO port to 6809xmax:   cpy     #$FF        ; Patched
; TODO port to 6809        bne     xloop
; TODO port to 6809
; TODO port to 6809        inx
; TODO port to 6809ymax:   cpx     #$FF        ; Patched
; TODO port to 6809        bne     yloop
        rts

;.endproc

_charset
	INCLUDEBIN ../../support/apple2/gen/charset
_charset_end
