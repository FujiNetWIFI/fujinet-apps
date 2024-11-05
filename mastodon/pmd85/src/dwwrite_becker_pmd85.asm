SECTION code_user

PUBLIC _dwwrite

defc    BECKRBASE=0x3E

_dwwrite:
    pop     hl      ; return address
    pop     de      ; count
    ex      (sp), hl ; buffer
; HL = starting address of data to send
; DE = number of bytes to send
dwwrite_asm:
    ld b,e          ; Number of loops is in DE
    dec de          ; Calculate DB value (destroys B, D and E)
    inc d
loop1:
    ld a,(hl)
    out BECKRBASE+1
    ; next byte
    inc hl
    djnz loop1
    dec d
    jp nz,loop1
    ld l,1
    ret
