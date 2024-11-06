; SECTION rodata_user
; PUBLIC _font_6x5
; _font_6x5:
;    INCBIN "font_8x8_coleco_adam_system.bin"

SECTION code_user

PUBLIC _screen_fill

PUBLIC _screen_set_region
PUBLIC _screen_fill_region

EXTERN generic_console_xypos

defc    DISPLAY=0xC000

_screen_fill:
    ld      (fill_pattern), hl
    ld      hl, DISPLAY
    ld      c, 0x80
    ld      de, 16
loop1:
    ld      b, 48
    ld      a, (fill_pattern)
loop2:
    ld      (hl), a
    inc     hl
    djnz    loop2
    add     hl, de
    ld      b, 48
    ld      a, (fill_pattern+1)
loop3:
    ld      (hl), a
    inc     hl
    djnz    loop3
    add     hl, de
    dec     c
    jr      nz, loop1
    ret

_screen_set_region:
    pop     hl      ; return address
    pop     de      ; h
    pop     bc      ; w
    ld      d, c    ; d = width, e = height
    pop     bc      ; y
    ex      (sp), hl ; x
    ld      b, c    ; b = y
    ld      c, l    ; c = x
; de = width, height
; bc = y, x
screen_set_region_asm:
    call    generic_console_xypos
    ld      (video_ptr), hl
    ex      de, hl
    ld      (region_wh), hl
    ret

_screen_fill_region:
; hang:
;     jmp hang
    ; save fill pattern
    ld      (fill_pattern), hl
    ex      de, hl
    ; save fill mask
    ld      (fill_mask), hl
    ; preapre variables for loop
    ld      hl, (region_wh)
    ld      a, l
    rlc
    rlc
    ld      c, a    ; c = number of lines to fill / 2 = text rows * (8 / 2)
    ld      a, 64
    sub     a, h    ; 64 - text columns
    ld      e, a    ; e = offset to next line
    ld      hl, (video_ptr)
floop1:
    ; even line
    ld      a, (region_wh+1)
    ld      b, a    ; b = number of bytes to fill
    ld      a, (fill_pattern) ; even line pattern
    ld      d, a    ; d = pattern
floop2:
    ld      a, (fill_mask)
    and     a, (hl) ; apply mask
    xor     a, d    ; apply pattern
    ld      (hl), a
    inc     hl
    djnz    floop2
    ld      d, 0
    add     hl, de  ; next line

    ; odd line
    ld      a, (region_wh+1)
    ld      b, a    ; b = number of bytes to fill
    ld      a, (fill_pattern+1) ; odd line pattern
    ld      d, a    ; d = pattern
floop3:
    ld      a, (fill_mask)
    and     a, (hl) ; apply mask
    xor     a, d    ; apply pattern
    ld      (hl), a ; fill
    inc     hl
    djnz    floop3
    ld      d, 0
    add     hl, de  ; next line
    dec     c       ; decrement line pair counter
    jr      nz, floop1
    ret

SECTION data_user

fill_pattern:
    dw 0

fill_mask:
    dw 0

video_ptr:
    dw DISPLAY

region_wh:
    dw 0
