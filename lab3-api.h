/*****************************************************************
 *
 * Various APIs for Lab 2 of OS469
 * Copyright: Uday Savagaonkar, Jan 2002
 *
 *****************************************************************/

#ifndef _LAB3_API_H_
#define _LAB3_API_H_

#define NULL (void *)0x0
#define INVALID_SEM	-1

typedef unsigned int uint32;
typedef int sem_t;
typedef int lock_t;
typedef int cond_t;
typedef int mbox_t;

//Related to processes
uint32 getpid();			//trap 0x431
void process_create(int p_nice, int p_info, char *arg1, ...);  	
					//trap 0x432

//Related to shared memory
uint32 shmget();			//trap 0x440
void *shmat(uint32 handle);		//trap 0x441

//Related to semaphores
sem_t sem_create(int count);		//trap 0x450
int sem_wait(sem_t sem);		//trap 0x451
int sem_signal(sem_t sem);		//trap 0x452

lock_t lock_create();			//trap 0x453
int lock_acquire(lock_t lock);		//trap 0x454
int lock_release(lock_t lock);		//trap 0x455

cond_t cond_create(lock_t lock);	//trap 0x456
int cond_wait(cond_t cond);		//trap 0x457
int cond_signal(cond_t cond);		//trap 0x458
int cond_broadcast(cond_t cond);	//trap 0x459

int mbox_open(mbox_t); 			//TRAP 0x460
int mbox_close(mbox_t);			//TRAP 0x461
int mbox_send(mbox_t,int,char*);	//TRAP 0x462
int mbox_recv(mbox_t, int*, char*);	//TRAP 0x463
int mbox_stat(mbox_t);			//TRAP 0x464

#endif _LAB3_API_H_

