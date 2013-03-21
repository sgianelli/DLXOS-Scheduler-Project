#
# This is a makefile for the DLX OS projects.
#

CC = gcc-dlx
AS = dlxasm
CFLAGS = -mtraps -O3

#INCS = $(wildcard *.h)
#SRCS = $(wildcard *.c)
#OBJS = $(addsuffix .o, $(basename $(wildcard *.c))) \
#       $(addsuffix .o, $(basename $(wildcard *.s)))
INCS = dlxos.h traps.h filesys.h memory.h misc.h process.h queue.h \
        syscall.h share_memory.h
SRCS = filesys.c memory.c misc.c process.c queue.c traps.c sysproc.c 
OBJS = $(addsuffix .o, $(basename $(SRCS)))

all: os.dlx.obj  userprog3 userprog4

part2: os.dlx.obj userprog3 userprog4 

os.dlx.obj: os.dlx
	$(AS) -i _osinit -l os.lst os.dlx
	mv os.dlx.obj ../execs

os.dlx: $(OBJS) dlxos.o trap_random.o osend.o share_memory.o synch.o
	$(CC) -mtraps -O3 dlxos.o trap_random.o share_memory.o synch.o $(OBJS) osend.o -o os.dlx

share_memory.o: share_memory.api
	cp share_memory.api share_memory.o

synch.o: synch.bin
	cp synch.bin synch.o

osend.o: osend.s
	$(CC) -c osend.s

trap_random.o: trap_random.s
	$(CC) -c trap_random.s

dlxos.o: dlxos.s
	$(CC) -c dlxos.s

usertraps.o: usertraps.s
	$(CC) -c usertraps.s

userprog3 : userprog3.o usertraps.o misc.o
	$(CC) -mtraps -O3 userprog3.o usertraps.o misc.o -o userprog3.dlx
	$(AS) -l userprog3.lst userprog3.dlx
	mv userprog3.dlx.obj ../execs

userprog4 : userprog4.o usertraps.o misc.o
	$(CC) -mtraps -O3 userprog4.o usertraps.o misc.o -o userprog4.dlx
	$(AS) -l userprog4.lst userprog4.dlx
	mv userprog4.dlx.obj ../execs

Makefile.depend: depend

depend: $(SRCS) $(INCS)
	$(CC) -MM $(SRCS) > Makefile.depend

clean:
	/bin/rm -f *.o *.dlx *.lst ../execs/*.obj Makefile.depend ../execs/vm

include Makefile.depend
