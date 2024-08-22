SECTION code_user

PUBLIC _dwread

defc    BECKRBASE=0x3E

_dwread:
    pop     hl      ; return address
    pop     de      ; count
    ex      (sp), hl ; buffer
; HL = starting address where data is to be stored
; DE = number of bytes expected
dwread_asm:
    ld b,e          ; Number of loops is in DE
    dec de          ; Calculate DB value (destroys B, D and E)
    inc d
loop1:
    in BECKRBASE
    and 2
    jz loop1
    in BECKRBASE+1
    ld (hl),a
    ; next byte
    inc hl
    djnz loop1
    dec d
    jp nz,loop1
    ld l,1
    ret
