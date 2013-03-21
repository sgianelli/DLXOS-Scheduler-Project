;
; Stub functions for DLX traps.
;
; Aaron Sawdey 1996; released to the Public Domain.
;
.text
.align 2
.proc _random
.global _random
_random:		
	trap	#0x2020
	jr	r31
	nop
.endproc _random

.proc _srandom
.global _srandom
_srandom:		
	trap	#0x2021
	jr	r31
	nop
.endproc _srandom

; I know this is not the right place to put it, but what the heck

.proc _timerget
.global _timerget
_timerget:		
	trap	#0x2002
	jr	r31
	nop
.endproc _timerget

