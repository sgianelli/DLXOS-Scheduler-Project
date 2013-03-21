/***********************************************************************
 *
 * share_memory.h
 * Copyright: Uday Savagaonkar (2002). All rights reserved.
 * Share memory API for dlxos
 *
 ***********************************************************************/

#ifndef _SHARE_MEMORY_H_
#define _SHARE_MEMORY_H_

#include "dlxos.h"
#include "memory.h"
#include "process.h"

typedef struct shared_page {
  uint32 page;	//The physical page corresponding to this block
  		//This page is not shared if ==0
  uint32 using[(PROCESS_MAX_PROCS+31)/32];
  		//List of process using this page
} shared_page;

#define MEMORY_MAX_SHARED_PAGES 32 //Maximum number of pages that can be
				   //shared amongst different processes
#define PROCESS_MAX_PAGES 16	   //Maximum number of pages allowed in Level
				   //1 page table

void SharedInitModule();	//Turns on the shared memory module
uint32 MemoryCreateSharedPage(PCB *pcb);
				//Creates a shared page in the memory
void *mmap(PCB *pcb, uint32 handle);
				//Maps a shared page to the virtual address
				//space
int MemoryFreeSharedPage(PCB *pcb, uint32 handle);
				//Releases a shared page. The page is not
				//actually deallocated untill all the process
				//release it.

#endif _SHARE_MEMORY_H_
