	;; Header for boot load Atari Disks

	.import	__STARTUP_SIZE__
	.import __CODE_SIZE__
	.import __DATA_SIZE__
	.import __BSS_SIZE__
	.import __NUM_SECTORS__
		
.segment	"BOOTHEADER"
	.byte	$00
	.byte	<__NUM_SECTORS__
	.byte	$00
	.byte	$07
	.byte	$C0
	.byte	$E4
