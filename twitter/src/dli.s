	.include "atari.inc"
	.export _dlih

	
_dlih:	PHA
	LDA	#$0F
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#$00
	STA	$D017
	LDA	#<dl1
	STA	$0200
	LDA	#>dl1
	STA	$0201
	PLA
	RTI

dl1:	PHA
	LDA	#$94
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#$0F
	STA	$D017
	LDA	#<dl2
	STA	$0200
	LDA	#>dl2
	STA	$0201
	PLA
	RTI
	
dl2:	PHA
	LDA	#$0f
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#$00
	STA	$D017
	LDA	#<dl3
	STA	$0200
	LDA	#>dl3
	STA	$0201
	PLA
	RTI

dl3:	PHA
	LDA	#$94
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#$0F
	STA	$D017
	LDA	#<_dlih
	STA	$0200
	LDA	#>_dlih
	STA	$0201
	PLA
	RTI
