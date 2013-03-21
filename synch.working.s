; Compiled by GCC
.text
	.align 2
.proc _isspace
.global _isspace
_isspace:
;  Function 'isspace'; 0 bytes of locals, 1 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	lb	r2,3(r30)
	addi	r31,r0,#0
	andi	r1,r2,#0x00ff
	seqi	r1,r1,#32
	bnez	r1,L6
	nop	; not filled.
	addi	r1,r2,#-9
	andi	r1,r1,#0x00ff
	sleui	r1,r1,#1
	beqz	r1,L5
	nop	; not filled.
L6:
	addi	r31,r0,#1
L5:
	add	r1,r0,r31
	lw	r2,0(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _isspace
	.align 2
.proc _isxdigit
.global _isxdigit
_isxdigit:
;  Function 'isxdigit'; 0 bytes of locals, 1 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	lb	r31,3(r30)
	addi	r2,r0,#0
	addi	r1,r31,#-48
	andi	r1,r1,#0x00ff
	sleui	r1,r1,#9
	bnez	r1,L12
	nop	; not filled.
	addi	r1,r31,#-97
	andi	r1,r1,#0x00ff
	sleui	r1,r1,#5
	bnez	r1,L12
	nop	; not filled.
	andi	r1,r31,#0x00ff
	snei	r1,r1,#65
	bnez	r1,L11
	nop	; not filled.
L12:
	addi	r2,r0,#1
L11:
	add	r1,r0,r2
	lw	r2,0(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _isxdigit
	.align 2
.proc _QueueLinkInit
.global _QueueLinkInit
_QueueLinkInit:
;  Function 'QueueLinkInit'; 0 bytes of locals, 1 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	lw	r1,(r30)
	lw	r2,4(r30)
	addi	r31,r0,#0
	sw	(r1),r31
	sw	12(r1),r2
	lw	r2,0(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueLinkInit
	.align 2
.proc _QueueNext
.global _QueueNext
_QueueNext:
;  Function 'QueueNext'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,(r1)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueNext
	.align 2
.proc _QueuePrev
.global _QueuePrev
_QueuePrev:
;  Function 'QueuePrev'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,4(r1)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueuePrev
	.align 2
.proc _QueueFirst
.global _QueueFirst
_QueueFirst:
;  Function 'QueueFirst'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,(r1)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueFirst
	.align 2
.proc _QueueLast
.global _QueueLast
_QueueLast:
;  Function 'QueueLast'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,4(r1)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueLast
	.align 2
.proc _QueueInsertAfter
.global _QueueInsertAfter
_QueueInsertAfter:
;  Function 'QueueInsertAfter'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r3,(r30)
	lw	r2,4(r30)
	lw	r1,8(r30)
	sw	8(r1),r3
	sw	4(r1),r2
	lw	r31,(r2)
	sw	(r1),r31
	sw	(r2),r1
	lw	r2,(r1)
	sw	4(r2),r1
	lw	r1,8(r3)
	addi	r1,r1,#1
	sw	8(r3),r1
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueInsertAfter
	.align 2
.proc _QueueInsertFirst
.global _QueueInsertFirst
_QueueInsertFirst:
;  Function 'QueueInsertFirst'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r3,(r30)
	lw	r1,4(r30)
	sw	8(r1),r3
	sw	4(r1),r3
	lw	r31,(r3)
	sw	(r1),r31
	sw	(r3),r1
	lw	r2,(r1)
	sw	4(r2),r1
	lw	r1,8(r3)
	addi	r1,r1,#1
	sw	8(r3),r1
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueInsertFirst
	.align 2
.proc _QueueInsertLast
.global _QueueInsertLast
_QueueInsertLast:
;  Function 'QueueInsertLast'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r3,(r30)
	lw	r1,4(r30)
	lw	r2,4(r3)
	sw	8(r1),r3
	sw	4(r1),r2
	lw	r31,(r2)
	sw	(r1),r31
	sw	(r2),r1
	lw	r2,(r1)
	sw	4(r2),r1
	lw	r1,8(r3)
	addi	r1,r1,#1
	sw	8(r3),r1
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueInsertLast
	.align 2
.proc _QueueRemove
.global _QueueRemove
_QueueRemove:
;  Function 'QueueRemove'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r31,(r30)
	lw	r1,8(r31)
	lw	r1,8(r1)
	sgti	r1,r1,#0
	beqz	r1,L38
	nop	; not filled.
	lw	r1,4(r31)
	lw	r3,(r31)
	sw	(r1),r3
	lw	r1,(r31)
	lw	r3,4(r31)
	sw	4(r1),r3
	lw	r2,8(r31)
	lw	r1,8(r2)
	addi	r1,r1,#-1
	sw	8(r2),r1
L38:
	addi	r3,r0,#0
	sw	(r31),r3
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueRemove
	.align 2
.proc _QueueLength
.global _QueueLength
_QueueLength:
;  Function 'QueueLength'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,8(r1)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueLength
	.align 2
.proc _QueueEmpty
.global _QueueEmpty
_QueueEmpty:
;  Function 'QueueEmpty'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r1,(r30)
	lw	r1,8(r1)
	seqi	r1,r1,#0
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _QueueEmpty
	.align 2
.proc _SemInit
.global _SemInit
_SemInit:
;  Function 'SemInit'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r2,(r30)
	lw	r3,4(r30)
	addi	r29,r29,#-8
	sw	(r29),r2
	jal	_QueueInit
	nop	; not filled.
	sw	12(r2),r3
	addi	r29,r29,#8
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _SemInit
.data
	.align 2
LC0:
	.ascii "Proc 0x%x waiting on sem 0x%x, count=%d.\n\000"
	.align 2
LC1:
	.ascii "Suspending current proc (0x%x).\n\000"
.text
	.align 2
.proc _SemWait
.global _SemWait
_SemWait:
;  Function 'SemWait'; 0 bytes of locals, 5 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#32	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	sw	8(r29),r4
	sw	12(r29),r5
	sw	16(r29),r6
	lw	r3,(r30)
	jal	_DisableIntrs
	nop	; not filled.
	addi	r29,r29,#-8
	lhi	r2,((_debugstr)>>16)&0xffff
	addui	r2,r2,(_debugstr)&0xffff
	sw	(r29),r2
	addi	r6,r0,#115
	sw	4(r29),r6
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	bnez	r1,L49
	nop	; not filled.
	addi	r29,r29,#-8
	sw	(r29),r2
	addi	r6,r0,#43
	sw	4(r29),r6
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	beqz	r1,L48
	nop	; not filled.
L49:
	addi	r29,r29,#-16
	lhi	r6,((LC0)>>16)&0xffff
	addui	r6,r6,(LC0)&0xffff
	sw	(r29),r6
	lhi	r1,((_currentPCB)>>16)&0xffff
	addui	r1,r1,(_currentPCB)&0xffff
	lw	r1,(r1)
	sw	4(r29),r1
	sw	8(r29),r3
	lw	r6,12(r3)
	sw	12(r29),r6
	jal	_printf
	nop	; not filled.
	addi	r29,r29,#16
L48:
	lw	r1,12(r3)
	addi	r1,r1,#-1
	sw	12(r3),r1
	slti	r1,r1,#0
	beqz	r1,L50
	nop	; not filled.
	jal	_QueueAllocLink
	nop	; not filled.
	add	r2,r0,r1
	lhi	r5,((_currentPCB)>>16)&0xffff
	addui	r5,r5,(_currentPCB)&0xffff
	lw	r1,(r5)
	addi	r6,r0,#0
	sw	(r2),r6
	sw	12(r2),r1
	addi	r29,r29,#-8
	lhi	r4,((_debugstr)>>16)&0xffff
	addui	r4,r4,(_debugstr)&0xffff
	sw	(r29),r4
	addi	r6,r0,#115
	sw	4(r29),r6
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	bnez	r1,L53
	nop	; not filled.
	addi	r29,r29,#-8
	sw	(r29),r4
	addi	r6,r0,#43
	sw	4(r29),r6
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	beqz	r1,L52
	nop	; not filled.
L53:
	addi	r29,r29,#-8
	lhi	r6,((LC1)>>16)&0xffff
	addui	r6,r6,(LC1)&0xffff
	sw	(r29),r6
	lw	r1,(r5)
	sw	4(r29),r1
	jal	_printf
	nop	; not filled.
	addi	r29,r29,#8
L52:
	lw	r1,4(r3)
	sw	8(r2),r3
	sw	4(r2),r1
	lw	r6,(r1)
	sw	(r2),r6
	sw	(r1),r2
	lw	r1,(r2)
	sw	4(r1),r2
	lw	r1,8(r3)
	addi	r1,r1,#1
	sw	8(r3),r1
	jal	_ProcessSleep
	nop	; not filled.
L50:
	jal	_EnableIntrs
	nop	; not filled.
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r4,8(r29)
	lw	r5,12(r29)
	lw	r6,16(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _SemWait
.data
	.align 2
LC2:
	.ascii "Signalling on sem 0x%x, count=%d.\n\000"
	.align 2
LC3:
	.ascii "Waking up PCB 0x%x.\n\000"
.text
	.align 2
.proc _SemSignal
.global _SemSignal
_SemSignal:
;  Function 'SemSignal'; 0 bytes of locals, 3 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#24	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	sw	8(r29),r4
	lw	r2,(r30)
	jal	_DisableIntrs
	nop	; not filled.
	addi	r29,r29,#-8
	lhi	r3,((_debugstr)>>16)&0xffff
	addui	r3,r3,(_debugstr)&0xffff
	sw	(r29),r3
	addi	r4,r0,#115
	sw	4(r29),r4
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	bnez	r1,L59
	nop	; not filled.
	addi	r29,r29,#-8
	sw	(r29),r3
	addi	r4,r0,#43
	sw	4(r29),r4
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	beqz	r1,L58
	nop	; not filled.
L59:
	addi	r29,r29,#-16
	lhi	r4,((LC2)>>16)&0xffff
	addui	r4,r4,(LC2)&0xffff
	sw	(r29),r4
	sw	4(r29),r2
	lw	r4,12(r2)
	sw	8(r29),r4
	jal	_printf
	nop	; not filled.
	addi	r29,r29,#16
L58:
	lw	r1,12(r2)
	addi	r1,r1,#1
	sw	12(r2),r1
	slei	r1,r1,#0
	beqz	r1,L60
	nop	; not filled.
	lw	r3,(r2)
	lw	r1,8(r3)
	lw	r1,8(r1)
	sgti	r1,r1,#0
	beqz	r1,L63
	nop	; not filled.
	lw	r1,4(r3)
	lw	r4,(r3)
	sw	(r1),r4
	lw	r1,(r3)
	lw	r4,4(r3)
	sw	4(r1),r4
	lw	r2,8(r3)
	lw	r1,8(r2)
	addi	r1,r1,#-1
	sw	8(r2),r1
L63:
	addi	r4,r0,#0
	sw	(r3),r4
	addi	r29,r29,#-8
	lhi	r2,((_debugstr)>>16)&0xffff
	addui	r2,r2,(_debugstr)&0xffff
	sw	(r29),r2
	addi	r4,r0,#115
	sw	4(r29),r4
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	bnez	r1,L65
	nop	; not filled.
	addi	r29,r29,#-8
	sw	(r29),r2
	addi	r4,r0,#43
	sw	4(r29),r4
	jal	_dindex
	nop	; not filled.
	addi	r29,r29,#8
	snei	r1,r1,#0
	beqz	r1,L64
	nop	; not filled.
L65:
	addi	r29,r29,#-8
	lhi	r4,((LC3)>>16)&0xffff
	addui	r4,r4,(LC3)&0xffff
	sw	(r29),r4
	lw	r1,12(r3)
	sw	4(r29),r1
	jal	_printf
	nop	; not filled.
	addi	r29,r29,#8
L64:
	addi	r29,r29,#-8
	lw	r4,12(r3)
	sw	(r29),r4
	jal	_ProcessWakeup
	nop	; not filled.
	sw	(r29),r3
	jal	_QueueFreeLink
	nop	; not filled.
	addi	r29,r29,#8
L60:
	jal	_EnableIntrs
	nop	; not filled.
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r4,8(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _SemSignal
	.align 2
.proc _LockInit
.global _LockInit
_LockInit:
;  Function 'LockInit'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r2,(r30)
	addi	r29,r29,#-8
	sw	(r29),r2
	jal	_QueueInit
	nop	; not filled.
	addi	r3,r0,#1
	sw	12(r2),r3
	addi	r29,r29,#8
	addi	r1,r2,#16
	addi	r29,r29,#-8
	sw	(r29),r1
	jal	_QueueInit
	nop	; not filled.
	addi	r3,r0,#0
	sw	28(r2),r3
	addi	r29,r29,#8
	sw	32(r2),r3
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _LockInit
	.align 2
.proc _LockAcquire
.global _LockAcquire
_LockAcquire:
;  Function 'LockAcquire'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	addi	r29,r29,#-8
	lw	r1,(r30)
	sw	(r29),r1
	jal	_SemWait
	nop	; not filled.
	addi	r29,r29,#8
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _LockAcquire
	.align 2
.proc _LockRelease
.global _LockRelease
_LockRelease:
;  Function 'LockRelease'; 0 bytes of locals, 0 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#8	; alloc local storage
	lw	r31,(r30)
	lw	r1,32(r31)
	sgti	r1,r1,#0
	beqz	r1,L78
	nop	; not filled.
	addi	r29,r29,#-8
	addi	r1,r31,#16
	sw	(r29),r1
	j	L80
	nop	; not filled.
L78:
	addi	r29,r29,#-8
	sw	(r29),r31
L80:
	jal	_SemSignal
	nop	; not filled.
	addi	r29,r29,#8
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _LockRelease
	.align 2
.proc _CondInit
.global _CondInit
_CondInit:
;  Function 'CondInit'; 0 bytes of locals, 2 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	lw	r2,(r30)
	lw	r3,4(r30)
	sw	(r2),r3
	addi	r1,r2,#4
	addi	r29,r29,#-8
	sw	(r29),r1
	jal	_QueueInit
	nop	; not filled.
	addi	r3,r0,#0
	sw	16(r2),r3
	addi	r29,r29,#8
	sw	20(r2),r3
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _CondInit
	.align 2
.proc _CondWait
.global _CondWait
_CondWait:
;  Function 'CondWait'; 0 bytes of locals, 1 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	lw	r2,(r30)
	lw	r1,20(r2)
	addi	r1,r1,#1
	sw	20(r2),r1
	lw	r31,(r2)
	lw	r1,32(r31)
	sgti	r1,r1,#0
	beqz	r1,L89
	nop	; not filled.
	addi	r29,r29,#-8
	addi	r1,r31,#16
	sw	(r29),r1
	j	L91
	nop	; not filled.
L89:
	addi	r29,r29,#-8
	sw	(r29),r31
L91:
	jal	_SemSignal
	nop	; not filled.
	addi	r29,r29,#8
	addi	r29,r29,#-8
	addi	r1,r2,#4
	sw	(r29),r1
	jal	_SemWait
	nop	; not filled.
	lw	r1,(r2)
	addi	r1,r1,#16
	sw	(r29),r1
	jal	_SemWait
	nop	; not filled.
	lw	r2,(r2)
	lw	r1,32(r2)
	addi	r1,r1,#-1
	sw	32(r2),r1
	addi	r29,r29,#8
	lw	r2,0(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _CondWait
	.align 2
.proc _CondSignal
.global _CondSignal
_CondSignal:
;  Function 'CondSignal'; 0 bytes of locals, 1 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#16	; alloc local storage
	sw	0(r29),r2
	lw	r31,(r30)
	lw	r2,20(r31)
	sgti	r1,r2,#0
	beqz	r1,L95
	nop	; not filled.
	addi	r1,r2,#-1
	sw	20(r31),r1
	lw	r2,(r31)
	lw	r1,32(r2)
	addi	r1,r1,#1
	sw	32(r2),r1
	addi	r29,r29,#-8
	addi	r1,r31,#4
	sw	(r29),r1
	jal	_SemSignal
	nop	; not filled.
	addi	r29,r29,#8
L95:
	lw	r2,0(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _CondSignal
	.align 2
.proc _CondBroadcast
.global _CondBroadcast
_CondBroadcast:
;  Function 'CondBroadcast'; 0 bytes of locals, 5 regs to save.
	sw	-4(r29),r30	; push fp
	add	r30,r0,r29	; fp = sp
	sw	-8(r29),r31	; push ret addr
	subui	r29,r29,#32	; alloc local storage
	sw	0(r29),r2
	sw	4(r29),r3
	sw	8(r29),r4
	sw	12(r29),r5
	sw	16(r29),r6
	lw	r4,(r30)
	lw	r5,20(r4)
	addi	r3,r0,#0
	slt	r1,r3,r5
	beqz	r1,L106
	nop	; not filled.
	addi	r6,r4,#4
L108:
	lw	r2,20(r4)
	sgti	r1,r2,#0
	beqz	r1,L107
	nop	; not filled.
	addi	r1,r2,#-1
	sw	20(r4),r1
	lw	r2,(r4)
	lw	r1,32(r2)
	addi	r1,r1,#1
	sw	32(r2),r1
	addi	r29,r29,#-8
	sw	(r29),r6
	jal	_SemSignal
	nop	; not filled.
	addi	r29,r29,#8
L107:
	addi	r3,r3,#1
	slt	r1,r3,r5
	bnez	r1,L108
	nop	; not filled.
L106:
	lw	r2,0(r29)
	lw	r3,4(r29)
	lw	r4,8(r29)
	lw	r5,12(r29)
	lw	r6,16(r29)
	lw	r31,-8(r30)
	add	r29,r0,r30
	lw	r30,-4(r30)
	jr	r31
	nop
.endproc _CondBroadcast
