//
//	synch.h
//
//	Include file for synchronization stuff.  The synchronization
//	primitives include:
//	Semaphore
//	Lock
//	Condition
//
//	Semaphores are the only "native" synchronization primitive.
//	Condition variables and locks are implemented using semaphores.
//

#ifndef	_synch_h_
#define	_synch_h_

#include "queue.h"

typedef struct Sem {
    Queue	waiting;
    int		count;
} Sem;

extern void	SemInit (Sem *, int);
extern void	SemWait (Sem *);
extern void	SemSignal (Sem *);

typedef struct Lock {
  Sem	mutex;		// Overall mutex for the Lock.
  Sem	pri;		// Semaphore on which "high-priority" processes wait
  int	npri;		// Number of high-priority processes waiting
} Lock;

typedef struct Cond {
  Lock	*condLock;
  Sem	condSem;	// Semaphore on which condition variables wait
  int	nwaiters;	// Number of processes waiting for this condition
} Cond;

extern void	LockInit (Lock *);
extern void	LockAcquire (Lock *);
extern void	LockRelease (Lock *);
extern void	CondInit (Cond *, Lock *);
extern void	CondWait (Cond *);
extern void	CondSignal (Cond *);
extern void	CondBroadcast (Cond *);

#endif	//_synch_h_


