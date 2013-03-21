#include "lab2-api.h"

//----------------------------------
//
// your solution to this question should look something like this
//
//  if you run userprog3.dlx.obj 2, it should output
//
//	A0
//	B0
//	A1
//	A2
//	A3
//	A4
//	A5
//	A6
//	B1
//	A7
//	A8
//	A9
//	B2
//	A10
//	B3
//	A11
//	B4
//	A12
//	B5
//	A13
//	B6
//	A14
//	B7
//	A15
//	B8
//	A16
//	B9
//	A17
//	B10
//	A18
//	B11
//	A19
//	B12
//	A20
//	B13
//	A21
//	B14
//	A22
//	B15
//	A23
//	B16
//	A24
//	B17
//	A25
//	B18
//	A26
//	B19
//	A27
//	B20
//	A28
//	B21
//	A29
//	B22
//	A0
//	B23
//	A1
//	B24
//	A2
//	B25
//	A3
//	B26
//	A4
//	B27
//	A5
//	B28
//	A6
//	B29
//	A7
//	B0
//	A8
//	B1
//	A9
//	B2
//	A10
//	B3
//	A11
//	B4
//	A12
//	B5
//	A13
//	B6
//	A14
//	B7
//	A15
//	B8
//	A16
//	B9
//	A17
//	B10
//	A18
//	B11
//	A19
//	B12
//	A20
//	B13
//	A21
//	B14
//	A22
//	B15
//	A23
//	B16
//	A24
//	B17
//	A25
//	B18
//	A26
//	B19
//	A27
//	B20
//	A28
//	B21
//	A29
//	B22
//	B23
//	B24
//	B25
//	B26
//	B27
//	B28
//	B29
// make sure you understand the scheduling algorithm works b4 you strart hacking
//
//   You are free to modify this file (and other userprogs) to test your solution
//   We will not use the same files to test your code.
//
//----------------------------------------------------------------------------
main (int argc, char *argv[])
{
  int number, i,j,offset;
  uint32 handle;
  sem_t semaphore;
  char num_str[10], semaphore_str[10];

  switch(argc)
  {
    case 2:
    Printf("timer = %d\n", TimerGet());
      number = dstrtol(argv[1], NULL, 10);
      Printf("Setting number = %d\n", number);

      for(i=0; i<50; i++)
      	Printf("1");

	 Printf("timer = %d\n", TimerGet());


      semaphore = sem_create(1);
      ditoa(semaphore, semaphore_str);	//Convert the semaphore to a string

      Printf("timer = %d\n", TimerGet());

      for(i=0; i<number; i++)
      {
        ditoa(i, num_str);
        process_create(i,0,"userprog4.dlx.obj", num_str,semaphore_str,
  			NULL);
      }
      Printf("timer = %d\n", TimerGet());
      yield();
      break;
    case 3:
      offset = dstrtol(argv[1], NULL, 10);       //Get semaphore
      semaphore = dstrtol(argv[2], NULL, 10);       //Get semaphore
      for(i=0;i<30;i++)
      {
        for(j=0;j<50000;j++);
        Printf("%c%d\n",'A'+offset,i);
      }
      for(i=0;i<30;i++)
      {
        sem_wait(semaphore);
        for(j=0;j<50000;j++);
        Printf("%c%d\n",'A'+offset,i);
        sem_signal(semaphore);
      }

      break;
    default:
      Printf("Usage: ");
      Printf(argv[0]);
      Printf(" number\n");
      Printf("argc = %d\n", argc);
      exit();
  }



}
