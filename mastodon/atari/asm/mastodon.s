        ;; Show latest mastodon post from OLDBYTES.SPACE
        ;; Author: Thomas Cherryhomes
        ;; Email: thom dot cherryhomes at gmail dot com
        ;; License: GPL v. 3, see LICENSE for details

        ;; ZP
RTCLOK  = $12       ;   First byte of Jiffy clock
        
; CIO
ICHID   = $0340     ;   Set by OS. Handler Identifier. If not 
                    ;   in use, the value is 255 ($FF), which 
                    ;   is also the initialization value.
ICDNO   = ICHID+1   ;   Set by OS. Device number (eg: D1: D2:).
ICCOM   = ICHID+2   ;   Set by User. Command
ICSTA   = ICHID+3   ;   Set by OS. May or may not be the same 
                    ;   value as CMD_STATUS returned
ICBAL   = ICHID+4   ;   Set by User. Buffer address (low byte)
ICBAH   = ICHID+5   ;   Set by User. buffer address (high byte)
ICPTL   = ICHID+6   ;   Used by BASIC. Address of put byte routine. 
ICPTH   = ICHID+7   ;   Used by BASIC. Address of put byte routine. 
ICBLL   = ICHID+8   ;   buffer length (low byte) in put/get operations
ICBLH   = ICHID+9   ;   buffer length (high byte)
ICAX1   = ICHID+10  ;   auxiliary information.  Used by Open cmd 
                    ;   for READ/WRITE/UPDATE
                    ;   Bit  7   6   5   4   3   2   1   0
                    ;   Use  ....unused....  W   R   D   A
                    ;   W equals write, R equals read, 
                    ;   D equals directory, A equals append.
ICAX2   = ICHID+11  ;   Auxiliary byte two
ICAX3   = ICHID+12  ;   Auxiliary bytes three
ICAX4   = ICHID+13  ;   Auxiliary bytes four
ICAX5   = ICHID+14  ;   Auxiliary bytes five
ICAX6   = ICHID+15  ;   Auxiliary bytes six
;
; BASIC CIO assignments
;      Channel   0    Permanently assigned to the screen editor
;                6    Used for graphics commands
;                7    Used for the Cassette, disk and printer
;
; Only use these channels if you have BASIC running, 
;  otherwise they're all available
;IOCB1   = $10       ; Channel 1
;IOCB2   = $20       ; Channel 2
;IOCB3   = $30       ; Channel 3
;IOCB4   = $40       ; Channel 4
;IOCB5   = $50       ; Channel 5
;
CIOV    = $E456	; Y will contain status code after call

        ;; ICCOM Equates
CMD_PUT_CHAR     = $0b		 ; Put Characters
ATASCII_CLRSCR   = $7D           ; ATASCII clear screen character

; SIO 
SIOV	=       $E459	; SIO VECTOR
EOL	=	$9B     ; EOL

DDEVIC	=	$300    ; DEVICE SERIAL BUS ID
DUNIT	=	$301    ; SERIAL UNIT NUMBER
DCOMND	=	$302    ; SIO COMMAND
DSTATS	=	$303    ; SIO DATA DIRECTION
DBUFLO	=	$304    ; BUFFER LOW ADDRESS
DBUFHI	=	$305    ; BUFFER HIGH ADDRESS
DTIMLO	=	$306    ; SIO TIMEOUT
DTIMHI	=	$307
DBYTLO	=	$308    ; BUFFER LENGTH
DBYTHI	=	$309
DAUX1	=	$30A    ; AUXILARY BYTE (LO)
DAUX2	=	$30B    ; AUXILARY BYTE (HI)

DSTATS_NONE  = $00              ; No payload.
DSTATS_READ  = $40              ; Read payload to Atari
DSTATS_WRITE = $80              ; Write payload to FujiNet

AUX1_HTTP_GET = $0C
AUX2_NONE     = $00

NOERROR       = $01
        
        org $4000

src     = $80
        
        ;; -- Main Loop ------------------------------------------------
start:  jsr banner
        jsr fetch
        jsr display
        jsr wait
        jmp start
        ;; -------------------------------------------------------------

        ;; -- Display Banner -------------------------------------------
banner: ldx #$00                ; Channel #0
        lda #CMD_PUT_CHAR
        sta ICCOM,X
        lda #.lo(banner_msg)
        sta ICBAL,X
        lda #.hi(banner_msg)
        sta ICBAH,X
        lda #banner_msg_end - banner_msg
        sta ICBLL,X
        lda #$00
        sta ICBLH,X
        jsr CIOV
        rts

banner_msg:
        .byte 'LATEST POST FROM: OLDBYTES.SPACE',EOL
        .byte '--------------------------------',EOL,EOL
banner_msg_end:
        ;; -------------------------------------------------------------

        ;; -- Fetch Mastodon Post --------------------------------------
fetch:  lda #.lo(fetch_open_dcb) ; Do Open
        ldx #.hi(fetch_open_dcb)
        jsr setup_dcb
        jsr SIOV
        cmp #NOERROR
        beq fetch2
        jmp err

fetch2: lda #.lo(fetch_chan_mode_dcb) ; Set Channel mode
        ldx #.hi(fetch_chan_mode_dcb)
        jsr setup_dcb
        jsr SIOV
        cmp #NOERROR
        beq fetch3
        jmp err

fetch3: lda #.lo(fetch_parse_json_dcb) ; Parse JSON
        ldx #.hi(fetch_parse_json_dcb)
        jsr setup_dcb
        jsr SIOV
        cmp #NOERROR
        beq fetch4
        jmp err

fetch4: lda #.lo(fetch_query_name_dcb) ; Query for name
        ldx #.hi(fetch_query_name_dcb)
        jsr setup_dcb
        jsr SIOV
        cmp #NOERROR
        beq fetch5
        jmp err

fetch5: 

        rts

fetch_open_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte 'O'               ; (DCOMND) Open command
        .byte DSTATS_WRITE      ; (DSTATS) Write to FujiNet
        .word fetch_open_url    ; (DBUF)   Pointer to Open URL
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 256               ; (DBYT)   Open wants 256 bytes.
        .byte AUX1_HTTP_GET     ; Do an HTTP GET
        .byte AUX2_NONE         ; No translation

fetch_chan_mode_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte $FC               ; (DCOMND) Set Channel mode
        .byte DSTATS_NONE       ; (DSTATS) Write to FujiNet
        .word 0                 ; (DBUF)   NULL
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 0                 ; (DBYT)   No Payload
        .byte 0                 ;
        .byte 1                 ; JSON mode

fetch_parse_json_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte 'P'               ; (DCOMND) Parse JSON
        .byte DSTATS_NONE       ; (DSTATS) Write to FujiNet
        .word 0                 ; (DBUF)   NULL
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 0                 ; (DBYT)   No Payload
        .byte 0                 ;
        .byte 0                 ;

fetch_query_name_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte 'Q'               ; (DCOMND) Query JSON
        .byte DSTATS_WRITE      ; (DSTATS) Write to FujiNet
        .word fetch_query_name  ; (DBUF)   Pointer to json query str
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 256               ; (DBYT)   Query wants 256 bytes.
        .byte 0                 ; 
        .byte 0                 ;

fetch_query_created_at_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte 'Q'               ; (DCOMND) Query JSON
        .byte DSTATS_WRITE      ; (DSTATS) Write to FujiNet
        .word fetch_query_created_at
                                ; (DBUF)   Pointer to json query str
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 256               ; (DBYT)   Query wants 256 bytes.
        .byte 0                 ; 
        .byte 0                 ;

fetch_query_content_dcb:
        .byte $71               ; (DDEVIC) Use N:
        .byte $01               ; (DUNIT)  Use N1:
        .byte 'Q'               ; (DCOMND) Query JSON
        .byte DSTATS_WRITE      ; (DSTATS) Write to FujiNet
        .word fetch_query_content
                                ; (DBUF)   Pointer to json query str
        .byte $0F               ; (DTIMLO) 15 sec timeout.
        .byte $00               ; (DRESVD) Reserved
        .word 256               ; (DBYT)   Query wants 256 bytes.
        .byte 0                 ; 
        .byte 0                 ;

        ;; While we need to send 256 bytes, Open will terminate the
        ;; string on a null ($00) or EOL (EOL) byte.
        
fetch_open_url:
        .byte "N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1"
        .byte EOL

fetch_query_name:
        .byte "N:/0/account/display_name",EOL

fetch_query_created_at:
        .byte "N:/0/created_at",EOL

fetch_query_content:
        .byte "N:/0/content",EOL
        
        ;; -- Display Mastodon Post ------------------------------------
display:
        rts
        ;; -------------------------------------------------------------

        ;; -- Wait a bit using RTCLOK ----------------------------------
wait:
        lda #$00                ; Clear the rtclok
        sta RTCLOK              ;
        sta RTCLOK+1            ;
        sta RTCLOK+2            ;

wait1:  lda RTCLOK+1            ; Check rtclok+1
        cmp #$0F                ; approx 60 seconds?
        bne wait1               ; not yet, go back

        rts                     ; Otherwise, we're done, return.
        
        ;; -- Subroutine: Setup DCB from buffer ------------------------
        ;; LDA: Lo byte of buffer
        ;; LDX: Hi Byte of buffer
setup_dcb:
        sta src
        stx src+1
        ldy #$00
setup_next:
        lda (src),y
        sta DDEVIC,y
        iny
        cpy #$0c
        bne setup_next
        rts

        ;; -- Indicate error -------------------------------------------

err:    jmp err
        
        ;; Load and go bytes, start at start:
        org $02E0
        .word start
