	icl "..\COMMON\ATARI_EQU.ASM"

	
dd_DOS     = $0700
dd_COMTAB  = 0             ;DOS entry jump vector
dd_ZCRNAME = 3             ;file name crunch routine jump vector

dosdetect:	
		LDA     dd_DOS
        	CMP     #'S'            ; SpartaDOS
        	BEQ     dd_spdos
        	CMP     #'M'            ; MyDOS
        	BEQ     dd_mydos
        	CMP     #'X'            ; XDOS
        	BEQ     dd_xdos
        	CMP     #'R'            ; RealDOS
        	BEQ     dd_rdos

        	LDA     #$4C            ; probably default
        	LDY     #dd_COMTAB
        	CMP     (DOSVEC),y
        	BNE     dd_done
        	LDY     #dd_ZCRNAME
        	CMP     (DOSVEC),y
        	BNE     dd_done

        	LDY     #6              ; OS/A+ has a jmp here
        	CMP     (DOSVEC),y
        	BEQ     dd_done
        	LDA     #OSADOS
        	BNE     dd_set

dd_spdos: 	LDA     dd_DOS+3           ; 'B' in BW-DOS
        	CMP     #'B'
        	BNE     dd_spdos_real
        	LDA     dd_DOS+4           ; 'W' in BW-DOS
        	CMP     #'W'
        	BNE     dd_spdos_real

	        LDA     #BWDOS
	        .byte   $2C             ; BIT <abs>

dd_spdos_real:
	        LDA     #SPARTADOS
        	.byte   $2C             ; BIT <abs>

dd_mydos:  	LDA     #MYDOS
        	.byte   $2C             ; BIT <abs>

dd_rdos:   	LDA     #REALDOS
        	.byte   $2C             ; BIT <abs>

dd_xdos:   	LDA     #XDOS
dd_set:    	STA     __dos_type
dd_done:   	RTS


