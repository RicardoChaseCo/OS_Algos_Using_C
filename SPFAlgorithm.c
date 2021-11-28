#include <stdio.h>
#include <string.h>
//使用timer()函数
#include <windows.h>
//时间延迟
#define DELAY 100
//时间片
#define SJP 4
/**********全局变量声明**********/
unsigned short TIME=0;           //时间
unsigned short NUM=0;         //进程数量
char TYPE='1';                          //模拟类型
//PCB结构体定义
typedef struct PCB
{
    char name[16];
    char state;	                                   //[R]Run,[F]Finish,[P]Pause,[N]New
    unsigned short priority;                  //数字越大，优先级越高，最小为1
    unsigned short t_arrive;                 //到达时间
    unsigned  short t_start;                  //开始时间
    unsigned short t_finish;                  //完成时间
    unsigned  short t_service;               //服务时间
    unsigned  short t_run;                    //运行时间
    unsigned  short t_wait;                    //等待时间
    struct PCB *next;
} pcb;

pcb  *now=NULL,                   //现在运行的pcb
     *head=NULL;             //pcb链头部指针

/**********函数声明**********/

void fcfs();                  //先到先服务
void spf();                    //短作业优先
void gyxb();                //高优先比
void sjplz();                  //时间片轮转
void init();                  //初始化，完成pcb录入
pcb *sort(pcb*);          //对init()录入的pcb按到达时间排序
void timer();               //定时器，每一个延迟自我调用一次
void result();               //打印结果


//短进程或作业优先算法
void spf()
{
    pcb *p=head,*p_min=NULL;
    unsigned short t_min=9999;
    //从现在时间以前并且未结束的进程中，选出服务时间最小的进程
    while(p!=NULL  && p->t_arrive<=TIME)
    {
        if(p->state=='F')
        {
            p=p->next;
            continue;
        }
        if((p->t_service-p->t_run)<t_min)
        {
            t_min=p->t_service;
            p_min=p;
        }
        p=p->next;
    }
//如果为空，判断全部进程是否都已完成
    if(p_min==NULL)
    {
        char k='Y';
        p=head;
        while(p!=NULL)
        {
            if(p->state!='F')
                k='N';
            p=p->next;
        }
        if(k=='Y')
            now=NULL;
        else printf("[Time: %d]\t No process running \n",TIME);

        return;
    }
//如果选出的进程和之前的不同
    if(p_min!=now)
    {
        if(now->state=='R')
        {
            now->state='P';
            printf("[Time: %d]\t Process: %s Suspend operation \n",TIME,now->name);
        }

    }
    if(p_min==NULL) now=head;
    else now=p_min;

    if(now->state=='N')
    {
        now->state='R';
        now->t_start=TIME;
        printf("[Time: %d]\t Process: %s First running\n",TIME,now->name);
    }
    else
    {
        if(now->state=='P')
        {
            now->state='R';
            printf("[Time: %d]\t Process: %s Continue running\n",TIME,now->name);
        }
        (now->t_run)++;
        if(now->t_run>=now->t_service)
        {
            now->state='F';
            now->t_finish=TIME;
            printf("[Time: %d]\t Process: %s Task accomplished\n",TIME,now->name);
            gyxb();
        }
        else printf("[Time: %d]\t Process: %s Running, Running time: %d\n",TIME,now->name,now->t_run);
    }
}

//高优先比算法
void gyxb()
{
    pcb *p=head,*p_min=NULL;
    unsigned short t_min=0;
    //从现在时间以前并且未结束的进程中，选出服务时间最小的进程
    while(p!=NULL  && p->t_arrive<=TIME)
    {

        if(p->state=='F')
        {
            p=p->next;
            continue;
        }

        //动态优先比
        if(p->state=='P')
        {
            p->t_wait++;
            p->priority+=p->t_wait/p->t_service+1;
        }

        if(p->priority>t_min)
        {
            t_min=p->priority;
            p_min=p;
        }
        p=p->next;
    }
//如果为空，判断全部进程是否都已完成
    if(p_min==NULL)
    {
        char k='Y';
        p=head;
        while(p!=NULL)
        {
            if(p->state!='F')
                k='N';
            p=p->next;
        }
        if(k=='Y')
            now=NULL;
        else printf("[Time: %d]\t No process running \n",TIME);
        return;
    }
//如果选出的进程和之前的不同
    if(p_min!=now)
    {
        if(now->state=='R')
        {
            now->state='P';
            printf("[Time: %d]\t Process: %s Suspend operation \n",TIME,now->name);
        }

    }
    if(p_min==NULL) now=head;
    else now=p_min;

    if(now->state=='N')
    {
        now->state='R';
        now->t_start=TIME;
        printf("[Time: %d]\t Process: %s First running\n",TIME,now->name);
    }
    else
    {
        if(now->state=='P')
        {
            now->state='R';
            now->t_wait=0;
            printf("[Time: %d]\t Process: %s Continue running\n",TIME,now->name);
        }
        (now->t_run)++;
        if(now->t_run>=now->t_service)
        {
            now->state='F';
            now->t_finish=TIME;
            printf("[Time: %d]\t Process: %s Task accomplished\n",TIME,now->name);
            spf();
        }
        else printf("[Time: %d]\t Process: %s Running，Running time：%d\n",TIME,now->name,now->t_run);
    }

}
  
	void result()
    {
        pcb *p=head;
        printf("\n=========Running results=========\n\n");
        printf("ProcessName Priority ArrivalTime StartTime  FinishingTime  ServiceTime  TurnaroundTime  TurnaroundTimeWithRights\n");
        while(p!=NULL)
        {
            printf(" %s\t     %d\t     %d\t         %d\t     %d\t           %d\t         %d\t          %.2f\n",p->name,p->priority,p->t_arrive,
                   p->t_start,p->t_finish,p->t_service,p->t_finish-p->t_arrive,
                   1.0*(p->t_finish-p->t_arrive)/p->t_service);
            p=p->next;
        }
    }

    void timer()
    {
        spf();
		if(now==NULL) return;
        TIME++;
        Sleep(DELAY);
        timer();
    }

    void init()
    {
        pcb *p,*q;
        unsigned short i;
		printf("Job scheduling algorithm: Short process first (SPF)\n ");
        //printf("\n1:2: Short process first (SPF)\n3:High priority ratio 4:Time slice circular scheduling\n");
		//printf("Input simulation type: [ ]\b\b");
        //scanf("%c",&TYPE);
        printf("Input number of processes:\n");
        scanf("%d",&NUM);
        for(i=0; i<NUM; i++)
        {
            p=(pcb *)malloc(sizeof(pcb));
            printf("[No %d] Input: Name   Priority   ArrivalTime   ServiceTime\n",i+1);
            scanf("%s\t%d\t%d\t %d",&p->name,&p->priority,&p->t_arrive,&p->t_service);
            if(head==NULL)
            {
                head=p;
                q=p;
            }
            q->next=p;
            p->t_start=0;
            p->t_finish=0;
            p->t_run=0;
            p->t_wait=0;
            p->next=NULL;
            p->state='N';
            q=p;
        }
    }


//按到达时间冒泡排序
    pcb* sort_pcb(pcb *h_head)
    {
        pcb *p,*p1,*p2,*p3;
        pcb h, t;
        if (h_head == NULL) return NULL;
        h.next=h_head;
        p=&h;
        while (p->next!=NULL)
        {
            p=p->next;
        }
        p=p->next=&t;
        while (p!=h.next)
        {
            p3=&h;
            p1=p3->next;
            p2=p1->next;
            while (p2!=p)
            {
                if ((p1->t_arrive)>(p2->t_arrive))
                {
                    p1->next=p2->next;
                    p2->next=p1;
                    p3->next=p2;
                    p3=p2;
                    p2=p1->next;
                }
                else
                {
                    p3=p1;
                    p1=p2;
                    p2=p2->next;
                }
            }
            p=p1;
        }
        while (p->next!=&t)
        {
            p=p->next;
        }
        p->next=NULL;
        return h.next;
    }

//void main()
int main()
    {
        init();
        system("CLS");
        head=sort_pcb(head);
        now=head;
        printf("The processes are running in simulation……\n");
        timer();
        result();
        //return 0;
}
