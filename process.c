//
//	process.c
//
//	This file defines routines for dealing with processes.  This
//	includes the "main" routine for the OS, which creates a process
//	for the initial thread of execution.  It also includes
//	code to create and delete processes, as well as context switch
//	code.  Note, however, that the actual context switching is
//	done in assembly language elsewhere.

#include "dlxos.h"
#include "process.h"
//#include "synch.h"
#include "memory.h"
#include "filesys.h"
float MyFuncRetZero();

// Lab3: These two string are defined for you to print out your
// timming analysis.  Do not use your own strings.
// Your printf statement should be printf(TIMESTRING1,data1...);...
// note that we need to print the floating point value by itself,
// otherwise it is not going to print right

#define TIMESTRING1  "Process %d has run for"
#define TIMESTRING2  " %.3f s\n"
#define TIMESTRING3  "Process %d 's priority is %d \n"

#define TOTAL_QUANTA_MAX 10
#define PROCESS_QUANTA_MAX 4

#define FALSE 0
#define TRUE 1

// Pointer to the current PCB.  This is used by the assembly language
// routines for context switches.
PCB		*currentPCB;

// List of free PCBs.
static Queue	freepcbs;

// List of processes that are ready to run (ie, not waiting for something
// to happen).
static Queue	runQueue[32];

// List of processes that are waiting for something to happen.  There's no
// reason why this must be a single list; there could be many lists for many
// different conditions.
static Queue	waitQueue;

// List of processes waiting to be deleted.  See below for a description of
// the reason that we need a separate queue for processes about to die.
static Queue	zombieQueue;

// Static area for all process control blocks.  This is necessary because
// we can't use malloc() inside the OS.
static PCB	pcbs[PROCESS_MAX_PROCS];

// Default value for scheduler quantum.  This could be set to any value.
// In fact, it could even be dynamic, though that would require modifying
// the timer trap handler....
static processQuantum = DLX_PROCESS_QUANTUM;

// String listing debugging options to print out.
char	debugstr[200];

// BEGIN BRIAN CODE
static uint32 totalQuanta = 0;
// END BRIAN CODE

uint32 my_timer_get() {
  return totalQuanta * 100 + totalQuanta;
}

//----------------------------------------------------------------------
//
//	ProcessModuleInit
//
//	Initialize the process module.  This involves initializing all
//	of the process control blocks to appropriate values (ie, free
//	and available).  We also need to initialize all of the queues.
//
//----------------------------------------------------------------------
  void
ProcessModuleInit ()
{
  int		i;

  dbprintf ('p', "Entering ProcessModuleInit\n");
  QueueInit (&freepcbs);
  for (i = 0; i< NUM_OF_RUNQUEUE; i++)
    QueueInit (&runQueue[i]);
  QueueInit (&waitQueue);
  QueueInit (&zombieQueue);
  for (i = 0; i < PROCESS_MAX_PROCS; i++) {
    dbprintf ('p', "Initializing PCB %d @ 0x%x.\n", i, &(pcbs[i]));
    pcbs[i].flags = PROCESS_STATUS_FREE;
    QueueLinkInit (&(pcbs[i].l), (void *)&pcbs[i]);
    QueueInsertFirst(&freepcbs, &(pcbs[i].l));
  }
  currentPCB = NULL;
  dbprintf ('p', "Leaving ProcessModuleInit\n");
}

//----------------------------------------------------------------------
//
//	ProcessSetStatus
//
//	Set the status of a process.
//
//----------------------------------------------------------------------
  void
ProcessSetStatus (PCB *pcb, int status)
{
  pcb->flags &= ~PROCESS_STATUS_MASK;
  pcb->flags |= status;
}

//----------------------------------------------------------------------
//
//	ProcessFreeResources
//
//	Free the resources associated with a process.  This assumes the
//	process isn't currently on any queue.
//
//----------------------------------------------------------------------
  void
ProcessFreeResources (PCB *pcb)
{
  int		i;
  int		npages;

  QueueInsertLast (&freepcbs, &pcb->l);
  // Free the process's memory.  This is easy with a one-level page
  // table, but could get more complex with two-level page tables.
  npages = pcb->npages;

  for (i=0; i<npages; i++)
  {
    MemoryFreeSharedPte(pcb, i); // *MUST* be called before calling
    // MemoryFreePte. MemoryFreePte does not know
    // anything about shared pages, and hence it
    // might screw up big time
  }
  for (i = 0; i < pcb->npages; i++) {
    MemoryFreePte (pcb->pagetable[i]);
  }
  // Free the page allocated for the system stack
  MemoryFreePage (pcb->sysStackArea / MEMORY_PAGE_SIZE);
  ProcessSetStatus (pcb, PROCESS_STATUS_FREE);
}

//----------------------------------------------------------------------
//
//	ProcessSetResult
//
//	Set the result returned to a process.  This is done by storing
//	the value into the current register save area for r1.  When the
//	context is restored, r1 will contain the return value.  This
//	routine should only be called from a trap.  Calling it at other
//	times (such as an interrupt handler) will cause unpredictable
//	results.
//
//----------------------------------------------------------------------
  void
ProcessSetResult (PCB * pcb, uint32 result)
{
  pcb->currentSavedFrame[PROCESS_STACK_IREG+1] = result;
}

//----------------------------------------------------------------------
//
//  ProcessHighestPriority
//
//  Grabs the next process that should be run
//
//----------------------------------------------------------------------

PCB *
ProcessHighestPriority() {
  PCB *ret = NULL;
  int i;

  for (i = 0; i < 32; i++) {
    if ((&runQueue[i])->nitems != 0) {
      dbprintf('p', "Found process to run in queue [%i]\n",i);
      ret = (PCB *)(QueueFirst(&runQueue[i])->object);
      break;
    }
  }

  return ret;
}

//----------------------------------------------------------------------
//
//	ProcessSchedule
//
//	Schedule the next process to run.  If there are no processes to
//	run, exit.  This means that there should be an idle loop process
//	if you want to allow the system to "run" when there's no real
//	work to be done.
//
//	NOTE: the scheduler should only be called from a trap or interrupt
//	handler.  This way, interrupts are disabled.  Also, it must be
//	called consistently, and because it might be called from an interrupt
//	handler (the timer interrupt), it must ALWAYS be called from a trap
//	or interrupt handler.
//
//	Note that this procedure doesn't actually START the next process.
//	It only changes the currentPCB and other variables so the next
//	return from interrupt will restore a different context from that
//	which was saved.
//
//----------------------------------------------------------------------
// You should modify this function to use 4.4BSD scheduling policy
//
  void
ProcessSchedule ()
{
  PCB           *pcb;
  int           i, j, n;
  int           atEndOfQueue = FALSE; // To be used as a boolean value
  Link          *links[32];

  // The OS exits if there's no runnable process.  This is a feature, not a
  // bug.  An easy solution to allowing no runnable "user" processes is to
  // have an "idle" process that's simply an infinite loop.
  /*  if (QueueEmpty (&runQueue)) {
      printf ("No runnable processes - exiting!\n");
      exitsim (); // NEVER RETURNS
      }*/
  dbprintf('p', "Entering ProcessSchedule [context switch] with current PCB: %p\n",currentPCB);

  currentPCB->p_quanta++;	// do this here or later?
  currentPCB->estcpu++;	// do this here or later?

  totalQuanta++;

  pcb = ProcessHighestPriority();

  dbprintf('p', "PCB (%p) currentPCB (%p)\n",pcb,currentPCB);


  // If last run process is still the highest priority (ie. not asleep/a zombie)
  if (pcb == currentPCB) {
    QueueRemove (&pcb->l);
    QueueInsertLast (&runQueue[pcb->runQueueNum], &pcb->l);

    dbprintf('p', "\tProcess quanta: %i estcpu: %i\n",currentPCB->p_quanta,currentPCB->estcpu);
    if((currentPCB->p_quanta % PROCESS_QUANTA_MAX) == 0) {
      dbprintf('p', "Recalculating priority of currentPCB\n");
      currentPCB->prio = PUSER + (currentPCB->estcpu/4) + (2*currentPCB->p_nice);
      dbprintf('p', "run queue: %i new run queue: %i prio: %i\n",currentPCB->runQueueNum,currentPCB->prio/4,currentPCB->prio);

      currentPCB->runQueueNum = currentPCB->prio/4;
      QueueRemove(&currentPCB->l);
      QueueInsertLast(&runQueue[currentPCB->runQueueNum], &currentPCB->l);

      dbprintf('p', "Recalculated priority\n");
    }
  }

  if(totalQuanta % TOTAL_QUANTA_MAX == 0) {
    dbprintf('p', "Full reshuffle\n");
    // dbprintf('p', "Process quanta exceeded max\n");
    // Store all the tails of all of the RunQueues
    for(i = 0; i < 32; i++) {
      links[i] = QueueLast(&runQueue[i]);
    }
    dbprintf('p', "Last links registered\n");

    for(i = 0; i < 32; i++) {
//        if(QueueEmpty(&runQueue[i])) atEndOfQueue = TRUE;
      n = (&runQueue[i])->nitems;

//        while(!atEndOfQueue) {
      for (j = 0; j < n; j++) {
        pcb = (PCB *)((QueueFirst(&runQueue[i]))->object);
        pcb->estcpu = (int)((((float)2 * pcb->load)/((float)2 * pcb->load + 1)) * pcb->estcpu) + pcb->p_nice;  // Decay the estimated CPU time of all processes
        pcb->prio = PUSER + (pcb->estcpu/4) + (2 * pcb->p_nice);                            // Recalculate the priority of all processes
        dbprintf('p', "\tRun queue shift (%p->%d)\n",pcb,pcb->prio);

        dbprintf('p', "At link: %p for last link: %p\n",&pcb->l,links[i]);
        if(links[i] == &pcb->l || (&pcb->l)->next == NULL) atEndOfQueue = TRUE;

        pcb->runQueueNum = pcb->prio/4;
        QueueRemove(&pcb->l);
        QueueInsertLast(&runQueue[pcb->runQueueNum], &pcb->l);
      }
    }

    pcb = ProcessHighestPriority();
    if (currentPCB == pcb) {
      QueueRemove(&currentPCB->l);
      QueueInsertLast(&runQueue[currentPCB->runQueueNum], &currentPCB->l);
    }
  }
  //}

  currentPCB = ProcessHighestPriority();

  // Move the front of the queue to the end, if it is the running process.
  /*
     pcb = (PCB *)((QueueFirst (&runQueue))->object);
     if (pcb == currentPCB)
     {
     QueueRemove (&pcb->l);
     QueueInsertLast (&runQueue, &pcb->l);
     }

  // Now, run the one at the head of the queue.
  pcb = (PCB *)((QueueFirst (&runQueue))->object);
  currentPCB = pcb;
  dbprintf ('p',"About to switch to PCB 0x%x,flags=0x%x @ 0x%x\n",
  pcb, pcb->flags,
  pcb->sysStackPtr[PROCESS_STACK_IAR]);
   */

  // Clean up zombie processes here.  This is done at interrupt time
  // because it can't be done while the process might still be running
  while (!QueueEmpty (&zombieQueue)) {
    pcb = (PCB *)(QueueFirst (&zombieQueue)->object);
    dbprintf ('p', "Freeing zombie PCB 0x%x.\n", pcb);
    QueueRemove (&pcb->l);
    ProcessFreeResources (pcb);
  }
  // Set the timer so this process gets at most a fixed quantum of time.
  TimerSet (processQuantum);
  dbprintf ('p', "Leaving ProcessSchedule (cur=0x%x)\n", currentPCB);
}

//----------------------------------------------------------------------
//
//	ProcessSuspend
//
//	Place a process in suspended animation until it's
//	awakened by Processwakeup.
//
//	NOTE: This must only be called from an interrupt or trap.  It
//	should be immediately followed by ProcessSchedule().
//
//----------------------------------------------------------------------
  void
ProcessSuspend (PCB *suspend)
{

  // Make sure it's already a runnable process.
  dbprintf ('p', "Suspending PCB 0x%x (%s) at %d.\n", suspend, suspend->name,my_timer_get());
  suspend->sleeptime = my_timer_get();
  ASSERT (suspend->flags & PROCESS_STATUS_RUNNABLE,
      "Trying to suspend a non-running process!\n");
  ProcessSetStatus (suspend, PROCESS_STATUS_WAITING);
  QueueRemove (&suspend->l);
  QueueInsertLast (&waitQueue, &suspend->l);
}

//----------------------------------------------------------------------
//
//	ProcessWakeup
//
//	Wake up a process from its slumber.  This only involves putting
//	it on the run queue; it's not guaranteed to be the next one to
//	run.
//
//	NOTE: This must only be called from an interrupt or trap.  It
//	need not be followed immediately by ProcessSchedule() because
//	the currently running process is unaffected.
//
//----------------------------------------------------------------------
  void
ProcessWakeup (PCB *wakeup)
{
  dbprintf ('p',"Waking up PCB 0x%x.\n", wakeup);
  // Make sure it's not yet a runnable process.
  ASSERT (wakeup->flags & PROCESS_STATUS_WAITING,
      "Trying to wake up a non-sleeping process!\n");
  ProcessSetStatus (wakeup, PROCESS_STATUS_RUNNABLE);
  QueueRemove (&wakeup->l);
  QueueInsertLast (&runQueue, &wakeup->l);

}


//----------------------------------------------------------------------
//
//	ProcessDestroy
//
//	Destroy a process by setting its status to zombie and putting it
//	on the zombie queue.  The next time the scheduler is called, this
//	process will be marked as free.  We can't necessarily do it now
//	because we might be the currently running process.
//
//	NOTE: This must only be called from an interrupt or trap.  However,
//	it need not be followed immediately by a ProcessSchedule() because
//	the process can continue running.
//
//----------------------------------------------------------------------
  void
ProcessDestroy (PCB *pcb)
{
  dbprintf('p', "Entering ProcessDestroy for 0x%x.\n", pcb);
  ProcessSetStatus (pcb, PROCESS_STATUS_ZOMBIE);
  QueueRemove (&pcb->l);
  QueueInsertFirst (&zombieQueue, &pcb->l);
  dbprintf('p', "Leaving ProcessDestroy for 0x%x.\n", pcb);
}

//----------------------------------------------------------------------
//
//	ProcessExit
//
//	This routine is called to exit from a system process.  It simply
//	calls an exit trap, which will be caught to exit the process.
//
//----------------------------------------------------------------------
static
  void
ProcessExit ()
{
  exit ();
}

uint32 get_argument(char *string)
{
  static char *str;
  static int location=0;
  int location2;

  if(string)
  {
    str=string;
    location = 0;
  }

  location2 = location;

  if(str[location]=='\0'||location>=99)
    return 0;

  for(;location<100;location++)
  {
    if(str[location]=='\0')
    {
      location++;
      break;
    }
  }
  return location2;
}


//----------------------------------------------------------------------
//
//	ProcessFork
//
//	Create a new process and make it runnable.  This involves the
//	following steps:
//	* Allocate resources for the process (PCB, memory, etc.)
//	* Initialize the resources
//	* Place the PCB on the runnable queue
//
//	NOTE: This code has been tested for system processes, but not
//	for user processes.
//
//----------------------------------------------------------------------
  int
ProcessFork (VoidFunc func, uint32 param, int p_nice, int p_info,char *name, int isUser)
{
  int		i, j, fd, n;
  Link		*l;
  int		start, codeS, codeL, dataS, dataL;
  uint32	*stackframe;
  int		newPage;
  PCB		*pcb;
  int	addr = 0;
  int		intrs;
  unsigned char buf[100];
  uint32 dum[MAX_ARGS+8], count, offset;
  char *str;


  intrs = DisableIntrs ();
  dbprintf ('I', "Old interrupt value was 0x%x.\n", intrs);
  dbprintf ('p', "Entering ProcessFork args=0x%x 0x%x %s %d\n", func,
      param, name, isUser);
  // Get a free PCB for the new process
  if (QueueEmpty (&freepcbs)) {
    printf ("FATAL error: no free processes!\n");
    exitsim ();	// NEVER RETURNS!
  }
  l = QueueFirst (&freepcbs);
  dbprintf ('p', "Got a link @ 0x%x\n", l);
  QueueRemove (l);
  pcb = (PCB *)(l->object);
  // This prevents someone else from grabbing this process
  ProcessSetStatus (pcb, PROCESS_STATUS_RUNNABLE);

  // At this point, the PCB is allocated and nobody else can get it.
  // However, it's not in the run queue, so it won't be run.  Thus, we
  // can turn on interrupts here.
  dbprintf ('I', "Before restore interrupt value is 0x%x.\n", CurrentIntrs());
  RestoreIntrs (intrs);
  dbprintf ('I', "New interrupt value is 0x%x.\n", CurrentIntrs());

  // Copy the process name into the PCB.
  dstrcpy (pcb->name, name);

  //----------------------------------------------------------------------
  // This section initializes the memory for this process
  //----------------------------------------------------------------------
  // For now, we'll use one user page and a page for the system stack.
  // For system processes, though, all pages must be contiguous.
  // Of course, system processes probably need just a single page for
  // their stack, and don't need any code or data pages allocated for them.
  pcb->npages = 1;
  newPage = MemoryAllocPage ();
  if (newPage == 0) {
    printf ("aFATAL: couldn't allocate memory - no free pages!\n");
    exitsim ();	// NEVER RETURNS!
  }
  pcb->pagetable[0] = MemorySetupPte (newPage);
  newPage = MemoryAllocPage ();
  if (newPage == 0) {
    printf ("bFATAL: couldn't allocate system stack - no free pages!\n");
    exitsim ();	// NEVER RETURNS!
  }
  pcb->sysStackArea = newPage * MEMORY_PAGE_SIZE;


  //---------------------------------------
  // Lab3: initialized pcb member for your scheduling algorithm here
  // BEGIN BRIAN CODE
  if((isUser && p_nice < 0) || p_nice > 19) {  // p_nice should never be greater than 19
    pcb->p_nice = 0;
  }
  else pcb->p_nice = p_nice;
  pcb->estcpu		= 0;
  pcb->runtime		= 0;
  pcb->sleeptime	= 0;
  pcb->prio		= PUSER;
  pcb->runQueueNum	= (pcb->prio)/4;
  pcb->load		= 1;
  pcb->p_info		= p_info;
  pcb->p_quanta		= 0;
  // END BRIAN CODE
  //--------------------------------------


  //----------------------------------------------------------------------
  // Stacks grow down from the top.  The current system stack pointer has
  // to be set to the bottom of the interrupt stack frame, which is at the
  // high end (address-wise) of the system stack.
  stackframe = ((uint32 *)(pcb->sysStackArea + MEMORY_PAGE_SIZE)) -
    (PROCESS_STACK_FRAME_SIZE + 8);
  // The system stack pointer is set to the base of the current interrupt
  // stack frame.
  pcb->sysStackPtr = stackframe;
  // The current stack frame pointer is set to the same thing.
  pcb->currentSavedFrame = stackframe;

  dbprintf ('p',
      "Setting up PCB @ 0x%x (sys stack=0x%x, mem=0x%x, size=0x%x)\n",
      pcb, pcb->sysStackArea, pcb->pagetable[0],
      pcb->npages * MEMORY_PAGE_SIZE);

  //----------------------------------------------------------------------
  // This section sets up the stack frame for the process.  This is done
  // so that the frame looks to the interrupt handler like the process
  // was "suspended" right before it began execution.  The standard
  // mechanism of swapping in the registers and returning to the place
  // where it was "interrupted" will then work.
  //----------------------------------------------------------------------

  // The previous stack frame pointer is set to 0, meaning there is no
  // previous frame.
  stackframe[PROCESS_STACK_PREV_FRAME] = 0;

  // Set the base of the level 1 page table.  If there's only one page
  // table level, this is it.  For 2-level page tables, put the address
  // of the level 1 page table here.  For 2-level page tables, we'll also
  // have to build up the necessary tables....
  stackframe[PROCESS_STACK_PTBASE] = (uint32)&(pcb->pagetable[0]);

  // Set the size (maximum number of entries) of the level 1 page table.
  // In our case, it's just one page, but it could be larger.
  stackframe[PROCESS_STACK_PTSIZE] = pcb->npages;

  // Set the number of bits for both the level 1 and level 2 page tables.
  // This can be changed on a per-process basis if desired.  For now,
  // though, it's fixed.
  stackframe[PROCESS_STACK_PTBITS] = (MEMORY_L1_PAGE_SIZE_BITS
      + (MEMORY_L2_PAGE_SIZE_BITS << 16));


  if (isUser) {
    dbprintf ('p', "About to load %s\n", name);
    fd = ProcessGetCodeInfo (name, &start, &codeS, &codeL, &dataS, &dataL);
    if (fd < 0) {
      dbprintf('p', "Could not get code info for: %s (%i)\n",name,fd);
      // Free newpage and pcb so we don't run out...
      ProcessFreeResources (pcb);
      return (-1);
    }
    dbprintf ('p', "File %s -> start=0x%08x\n", name, start);
    dbprintf ('p', "File %s -> code @ 0x%08x (size=0x%08x)\n", name, codeS,
        codeL);
    dbprintf ('p', "File %s -> data @ 0x%08x (size=0x%08x)\n", name, dataS,
        dataL);
    while ((n = ProcessGetFromFile (fd, buf, &addr, sizeof (buf))) > 0) {
      dbprintf ('p', "Placing %d bytes at vaddr %08x.\n", n, addr - n);
      // Copy the data to user memory.  Note that the user memory needs to
      // have enough space so that this copy will succeed!
      MemoryCopySystemToUser (pcb, buf, addr - n, n);
    }
    FsClose (fd);
    stackframe[PROCESS_STACK_ISR] = PROCESS_INIT_ISR_USER;
    // Set the initial stack pointer correctly.  Currently, it's just set
    // to the top of the (single) user address space allocated to this
    // process.
    str = (char *)param;
    stackframe[PROCESS_STACK_IREG+29] = MEMORY_PAGE_SIZE - SIZE_ARG_BUFF;
    // Copy the initial parameter to the top of stack

    MemoryCopySystemToUser (pcb, (char *)str,
        (char *)stackframe[PROCESS_STACK_IREG+29],
        SIZE_ARG_BUFF-32);

    offset = get_argument((char *)param);

    dum[2] = MEMORY_PAGE_SIZE - SIZE_ARG_BUFF + offset;
    for(count=3;;count++)
    {
      offset=get_argument(NULL);
      dum[count] = MEMORY_PAGE_SIZE - SIZE_ARG_BUFF + offset;
      if(offset==0)
      {
        break;
      }
    }
    dum[0] = count-2;
    dum[1] = MEMORY_PAGE_SIZE - SIZE_ARG_BUFF - (count-2)*4;
    MemoryCopySystemToUser (pcb, (char *)dum,
        (char *)(stackframe[PROCESS_STACK_IREG+29]-count*4),
        (count)*sizeof(uint32));
    stackframe[PROCESS_STACK_IREG+29] -= 4*count;
    // Set the correct address at which to execute a user process.
    stackframe[PROCESS_STACK_IAR] = (uint32)start;
    pcb->flags |= PROCESS_TYPE_USER;
  } else {
    // Set r31 to ProcessExit().  This will only be called for a system
    // process; user processes do an exit() trap.
    stackframe[PROCESS_STACK_IREG+31] = (uint32)ProcessExit;

    // Set the stack register to the base of the system stack.
    stackframe[PROCESS_STACK_IREG+29]=pcb->sysStackArea + MEMORY_PAGE_SIZE-32;

    // Set the initial parameter properly by placing it on the stack frame
    // at the location pointed to by the "saved" stack pointer (r29).
    *((uint32 *)(stackframe[PROCESS_STACK_IREG+29])) = param;

    // Set up the initial address at which to execute.  This is done by
    // placing the address into the IAR slot of the stack frame.
    stackframe[PROCESS_STACK_IAR] = (uint32)func;

    // Set the initial value for the interrupt status register
    stackframe[PROCESS_STACK_ISR] = PROCESS_INIT_ISR_SYS;

    // Mark this as a system process.
    pcb->flags |= PROCESS_TYPE_SYSTEM;
  }

  // Place the PCB onto the run queue.
  intrs = DisableIntrs ();
  dbprintf('p', "Inserting last [%i] (%p, %p)\n",PUSER/4,l,l->object);
  QueueInsertLast (&runQueue[PUSER/4], l);
  RestoreIntrs (intrs);

  // If this is the first process, make it the current one
  if (currentPCB == NULL) {
    dbprintf ('p', "Setting currentPCB=0x%x, stackframe=0x%x\n",
        pcb, pcb->currentSavedFrame);
    currentPCB = pcb;
  }
  dbprintf ('p', "Leaving ProcessFork (%s)\n", name);
  // Return the process number (found by subtracting the PCB number
  // from the base of the PCB array).
  return (pcb - pcbs);
}

//----------------------------------------------------------------------
//
//	getxvalue
//
//	Convert a hex digit into an actual value.
//
//----------------------------------------------------------------------
static
inline
  int
getxvalue (int x)
{
  if ((x >= '0') && (x <= '9')) {
    return (x - '0');
  } else if ((x >= 'a') && (x <= 'f')) {
    return (x + 10 - 'a');
  } else if ((x >= 'A') && (x <= 'F')) {
    return (x + 10 - 'A');
  } else {
    return (0);
  }
}

//----------------------------------------------------------------------
//
//	ProcessGetCodeSizes
//
//	Get the code sizes (stack & data) for a file.  A file descriptor
//	for the named file is returned.  This descriptor MUST be closed
//	(presumably by the caller) at some point.
//
//----------------------------------------------------------------------
  int
ProcessGetCodeInfo (const char *file, uint32 *startAddr,
    uint32 *codeStart, uint32 *codeSize,
    uint32 *dataStart, uint32 *dataSize)
{
  int		fd;
  int		totalsize;
  char		buf[100];
  char		*pos;

  // Open the file for reading.  If it returns a negative number, the open
  // didn't work.
  if ((fd = FsOpen (file, FS_MODE_READ)) < 0) {
    dbprintf ('f', "ProcessGetCodeInfo: open of %s failed (%d).\n",
        file, fd);
    return (-1);
  }
  dbprintf ('f', "File descriptor is now %d.\n", fd);
  if ((totalsize = FsRead (fd, buf, sizeof (buf))) != sizeof (buf)) {
    dbprintf ('f', "ProcessGetCodeInfo: read got %d (not %d) bytes from %s\n",
        totalsize, sizeof (buf), file);
    FsClose (fd);
    return (-1);
  }
  if (dstrstr (buf, "start:") == NULL) {
    dbprintf ('f', "ProcessGetCodeInfo: %s missing start line (not a DLX executable?)\n", file);
    return (-1);
  }
  pos = (char *)dindex (buf, ':') + 1;
  // Get the start address and overall size
  *startAddr = dstrtol (pos, &pos, 16);
  totalsize = dstrtol (pos, &pos, 16);
  // Get code & data section start & sizes
  *codeStart = dstrtol (pos, &pos, 16);
  *codeSize = dstrtol (pos, &pos, 16);
  *dataStart = dstrtol (pos, &pos, 16);
  *dataSize = dstrtol (pos, &pos, 16);
  // Seek to start of first real line
  FsSeek (fd, 1 + dindex (buf, '\n') - buf, 0);
  return (fd);
}


//----------------------------------------------------------------------
//
//	ProcessGetFromFile
//
//	Inputs:
//	addr -	points to an integer that contains the address of
//		the byte past that previously returned.  If this is the
//		first call to this routine, *addr should be set to 0.
//	fd -	File descriptor from which to read data.  The file format
//		is the same as that used by the DLX simulator.
//	buf -	points to a buffer that will receive data from the input
//		file.  Note that the data is NOT 0-terminated, and may
//		include any number of 0 bytes.
//	max -	maximum length of data to return.  The routine collects data
//		until either the address changes or it has read max bytes.
//
//	Returns the number of bytes actually stored into buf.  In addition,
//	*addr is updated to point to the byte following the last byte in
//	the buffer.
//
//	Load a file into memory.  The file format consists of a
//	leading address, followed by a colon, followed by the data
//	to go at that address.  If the address is omitted, the data
//	follows that from the previous line of the file.
//
//----------------------------------------------------------------------
  int
ProcessGetFromFile (int fd, unsigned char *buf, uint32 *addr, int max)
{
  char	localbuf[204];
  int	nbytes;
  int	seekpos;
  unsigned char *pos = buf;
  char	*lpos = localbuf;

  // Remember our position at the start of the routine so we can adjust
  // it later.
  seekpos = FsSeek (fd, 0, FS_SEEK_CUR);
  // The maximum number of characters we could read is limited to the
  // maximum buffer space available to the caller * 2 because each 2
  // characters in the input file result in a single byte of program
  // info being read in.
  max = max * 2;
  // If the callers maximum is greater than the available buffer space,
  // limit the buffer space further.
  if (max > (sizeof(localbuf)-4)) {
    max = sizeof(localbuf)-4;
  }
  if ((nbytes = FsRead (fd, localbuf, max)) <= 0) {
    return (0);
  }
  // 'Z' is unused in load file, so use it to mark the end of the buffer
  // Back up until just after the last newline in the data we read.
  dbprintf ('f', "Got %d bytes at offset %d ...", nbytes, seekpos);
  while (localbuf[--nbytes] != '\n') {
  }
  localbuf[nbytes+1] = 'Z';
  localbuf[nbytes+2] = '\0';
  dbprintf ('f', " terminated at %d.\n", nbytes);
  dbprintf ('f', "Buffer is '%s'\n", localbuf);
  nbytes = 0;
  while (dindex (lpos, 'Z') != NULL) {
    if (dindex (lpos, ':') == NULL) {
      break;
    }
    if (*lpos != ':') {
      // If we're going to go to a new address, we break out of the
      // loop and return what we've got already.
      if (nbytes > 0) {
        break;
      }
      *addr = dstrtol (lpos, &lpos, 16);
      dbprintf ('f', "New address is 0x%x.\n", *addr);
    }
    if (*lpos != ':') {
      break;
    }
    lpos++;	// skip past colon
    while (1) {
      while (((*lpos) == ' ') || (*lpos == '\t')) {
        lpos++;
      }
      if (*lpos == '\n') {
        lpos++;
        break;
      } else if (!(isxdigit (*lpos) && isxdigit (*(lpos+1)))) {
        // Exit loop if at least one digit isn't a hex digit.
        break;
      }
      pos[nbytes++] = (getxvalue(*lpos) * 16) + getxvalue(*(lpos+1));
      lpos += 2;
      (*addr)++;
    }
  }
  // Seek to just past the last line we read.
  FsSeek (fd, seekpos + lpos - localbuf, FS_SEEK_SET);
  dbprintf ('f', "Seeking to %d and returning %d bytes!\n",
      seekpos + lpos - localbuf, nbytes);
  return (nbytes);
}


//----------------------------------------------------------------------
//
//	main
//
//	This routine is called when the OS starts up.  It allocates a
//	PCB for the first process - the one corresponding to the initial
//	thread of execution.  Note that the stack pointer is already
//	set correctly by _osinit (assembly language code) to point
//	to the stack for the 0th process.  This stack isn't very big,
//	though, so it should be replaced by the system stack of the
//	currently running process.
//
//----------------------------------------------------------------------
main (int argc, char *argv[])
{
  int		i, j;
  int		n;
  char	buf[120];
  char		*userprog = (char *)0;
  static PCB	temppcb;
  uint32	addr;
  extern void	SysprocCreateProcesses ();
  char *param[12]={NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL};
  int base;

  //  debugstr[0] = 'p';
  debugstr[4] = '\0';
  MyFuncRetZero();
  printf ("Got %d arguments.\n", argc);
  printf ("Available memory: 0x%x -> 0x%x.\n", lastosaddress,
      MemoryGetSize ());
  printf ("Argument count is %d.\n", argc);
  for (i = 0; i < argc; i++) {
    printf ("Argument %d is %s.\n", i, argv[i]);
  }
  //  *((int *)0xfff00100) = 't';
  FsModuleInit ();
  for (i = 0; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      switch (argv[i][1])
      {
        case 'D':
          dstrcpy (debugstr, argv[++i]);
          break;
        case 'i':
          n = dstrtol (argv[++i], (void *)0, 0);
          ditoa (n, buf);
          printf ("Converted %s to %d=%s\n", argv[i], n, buf);
          break;
        case 'f':
          {
            int	start, codeS, codeL, dataS, dataL, fd, j;
            int	addr = 0;
            static unsigned char buf[200];
            fd = ProcessGetCodeInfo (argv[++i], &start, &codeS, &codeL, &dataS,
                &dataL);
            printf ("File %s -> start=0x%08x\n", argv[i], start);
            printf ("File %s -> code @ 0x%08x (size=0x%08x)\n", argv[i], codeS,
                codeL);
            printf ("File %s -> data @ 0x%08x (size=0x%08x)\n", argv[i], dataS,
                dataL);
            while ((n = ProcessGetFromFile (fd, buf, &addr, sizeof (buf))) > 0)
            {
              for (j = 0; j < n; j += 4)
              {
                printf ("%08x: %02x%02x%02x%02x\n", addr + j - n, buf[j], buf[j+1],
                    buf[j+2], buf[j+3]);
              }
            }
            close (fd);
            break;
          }
        case 'u':
          userprog = argv[++i];
          base = i;
          break;
        default:
          printf ("Option %s not recognized.\n", argv[i]);
          break;
      }
      if(userprog)
        break;
    }
  }
  dbprintf ('i', "About to initialize queues.\n");
  QueueModuleInit ();
  dbprintf ('i', "After initializing queues.\n");
  MemoryModuleInit ();
  dbprintf ('i', "After initializing memory.\n");

  ProcessModuleInit ();
  dbprintf ('i', "After initializing processes.\n");
  ShareModuleInit ();
  dbprintf ('i', "After initializing shared memory.\n");
  SynchModuleInit ();
  dbprintf ('i', "After initializing synchronization tools.\n");
  KbdModuleInit ();
  dbprintf ('i', "After initializing keyboard.\n");
  for (i = 0; i < 100; i++) {
    buf[i] = 'a';
  }
  i = FsOpen ("vm", FS_MODE_WRITE);
  dbprintf ('i', "VM Descriptor is %d\n", i);
  FsSeek (i, 0, FS_SEEK_SET);
  FsWrite (i, buf, 80);
  FsClose (i);
  if (userprog != (char *)0) {
    for(i=base;i<argc&&i-base<11; i++)
    {
      param[i-base] = argv[i];
    }
    process_create(0,0,param[0], param[1], param[2], param[3], param[4],
        param[5], param[6], param[7], param[8], param[9],
        param[10], param[11]);
    //    ProcessFork (0, (uint32)"Help Me man!", userprog, 1);
  }

  SysprocCreateProcesses ();
  dbprintf ('i', "Created processes - about to set timer quantum.\n");
  TimerSet (processQuantum);
  dbprintf ('i', "Set timer quantum to %d, about to run first process.\n",
      processQuantum);
  intrreturn ();
  // Should never be called because the scheduler exits when there
  // are no runnable processes left.
  exitsim();	// NEVER RETURNS!
}

float MyFuncRetZero()
{
  return 0;
}

unsigned GetCurrentPid()
{
  return (unsigned)(currentPCB - pcbs);
}

unsigned findpid(PCB *pcb)
{
  return (unsigned)(pcb - pcbs);
}

void process_create(int p_nice, int p_info, char *name, ...)
{
  char **args;
  int i, j, k;
  char allargs[1000];
  args = &name;

  k=0;
  for(i=0; args[i]!=NULL; i++)
  {
    j=0;
    do {
      allargs[k] = args[i][j];
      j++; k++;
    } while(args[i][j-1]!='\0');
  }
  allargs[k] = allargs[k+1] = 0;
  ProcessFork(0, (uint32)allargs, p_nice,p_info,name, 1);
}
