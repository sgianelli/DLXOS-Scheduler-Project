//
//	process.h
//
//	Definitions for process creation and manipulation.  These include
//	the process control block (PCB) structure as well as information
//	about the stack format for a saved process.
//

#ifndef	_process_h__
#define	_process_h_

#include "dlxos.h"
#include "queue.h"
//#include "share_memory.h"

#define	PROCESS_MAX_PROCS	32	// Maximum number of active processes

#define	PROCESS_INIT_ISR_SYS	0x140	// Initial status reg value for system processes
#define	PROCESS_INIT_ISR_USER	0x100	// Initial status reg value for user processes

#define	PROCESS_STATUS_FREE	0x1
#define	PROCESS_STATUS_RUNNABLE	0x2
#define	PROCESS_STATUS_WAITING	0x4
#define	PROCESS_STATUS_STARTING	0x8
#define	PROCESS_STATUS_ZOMBIE	0x10
#define	PROCESS_STATUS_MASK	0x3f
#define	PROCESS_TYPE_SYSTEM	0x100
#define	PROCESS_TYPE_USER	0x200

// Lab3 : These parameters are used for lab 3
#define NUM_OF_RUNQUEUE 32
#define PUSER 50
#define TRUE 1
#define FALSE 0

typedef	void (*VoidFunc)();


// Process control block
typedef struct PCB {
  uint32	*currentSavedFrame; // -> current saved frame.  MUST BE 1ST!
  uint32	*sysStackPtr;	// Current system stack pointer.  MUST BE 2ND!
  uint32	sysStackArea;	// System stack area for this process
  unsigned int	flags;
  char		name[80];	// Process name
  uint32	pagetable[16];	// Statically allocated page table
  int		npages;		// Number of pages allocated to this process
  Link		l;		// Used for keeping PCB in queues
  // put in data used for your scheduling algorithm here
} PCB;

// Offsets of various registers from the stack pointer in the register
// save frame.  Offsets are in WORDS (4 byte chunks)
#define	PROCESS_STACK_IREG	10	// Offset of r0 (grows upwards)
// NOTE: r0 isn't actually stored!  This is for convenience - r1 is the
// first stored register, and is at location PROCESS_STACK_IREG+1
#define	PROCESS_STACK_FREG	(PROCESS_STACK_IREG+32)	// Offset of f0
#define	PROCESS_STACK_IAR	(PROCESS_STACK_FREG+32) // Offset of IAR
#define	PROCESS_STACK_ISR	(PROCESS_STACK_IAR+1)
#define	PROCESS_STACK_CAUSE	(PROCESS_STACK_IAR+2)
#define	PROCESS_STACK_FAULT	(PROCESS_STACK_IAR+3)
#define	PROCESS_STACK_PTBASE	(PROCESS_STACK_IAR+4)
#define	PROCESS_STACK_PTSIZE	(PROCESS_STACK_IAR+5)
#define	PROCESS_STACK_PTBITS	(PROCESS_STACK_IAR+6)
#define	PROCESS_STACK_PREV_FRAME 10	// points to previous interrupt frame
#define	PROCESS_STACK_FRAME_SIZE 85	// interrupt frame is 85 words
#define SIZE_ARG_BUFF  	1024		// Max number of characters in the
					// command-line arguments
#define MAX_ARGS	128		// Max number of command-line
					// arguments
extern PCB	*currentPCB;

extern int	ProcessFork (VoidFunc, unsigned int,  int, int,char *, int);
extern void	ProcessSchedule ();
extern void	ContextSwitch(void *, void *, int);
extern void	ProcessSuspend (PCB *);
extern void	ProcessWakeup (PCB *);
extern void	ProcessSetResult (PCB *, uint32);
extern void	ProcessSleep ();
extern unsigned GetCurrentPid();
extern unsigned getpid();
void process_create(int p_nice, int p_info,char *name, ...);

#endif	/* _process_h_ */
