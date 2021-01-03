; 
            icl 'ctf.inc'
;
; Start of code
;
            org $2020
	;; Homesoft tag :)
L2020	.byte "  : Fileversion by HOMESOFT :  "
	
	;; Set coldstart 
            org $0244
            .byte $01
	;; Program Start
	
            org $4000

            jsr L628C
L4003	    jmp L5FC1
L4006       ldx #$00
            lda #$70
            jsr L40D1
            lda #$F0
            jsr L40D1
            lda #$4D
            jsr L40D1
            lda #$00
            sta L0083
            sta L0084
            jsr L40F1
            jsr L40C9
            ldy #$03
L4025       jsr L40E8
            jsr L40C9
            dey
            bne L4025
            ldy #$06
L4030       jsr L40C9
            dey
            bne L4030
            ldy #$05
L4038       jsr L40CC
            dey
            bne L4038
            jsr L40E8
            jsr L40E8
            jsr L40E8
            jsr L40E4
            lda #$C0
            sta L0083
            lda #$05
            sta L0084
            ldy #$23
L4054       jsr L40EC
            dey
            bne L4054
            lda #$C2
            jsr L40D1
            lda #$00
            ldy #$3F
            jsr L40DD
            lda #$60
            sta L0083
            dec L0084
            ldy #$04
L406E       jsr L40EC
            dey
            bne L406E
            lda #$00
            jsr L40D1
            lda #$4E
            jsr L40D1
            jsr L40D9
            ldy #$3F
L4083       jsr L40E4
            dey
            bne L4083
            jsr L40C5
            jsr L40D9
            lda #$41
            jsr L40D1
            lda #$00
            ldy #$10
            jsr L40DD
            lda #$00
            sta L10FE,X
            lda #$11
            sta L10FF,X
            ldx #$3A
            jsr L40C5
            ldx #$40
            jsr L40C5
            ldx #$49
            jsr L40C5
            ldx #$9A
            jsr L40C5
            lda #$B1
            sta L4278+1
            lda #$51
            sta L4278+2
            ldx #$A9
L40C5       lda #$CE
            bne L40D1
L40C9       jsr L40E8
L40CC       jsr L40E8
            lda #$8D
L40D1       sta L1000,X
            sta L1100,X
            inx
            rts
L40D9       lda #$00
            ldy #$08
L40DD       jsr L40D1
            tya
            jmp L40D1
L40E4       lda #$0E
            bne L40D1
L40E8       lda #$0D
            bne L40D1
L40EC       lda #$4E
            jsr L40D1
L40F1       lda L0083
            jsr L40D1
            lda L0084
            clc
            adc #$30
            sta L1000,X
            lda L0084
            clc
            adc #$36
            sta L1100,X
            lda L0083
            sec
            sbc #$20
            sta L0083
            lda L0084
            sbc #$00
            sta L0084
            inx
            rts
L4115       ldx #$3C
            lda #$3E
            sta VDSLST
            lda #$41
            sta VDSLST+1
            lda #$01
            sta L3F1C,X
            lda #$C0
            sta NMIEN
            rts
L412C       pla
            clc
            adc #$01
            sta VDSLST
            pla
            adc #$00
            sta VDSLST+1
            pla
            rti
L413B       jsr L412C
            pha
            lda #$FF
            sta GRAFP2
            sta GRAFP3
            lda #$02
            eor COLRSH
            and DRKMSK
            sta COLPF0
            lda #$88
            sta L00B9
            eor COLRSH
            and DRKMSK
            sta COLPF2
            lda L00BA
            bne L4162
            lda RANDOM
L4162       eor #$40
            eor COLRSH
            and DRKMSK
            sta COLPM2
            sta COLPM3
            eor #$40
            sta COLBK
            lda #$AA
            sta GRAFM
            lda ATRACT
            sta L00FF
            bmi L4183
            lda RTCLOK+2
            ror
            bcc L4187
L4183       jsr L412C
            pha
L4187       jsr L412C
            pha
            lda L00B9
            clc
            adc #$F0
            sta L00B9
            eor COLRSH
            and DRKMSK
            sta WSYNC
            sta COLPF2
            lda L00B9
            cmp #$18
            beq L41A5
            jmp L4183
L41A5       lda L00FF
            bmi L41B2
            lda RTCLOK+2
            ror
            bcs L41B2
            jsr L412C
            pha
L41B2       jsr L412C
            pha
            lda #$D6
            sta L00B9
            lda RTCLOK+2
            asl
            and #$02
            eor COLRSH
            and DRKMSK
            sta WSYNC
            sta COLPF2
L41C9       jsr L412C
            pha
            pla
            pha
            nop
            lda L00B9
            pha
            eor COLRSH
            and DRKMSK
            sta COLPF2
            pla
            cmp #$D2
            beq L41E6
            dec L00B9
            dec L00B9
            jmp L41C9
L41E6       jsr L412C
            pha
            pla
            pha
            pla
            pha
            lda #$B4
            eor COLRSH
            and DRKMSK
            sta COLPF2
            lda #$0A
            eor COLRSH
            and DRKMSK
            sta COLPF1
            lda #$00
            sta GRAFM
            jsr L412C
            pha
            lda L00B9
            eor COLRSH
            and DRKMSK
            sta WSYNC
            sta COLPF2
            lda COLRSH
            and DRKMSK
            sta COLPF1
            lda #$AA
            sta GRAFM
            jsr L412C
            pha
            lda COLRSH
            and DRKMSK
            sta WSYNC
            sta COLBK
            lda #$00
            sta GRAFM
            sta GRAFP2
            sta GRAFP3
            lda #$D8
            eor COLRSH
            and DRKMSK
            sta COLPF0
            lda #$F6
            eor COLRSH
            and DRKMSK
            sta COLPF1
            lda #$F2
            eor COLRSH
            and DRKMSK
            sta COLPF2
            lda #$7A
            eor COLRSH
            and DRKMSK
            sta WSYNC
            sta COLBK
            jsr L412C
            pha
            lda COLRSH
            and DRKMSK
            sta COLBK
            sta WSYNC
            sta COLPF1
            sta COLPM2
            sta COLPM3
L4278       jmp L413B
L427B       ldy #$00
            sta LOMEM+1
            cmp #$00
            bpl L428A
            tya
            sec
            sbc LOMEM+1
            sta LOMEM+1
            iny
L428A       lda L0082
            bpl L4296
            lda #$00
            sec
            sbc L0082
            sta L0082
            iny
L4296       jsr L42AC
            cpy #$01
            bne L42A9
            eor #$FF
            pha
            tya
            clc
            sbc L0082
            sta L0082
            pla
            adc #$00
L42A9       rts
L42AA       sta LOMEM+1
L42AC       ldx #$08
            lda #$00
            clc
L42B1       ror
            ror L0082
            bcc L42B9
            clc
            adc LOMEM+1
L42B9       dex
            bne L42B1
            ror
            ror L0082
            rts
L42C0       ldx #$09
            clc
L42C3       bcs L42C9
            cmp LOMEM
            bcc L42CC
L42C9       sbc LOMEM
            sec
L42CC       rol L0082
            rol
            dex
            bne L42C3
            rts
L42D3       ldx #$08
            clc
L42D6       bcs L42DC
            cmp LOMEM
            bcc L42DF
L42DC       sbc LOMEM
            sec
L42DF       rol L0082
            rol
            dex
            bne L42D6
            rts
L42E6       clc
            adc #$40
L42E9       ldy #$00
            cmp #$00
            bpl L42F5
            iny
            eor #$FF
            clc
            adc #$01
L42F5       cmp #$40
            bmi L42FE
            eor #$FF
            sec
            adc #$80
L42FE       tax
            lda L430B,X
            dey
            bne L430A
            eor #$FF
            clc
            adc #$01
L430A       rts
L430B       .byte $00,$03
            asl BOOT
            .byte $0C,$0F,$12
            ora DSKFMS,X
            .byte $1C,$1F,$22
            and ICBLLZ
            .byte $2B
            rol L3330
            rol RECVDN,X
            .byte $3C,$3F
            eor (FMSZPG+1,X)
            .byte $47
            eor #$4C
            lsr L5351
            eor SAVMSC,X
            .byte $5A,$5C
            lsr L6260,X
            .byte $64
            ror SAVADR
            ror
            jmp (L6F6D)
            bvs L43AB
            .byte $73
            adc DELTAR,X
            .byte $77
            sei
            adc L7B7A,Y
            .byte $7C,$7C
            adc L7E7E,X
            .byte $7F,$7F,$7F,$7F,$7F,$7F
L434C       lda L0087
            bmi L435C
            lda L0088
            clc
            adc L0086
            lda L0089
            adc L0087
            cmp #$03
            rts
L435C       lda L0088
            sec
            sbc L0086
            lda L0089
            sbc L0087
            cmp #$03
            rts
L4368       ldx L00C0
            stx L00BD
            lda #$00
            sta L3C00,X
            sta L3D00,X
            lda #$00
            sta L3C80,X
            sta L3D80,X
            lda L3E80,X
            tax
            lda L3E80,X
            sta L00C0
            lda #$3F
            sta L3C00,X
            sta L3D00,X
            lda #$2E
            sta L3C80,X
            sta L3D80,X
            lda #$00
            sta L3E80,X
            lda L00E1
            jsr L42E9
            sta L00C1
            lda L00E1
            jsr L42E6
            sta L00C2
            lda L00E7
            pha
L43AB       lda #$02
            sta L00E7
            lda #$00
            sec
            sbc L00C1
            jsr L57A3
            jsr L57CF
            lda #$00
            sec
            sbc L00C2
            jsr L57A3
            jsr L57EE
            pla
            sta L00E7
            lda #$00
            sta L00BE
            sta L00BF
            lda L00F2
            sta L00B7
            lda L00F1
            rol
            rol L00B7
            lda L00F4
            sta L00B8
            lda L00F3
            rol
            rol L00B8
            lda L00E1
            clc
            adc #$E0
            rol
            bcc L440D
            bpl L43FB
            inc L00BE
            inc L00B7
            inc L00B8
            lda L00C2
            sta L00C3
            lda L00C1
            sta L00C5
            jmp L4431
L43FB       inc L00BF
            inc L00B8
            sec
            lda #$00
            sbc L00C1
            sta L00C3
            lda L00C2
            sta L00C5
            jmp L4431
L440D       bpl L4422
            dec L00BE
            sec
            lda #$00
            sbc L00C2
            sta L00C3
            sec
            lda #$00
            sbc L00C1
            sta L00C5
            jmp L4431
L4422       dec L00BF
            inc L00B7
            lda L00C1
            sta L00C3
            sec
            lda #$00
            sbc L00C2
            sta L00C5
L4431       lda #$00
            sta L00C4
            sta L00C6
            lda L00C3
            bpl L443D
            dec L00C4
L443D       lda L00C5
            bpl L4443
            dec L00C6
L4443       ldx L00E1
            lda L00D7
            rol
            lda L00D8
            rol
            sta L00C7
            lda L00DA
            rol
            lda L00DB
            rol
            sta L00C8
            lda L00D7
            and #$7F
            cpx #$20
            bpl L4464
            tay
            beq L4469
            inc L00C7
            eor #$80
L4464       eor #$FF
            clc
            adc #$01
L4469       sta L00C9
            lda L00DA
            and #$7F
            cpx #$E0
            bpl L447A
            tay
            beq L447F
            inc L00C8
            eor #$80
L447A       eor #$FF
            clc
            adc #$01
L447F       sta L00CA
            lda L00C9
            sta L0082
            lda L00C2
            jsr L427B
            sta L00CC
            lda L0082
            sta L00CB
            lda L00CA
            sta L0082
            lda L00C1
            jsr L427B
            eor #$FF
            pha
            sec
            lda L00CB
            sbc L0082
            sta L00CB
            pla
            adc L00CC
            rol L00CB
            rol
            sta L00CB
            lda #$00
            bcc L44B1
            lda #$FF
L44B1       sta L00CC
            lda L00C9
            sta L0082
            lda L00C1
            jsr L427B
            sta L00CE
            lda L0082
            sta L00CD
            lda L00CA
            sta L0082
            lda L00C2
            jsr L427B
            pha
            lda L0082
            clc
            adc L00CD
            sta L00CD
            pla
            adc L00CE
            rol L00CD
            rol
            sta L00CD
            lda #$00
            bcc L44E1
            lda #$FF
L44E1       sta L00CE
            lda #$00
            sta L00D3
            lda L00BF
            beq L44ED
            inc L00D3
L44ED       lda #$00
            sta L00CF
            sta L00D0
            sta L00D1
            sta L00D2
            lda L00BE
            cmp #$FF
            bne L4503
            inc L00CF
            inc L00D2
            inc L00B8
L4503       lda L00BF
            cmp #$FF
            bne L450B
            inc L00D0
L450B       cmp #$01
            bne L4513
            inc L00D1
            inc L00B7
L4513       lda #$05
            sta L00D4
L4517       lda L00C7
            sec
            sbc L00BF
            sta L00C7
            lda L00C8
            clc
            adc L00BE
            sta L00C8
            lda L00CB
            sec
            sbc L00C5
            sta L00CB
            lda L00CC
            sbc L00C6
            sta L00CC
            lda L00CD
            clc
            adc L00C3
            sta L00CD
            lda L00CE
            adc L00C4
            sta L00CE
            lda #$01
            sta L00D6
            lda L00C7
            clc
            adc L00D1
            sta L00A2
            lda L00C8
            clc
            adc L00D2
            sta L00A3
            lda L00CB
            sta L0086
            lda L00CC
            sta L0087
            lda L00CD
            sta L0088
            lda L00CE
            sta L0089
            lda L00D3
            eor #$01
            sta L00D5
L4567       lda L00A2
            cmp L00B7
            bne L4576
            lda L00A3
            cmp L00B8
            bne L4576
            jsr L4715
L4576       lda L0086
            sec
            sbc L0088
            lda L0087
            sbc L0089
            bpl L45B4
            jsr L434C
            bcs L45B4
            jsr L48B0
            lda L00A2
            clc
            adc L00BE
            sta L00A2
            lda L00A3
            clc
            adc L00BF
            sta L00A3
            lda L0086
            clc
            adc L00C3
            sta L0086
            lda L0087
            adc L00C4
            sta L0087
            lda L0088
            clc
            adc L00C5
            sta L0088
            lda L0089
            adc L00C6
            sta L0089
            jmp L4567
L45B4       lda #$01
            sta L00D6
            lda L00C7
            clc
            adc L00D1
            sec
            sbc L00BE
            sta L00A2
            lda L00C8
            clc
            adc L00D2
            sec
            sbc L00BF
            sta L00A3
            lda L00CB
            sec
            sbc L00C3
            sta L0086
            lda L00CC
            sbc L00C4
            sta L0087
            lda L00CD
            sec
            sbc L00C5
            sta L0088
            lda L00CE
            sbc L00C6
            sta L0089
L45E6       lda L0086
            clc
            adc L0088
            lda L0087
            adc L0089
            bmi L4633
            jsr L434C
            bcs L4633
            jsr L48B0
            lda L00A2
            cmp L00B7
            bne L4608
            lda L00A3
            cmp L00B8
            bne L4608
            jsr L4715
L4608       lda L00A2
            sec
            sbc L00BE
            sta L00A2
            lda L00A3
            sec
            sbc L00BF
            sta L00A3
            lda L0086
            sec
            sbc L00C3
            sta L0086
            lda L0087
            sbc L00C4
            sta L0087
            lda L0088
            sec
            sbc L00C5
            sta L0088
            lda L0089
            sbc L00C6
            sta L0089
            jmp L45E6
L4633       lda L00A2
            cmp L00B7
            bne L4642
            lda L00A3
            cmp L00B8
            bne L4642
            jsr L4715
L4642       lda #$00
            sta L00D6
            lda L00C7
            clc
            adc L00CF
            sec
            sbc L00BE
            sta L00A2
            pha
            lda L00C8
            clc
            adc L00D0
            sec
            sbc L00BF
            sta L00A3
            pha
            lda L00CB
            sec
            sbc L00C3
            sta L0086
            pha
            lda L00CC
            sbc L00C4
            sta L0087
            pha
            lda L00CD
            sec
            sbc L00C5
            sta L0088
            pha
            lda L00CE
            sbc L00C6
            sta L0089
            pha
            lda L00D3
            sta L00D5
L467E       lda L00A2
            clc
            adc L00BE
            sta L00A2
            lda L00A3
            clc
            adc L00BF
            sta L00A3
            lda L0086
            clc
            adc L00C3
            sta L0086
            lda L0087
            adc L00C4
            sta L0087
            lda L0088
            clc
            adc L00C5
            sta L0088
            lda L0089
            adc L00C6
            sta L0089
            jsr L48B0
            lda L0086
            sec
            sbc L0088
            lda L0087
            sbc L0089
            bpl L46B9
            jsr L434C
            bcc L467E
L46B9       lda #$00
            sta L00D6
            pla
            sta L0089
            pla
            sta L0088
            pla
            sta L0087
            pla
            sta L0086
            pla
            sta L00A3
            pla
            sta L00A2
L46CF       lda L0086
            clc
            adc L0088
            lda L0087
            adc L0089
            bmi L470D
            jsr L434C
            bcs L470D
            jsr L48B0
            lda L00A2
            sec
            sbc L00BE
            sta L00A2
            lda L00A3
            sec
            sbc L00BF
            sta L00A3
            lda L0086
            sec
            sbc L00C3
            sta L0086
            lda L0087
            sbc L00C4
            sta L0087
            lda L0088
            sec
            sbc L00C5
            sta L0088
            lda L0089
            sbc L00C6
            sta L0089
            jmp L46CF
L470D       dec L00D4
            beq L4714
            jmp L4517
L4714       rts
L4715       ldx #$11
            lda L00DD
            and #$02
            bne L471F
            ldx #$10
L471F       stx L008A
            lda L0086
            sta L008F
            lda L0087
            sta L0090
            lda L0088
            sta L0091
            lda L0089
            sta L0092
            lda L00E1
            clc
            adc #$20
            bmi L4752
            lda L008F
            clc
            adc L00C5
            sta L008F
            lda L0090
            adc L00C6
            sta L0090
            lda L0091
            sec
            sbc L00C3
            sta L0091
            lda L0092
            sbc L00C4
            sta L0092
L4752       lda L00E1
            clc
            adc #$60
            bmi L4773
            lda L008F
            sec
            sbc L00C3
            sta L008F
            lda L0090
            sbc L00C4
            sta L0090
            lda L0091
            sec
            sbc L00C5
            sta L0091
            lda L0092
            sbc L00C6
            sta L0092
L4773       lda L00F1
            and #$7F
            sta L0082
            lda L00C2
            jsr L427B
            sta L008C
            lda L0082
            sta L008B
            lda L00F3
            and #$7F
            sta L0082
            lda L00C1
            jsr L427B
            eor #$FF
            pha
            sec
            lda L008B
            sbc L0082
            sta L008B
            pla
            adc L008C
            rol L008B
            rol
            bcc L47A3
            dec L0090
L47A3       clc
            adc L008F
            sta L008F
            lda L0090
L47AA       adc #$00
            sta L0090
            lda L00F1
            and #$7F
            sta L0082
            lda L00C1
            jsr L427B
            sta L008E
            lda L0082
            sta L008D
            lda L00F3
            and #$7F
            sta L0082
            lda L00C2
            jsr L427B
            pha
            lda L0082
            clc
            adc L008D
            sta L008D
            pla
            adc L008E
            rol L008D
            rol
            bcc L47DC
            dec L0092
L47DC       clc
            adc L0091
            sta L0091
            lda L0092
            adc #$00
            sta L0092
            lda L00DD
            and #$03
            tax
            lda L4837,X
            sta L0083
            lda L008F
            sta L008B
            clc
            adc L0083
            sta L008F
            lda L0090
            sta L008C
            adc #$00
            sta L0090
            lda L008B
            sec
            sbc L0083
            sta L008B
            lda L008C
            sbc #$00
            sta L008C
            lda L0091
            sta L008D
            lda L0092
            sta L008E
            bmi L4836
            ora L008D
            beq L4836
            lda L008F
            clc
L4820       adc L0091
            lda L0090
            adc L0092
            bmi L4836
            lda L008D
            sec
            sbc L008B
            lda L008E
            sbc L008C
            bmi L4836
            jmp L4900
L4836       rts
L4837       .byte $0F
            asl
            .byte $1C,$1F
L483B       lda #$14
            bne L4841
L483F       lda #$01
L4841       sta L0083
            lda L00A2
            ora L00A3
            beq L4884
            lda L00A3
            beq L4887
            cmp #$11
            bcs L4884
L4851       lda L00A2
            beq L488E
            cmp #$11
            bcs L4884
            adc #$FF
            asl
            asl
            asl
            asl
            dec L00A3
            ora L00A3
            inc L00A3
            tax
            lda L0600,X
            ldx L00D5
            bne L4880
            and L0083
            bne L487F
            lda L00A3
            cmp #$10
            bne L4884
            lda L00E0
            beq L487F
            lda #$04
            sta L008A
L487F       rts
L4880       ror
            and L0083
            rts
L4884       lda #$00
            rts
L4887       lda L00D5
            bne L4884
            lda #$FF
            rts
L488E       lda L00D5
L4890       rts
L4891       lda #$00
            sta L008A
            jsr L483F
            bne L48AF
            lda L00A3
            eor #$10
            ora L00D5
            bne L48AD
            lda L00DD
            and #$02
            bne L48AD
            lda #$01
            sta L008A
            rts
L48AD       lda #$00
L48AF       rts
L48B0       jsr L4891
            beq L4890
            lda L0086
            sta L008F
            lda L0087
            sta L0090
            lda L0088
            sta L0091
            lda L0089
            sta L0092
            lda L00D6
            bne L48E6
            lda L008F
            sec
            sbc L00C3
            sta L008B
            lda L0090
            sbc L00C4
            sta L008C
            lda L0091
            sec
            sbc L00C5
            sta L008D
            lda L0092
            sbc L00C6
            sta L008E
            jmp L4900
L48E6       lda L008F
            clc
            adc L00C5
            sta L008B
            lda L0090
            adc L00C6
            sta L008C
            lda L0091
            sec
            sbc L00C3
            sta L008D
            lda L0092
            sbc L00C4
            sta L008E
L4900       lda L00D5
            beq L490A
            lda L008A
            ora #$80
            sta L008A
L490A       lda L008B
            clc
            adc L008D
            lda L008C
            adc L008E
            bpl L4973
            ldy #$00
            lda L0091
            sec
            sbc L008D
            sta L0082
            lda L0092
            sbc L008E
            bpl L492C
            lda L008D
            sec
            sbc L0091
            sta L0082
            iny
L492C       lda #$00
            sec
            sbc L008B
            sec
            sbc L008D
            pha
            clc
            adc L008F
            clc
            adc L0091
            sta LOMEM
            pla
            jsr L42AA
            jsr L42C0
            lda L008D
            dey
            beq L4955
            clc
            adc L0082
            sta L008D
            lda L008E
            adc #$00
            jmp L495E
L4955       sec
            sbc L0082
            sta L008D
            lda L008E
            sbc #$00
L495E       sta L008E
            lda #$00
            sec
            sbc L008D
            sta L008B
            lda #$00
            sbc L008E
            sta L008C
            lda L008A
            ora #$02
            sta L008A
L4973       lda L008D
            sec
            sbc L008B
            lda L008E
            sbc L008C
            bpl L49D5
            ldy #$00
            lda L0091
            sec
            sbc L008D
            sta L0082
            lda L0092
            sbc L008E
            bpl L4995
            lda L008D
            sec
            sbc L0091
            sta L0082
            iny
L4995       lda L008B
            sec
            sbc L008D
            pha
            sec
            sbc L008F
            clc
            adc L0091
            sta LOMEM
            pla
            jsr L42AA
            jsr L42C0
            lda L008D
            dey
            beq L49BB
            clc
            adc L0082
            sta L008D
            lda L008E
            adc #$00
            jmp L49C4
L49BB       sec
            sbc L0082
            sta L008D
            lda L008E
            sbc #$00
L49C4       sta L008E
            sta L008C
            lda L008D
            sta L008B
            lda L008A
            ora #$02
            sta L008A
            jmp L4A37
L49D5       lda L0091
            sec
            sbc L008F
            lda L0092
            sbc L0090
            bpl L4A37
            ldy #$00
            lda L008D
            sec
            sbc L0091
            sta L0082
            lda L008E
            sbc L0092
            bpl L49F7
            lda L0091
            sec
            sbc L008D
            sta L0082
            iny
L49F7       lda L008F
            sec
            sbc L0091
            pha
            sec
            sbc L008B
            clc
            adc L008D
            sta LOMEM
            pla
            jsr L42AA
            jsr L42C0
            lda L0091
            dey
            beq L4A1D
            clc
            adc L0082
            sta L0091
            lda L0092
            adc #$00
            jmp L4A26
L4A1D       sec
            sbc L0082
            sta L0091
            lda L0092
            sbc #$00
L4A26       sta L0092
            sta L0090
            lda L0091
            sta L008F
            lda L008A
            ora #$08
            sta L008A
            jmp L4A37
L4A37       lda L008E
            bne L4A65
            lda L008D
            bne L4A68
            lda L008F
            sta L0093
            lda L0090
            sta L0094
            lda L0091
            sta L0095
            lda L0092
            sta L0096
            jsr L4BE1
            lda L0093
            sta L0097
            lda L0094
            sta L0098
            lda L0095
            sta L0099
            lda L0096
            sta L009A
            jmp L4C76
L4A65       jmp L4B2B
L4A68       cmp #$40
            bcs L4A65
            lda L0092
            bne L4AAF
            lda L0091
            cmp #$40
            bcs L4AAF
            lda L008F
            sta L0093
            lda L0090
            sta L0094
            lda L0091
            sta L0095
            lda L0092
            sta L0096
            jsr L4BE1
            lda L0093
            sta L0097
            lda L0094
            sta L0098
            lda L0095
            sta L0099
            lda L0096
            sta L009A
            lda L008B
            sta L0093
            lda L008C
            sta L0094
            lda L008D
            sta L0095
            lda L008E
            sta L0096
            jsr L4BE1
            jmp L4C76
L4AAF       lda L008B
            sta L0093
            lda L008C
            sta L0094
            lda L008D
            sta L0095
            lda L008E
            sta L0096
            jsr L4BE1
            ldy #$00
            lda L008F
            sec
            sbc L008B
            sta L0082
            lda L0090
            sbc L008C
            bpl L4AD9
            lda L008B
            sec
            sbc L008F
            sta L0082
            iny
L4AD9       lda L0091
            sec
            sbc L008D
            sta LOMEM
            lda #$40
            sec
            sbc L008D
            jsr L42AA
            jsr L42C0
            lda L008B
            dey
            beq L4AFC
            clc
            adc L0082
            sta L008B
            lda L008C
            adc #$00
            jmp L4B05
L4AFC       sec
            sbc L0082
            sta L008B
            lda L008C
            sbc #$00
L4B05       sta L008C
            sta L0098
            lda L008B
            sta L0097
            lda #$40
            sta L008D
            sta L0099
            lda #$00
            sta L008E
            sta L009A
            lda L008A
            pha
            ora #$08
            sta L008A
            jsr L4C76
            pla
            ora #$02
            sta L008A
L4B28       jmp L4BBE
L4B2B       lda L0092
            bne L4B28
            lda L0091
            cmp #$40
            bcs L4B28
            lda L008F
            sta L0093
            lda L0090
            sta L0094
            lda L0091
            sta L0095
            lda L0092
            sta L0096
            jsr L4BE1
            lda L0093
            sta L0097
            lda L0094
            sta L0098
            lda L0095
            sta L0099
            lda L0096
            sta L009A
            ldy #$00
            lda L008B
            sec
            sbc L008F
            sta L0082
            lda L008C
            sbc L0090
            bpl L4B6F
            lda L008F
            sec
            sbc L008B
            sta L0082
            iny
L4B6F       lda L008D
            sec
            sbc L0091
            sta LOMEM
            lda #$40
            sec
            sbc L0091
            jsr L42AA
            jsr L42C0
            lda L008F
            dey
            beq L4B92
            clc
            adc L0082
            sta L008F
            lda L0090
            adc #$00
            jmp L4B9B
L4B92       sec
            sbc L0082
            sta L008F
            lda L0090
            sbc #$00
L4B9B       sta L0090
            sta L0094
            lda L008F
            sta L0093
            lda #$40
            sta L0091
            sta L0095
            lda #$00
            sta L0092
            sta L0096
            lda L008A
            pha
            ora #$02
            sta L008A
            jsr L4C76
            pla
            ora #$08
            sta L008A
L4BBE       lda L008B
            sta L0093
            lda L008C
            sta L0094
            lda L008D
            sta L0095
            lda L008E
            sta L0096
            lda L008F
            sta L0097
            lda L0090
            sta L0098
            lda L0091
            sta L0099
            lda L0092
            sta L009A
            jmp L4C76
L4BE1       ldy #$00
            sty L0082
            lda L0095
            sta LOMEM
            lda L0093
            ldx L0094
            bpl L4BF5
            iny
            eor #$FF
            clc
            adc #$01
L4BF5       lsr
            ror L0082
            jsr L42D3
            lda #$00
            sta L0094
            lda L0082
            beq L4C0D
            dey
            bne L4C0D
            eor #$FF
            clc
            adc #$01
            dec L0094
L4C0D       sta L0093
            lda #$40
            sta L0095
            lda #$00
            sta L0096
            rts
L4C18       ldy #$00
            sty L00A0
            lda #$40
            sta L00A1
            lda L0094
            bpl L4C32
            lda #$00
            sec
            sbc L0093
            sta L0093
            lda #$00
            sbc L0094
            sta L0094
            iny
L4C32       lda L0096
            beq L4C45
            lsr L0096
            ror L0095
            lsr L0094
            ror L0093
            lsr L00A1
            ror L00A0
            jmp L4C32
L4C45       lda L0095
            sta LOMEM
            lda #$00
            sta L0082
            lda L0093
            jsr L42D3
            lda #$80
            dey
            beq L4C60
            clc
            adc L0082
            bcc L4C63
            lda #$FF
            bne L4C63
L4C60       sec
            sbc L0082
L4C63       sta L0093
            lda L00A0
            sta L0082
            lda L00A1
            jsr L42D3
            lda #$80
            sec
            sbc L0082
            sta L0095
            rts
L4C76       lda L008A
            sta L009D
            lda L0093
            cmp L0097
            bne L4CA0
            lda L0094
            cmp L0098
            bne L4CA0
            lda L0095
            cmp L0099
            bne L4CA0
            lda L0096
            cmp L009A
            bne L4CA0
            jsr L4C18
            lda L0093
            sta L0097
            lda L0095
            sta L0099
            jmp L4CF6
L4CA0       lda L008A
            and #$F5
            sta L009D
            lda L008A
            and #$02
            beq L4CB2
            lda L009D
            ora #$08
            sta L009D
L4CB2       lda L008A
            and #$08
            beq L4CBE
            lda L009D
            ora #$02
            sta L009D
L4CBE       jsr L4C18
            ldx L0093
            lda L0097
            sta L0093
            lda L0098
            sta L0094
            stx L0097
            ldx L0095
            lda L0099
            sta L0095
            lda L009A
            sta L0096
            stx L0099
            jsr L4C18
            lda L0093
            cmp L0097
            bcc L4CF6
            lda L008A
            sta L009D
            ldx L0093
            lda L0097
            sta L0093
            stx L0097
            ldx L0095
            lda L0099
            sta L0095
            stx L0099
L4CF6       ldy #$13
            lda L00DE
            ldx L3F3F,Y
            bne L4D03
            inc L00DF
            sta (L00DE),Y
L4D03       ldy L00BD
            lsr L0093
            lsr L0093
            lsr L0097
            lsr L0097
            lda L0095
            lsr
            clc
            adc L0095
            lsr
            lsr
            sta L0095
            lda L0099
            lsr
            clc
            adc L0099
            lsr
            lsr
            sta L0099
L4D21       lda L009D
            sta L009E
L4D25       tya
            tax
            ldy L3E80,X
            bne L4D2D
            rts
L4D2D       lda L3C00,Y
            cmp L3D00,X
            beq L4D25
            cmp L0093
            bcc L4D25
            lda L3D00,X
            cmp L0093
            beq L4D90
            bcc L4D90
            cmp L0097
            beq L4D49
            bcc L4D49
            rts
L4D49       tya
            pha
            ldy #$00
            lda L0099
            sec
            sbc L0095
            bpl L4D5A
            eor #$FF
            clc
            adc #$01
            iny
L4D5A       sta L0082
            lda L0097
            sec
            sbc L0093
            sta LOMEM
            lda L3D00,X
            sec
            sbc L0093
            stx L009F
            jsr L42AA
            jsr L42C0
            ldx L009F
            lda L0095
            dey
            beq L4D7E
            clc
            adc L0082
            jmp L4D81
L4D7E       sec
            sbc L0082
L4D81       sta L0095
            lda L3D00,X
            sta L0093
            pla
            tay
            lda L009E
            ora #$20
            sta L009E
L4D90       lda L3C00,Y
            cmp L0097
            bcs L4DE1
            txa
            pha
            ldx #$00
            lda L0099
            sec
            sbc L0095
            bpl L4DA8
            eor #$FF
            clc
            adc #$01
            inx
L4DA8       sta L0082
            lda L0097
            sec
            sbc L0093
            sta LOMEM
            txa
            pha
            lda L3C00,Y
            sec
            sbc L0093
            jsr L42AA
            jsr L42C0
            pla
            tax
            lda L0095
            dex
            beq L4DCC
            clc
            adc L0082
            jmp L4DCF
L4DCC       sec
            sbc L0082
L4DCF       sta L009C
            lda L3C00,Y
            sta L009B
            pla
            tax
            lda L009E
            ora #$40
            sta L009E
            jmp L4DE9
L4DE1       lda L0097
            sta L009B
            lda L0099
            sta L009C
L4DE9       lda L00C0
            bne L4DF0
L4DED       jmp L4DED
L4DF0       sta L3E80,X
            tax
            lda L0093
            sta L3C00,X
            lda L0095
            sta L3C80,X
            lda L009B
            sta L3D00,X
            lda L009C
            sta L3D80,X
            lda L009E
            sta L3E00,X
            lda L3E80,X
            sta L00C0
            tya
            sta L3E80,X
            lda L0093
            cmp L009B
            beq L4E25
            lda L009E
            and #$10
            bne L4E25
            jsr L4E30
L4E25       lda L3C00,Y
            cmp L0097
            bcs L4E2F
            jmp L4D21
L4E2F       rts
L4E30       txa
            pha
            tya
            pha
            lda #$14
            ldy L00FE
            bne L4E42
            lda #$04
            ldy L00E0
            beq L4E42
            lda #$10
L4E42       ldy L00D5
            beq L4E47
            asl
L4E47       sta L0083
            lda L00A3
            beq L4E80
            cmp #$11
            bcs L4E80
            lda L00A2
            beq L4E80
            cmp #$11
            bcs L4E80
            adc #$FF
            asl
            asl
            asl
            asl
            dec L00A3
            ora L00A3
            inc L00A3
            tax
            lda L0600,X
            and L0083
            bne L4E80
            lda L0083
            ora L0600,X
            sta L0600,X
            lda L00FE
            beq L4E80
            lda #$AA
            sta L00B1
            jsr L5EBD
L4E80       pla
            tay
            pla
            tax
            rts
L4E85       ldx L00BD
L4E87       lda L3E80,X
            beq L4E90
            tax
            jmp L4E87
L4E90       lda L00C0
            sta L3E80,X
            lda L00BD
            sta L00C0
            rts
L4E9A       txa
            pha
            ldx L00A5
            lda L00A7
            stx L00A7
            sta L00A5
            ldx L00A6
            lda L00A8
            stx L00A8
            sta L00A6
            pla
            tax
            rts
L4EAF       lda #$00
            sta L00AE
            lda L00A6
            asl
            asl
            asl
            rol L00AE
            asl
            rol L00AE
            asl
            rol L00AE
            sta L0083
            lda L00A5
            lsr
            lsr
            ora L0083
            ldy L00E0
            beq L4ECE
            ora #$10
L4ECE       sta L00AD
            lda L00AE
            clc
            adc L00A4
            sta L00AE
            lda L00A5
            and #$03
            tay
            lda L4EE2,Y
            sta L00AC
            rts
L4EE2       .byte $3F,$CF,$F3,$FC
L4EE6       tya
            pha
            jsr L4EAF
            ldy #$00
            lda (L00AD),Y
            eor L00B1
            and L00AC
            eor L00B1
            sta (L00AD),Y
            pla
            tay
            rts
L4EFA       stx L00A5
            sty L00A6
L4EFE       lda L00A6
            cmp #$40
            bcs L4F1B
            lda L00A4
            pha
            lda L00E0
            pha
            lda #$00
            sta L00E0
            lda #$08
            sta L00A4
            jsr L4EE6
            pla
            sta L00E0
            pla
            sta L00A4
L4F1B       rts
L4F1C       lda L00B1
            cmp #$FF
            bne L4F23
            rts
L4F23       txa
            pha
            tya
            pha
            lda #$00
            sta L00AB
            lda L00A8
            cmp L00A6
            bcs L4F34
            jsr L4E9A
L4F34       jsr L4EAF
            ldy #$00
            lda L00AC
            ora (L00AD),Y
            cmp #$FF
            bne L4F49
            lda L00AC
            ora #$AA
            and (L00AD),Y
            sta (L00AD),Y
L4F49       lda L00A8
            sec
            sbc L00A6
            sta L00AA
            ldx L00A5
            lda L00A7
            sec
            sbc L00A5
            beq L4FA7
            bcc L4FAC
            sta L00A9
            jmp L4F6D
L4F60       inx
            cpx L00A7
            beq L4F97
            lda L00AC
            ora L00B1
            and (L00AD),Y
            sta (L00AD),Y
L4F6D       sec
            ror L00AC
            ror L00AC
            bcs L4F7A
            inc L00AD
            lda #$3F
            sta L00AC
L4F7A       lda L00AB
            clc
            adc L00AA
L4F7F       sta L00AB
            cmp L00A9
            bcc L4F60
            sbc L00A9
            pha
            lda L00AD
            clc
            adc #$20
            sta L00AD
            bcc L4F93
            inc L00AE
L4F93       pla
            jmp L4F7F
L4F97       lda L00AC
            ora (L00AD),Y
            cmp #$FF
            bne L4FA7
            lda L00AC
            ora #$AA
            and (L00AD),Y
            sta (L00AD),Y
L4FA7       pla
            tay
            pla
            tax
            rts
L4FAC       eor #$FF
            adc #$01
            sta L00A9
            jmp L4FC2
L4FB5       dex
            cpx L00A7
            beq L4F97
            lda L00AC
            ora L00B1
            and (L00AD),Y
            sta (L00AD),Y
L4FC2       sec
            rol L00AC
            rol L00AC
            bcs L4FCF
            dec L00AD
            lda #$FC
            sta L00AC
L4FCF       lda L00AB
            clc
            adc L00AA
L4FD4       sta L00AB
            cmp L00A9
            bcc L4FB5
            sbc L00A9
            pha
            lda L00AD
            clc
            adc #$20
            sta L00AD
            bcc L4FE8
            inc L00AE
L4FE8       pla
            jmp L4FD4
L4FEC       lda #$FF
L4FEE       ldy L00A4
            sty L00AE
            ldy #$00
            sty L00AD
            ldx #$06
L4FF8       sta (L00AD),Y
            iny
            bne L4FF8
            inc L00AE
            dex
            bne L4FF8
            rts
L5003       ldy L00BD
            ldx L3E80,Y
            beq L500F
            lda L3E80,X
            bne L5010
L500F       rts
L5010       lda #$00
            sta L00B4
            sta L00B2
            lda #$55
            sta L00B1
            lda #$2F
            sta L00B5
            sta L00B3
L5020       lda L00B4
            cmp L3C00,X
            beq L5046
            sta L00A5
            lda L00B3
            cmp L00B5
            beq L5038
            sta L00A6
            lda #$AA
            sta L00B1
            jsr L4EE6
L5038       lda #$55
            sta L00B1
            lda #$00
            sta L00B2
            lda #$2F
            sta L00B5
            sta L00B3
L5046       lda L3C00,X
            cmp L3D00,X
            beq L5053
            lda L3E80,X
            bne L5056
L5053       jmp L50B5
L5056       lda L00B4
            sta L00A5
            lda L00B3
            sta L00A6
            cmp L3C80,X
            beq L5065
            bcs L5083
L5065       cmp L00B5
            beq L5070
            lda #$AA
            sta L00B1
            jmp L5080
L5070       cmp L3C80,X
            bne L5083
            lda L00B2
            beq L5083
            lda L3E00,X
            and #$02
            beq L5083
L5080       jsr L4EE6
L5083       lda #$55
            sta L00B1
            jsr L50ED
            lda L3C00,X
            sta L00A5
            lda L3C80,X
            sta L00A6
            lda L3D00,X
            sta L00A7
            sta L00B4
            lda L3D80,X
            sta L00A8
            sta L00B5
            sta L00B3
            jsr L4F1C
            lda #$00
            sta L00B2
            lda L3E00,X
            and #$08
            sta L00B2
            jmp L50D8
L50B5       lda L3D00,X
            sta L00B4
            lda L3C80,X
            cmp L00B3
            beq L50C3
            bcs L50C9
L50C3       sta L00B3
            lda #$00
            sta L00B2
L50C9       lda L3D80,X
            cmp L00B3
            beq L50D2
            bcs L50D8
L50D2       sta L00B3
            lda #$00
            sta L00B2
L50D8       lda L3E80,X
            tax
            beq L50E1
            jmp L5020
L50E1       nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            rts
L50ED       lda L3E00,X
            and #$01
            beq L50F9
            lda #$00
            sta L00B1
            rts
L50F9       lda L3E00,X
            and #$04
            beq L5104
L5100       lda #$AA
            sta L00B1
L5104       rts
L5105       lda #$FF
            sta CRITIC
            lda L00A4
            cmp #$30
            bne L5119
            lda #$00
            sta SDLSTL
            lda #$10
            jmp L5120
L5119       lda #$00
            sta SDLSTL
            lda #$11
L5120       sta SDLSTH
            lda #$21
            sta SDMCTL
            lda #$00
            sta L00BC
            sta CRITIC
L512E       lda L00BC
            beq L512E
            lda L00A4
            eor #$06
            sta L00A4
            rts
L5139       lda L00D7
            clc
            adc #$20
            sta L00A2
            lda L00D8
            adc #$00
            rol L00A2
            rol
            rol L00A2
            rol
            sta L00A2
            lda L00DA
            clc
            adc #$20
            sta L00A3
            lda L00DB
            adc #$00
            rol L00A3
            rol
            rol L00A3
            sta L00A3
            lda #$00
            sta L00D5
            bcs L5181
            lsr L00A2
L5166       bcs L5179
            jsr L51A7
            inc L00D5
            jsr L51A7
            inc L00A3
            jsr L51A7
            dec L00D5
            dec L00A3
L5179       inc L00A2
            jsr L51A7
            jmp L518C
L5181       lsr L00A2
            bcs L518C
            inc L00D5
            inc L00A3
            jsr L51A7
L518C       lda L00D7
            sta L00E2
            lda L00D8
            sta L00E3
            lda L00D9
            sta L00EB
            lda L00DA
            sta L00E4
            lda L00DB
            sta L00E5
            lda L00DC
            sta L00EC
            lda #$00
            rts
L51A7       jsr L483F
            beq L51B0
            pla
            pla
            lda #$01
L51B0       rts
            txa
            pha
            ldx #$23
            lda L3F35,X
            bne L51F9
            inc L3F35,X
            pha
            pha
            tsx
            lda L0103,X
            sta L0101,X
            clc
            adc #$DF
            sta L0084
            sta L0107,X
            lda L0105,X
            sta L0103,X
            lda #$65
            sta L00AD
            tya
            clc
            adc #$A1
            sta L00AE
            tya
            clc
            adc #$CB
            sta L0083
            sta L0106,X
            lda L0101,X
            clc
            adc #$E7
            sta L0105,X
            tya
            clc
            adc #$9C
            ldy #$71
            sta L0104,X
L51F9       pla
            tax
            jmp L413B
L51FE       .byte $03,$3F,$3F,$3F,$03,$03,$33,$03,$33,$33,$03,$33,$03,$3F,$3F,$03
            .byte $CF,$CF,$CF,$CF,$33,$33,$33,$33,$03,$03,$33,$03,$0F,$33,$03,$3F
            .byte $0F,$3F,$03,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$03,$CF,$CF
            .byte $CF,$CF,$33,$33,$03,$33,$33,$03
L5236       .byte $3F,$0F,$3F,$03,$FF,$FF,$FF,$FF,$FF,$03,$3F,$0F,$3F,$3F,$3F
L5245       .byte $3F,$3F,$3F,$03,$03,$33,$03,$33,$33,$00,$3F
            bmi L528E
            .byte $00,$03,$33,$33,$33,$03,$0F,$CF,$CF,$CF,$03,$03,$F3,$03,$3F,$03
            .byte $03,$F3,$C3,$F3,$03,$33,$33,$03,$F3,$F3,$03,$3F,$03,$F3,$03,$03
            .byte $3F,$03,$33,$03,$03,$F3,$F3,$F3,$F3,$03,$33,$03,$33,$03,$03,$33
            .byte $03,$F3,$F3
            sty L0000
            .byte $A7
            tay
            lsr L00FC
            bpl L5236
            .byte $73
L528E       .byte $FC
            inc FKDEF+1
            rol
            .byte $F4
            ror L97CE
            .byte $14
            lda L6C85,Y
            cmp CASINI+1,X
L529C       bmi L5245
            .byte $F4,$EB
            sec
            .byte $9C
            ora BUFCNT
            .byte $FF,$04
            ldx #$54
            cpy ICDNOZ
            adc L00A1
            .byte $2F
            inc L1482,X
            ldx #$70
            bne L531B
            .byte $DC,$37
            eor LFDF4,Y
            sta LCA63,X
            .byte $1F
            jmp L2323
            .byte $52
            lda (DSTAT,X)
            lda LF103
            eor L529C
            .byte $74,$9B
            eor L816D,X
            bit L7ABF
            .byte $17
            jmp L47AA
            ldy COLRSH,X
            lda (STATUS),Y
            and (L009B),Y
            .byte $33,$BF,$33
            and (L009B),Y
            rol L00DC,X
            eor L00FC
            .byte $5A
            cpy LA654
            beq L535A
            dec L727E
            .byte $8B
            bmi L52F6+1
            .byte $14
            dec L00A1
            sbc (L00F1,X)
            .byte $17
L52F6       lda #$05
            sta L0085
            lda #$80
            sta L0083
            lda L00A4
            sta L0084
L5302       lda L51FE,X
            and (L0083),Y
            sta (L0083),Y
            lda L51FE,X
            rol
            ora L51FE,X
            and #$AA
            eor #$AA
            ora (L0083),Y
            sta (L0083),Y
            inx
            tya
            clc
L531B       adc #$20
            tay
            dec L0085
            bne L5302
            sec
            sbc #$9F
            tay
            rts
L5327       ldx #$00
            ldy #$48
L532B       jsr L52F6
            cpy #$59
            bcc L532B
            ldy #$02
L5334       ldx L3F3E,Y
            jsr L52F6
            cpy #$06
            bcc L5334
            ldy #$1A
L5340       ldx L3F2A,Y
L5343       jsr L52F6
            cpy #$1E
            bcc L5340
            rts
L534B       lda L3F40,X
            clc
            adc #$05
L5351       cmp #$87
            bne L535E
            lda #$55
            sta L3F40,X
L535A       dex
            bpl L534B
            rts
L535E       sta L3F40,X
            lda #$00
            sta L3F57
            lda #$DD
            sta L3F4F
            lda #$E6
            sta L3F51
            lda #$01
            sta L3F4E
            sta L3F50
            sta L3F57
            lda RTCLOK+2
            clc
            adc #$7A
L5380       cmp RTCLOK+2
            bne L5380
            lda #$00
            sta L3F52
            sta CAUX1
            rts
L538D       lda #$55
            ldx #$07
L5391       sta L3F40,X
            dex
            bpl L5391
            rts
            jsr L4FEC
            rts
L539C       .byte $00
L539D       ora (NGFLAG,X)
            ora (L0000,X)
            .byte $FF,$FF,$FF,$00
L53A5       ora (L0085,X)
            lda (L00A5),Y
            sbc (DSKFMS,X)
            adc #$10
            lsr
            lsr
            lsr
            lsr
            lsr
            tax
            lda L00E4
            clc
            adc #$60
            sta L0083
            lda L00E5
            adc #$00
            asl L0083
            rol
            asl L0083
            rol
            ldy L0083
            bne L53CA
            and #$FE
L53CA       asl
            clc
            adc #$16
            sta L00A5
            sta L00A7
            lda L00E2
            clc
            adc #$60
            sta L0083
            lda L00E3
            adc #$00
            asl L0083
            rol
            asl L0083
            rol
            ldy L0083
            bne L53E9
            and #$FE
L53E9       asl
            sec
            sbc #$02
            sta L00A6
            sta L00A8
            lda #$02
            jsr L541D
            lda L00A7
            clc
            adc L539D+1,X
            sta L00A5
            lda L00A8
            clc
            adc L539C,X
            sta L00A6
            lda #$03
            jsr L541D
            lda L00A7
            sec
            sbc L539D+1,X
            sta L00A5
            lda L00A8
            sec
            sbc L539C,X
            sta L00A6
            lda #$01
L541D       ldy L00B1
            eor L00DD
            and #$03
            bne L5429
            lda #$FF
            sta L00B1
L5429       jsr L4EFE
            sty L00B1
            rts
L542F       jsr L4FEC
            ldx #$0F
L5434       lda L3F30,X
            sta L00E1,X
            dex
            bpl L5434
            ldx #$03
L543E       lda L3F21,X
            sta L00F1,X
            dex
            bpl L543E
            lda #$01
            sta L00E0
            lda #$FF
            jsr L53A5+1
            jsr L565C
            lda #$00
            jsr L53A5+1
            ldx #$0F
L5459       lda L00E1,X
            sta L3F30,X
            lda L3F20,X
            sta L00E1,X
            dex
            bpl L5459
            ldx #$03
L5468       lda L3F31,X
            sta L00F1,X
            dex
            bpl L5468
            lda #$00
            sta L00E0
            lda #$FF
            jsr L53A5+1
            jsr L565C
            lda #$55
            jsr L53A5+1
            jsr L4368
            jsr L5003
            jsr L4E85
            lda #$01
            sta L00E0
            ldx #$0F
L5490       lda L00E1,X
            sta L3F20,X
            lda L3F30,X
            sta L00E1,X
            dex
            bpl L5490
            ldx #$03
L549F       lda L3F21,X
            sta L00F1,X
            dex
            bpl L549F
            jsr L4368
            jsr L5003
            jsr L4E85
            jsr L5A3B
            jsr L5327
            jsr L5105
            inc L00DD
            lda L00DD
            and #$07
            sta L00DD
            tay
            lda L55AC,Y
            sta L00BA
            ldx #$02
L54C9       lda L3F22,X
            cmp L3F32,X
            bne L54E0
            lda L3F21,X
            eor L3F31,X
            bmi L54E0
            dex
            dex
            bpl L54C9
            jmp L5549
L54E0       ldx #$02
L54E2       lda L3F21,X
            sec
            sbc L3F31,X
            sta L00F5,X
            lda L3F22,X
            sbc L3F32,X
            sta L00F6,X
            bpl L5502
            lda #$00
            sec
            sbc L00F5,X
            sta L00F5,X
            lda #$00
            sbc L00F6,X
            sta L00F6,X
L5502       dex
            dex
            bpl L54E2
            ldx L00F8
            lda L00F7
            cpx L00F6
            bcc L5518
            bne L5514
            cmp L00F5
            bcc L5518
L5514       stx L00F6
            sta L00F5
L5518       ldx #$23
            ldy #$00
            lda L00FD
            bmi L5539
            ldx #$66
            ldy #$02
            lda L00F6
            cmp #$02
            bcc L5539
            lda L3F24
            cmp #$07
            bcs L5539
            dey
            cmp #$03
            bcs L5539
            ldx #$23
            dey
L5539       stx L3F54
            sty L3F55
            lda L00F6
            bne L554D
            lda L00F5
            cmp #$3F
            bcs L554D
L5549       ldx #$07
            bne L55A9
L554D       lda L3F24
            cmp #$07
            bcc L555D
            bne L55A4
            lda L3F23
            cmp #$E0
            bcs L55A4
L555D       lda #$01
            jsr L55B3+1
            bne L556B
            sta L00FD
            sta ATRACT
            jmp L6117
L556B       lda #$04
            jsr L55B3+1
            beq L5582
            lda CH
            cmp #$FF
            beq L5591
            ldx #$FF
            stx CH
            cmp #$1C
            bne L5591
L5582       lda L00FD
            eor #$80
            sta L00FD
            bmi L558E
            lda #$00
            sta ATRACT
L558E       jmp L542F
L5591       lda #$02
            jsr L55B3+1
            bne L55A1
            lda L00FE
            eor #$FF
            sta L00FE
            jsr L5EE7
L55A1       jmp L542F
L55A4       jsr L6208
            ldx #$03
L55A9       jmp L534B
L55AC       dec L0000
            .byte $00
            cpy.a L00CE
            .byte $00
L55B3       cpy L0085
            .byte $83
            lda #$08
            sta CONSOL
            lda CONSOL
            and L0083
            bne L55CD
L55C2       lda CONSOL
            and L0083
            beq L55C2
            lda #$00
            sta ATRACT
L55CD       rts
L55CE       ldx #$0F
            lda #$00
L55D2       sta L3F20,X
            sta L3F30,X
            dex
            bpl L55D2
            lda RANDOM
            and #$80
            ora #$40
            sta L3F21
            lda RANDOM
            and #$07
            sta L3F22
            lda #$40
            sta L3F23
            lda #$00
            sta L3F24
            lda RANDOM
            and #$80
            ora #$40
            sta L3F31
            lda RANDOM
            and #$07
            sta L3F32
            lda #$C0
            sta L3F33
            lda #$07
            sta L3F34
            lda RANDOM
            lsr
            lsr
            sec
            sbc #$20
            sta L3F20
            sta L3F27
            lda RANDOM
            lsr
            lsr
            sec
            sbc #$A0
            sta L3F30
            sta L3F37
            lda #$00
            sta L3F3C
            lda #$FF
            sta L3F2C
            lda #$01
            sta L00C0
            ldy #$00
L563F       sta L3E80,Y
            clc
            adc #$01
            iny
            cpy #$7F
            bne L563F
            lda #$00
            sta L3E80,Y
            sta L00DD
            lda #$85
            sta L3F26
            sta L3F36
            jmp L542F
L565C       ldx L00E0
            ldy L00FD
            bpl L5663
            rts
L5663       cpy #$1E
            beq L5696
            cpx #$01
            bne L5677
            cpy #$32
            beq L5683
            jsr L58F4
            sta L0083
            jmp L56C3
L5677       cpy #$1F
            beq L5683
            jsr L599C
            sta L0083
            jmp L56C3
L5683       lda STICK0
            and STICK1
            and #$0F
            sta L0083
	    lda STRIG0
            and STRIG1
            jmp L56A0
L5696       lda STICK0,X
            and #$0F
        sta L0083
            lda STRIG0,X
L56A0       bne L56C3
            ldx L0083
            lda L56B3,X
            cmp #$FF
            beq L56B0
            sta L00E8
            jmp L5732
L56B0       jmp L5745
L56B3       .byte $FF,$FF,$FF,$FF,$FF
            jsr L00E0
            .byte $FF
            rts
            ldy #$80
            .byte $FF
            rti
            cpy #$FF
L56C3       ldx #$FF
            lda L0083
            beq L5732
            cmp #$0D
            bne L56D7
            lda L00E1
            eor #$80
            cmp L00E8
            beq L5732
            bne L5747
L56D7       and #$04
            beq L56E9
            ldx #$01
            lda L0083
            and #$08
            beq L56E9
            lda #$00
            sta L00EA
            beq L56F2
L56E9       stx L00EA
            txa
            eor L00E9
            bpl L56F4
            lda #$00
L56F2       sta L00E9
L56F4       lda L00E9
            clc
            adc #$08
            cmp #$11
            bcc L570D
            lda L00E7
            cmp #$05
            bcs L5713
            lda #$04
            sta L00E7
            lda #$00
            sta L00E6
            beq L5713
L570D       lda L00E9
            adc L00EA
            sta L00E9
L5713       lda L00E9
            sta L0084
            lda L0083
            and #$02
            bne L571F
            asl L0084
L571F       lda L00E1
            cmp L00E8
            bne L5745
            clc
            adc L0084
            sta L00E1
            sta L00E8
            lda L0083
            and #$01
            bne L573B
L5732       lda L00E7
            and #$7F
            sta L00E7
            jmp L5751
L573B       lda L0083
            cmp #$0F
            beq L5745
            and #$02
            bne L5751
L5745       lda L00E1
L5747       sta L00E8
            lda #$00
            sta L00E6
            lda #$85
            sta L00E7
L5751       lda L00E7
            bpl L5756
            rts
L5756       cmp #$01
            beq L5764
            lda L00E6
            and #$07
            bne L5762
            dec L00E7
L5762       inc L00E6
L5764       lda L00E4
            sta L00DA
            lda L00E5
            sta L00DB
            lda L00EC
            sta L00DC
            lda L00E8
            jsr L42E9
            jsr L57A3
            jsr L57E3
            jsr L57C1
            bne L5783
            jsr L57E3
L5783       lda L00E2
            sta L00D7
            lda L00E3
            sta L00D8
            lda L00EB
            sta L00D9
            lda L00E8
            jsr L42E6
            jsr L57A3
            jsr L5802
            jsr L57C1
            bne L57A2
            jsr L5802
L57A2       rts
L57A3       ldx #$00
            stx L00FB
            sta L00FA
            tay
            bpl L57AD
            dex
L57AD       stx L00F9
            ldx L00E7
            cpx #$01
            bne L57B6
            inx
L57B6       lda L00F9
            ror
            ror L00FA
            ror L00FB
            dex
            bne L57B6
            rts
L57C1       ldx L00E7
            bne L57CE
            lda L00E5
            cmp #$10
            rol
            eor L00E0
            and #$01
L57CE       rts
L57CF       clc
            lda L00EB
            adc L00FB
            sta L00D9
            lda L00E2
            adc L00FA
            sta L00D7
            lda L00E3
            adc L00F9
            sta L00D8
            rts
L57E3       jsr L57CF
            jsr L5139
            ora L00EF
            sta L00EF
            rts
L57EE       clc
            lda L00EC
            adc L00FB
            sta L00DC
            lda L00E4
            adc L00FA
            sta L00DA
            lda L00E5
            adc L00F9
            sta L00DB
            rts
L5802       jsr L57EE
            jsr L5139
            ora L00EF
            sta L00EF
            rts
L580D       sta L0085
            ldx #$00
            lda L00EE
            cmp L00ED
            beq L581B
            cmp L00FC
            bne L581E
L581B       tax
            tay
L581D       rts
L581E       lda L0600,X
            and #$7F
            sta L0600,X
            inx
            bne L581E
            ldx L0084
            lda L0600,X
            ora #$80
            sta L0600,X
            ldx L00EE
            lda L0600,X
            ora #$80
            sta L0600,X
            ldy L00ED
            txa
            sta L0700,Y
L5843       sty L0083
            cpx L0083
            beq L581D
            lda L0700,Y
            tay
            and #$0F
            beq L5868
            lda L05FF,Y
            bmi L5868
            and L0085
            bne L5868
            lda L05FF,Y
            ora #$80
            sta L05FF,Y
            dey
            tya
            iny
            jsr L58C7
L5868       tya
            and #$0F
            cmp #$0F
            beq L5886
            lda L0600,Y
            and L0085
            bne L5886
            lda L0601,Y
            bmi L5886
            ora #$80
            sta L0601,Y
            iny
            tya
            dey
            jsr L58C7
L5886       tya
            and #$F0
            beq L58A4
            lda L05F0,Y
            bmi L58A4
            ror
            and L0085
            bne L58A4
            lda L05F0,Y
            ora #$80
            sta L05F0,Y
            tya
            sec
            sbc #$10
            jsr L58C7
L58A4       tya
            and #$F0
            cmp #$F0
            beq L58C4
            lda L0600,Y
            ror
            and L0085
            bne L58C4
            lda L0610,Y
            bmi L58C4
            ora #$80
            sta L0610,Y
            tya
            clc
            adc #$10
            jsr L58C7
L58C4       jmp L5843
L58C7       cmp L00ED
            beq L58D4
            cmp L00FC
            beq L58D4
            sta L0700,X
            tax
            rts
L58D4       tax
            pla
            pla
            rts
L58D8       lda L00E4,Y
            rol
            lda L00E5,Y
            rol
            and #$0F
            sta L0083
            lda L00E2,Y
            rol
            lda L00E3,Y
            ror
            ror
            ror
            ror
            and #$F0
            ora L0083
            rts
L58F4       ldy #$00
            jsr L58D8
            ldx L00EF
            bne L5901
            cmp L00ED
            beq L5915
L5901       sta L00ED
            sta L00FC
            ldy #$0F
            jsr L58D8
            sta L00EE
            sta L0084
            lda #$10
            jsr L580D
            sty L00EE
L5915       lda L00F0
            ora L00EF
            sta L00F0
            lda #$00
            sta L00EF
            lda L00EE
            sec
            sbc L00ED
            ldx #$00
            ldy L00E2
            cmp #$01
            beq L5940
            ldx #$40
            ldy L00E4
            cmp #$10
            beq L5940
            ldx #$80
            ldy L00E2
            cmp #$FF
            beq L5940
            ldx #$C0
            ldy L00E4
L5940       tya
            and #$7F
            sec
            sbc #$40
            sta L0083
            txa
            cmp #$C0
            bmi L5953
            sec
            sbc L0083
            jmp L5956
L5953       clc
            adc L0083
L5956       sta L0083
            sec
            sbc L00E1
            ldx L00F0
            bne L5975
            cmp #$91
            bcs L596A
            cmp #$70
            bcc L596A
            lda #$0D
            rts
L596A       ldx L00E0
            bne L5975
            lda L0083
            sta L00E8
            lda #$00
            rts
L5975       cmp #$10
            bcc L5995
            cmp #$F1
            bcs L5995
            ldx #$FE
            cmp #$20
            bcc L5989
            cmp #$E1
            bcs L5989
            ldx #$FD
L5989       cmp #$00
            bpl L5991
            txa
            and #$FB
            rts
L5991       txa
            and #$F7
            rts
L5995       lda #$00
            sta L00F0
            lda #$FE
            rts
L599C       ldy #$00
            jsr L58D8
            cmp L3F48
            beq L59E7
            cmp L3F49
            beq L59E7
            ldx L00EF
            bne L59B3
            cmp L00ED
            beq L59E4
L59B3       sta L00EE
            sta L0084
            lda L3F48
            sta L00ED
            lda L3F49
            sta L00FC
            ldy #$0F
            jsr L58D8
            sta L0084
            lda #$04
            jsr L580D
            lda L00EE
            sta L00ED
            sta L00FC
            stx L00EE
            cpy L00EE
            bne L59DD
            lda #$FF
            sta L00F0
L59DD       lda #$04
            jsr L580D
            sty L00EE
L59E4       jmp L5915
L59E7       ldx #$00
            ldy L00E2
            lda #$FF
            sta L00F0
            jmp L5940
L59F2       lda #$00
            sta L00AD
            lda #$12
            sta L00AE
            lda #$00
            sta L0083
            lda #$30
            sta L0084
            jsr L5A05
L5A05       ldx #$30
            ldy #$00
L5A09       lda #$3D
            jsr L5A2E
            lda #$9D
            jsr L5A2E
            lda L0083
            clc
            adc #$20
            sta L0083
            lda L0084
            adc #$00
            sta L0084
            dex
            bne L5A09
            lda #$60
L5A25       sta (L00AD),Y
            inc L00AD
            bne L5A2D
            inc L00AE
L5A2D       rts
L5A2E       jsr L5A25
            lda L0083
            jsr L5A25
            lda L0084
            jmp L5A25
L5A3B       ldx #$1F
            lda L00A4
            cmp #$30
            bne L5A4C
L5A43       lda #$FF
            jsr L1200
            dex
            bpl L5A43
            rts
L5A4C       lda #$FF
            jsr L1321
            dex
            bpl L5A4C
            rts
L5A55       lda (L00AD),Y
            eor (L0083),Y
            sta (L0083),Y
            dey
            bne L5A55
            rts
L5A5F       .byte $3C
            plp
            .byte $00,$3C
            eor (ICBAHZ),Y
            plp
            and L2A00
            .byte $FF
            adc L5179,Y
            .byte $00
            adc L6679,Y
            adc L5166,Y
            .byte $3C
            and BUFRLO,X
            .byte $00,$3C,$00
            jmp L5100
            .byte $00
            eor HOLD1,X
            .byte $FF,$00
            adc LA280,Y
            sta LA2AD,Y
            cmp LF3CC,Y
            cpy LA2A2
            cpy L9980
            lda L79A2
            ror HOLD1
            adc L7966,Y
            lda LF3A2
            cpy LCCD9
            .byte $F3,$F3
            ldx #$A2
            ldx #$FF
            .byte $3C
            eor (NOCKSM),Y
            and BUFRLO,X
            .byte $3C,$32
            and L2825
            and L2D32
            plp
            .byte $00,$FF
            ora L1D1D,X
            .byte $3C,$32,$32,$32,$32,$32,$32,$32,$32,$32
            eor (FREQ),Y
            .byte $FF,$F3
            cpy LA2D9
            .byte $F3
            cmp LF3CC,Y
            adc L9988,Y
            ldx #$99
            cpy LADA2
            ldx #$F3
            ldx #$F3
            adc LB688,Y
            ldx #$66
            adc LA2B6,Y
            ldx #$B6
            ldx #$99
            .byte $80,$FF
            eor (COLCRS),Y
            eor (NOCKSM),Y
            .byte $32
            and NOCKSM,X
            rti
            .byte $3C,$32
            plp
            .byte $3C,$00,$3C
            rti
            plp
            plp
            .byte $00,$3C
            rti
            and ICBLLZ
            and ICDNOZ
            and HOLD1
            .byte $3C,$3C,$FF,$3C
            and BUFRLO,X
            and BUFRLO,X
            and BUFRLO,X
            and BUFRLO,X
            and L3C28
            .byte $32,$3C
            jmp LFF51
            .byte $3C
            plp
            .byte $3C
            and.w ICAX4Z
            plp
            .byte $3C
            and.w HOLD1
            eor (HOLD1),Y
            jmp L4851
            eor (FMSZPG+1),Y
            eor (FREQ),Y
            eor (NOCKSM),Y
            .byte $44,$00,$FF,$3C
            and NOCKSM,X
            and NOCKSM,X
            and BUFRLO,X
            and L2528
            .byte $23
            and (ABUFPT+3,X)
            .byte $FF,$00,$3C,$00,$3C,$00,$3C,$3C,$00
            ora L7900,X
            dey
            sta LB6A2,Y
            cpy.a L00F3
            .byte $F3
L5B4F       jsr L0820
            bpl L5B5C
            php
            bpl L5B97
            php
            rti
            .byte $FF
            clc
            php
L5B5C       jsr L0408
            .byte $04,$04,$04,$04,$04
            clc
            php
            php
            php
            php
            php
            php
            php
            php
            php
            jsr LFF20
            php
            bpl L5B84
            bpl L5B86
            bpl L5B88
            bpl L5B8A
            bpl L5B8C
            .byte $0C,$04
            bpl L5B90
            bpl L5B91+1
            bpl L5B93+1
L5B84       bpl L5B96
L5B86       bpl L5B98
L5B88       bpl L5B9A
L5B8A       bpl L5B9C
L5B8C       bpl L5B9E
            bpl L5B9C
L5B90       .byte $04
L5B91       bpl L5BA2+1
L5B93       bpl L5B93+1
            .byte $04
L5B96       .byte $04
L5B97       .byte $04
L5B98       .byte $04,$04
L5B9A       .byte $04,$04
L5B9C       .byte $04
            php
L5B9E       clc
            .byte $0C,$04
            php
L5BA2       jsr LFF08
            bpl L5BB7
            bpl L5BB9
            .byte $04,$04,$04,$04,$04,$04,$04,$04
            php
            php
L5BB3       bpl L5BB3+1
            php
            php
L5BB7       php
            php
L5BB9       php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            asl CASINI
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            php
            .byte $FF
            asl CASINI
            php
            php
            asl CASINI
            asl CASINI
            asl CASINI
            asl CASINI
            php
            asl CASINI
            asl CASINI
            php
            asl CASINI
            asl CASINI
            asl CASINI
            asl CASINI
            asl CASINI
            .byte $FF
            bpl L5C06
            bpl L5C08
            bpl L5C0A
            bpl L5C0B+1
            bpl L5C0D+1
            bpl L5C10
            bpl L5C12
            bpl L5C14
            .byte $FF
            php
L5C06       asl CASINI
L5C08       asl CASINI
L5C0A       php
L5C0B       asl CASINI
L5C0D       asl CASINI
            php
L5C10       asl CASINI
L5C12       asl CASINI
L5C14       asl CASINI
            asl CASINI
            asl CASINI
            asl CASINI
            clc
            .byte $FF,$0C,$0C,$0C,$0C
            php
            php
            jsr L2020
            jsr L1020
L5C2A       bpl L5C2A+1
            ora (WARMST,X)
            bpl L5C38
            php
            .byte $04
            php
            .byte $04,$FF
            ora (WARMST,X)
            php
L5C38       php
            php
            php
            asl TRAMSZ
            .byte $04,$FF
            lda #$3E
            sta VDSLST
            lda #$41
            sta VDSLST+1
            lda #$C0
            sta NMIEN
            jsr L5C7A
            lda L00FD
            bmi L5C69
            lda STICK0
            and STICK1
            cmp #$0F
            bne L5C67
            lda STRIG0
            and STRIG1
            bne L5C6B
L5C67       lda #$00
L5C69       sta ATRACT
L5C6B       jmp SYSVBV
            lda #$FF
            sta L00BC
            lda #$00
            sta SRTIMR
            jmp XITVBV
L5C7A       lda RTCLOK+2
            and L3F57
            ror
            bcc L5CB9
            lda RANDOM
            sta L00DE
            lda RANDOM
            sta L00DF
            ldx #$00
            stx L3F53
            jsr L5CBA
            ldx #$02
            jsr L5CBA
            lda L3F53
            beq L5CAD
            lda L3F4A
            sta AUDF1
            lda L3F4C
            sta AUDF2
            sta STIMER
L5CAD       lda L3F4B
            sta AUDC1
            lda L3F4D
            sta AUDC2
L5CB9       rts
L5CBA       lda L3F4B,X
            cmp #$A4
            bcc L5CC4
            dec L3F4B,X
L5CC4       dec L3F4E,X
            bne L5CE9
L5CC9       ldy L3F4F,X
            lda L5B4F,Y
            sta L3F4E,X
            lda L5A5F,Y
            beq L5CEA
            cmp #$FF
            beq L5CF1
            sta L3F4A,X
            lda #$AC
L5CE0       sta L3F4B,X
            inc L3F53
            inc L3F4F,X
L5CE9       rts
L5CEA       sta L3F4A,X
            lda #$A0
            bne L5CE0
L5CF1       txa
            beq L5D02
            lda #$FF
            sta L3F56
            lda L3F54
L5CFC       sta L3F4F,X
            jmp L5CC9
L5D02       lda L3F55
            lsr
            bne L5D2E
            bcc L5D10
            lda RANDOM
            rol
            bmi L5D26
L5D10       lda RANDOM
            rol
            bmi L5D1E
            lda #$00
            bcc L5CFC
            lda #$46
            bcs L5CFC
L5D1E       lda #$56
            bcc L5CFC
            lda #$CF
            bcs L5CFC
L5D26       lda #$0B
            bcc L5CFC
            lda #$A5
            bcs L5CFC
L5D2E       lda #$88
            ldy RANDOM
            bmi L5CFC
            lda #$B6
            jmp L5CFC
L5D3A       lda #$00
            sta L3F57
            sta AUDCTL
            sta AUDC1
            sta AUDC2
            sta AUDC3
            sta AUDC4
            lda #$03
            sta SSKCTL
            sta SKCTL
            lda #$22
            sta L3F4F
            sta L3F51
            lda #$23
            sta L3F54
            lda #$00
            sta L3F55
            sta L00BC
L5D6A       lda L00BC
            beq L5D6A
            lda #$00
            sta SDMCTL
            sta L3F52
            sta L3F56
            lda #$01
            sta L3F4E
            sta L3F50
            sta L3F57
            rts
L5D85       lda #$00
            sta L0083
            ldy RANDOM
            lda #$03
            ldx #$00
L5D90       sta L0600,X
            inx
            bne L5D90
L5D96       inc L0083
            bne L5DDE
            lda RANDOM
            ora #$0F
            tay
            sty L3F48
            lda L0600,Y
            and #$FE
            ora #$14
            sta L0600,Y
            tya
            eor #$80
            tay
            sty L3F49
            lda L0600,Y
            and #$FE
            ora #$14
            sta L0600,Y
            jsr L5DC4
            jsr L5DC4
L5DC4       ldy RANDOM
            tya
            and #$0F
            beq L5DC4
            cmp #$0E
            bcs L5DC4
            lda L0600,Y
            and #$01
            beq L5DC4
            eor L0600,Y
            sta L0600,Y
            rts
L5DDE       lda L0600,Y
            ora #$80
            sta L0600,Y
            lda #$00
            sta L0084
            sta L0085
            tya
            and #$0F
            beq L5DFA
            ldx L05FF,Y
            bmi L5DFA
            inc L0084
            inc L0085
L5DFA       asl L0084
            cmp #$0F
            beq L5E09
            ldx L0601,Y
            bmi L5E09
            inc L0084
            inc L0085
L5E09       asl L0084
            cpy #$10
            bcc L5E20
            cpy #$8A
            bcc L5E17
            cpy #$90
            bcc L5E20
L5E17       ldx L05F0,Y
            bmi L5E20
            inc L0084
            inc L0085
L5E20       asl L0084
            cpy #$F0
            bcs L5E37
            cpy #$7A
            bcc L5E2E
            cpy #$80
            bcc L5E37
L5E2E       ldx L0610,Y
            bmi L5E37
            inc L0084
            inc L0085
L5E37       lda L0085
            beq L5EA5
            cmp #$03
            bcs L5E46
            lda RANDOM
            cmp #$33
            bcc L5EA5
L5E46       lda RANDOM
            and #$03
            beq L5E67
            cmp #$01
            beq L5E79
            cmp #$02
            beq L5E8F
            lda L0084
            and #$08
            beq L5E46
            lda L05FF,Y
            and #$FE
            sta L05FF,Y
            dey
            jmp L5D96
L5E67       lda L0084
            and #$04
            beq L5E46
            lda L0600,Y
            and #$FE
            sta L0600,Y
            iny
            jmp L5D96
L5E79       lda L0084
            and #$02
            beq L5E46
            lda L05F0,Y
            and #$FD
            sta L05F0,Y
            tya
            sec
            sbc #$10
            tay
            jmp L5D96
L5E8F       lda L0084
            and #$01
            beq L5E46
            lda L0600,Y
            and #$FD
            sta L0600,Y
            tya
            clc
            adc #$10
            tay
            jmp L5D96
L5EA5       ldy RANDOM
            lda L0600,Y
            bpl L5EA5
            jmp L5DDE
L5EB0       lda L00A3
            asl
            asl
            adc #$18
            tax
            lda L00A2
            asl
            asl
            tay
            rts
L5EBD       jsr L5EB0
            jsr L4EFA
            lda L00D5
            beq L5ED7
            dex
            jsr L4EFA
            dex
            jsr L4EFA
            dex
            jsr L4EFA
            dex
            jmp L4EFA
L5ED7       dey
            jsr L4EFA
            dey
            jsr L4EFA
            dey
            jsr L4EFA
            dey
            jmp L4EFA
L5EE7       ldx #$00
            stx L00D5
L5EEB       lda L0600,X
            and #$3C
            sta L0083
            asl
            asl
            lsr L0083
            lsr L0083
            ora L0083
            and #$3C
            ora L0600,X
            sta L0600,X
            dex
            bne L5EEB
            lda #$FF
L5F07       sta L0800,X
            sta L0900,X
            sta L0A00,X
            sta L0B00,X
            sta L0C00,X
            sta L0D00,X
            sta L0E00,X
            sta L0F00,X
            dex
            bne L5F07
            jsr L61D0
            lda #$AA
            sta L00B1
            lda L00FE
            beq L5F60
            lda #$01
            sta L00E0
            lda #$00
            sta L00A2
L5F35       lda #$00
            sta L00A3
L5F39       inc L00D5
            jsr L483B
            beq L5F43
            jsr L5EBD
L5F43       dec L00D5
            jsr L483B
            beq L5F4D
            jsr L5EBD
L5F4D       inc L00A3
            lda L00A3
            cmp #$11
            bne L5F39
            inc L00A2
            lda L00A2
            cmp #$11
            bne L5F35
            jmp L5F92
L5F60       ldx #$18
            ldy #$40
L5F64       dey
            jsr L4EFA
            tya
            bne L5F64
L5F6B       inx
            jsr L4EFA
            cpx #$58
            bne L5F6B
L5F73       iny
            jsr L4EFA
            cpy #$3F
            bne L5F73
            ldy #$04
L5F7D       ldx #$1C
L5F7F       jsr L4EFA
            inx
            inx
            inx
            inx
            cpx #$58
            bcc L5F7F
            iny
            iny
            iny
            iny
            cpy #$40
            bcc L5F7D
L5F92       lda #$00
            sta L00B1
            lda #$00
            sta L00E0
            lda #$01
            sta L00A2
            lda #$10
            sta L00A3
            ldy #$3F
L5FA4       jsr L483F
            bne L5FB8
            jsr L5EB0
            dey
            jsr L4EFA
            dey
            jsr L4EFA
            dey
            jsr L4EFA
L5FB8       inc L00A2
            lda L00A2
            cmp #$11
            bne L5FA4
            rts
L5FC1       lda #$00
            sta L00BA
            lda #$00
            sta COLOR1
            lda #$06
            sta COLOR3
            lda #$C2
            sta PCOLR0
            lda #$60
            sta PCOLR1
            lda #$35
            sta SIZEM
            lda #$03
            sta SIZEP0
            sta SIZEP1
            sta SIZEP2
            sta SIZEP3
            lda #$3F
            sta HPOSM0
            lda #$BF
            sta HPOSM1
            lda #$7E
            sta HPOSM2
            lda #$20
            sta HPOSP0
            lda #$C0
            sta HPOSP1
            lda #$40
            sta HPOSP2
            lda #$60
            sta HPOSP3
            lda #$14
            sta GPRIOR
            lda #$1A
            sta L00FD
            ldx #$5C
            ldy #$6E
            lda #$07
            jsr SETVBV
            jsr L5D3A
            ldx #$5C
            ldy #$3F
            lda #$06
            sta L3F52
            jsr SETVBV
            jsr L4006
            jsr L4115
            jsr L59F2
            lda POKMSK
            and #$7F
            sta POKMSK
            sta IRQEN
            jsr L538D
            lda #$30
            sta L00A4
            ldx #$00
            jsr L6107
            lda #$FF
            sta L00FE
L6052       jsr L5D85
            jsr L5EE7
            lda #$FF
            sta GRAFP0
            sta GRAFP1
            jsr L55CE
            jsr L5D3A
            ldx #$73
            ldy #$B9
            stx L0083
            sty L0084
            lsr L3EE5,X
            lda RTCLOK+2
            clc
            adc #$12
L6076       cmp RTCLOK+2
            bne L6076
            jmp L6052
L607D       .byte $22
            adc L3000,Y
            adc (ENDPT+1,X)
            jmp (L2500)
            .byte $64
            adc BUFSTR
            .byte $73,$74
            adc SAVADR+1
            ror L0800
            .byte $63
            ora #$11
            ora L1318,Y
            .byte $00,$33
            adc #$72
            adc #$75
            .byte $73,$FF
            ora (L0000),Y
            .byte $2F
            ror.w ADRESS+1
            bvs L6112
            adc (KEYDEF,X)
            adc COLAC
            .byte $0F,$12,$00,$34,$77,$6F,$00
            bvs L611E+1
            adc (KEYDEF,X)
            adc COLAC
            .byte $0F,$13,$00
            bit ADRESS+1
            adc LFF6F
            and L756F,Y
            .byte $00
            adc (COLAC,X)
            adc DSKUTL
            .byte $00
            ora (L0000),Y
            and #$6E
            ror FKDEF+1,X
            .byte $64
            adc COLAC
            .byte $00,$0F,$00,$12,$00
            bit ADRESS+1
            ror ADRESS+1
            ror L6564
            .byte $72,$00,$FF
            and #$6E
            ror FKDEF+1,X
            .byte $64
            adc COLAC
            .byte $1A,$00
            plp
            adc BUFSTR+1,X
            adc (BITMSK,X)
            .byte $00,$0F,$00
            bit ADRESS+1
            ror ADRESS+1
            ror L6564
            .byte $72,$1A,$00
            plp
            adc BUFSTR+1,X
            adc (BITMSK,X)
            .byte $FF,$23
            adc L7470
            .byte $72,$FF
L6107       ldy #$00
L6109       lda L607D,X
            bmi L6116
            sta L3F00,Y
            inx
L6112       iny
            jmp L6109
L6116       rts
L6117       lda #$00
            jsr L4FEE
            lda #$C6
L611E       sta L00BA
            jsr L538D
            jsr L5327
            jsr L5105
            ldx #$21
            jsr L6107
            lda #$66
            sta L3F54
            lda #$01
            sta L3F55
L6138       lda #$FF
            sta CH
L613D       lda CH
            cmp #$FF
            beq L613D
            sta L00FD
            ldx #$00
            cmp #$1A
            beq L6152
            ldx #$63
            cmp #$1E
            bne L6158
L6152       jsr L6107
            jmp L618B
L6158       cmp #$1F
            bne L6138
            ldx #$42
            jsr L6107
L6161       lda #$FF
            sta CH
L6166       lda CH
            cmp #$FF
            beq L6166
            ldy #$1B
            cmp #$1F
            beq L617D
            ldy #$32
            sty L00FD
            ldy #$09
            cmp #$1E
            bne L6161
L617D       tya
            pha
            ldx #$63
            jsr L6107
            pla
            tay
            ldx #$84
            jsr L6109
L618B       lda #$FF
            sta CH
            rts
L6191       .byte $BF,$FF,$FF,$AF,$FF,$FF,$BF,$FF,$FF
            rti
            .byte $FF,$FF
            rti
            .byte $0F,$FF
            rti
            .byte $03,$FF
            rti
            .byte $03,$FF
            rti
            .byte $03,$03
            rti
            .byte $03,$00
            rti
            .byte $03,$00
            rti
            .byte $03,$00,$7F,$03,$00,$7F,$E3,$00,$7F,$EF,$00,$7F,$FC,$00,$7F,$FF
            .byte $FC,$7F,$FF,$FF,$7F,$FF,$FF,$7F,$FF,$FF,$7F,$FF,$FF,$7F,$FF,$FF
L61D0       lda #$D8
            sta L0083
            lda #$0A
            sta L0084
            ldx #$00
L61DA       ldy #$00
            jsr L61F2
            jsr L61EF
            jsr L61FA
            cpx #$3F
            bne L61DA
            lda #$FE
            sta L0AF7
            rts
L61EF       jsr L61F2
L61F2       lda L6191,X
L61F5       sta (L0083),Y
            inx
            iny
            rts
L61FA       lda L0083
            clc
            adc #$20
            sta L0083
            lda L0084
            adc #$00
            sta L0084
            rts
L6208       lda #$AF
            sta AUDC3
            lda #$C0
            sta L0085
L6211       lda L0085
            sta AUDF3
            jsr L61D0
            jsr L6226
            dec L0085
            bne L6211
L6220       lda #$A0
            sta AUDC3
            rts
L6226       lda #$D7
            sta L0083
            lda #$0A
            sta L0084
            ldx #$00
L6230       ldy #$00
            lda #$FF
            jsr L6242
            jsr L6242
            jsr L61FA
            cpx #$54
            bne L6230
            rts
L6242       jsr L61F5
            jmp L61F5
            nop
            nop
            lda #$39
            ldx L7BA9
            ldx LF3A9
            sta L00FE
            sta L00FF
            lda #$47
            sta L00FC
            lda #$62
            sta L00FD
            ldy #$00
L6260       lda (L00FC),Y
            eor L00FE
            sta (L00FC),Y
            lda L00FE
            clc
            adc L00FF
            sta L00FE
            inc L00FF
            rol L00FF
            rol L00FE
            bcc L6277
            inc L00FF
L6277       lda L00FC
            bne L627D
            dec L00FD
L627D       dec L00FC
            lda L00FD
            cmp #$40
            bne L6260
            lda L00FC
            cmp #$02
            bne L6260
            rts
L628C       ora (FTYPE),Y
            sec
            and (L00B6,X)
            bit LB9B0
L6294       lda LFBB0,X
            ldx #$00
            lda L6294
            ora (ICPTLZ),Y
            stx L00FF
            bne L6294+1
L62A2       eor (L00CE,X)
            bne L62A2+1
            rts

	;; PROCEED vector
	
PRCVEC:	LDA #$01		; store 1 into
	STA TRIP		; ...trip
	PLA			; clean up stack
	RTI			; and return from interrupt.

;;; COPY TABLE TO DCB AND DO SIO CALL ;;;;;;;;;;;

DOSIOV: STA	DODCBL+1	; Set source address
	STY	DODCBL+2
	LDY	#$0C		; 12 bytes
DODCBL	LDA	$FFFF,Y		; Changed above.
	STA	DCB,Y		; To DCB table
	DEY			; Count down
	BPL	DODCBL		; Until done

SIOVDST:	
	JSR	SIOV		; Call SIOV
	LDY	DSTATS		; Get STATUS in Y
	TYA			; Copy it into A
	RTS			; Done

FSTRT
	;; Set up PROCEED interrupt vector
	LDA	#<PRCVEC
	STA	VPRCED
	LDA	#>PRCVEC
	STA	VPRCED

	;; Display Banner
	
	LDX	#$00		; IOCB #0
	TXA			; ...
	STA	IOCB0+ICBLH		; Go ahead and zero ICBLH.
	LDA	#<BANNER	; Point to buffer
	STA	IOCB0+ICBAL		; and set in ICBAL
	LDA	#>BANNER	; and the high bit
	STA	IOCB0+ICBAH		; And set in ICBAH
	LDA	#$4F		; two lines below.
	STA	IOCB0+ICBLL
	LDA	#11		; PUT REC
	STA	IOCB0+ICCOM		; into IOCB command
	JSR	CIOV		; And call CIOV

	;; Get hostname

	LDA	#<BUF		; Point buffer to dspec
	STA	IOCB0+ICBAL		; set in ICBAL
	LDA	#>BUF		; hi byte
	STA	IOCB0+ICBAH		; Set in ICBAH
	LDA	#$7F		; 127 bytes
	STA	IOCB0+ICBLL		; into ICBLL
	LDA	#5		; GET RECORD
	STA	IOCB0+ICCOM		; and put into command
	JSR	CIOV		; call CIOV

	;; Open connection to host
FOPN:
	LDA	#<OPNDCB	; Set up open DCB
	LDY	#>OPNDCB	; ...
	JSR	DOSIOV		; Do SIOV

	LDA	DSTATS		; Get status
	BMI	FSTRT		; Retry again if failed.

	JMP	L4003		; Otherwise start game
	
OPNDCB:
	.BY	$71		; DDEVIC
	.BY	$01		; DUNIT
	.BY	'O'		; OPEN
	.BY	$80		; DSTATS
	.WO	BUF		; DBUF
	.BY	$1F		; DTIMLO
	.BY	$00		; DRESVD
	.WO	$100		; 256 bytes (DBYT)
	.BY	$0C		; READ/WRITE
	.BY	$00		; NO TRANSLATION

WRIDCB:
	.BY      $71	  	; DDEVIC
	.BY      $01     	; DUNIT
	.BY      'W'     	; DCOMND
	.BY      $80     	; DSTATS
	.WO      BUF    	; DBUFL
	.BY      $1F     	; DTIMLO
	.BY      $00     	; DRESVD
	.BYTE      $01     	; DBYTL
	.BYTE      $00     	; DBYTH
	.BYTE      $01     	; DAUX1
	.BYTE      $00     	; DAUX2

	
BANNER	.by	"       #FUJINET CAPTURE THE FLAG       ",$9B,$9B 
MSG1	.by	"DEVICESPEC OR RETURN TO LISTEN         ",$9B
BUF	.ds 256
LISTEN	.BY	$00
TRIP	.BY	$00
	
	;; RUNAD
            org $02E0
            .word FSTRT
         
