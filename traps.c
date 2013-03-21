//
//	traps.c
//
//	This file handles the low-level trap stuff for an operating system
//	running under DLX.

static char rcsid[] = "$Id: traps.c,v 1.1 2000/09/20 01:50:19 elm Exp elm $";

#include "dlx.h"
#include "dlxos.h"
#include "traps.h"
#include "process.h"

unsigned long MY_TIMER = 0;
//----------------------------------------------------------------------
//
//	TimerSet
//
//	Set the timer to go off after a particular number of microseconds.
//	The number of microseconds is passed to the routine.
//
//----------------------------------------------------------------------
void
TimerSet (int us)
{
  //MY_TIMER = *((int *)DLX_TIMER_ADDRESS);
  MY_TIMER += us;
  *((int *)DLX_TIMER_ADDRESS) = us;
}

int TimerGet(void)
{
  return (MY_TIMER / 1000);
}

//----------------------------------------------------------------------
//
//	KbdModuleInit
//
//	Initialize the keyboard module.  This involves turning on
//	interrupts for the keyboard.
//
//----------------------------------------------------------------------
void
KbdModuleInit ()
{
  *((uint32 *)DLX_KBD_INTR) = 1;
}

//--------------------------------------------------------------------
// GetUintFromTrapArg(uint32 *trapArgs, int sysmode)
//--------------------------------------------------------------------
static uint32 GetUintFromTrapArg(uint32 *trapArgs, int sysmode)
{
  uint32 arg;
  if(!sysmode)
  {
    MemoryCopyUserToSystem (currentPCB, trapArgs, &arg, sizeof (arg));
  }
  else
  {
    bcopy (trapArgs, &arg, sizeof (arg));
  }

  return arg;
}

//--------------------------------------------------------------------
// GetIntFromTrapArg(uint32 *trapArgs, int sysmode)
//--------------------------------------------------------------------
static int GetIntFromTrapArg(uint32 *trapArgs, int sysmode)
{
  int arg;
  if(!sysmode)
  {
    MemoryCopyUserToSystem (currentPCB, trapArgs, &arg, sizeof (arg));
  }
  else
  {
    bcopy (trapArgs, &arg, sizeof (arg));
  }

  return arg;
}
//--------------------------------------------------------------------
// Here we support reading the arguments
// Maximum 128 command-line arguments are allowed
// Also the total length of the arguments including the terminating
// '\0' should be less than or equal to 1024
//--------------------------------------------------------------------

static void TrapProcessCreateHandler(uint32 *trapArgs, int sysmode)
{
  char allargs[SIZE_ARG_BUFF];
  char name[100];
  int i=0, j=0, k=0;
  uint32 args[MAX_ARGS];
  int p_nice, p_info;
  char *destination;
  // The first argument is the name of the executable file

  i=0;
  for(i=0;i<SIZE_ARG_BUFF; i++)
    allargs[i] = 0;
  i=0;
  if(!sysmode)
  {
    //Get the arguments into the sytem space
    MemoryCopyUserToSystem (currentPCB, trapArgs, args, sizeof (args));
    do {
      MemoryCopyUserToSystem (currentPCB,((char*)args[2])+i,name+i,1);
      i++;
    } while ((i < sizeof (name)) && (name[i-1] != '\0'));
  } else {
    bcopy (trapArgs, args, sizeof (args));
    dstrncpy ((char *)args[2], name, sizeof (name));
  }
  name[sizeof(name)-1] = '\0';	// null terminate the name
  i=0;

  p_nice = args[0];
  p_info = args[1];
  if (p_nice <-20 || p_nice >19)
  {
    printf(" Invalid p_nice value! \n");
    exit();
  }

  if(!sysmode)
  {
    if (p_nice <0)
     p_nice == 0;
    //Copy the rest of the arguments to the system space
    for(j=2; (j<MAX_ARGS)&&(args[j]!=0); j++)
    {
      k=0;
      do
      {
        MemoryCopyUserToSystem (currentPCB,((char*)args[j])+k,allargs+i,1);
        i++; k++;
      } while ((i<sizeof(allargs)) && (allargs[i-1]!='\0'));
    }
  }
  else
  {
    destination = &allargs[0];
    for(j=0; (j<11)&&(args[j]!=0); j++)
    {
      k = dstrlen((char *)args[j]);  //length of the argument
      if(&destination[k]-allargs>SIZE_ARG_BUFF)
      {
        printf("Fatal: Cumulative length of all arguments > %d\n",
		SIZE_ARG_BUFF);
	exitsim();
      }
      dstrcpy(destination, (char *)args[j]);
      destination[k] = '\0';
    }
  }
  allargs[sizeof(allargs)-1] = '\0';	// null terminate the name

  ProcessFork(0, (uint32)allargs, p_nice,p_info,name, 1);
}


//----------------------------------------------------------------------
//
//	TrapPrintfHandler
//
//	Handle a printf trap.here.  This printf code will correctly
//	handle integers but will not correctly handle strings (yet).
//	Also, note that the maximum format string length is 79 characters.
//	Exceeding this length will cause the format to be truncated.
//
//----------------------------------------------------------------------
static
void
TrapPrintfHandler (uint32 *trapArgs, int sysMode)
{
  char	formatstr[80];
  int	i = 0;
  uint32 printfArgs[10];
  uint32 args[10];
  int	nargs = 0;
  char	*c;

  // The first argument is the print format string.  Copy it to system
  // space, truncating if necessary.
  i = 0;
  if (!sysMode) {
    // Get the arguments themselves into system space
    MemoryCopyUserToSystem (currentPCB, trapArgs, args, sizeof (args));
    do {
      MemoryCopyUserToSystem (currentPCB,((char*)args[0])+i,formatstr+i,1);
      i++;
    } while ((i < sizeof (formatstr)) && (formatstr[i-1] != '\0'));
  } else {
    bcopy (trapArgs, args, sizeof (args));
    dstrncpy ((char *)args[0], formatstr, sizeof (formatstr));
  }
  formatstr[sizeof(formatstr)-1] = '\0';	// null terminate the fmt str

  for (c = formatstr; *c != '\0'; c++) {
    if (*c == '%') {
      // if this is a %%, skip past second %
      if (*(c+1) == '%') {
	c++;
	continue;
      }
      // Get the current argument off the stack
      printfArgs[nargs] = args[nargs+1];
//      dbprintf ('t', "Argument %d at 0x%x is %d (0x%x).\n", nargs,
//		args[nargs], args[nargs]);
      while (1) {
	c++;
	if (*c == 's') {
	  // Handle strings here.  They don't work for user programs (yet...)
	  break;
	} else if (*c == 'l') {
	  continue;
	} else if ((*c == 'f') || (*c == 'g') || (*c == 'e')) {
	  // If it's a floating point number, it'll be passed as
	  // a double, so grab the second word also.
	  nargs += 1;
	  printfArgs[nargs] = args[nargs+1];
	  break;
	} else if ((*c >= 'a') && (*c <= 'z')) {
	  // If it's another formatting character, it's not
	  // a string, but we can leave the loop anyway.
	  break;
	}
      }
      nargs += 1;
    }
  }
  printf (formatstr,printfArgs[0],printfArgs[1],printfArgs[2], printfArgs[3],
	  printfArgs[4], printfArgs[5], printfArgs[6], printfArgs[7]);
}

static void Yield()
{
 	// Implement function Yield() here

  return;
}

//----------------------------------------------------------------------
//
//	doInterrupt
//
//	Handle an interrupt or trap.
//
//----------------------------------------------------------------------
void
dointerrupt (unsigned int cause, unsigned int iar, unsigned int isr,
	     uint32 *trapArgs)
{
  int	result;
  int	i;
  uint32	args[4];
  int	intrs;
  uint32 handle;
  int ihandle;

  dbprintf ('t',"Interrupt cause=0x%x iar=0x%x isr=0x%x args=0x%08x.\n",
	    cause, iar, isr, trapArgs);
  // If the TRAP_INSTR bit is set, this was from a trap instruction.
  // If the bit isn't set, this was a system interrupt.
  if (cause & TRAP_TRAP_INSTR) {
    cause &= ~TRAP_TRAP_INSTR;
    switch (cause) {
    case TRAP_CONTEXT_SWITCH:
      dbprintf ('t', "Got a context switch trap!\n");
      ProcessSchedule ();
      break;
    case TRAP_EXIT:
      dbprintf ('t', "Got an exit trap!\n");
      ProcessDestroy (currentPCB);
      ProcessSchedule ();
      break;
    case TRAP_PROCESS_FORK:
      dbprintf ('t', "Got a fork trap!\n");
      break;
    case TRAP_PROCESS_SLEEP:
      dbprintf ('t', "Got a process sleep trap!\n");
      ProcessSuspend (currentPCB);
      ProcessSchedule ();
      break;
    case TRAP_PRINTF:
      // Call the trap printf handler and pass the arguments and a flag
      // indicating whether the trap was called from system mode.
      dbprintf ('t', "Got a printf trap!\n");
      TrapPrintfHandler (trapArgs, isr & DLX_STATUS_SYSMODE);
      break;
    case TRAP_OPEN:
      // Get the arguments to the trap handler.  If this is a user mode trap,
      // copy them from user space.
      if (isr & DLX_STATUS_SYSMODE) {
	args[0] = trapArgs[0];
	args[1] = trapArgs[1];
      } else {
	char	filename[32];
	// trapArgs points to the trap arguments in user space.  There are
	// two of them, so copy them to to system space.  The first argument
	// is a string, so it has to be copied to system space and the
	// argument replaced with a pointer to the string in system space.
	MemoryCopyUserToSystem (currentPCB, trapArgs, args, sizeof(args[0])*2);
	MemoryCopyUserToSystem (currentPCB, args[0], filename, 31);
	// Null-terminate the string in case it's longer than 31 characters.
	filename[31] = '\0';
	// Set the argument to be the filename
	args[0] = (uint32)filename;
      }
      // Allow Open() calls to be interruptible!
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, args[1] + 0x10000);
      printf ("Got an open with parameters ('%s',0x%x)\n", args[0], args[1]);
      RestoreIntrs (intrs);
      break;
    case TRAP_CLOSE:
      // Allow Close() calls to be interruptible!
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, -1);
      RestoreIntrs (intrs);
      break;
    case TRAP_READ:
      // Allow Read() calls to be interruptible!
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, -1);
      RestoreIntrs (intrs);
      break;
    case TRAP_WRITE:
      // Allow Write() calls to be interruptible!
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, -1);
      RestoreIntrs (intrs);
      break;
    case TRAP_DELETE:
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, -1);
      RestoreIntrs (intrs);
      break;
    case TRAP_SEEK:
      intrs = EnableIntrs ();
      ProcessSetResult (currentPCB, -1);
      RestoreIntrs (intrs);
      break;
    case TRAP_PROCESS_CREATE:
      TrapProcessCreateHandler(trapArgs, isr & DLX_STATUS_SYSMODE);
      break;
    case TRAP_PROCESS_YIELD:
      Yield();
      //ProcessSetResult(currentPCB, -1);
      break;
    case TRAP_SHARE_CREATE_PAGE:
      handle = MemoryCreateSharedPage(currentPCB);
      ProcessSetResult(currentPCB, handle);
      break;
    case TRAP_SHARE_MAP_PAGE:
      handle = GetUintFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      handle = (uint32)mmap(currentPCB, handle);
      ProcessSetResult(currentPCB, handle);
      break;
    case TRAP_SEM_CREATE:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      ihandle = SemCreate(ihandle);
      ProcessSetResult(currentPCB, ihandle); //Return handle
      break;
    case TRAP_SEM_WAIT:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      handle = SemHandleWait(ihandle);
      ProcessSetResult(currentPCB, handle); //Return 1 or 0
      break;
    case TRAP_SEM_SIGNAL:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      handle = SemHandleSignal(ihandle);
      ProcessSetResult(currentPCB, handle); //Return 1 or 0
      break;
    case TRAP_LOCK_CREATE:
      ihandle = LockCreate();
      ProcessSetResult(currentPCB, ihandle); //Return handle
      break;
    case TRAP_LOCK_ACQUIRE:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      handle = LockHandleAcquire(ihandle);
      ProcessSetResult(currentPCB, handle); //Return 1 or 0
      break;
    case TRAP_LOCK_RELEASE:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      handle = LockHandleRelease(ihandle);
      ProcessSetResult(currentPCB, handle); //Return 1 or 0
      break;
    case TRAP_COND_CREATE:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      ihandle = CondCreate(ihandle);
      ProcessSetResult(currentPCB, ihandle); //Return handle
      break;
    case TRAP_COND_WAIT:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      ihandle = CondHandleWait(ihandle);
      ProcessSetResult(currentPCB, ihandle); //Return 1 or 0
      break;
    case TRAP_COND_SIGNAL:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      ihandle = CondHandleSignal(ihandle);
      ProcessSetResult(currentPCB, ihandle); //Return 1 or 0
      break;
    case TRAP_TIMERGET:
      ihandle = TimerGet();
      ProcessSetResult(currentPCB,ihandle);
      break;
    case TRAP_COND_BROADCAST:
      ihandle = GetIntFromTrapArg(trapArgs, isr & DLX_STATUS_SYSMODE);
      ihandle = CondHandleBroadcast(ihandle);
      ProcessSetResult(currentPCB, ihandle); //Return 1 or 0
      break;
    default:
      printf ("Got an unrecognized trap (0x%x) - exiting!\n",
	      cause);
      exitsim ();
      break;
    }
  } else {
    switch (cause) {
    case TRAP_TIMER:
      dbprintf ('t', "Got a timer interrupt!\n");
      ProcessSchedule ();
      break;
    case TRAP_KBD:
      do {
	i = *((uint32 *)DLX_KBD_NCHARSIN);
	result = *((uint32 *)DLX_KBD_GETCHAR);
	printf ("Got a keyboard interrupt (char=0x%x(%c), nleft=%d)!\n",
		result, result, i);
      } while (i > 1);
      break;
    case TRAP_ACCESS:
      printf ("Exiting after illegal access at iar=0x%x, isr=0x%x\n",
	      iar, isr);
      exitsim ();
      break;
    case TRAP_ADDRESS:
      printf ("Exiting after illegal address at iar=0x%x, isr=0x%x\n",
	      iar, isr);
      exitsim ();
      break;
    case TRAP_ILLEGALINST:
      printf ("Exiting after illegal instruction at iar=0x%x, isr=0x%x\n",
	      iar, isr);
      exitsim ();
      break;
    case TRAP_PAGEFAULT:
      printf ("Exiting after page fault at iar=0x%x, isr=0x%x\n",
	      iar, isr);
      exitsim ();
      break;
    default:
      printf ("Got an unrecognized system interrupt (0x%x) - exiting!\n",
	      cause);
      exitsim ();
      break;
    }
  }
  dbprintf ('t',"About to return from dointerrupt.\n");
  // Note that this return may schedule a new process!
  intrreturn ();
}


