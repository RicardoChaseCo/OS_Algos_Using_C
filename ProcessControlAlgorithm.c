#include "stdio.h"
#include "dos.h"
#include "stdlib.h"
#include "conio.h"
#include "windows.h"
#define SEC 3
//#define NULL 0
/*定义结构体*/
typedef struct PCB
{
  int PID; //进程控制号
  int UID;//内部ID号
  struct PCB * next;
}PCB;
PCB *really , *excute , *wait;
/*create queue header */
/*queue operation 入队*/
int enqueue(PCB *head , PCB *node)
{
   PCB *p;
   p = head;
   if(p -> next == NULL)
	{
		head -> next = node;
		return 1;
	}
   while(p)
   {
    if(p -> next == NULL)
	{
		p -> next = node;
		return 1;
	}
	else p = p -> next;
   }
}/*enquue*/
/*dequeue 出队列 */
PCB * dequeue(PCB *head)
{
  PCB *p;
  p = head;
  if(p -> next == NULL)
  {
	  return NULL;
  }
  else
  {
	  p = p -> next;
	  head -> next = p -> next;
	  p -> next = NULL;
	  return p;
  }/*head to next*/
}/*dequeue*/
/*PCB operate*/
/*新建进程*/
void create()
{
	PCB *p;
	p = (PCB*)malloc(sizeof(PCB));
	p -> next = NULL;
	printf("input PID and UID to a new process\n");
	scanf("%d %d",&p -> PID,&p -> UID);
	if(enqueue(really , p))
		printf("create a process: PID = %d UID = %d\n", p -> PID , p -> UID);
	else
		printf("create Failed\n");
}/*create*/
/*执行 fexcute*/
int fexcute()
{
   PCB *p = dequeue(really);
   if(p == NULL)
  {
	   printf("NO process in queue \n");
	   return 0;
  }
   else
  {
	   enqueue(excute , p);
	   printf("add a process into excute queue process: PID = %d UID= %d \n" ,p->PID , p->UID);
	   return 1;
  }
}/*excute*/
int suspend()
{
	PCB *p = dequeue(excute);
	if(p == NULL)
  {
		printf("NO process in queue \n");
		return 0;
  }
   else
  {
	   enqueue(really , p);
       printf("add a process into really queue process: PID = %d UID= %d \n" ,p->PID , p->UID);
       return 1;
  }
}
int wake()
{
  PCB *p = dequeue(wait);
   if(p == NULL)
  {
	   printf("NO process in queue \n");
	   return 0;
  }
   else
  {
	   enqueue(really , p);
	   printf("add a process into wait really process: PID = %d UID= %d \n" ,p->PID , p->UID);
	   return 1;
  }
}
int block()
{
	PCB *p = dequeue(excute);
	if(p == NULL)
	{
		printf("NO process in queue \n");
		return 0;
	}
   else
   {
	   enqueue(wait , p);
	   printf("add a process into wait queue process: PID = %d UID= %d \n" ,p->PID , p->UID);
	   return 1;
  }
}/*block*/
/*输出队列 outputqueue*/
int outputqueue(PCB *head)
{
   PCB *p;
   if(head -> next == NULL)
   {/*队列为空*/
	   printf("queue is null \n");
	   return 1;
   }
   p = head -> next; /*node pointer*/
   while(p)
   {/*打印process id UID*/
	   printf("PID = %d UID = %d \n" , p -> PID , p -> UID);
	   p = p -> next;
   }
   return 0;
}
/*output输出*/
int output()
{
  printf("REALLLY QUEUE:\n");
  outputqueue(really);
  printf("EXCUTE QUEUE: \n");
  outputqueue(excute);
  printf("WAIT QUEUE: \n");
  outputqueue(wait);
}/*output*/
/*init 初始化*/
int init()
{
  PCB *p;
  //clrscr();
  really = (PCB*)malloc(sizeof(PCB));
  really -> next=NULL;
  excute = (PCB*)malloc(sizeof(PCB));
  excute -> next=NULL;
  wait = (PCB*)malloc(sizeof(PCB));
  wait -> next = NULL;
  printf("____________PROCESS SECHUDLE__________\n");
  printf("now initing.....................\n");
  printf("input PID and UID as integer , 0 0 as over\n");
  while(1)
  {
	  p = (PCB*)malloc(sizeof(PCB));
	  p -> next = NULL;
	  scanf("%d %d",&p -> PID , &p -> UID);
	  if(p -> PID == 0 && p -> UID == 0)
		  break;
	  else
	  {
		  if(enqueue(really , p))
			  printf("new process PID = %d UID = %d added!\n",p -> PID , p -> UID);
		  else return 0;
	  }
  }
  return 1;
}/*init*/
/*运行一个process*/
int run()
{
  PCB *p = excute;
  int s = SEC;
  if(excute -> next == NULL)
  {
	  printf("no process in excute queue \n");
      return 0;
  }
  else
  {
	  p = excute -> next;
	  printf("system will sleep %ds as process running\n",s);
      Sleep(3);/*sleep as process runing time*/
      printf("process: PID = %d UID= %d excute successed..\n" , p -> PID , p -> UID );
      excute -> next = p -> next;
      free(p);
  }
}
/*离开*/
void leave()
{
  PCB *p,*t;
  while(really->next || excute->next || wait->next)
  {
	p = really -> next;
	while(p)
	{
		t = p -> next;
		free(p);
		p = t;
	}
	really -> next = NULL;
	p = wait -> next;
	while(p)
	{
		t = p -> next;
		free(p);
		p = t;
	}
	wait -> next = NULL;
	p = excute -> next;
	while(p)
	{
		t = p -> next;
		free(p);
		p = t;
	}
	excute -> next = NULL;
  }
  exit(0);
}/*leave*/
void help()
{
  printf("_____________________HELP MENU_____________________\n");
  printf("\t-h HELP show help option\n");
  printf("\t-c CREATE create a new process , and put to really queue\n");
  printf("\t-b BLOCK block a process in excute queue\n");
  printf("\t-w WAKE wake a process in wait queue\n");
  printf("\t-e EXCUTE excute a process in really queue\n");
  printf("\t-s SUSPEND suspend a process in excute queue\n");
  printf("\t-o OUTPUT output all processes in queues\n");
  printf("\t-r RUN excute a process in excute queue\n");
  printf("\t-x EXIT exit this program\n");
  printf("___________________________________________________\n");
  printf("\t type 'H' will show this menu\n");
}/*help*/
//void main()
int main()
{
  char COMMAND = NULL;
  if( init() != 1)
 {
   printf("init falied ! \n ");
   getch();
   exit(0);
 }
  else
 {
   printf("init...OK\n");
   output();
   help();
 }
  while(1)
  {
	  /*当三队列都不空 执行调度 */
	  printf(">");
	  scanf("%c",&COMMAND);
	  switch(COMMAND)
	  {
	  case '\n': break;
	  case 'H':
      case 'h': help(); break;
      case 'C':
      case 'c': create(); break;
      case 'B':
      case 'b': block(); break;
      case 'W':
      case 'w': wake(); break;
      case 'S':
      case 's': suspend(); break;
      case 'E':
      case 'e': fexcute(); break;
      case 'O':
      case 'o': output(); break;
      case 'X':
      case 'x': leave(); break;
      case 'R':
      case 'r': run(); break;
	  }
  }
}/*main*/
