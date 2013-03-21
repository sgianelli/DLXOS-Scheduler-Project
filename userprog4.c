#include "lab2-api.h"
main (int argc, char *argv[])
{
  int number, i,j,offset;
  uint32 handle;
  sem_t semaphore;
  char num_str[10], semaphore_str[10];

  switch(argc)
  {
    case 2:
      number = dstrtol(argv[1], NULL, 10);
      Printf("Setting number = %d\n", number);
      semaphore = sem_create(1);
      ditoa(semaphore, semaphore_str);	//Convert the semaphore to a string
  
      for(i=0; i<number; i++)
      {
        ditoa(i, num_str);
        process_create(i,1,"userprog5.dlx.obj", num_str,semaphore_str, 
  			NULL);
      }
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
