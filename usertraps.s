;;;
;;; Stub functions for DLX traps.
;;;
;;; Ethan L. Miller, 1999.  Released to the public domain.
;;;
;;; The traps such as Open and Close
;;; (note the capital letters) are for use by user programs.  The traps
;;; with names such as open and close (lower case) are for use by the OS.
;;;

	.text
	.align 2

;;; The following are the traps to be used by user-level programs
;;;
		.text
	.align 2
.proc _Open
.global _Open
_Open:		
;;; Note that trap #0x213 actually causes a trap vector in the OS of
;;; 0x1213.  The same is true for other user traps.  Thus, a user trap
;;; with trap #0x240 will result in a trap vector of 0x1240 in the OS.
	trap	#0x213
	jr	r31
	nop
.endproc _Open

.proc _Close
.global _Close
_Close:		
	trap	#0x214
	jr	r31
	nop
.endproc _Close

.proc _Read
.global _Read
_Read:		
	trap	#0x210
	jr	r31
	nop
.endproc _Read

.proc _Write
.global _Write
_Write:		
	trap	#0x211
	jr	r31
	nop
.endproc _Write

.proc _Lseek
.global _Lseek
_Lseek:		
	trap	#0x212
	jr	r31
	nop
.endproc _Lseek

.proc _Putchar
.global _Putchar
_Putchar:		
	trap	#0x280
	jr	r31
	nop
.endproc _Putchar

.proc _Printf
.global _Printf
_Printf:		
	trap	#0x201
	jr	r31
	nop
.endproc _Printf

.proc _getpid
.global _getpid
_getpid:
	trap 	#0x431
	jr	r31
	nop
.endproc _getpid


.proc _process_create
.global _process_create
_process_create:
	trap	#0x432
	jr	r31
	nop
.endproc _process_create

.proc _shmget
.global _shmget
_shmget:
	trap	#0x440
	jr	r31
	nop
.endproc _shmget

.proc _shmat
.global _shmat
_shmat:
	trap	#0x441
	jr	r31
	nop
.endproc _shmat

.proc _sem_create
.global _sem_create
_sem_create:
	trap	#0x450
	jr	r31
	nop
.endproc _sem_create

.proc _sem_wait
.global _sem_wait
_sem_wait:
	trap	#0x451
	jr	r31
	nop
.endproc _sem_wait

.proc _sem_signal
.global _sem_signal
_sem_signal:
	trap	#0x452
	jr	r31
	nop
.endproc _sem_signal

.proc _lock_create
.global _lock_create
_lock_create:
	trap	#0x453
	jr	r31
	nop
.endproc _lock_create

.proc _lock_acquire
.global _lock_acquire
_lock_acquire:
	trap	#0x454
	jr	r31
	nop
.endproc _lock_acquire

.proc _lock_release
.global _lock_release
_lock_release:
	trap	#0x455
	jr	r31
	nop
.endproc _lock_release

.proc _cond_create
.global _cond_create
_cond_create:
	trap	#0x456
	jr	r31
	nop
.endproc _cond_create

.proc _cond_wait
.global _cond_wait
_cond_wait:
	trap	#0x457
	jr	r31
	nop
.endproc _cond_wait

.proc _cond_signal
.global _cond_signal
_cond_signal:
	trap	#0x458
	jr	r31
	nop
.endproc _cond_signal

.proc _cond_broadcast
.global _cond_broadcast
_cond_broadcast:
	trap	#0x455
	jr	r31
	nop
.endproc _cond_broadcast

.proc _TimerGet
.global _TimerGet
_TimerGet:
	trap 	#0x50
	jr 	r31
	nop
.endproc _TimerGet

.proc _yield
.global _yield
_yield:
	trap    #0x433
	jr      r31
	nop
.endproc _yield
