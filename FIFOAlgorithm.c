#include <stdio.h>
#include <stdlib.h>
/*全局变量*/
int mSIZE; /*物理块数*/
int pSIZE; /*页面号引用串个数*/
static int memery[10]={0}; /*物理块中的页号*/
static int page[100]={0}; /*页面号引用串*/
static int temp[100][10]={0}; /*辅助数组*/
/*自定义置换算法函数*/
void FIFO(); 
void LRU();
void OPT();
/*自定义函数*/
void print(unsigned int t);
/*主函数*/
//void main()
int main()
{
    int i,k,code;
	system("color 0A");
	printf("Please press any key to initialize... \n");
	getchar();
	system("cls");
	system("color 0B");
	printf("Please enter the number of physical blocks (M<=10): ");
	scanf("%d",&mSIZE);
	while(mSIZE>10)
	{
		printf("Input error, please re-enter.");
		scanf("%d",&mSIZE);
	}
	printf("Please enter the number of page number reference strings (P<=100): ");
	scanf("%d",&pSIZE);
	while(pSIZE>100)
	{
		printf("Input error, please re-enter.");
		scanf("%d",&pSIZE);
	}
	puts("Please input page number reference string in sequence (continuous input, separated by spaces): ");
	for(i=0;i<pSIZE;i++)
        scanf("%d",&page[i]);
	system("cls");
	system("color 0E");
	
	puts("The input page number reference string is: ");
		for(k=0;k<=(pSIZE-1)/20;k++)
		{
			for(i=20*k;(i<pSIZE)&&(i<20*(k+1));i++)
			{
				if(((i+1)%20==0)||(((i+1)%20)&&(i==pSIZE-1)))
					printf("%d\n",page[i]);
				else
					printf("%d   ",page[i]);
			}
		}
		
		FIFO();
		
}
void print(unsigned int t)
{
	int i,j,k,l;
	int flag;
	for(k=0;k<=(pSIZE-1)/20;k++)
	{
		for(i=20*k;(i<pSIZE)&&(i<20*(k+1));i++)
		{
			if(((i+1)%20==0)||(((i+1)%20)&&(i==pSIZE-1)))
				printf("%d\n",page[i]);
			else
				printf("%d   ",page[i]);
		}
		for(j=0;j<mSIZE;j++)
		{
			for(i=20*k;(i<mSIZE+20*k)&&(i<pSIZE);i++)
			{
				if(i>=j)
					printf(" |%d|",temp[i][j]);
				else
					printf(" | |");
			}
			for(i=mSIZE+20*k;(i<pSIZE)&&(i<20*(k+1));i++)
			{
				for(flag=0,l=0;l<mSIZE;l++)
					if(temp[i][l]==temp[i-1][l])
						flag++;
				if(flag==mSIZE)/*页面在物理块中*/
					printf("    ");
				else
					printf(" |%d|",temp[i][j]);
			}
			/*每行显示20个*/
			if(i%20==0)
				continue;
			printf("\n");
		}
	}
	printf("----------------------------------------\n");
	printf("Page missing times: %d\t\t",t+mSIZE);
	printf("Page missing rate: %d/%d\n",t+mSIZE,pSIZE);
	printf("Replacement times: %d\t\t",t);
	printf("Access hit rate: %d%%\n",(pSIZE-(t+mSIZE))*100/pSIZE);
	printf("----------------------------------------\n");	
}
/*先进先出页面置换算法*/
void FIFO()
{
   int memery[10]={0};
    int time[10]={0}; /*记录进入物理块的时间*/
    int i,j,k,m;
    int max=0; /*记录换出页*/
    int count=0; /*记录置换次数*/
	/*前mSIZE个数直接放入*/
    for(i=0;i<mSIZE;i++)
    {
        memery[i]=page[i];
        time[i]=i;
        for(j=0;j<mSIZE;j++)
			temp[i][j]=memery[j];
    }
    for(i=mSIZE;i<pSIZE;i++)
    {
		/*判断新页面号是否在物理块中*/
        for(j=0,k=0;j<mSIZE;j++)
        {
            if(memery[j]!=page[i])
                k++;
        }
        if(k==mSIZE) /*如果不在物理块中*/
        {
            count++;
			/*计算换出页*/
            max=time[0]<time[1]?0:1;
			for(m=2;m<mSIZE;m++)
				if(time[m]<time[max])
					max=m;
            memery[max]=page[i];
            time[max]=i; /*记录该页进入物理块的时间*/
            for(j=0;j<mSIZE;j++)
				temp[i][j]=memery[j];
        }
        else
        {
            for(j=0;j<mSIZE;j++)
				temp[i][j]=memery[j];
        } 
    }
	print(count);
}
