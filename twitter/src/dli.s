	.include "atari.inc"
	.export _dlih

	
_dlih:	PHA
	LDA	#$9A
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#<dl2
	STA	$0200
	LDA	#>dl2
	STA	$0201
	PLA
	RTI

dl2:	PHA
	LDA	#$96
	STA	$D40A
	STA	$D018
	STA	$D01A
	LDA	#<_dlih
	STA	$0200
	LDA	#>_dlih
	STA	$0201
	PLA
	RTI

