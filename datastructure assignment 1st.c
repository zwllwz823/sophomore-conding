#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define MAXSIZE 100

typedef struct {
    char data[MAXSIZE];
    int level[MAXSIZE];
    int top1;
}infStack;
typedef struct {
    double value[MAXSIZE];
    int top2;
}rePolish;
infStack infix;
rePolish repolish;
char oper[] = "=)+-*/ICTGNQE^(";
int operlevel[15] = {-1,0,1,1,2,2,3,3,3,3,3,3,3,4,5};
int i,j=0;
char temp1[MAXSIZE],temp2[MAXSIZE];//temp1用来存输入，temp2存后缀表达式

void scanInput(void);//孩子出息了，针不戳，妈妈爱你 
void number1(void);
void alphabat1(void);
void operator1(char ch);
void scanTemp(void);
void number2(void);
void operator2(char ch);

int main()
{
    infix.top1=-1;
    repolish.top2=-1;
    scanf("%s",temp1);
    scanInput();
    //int k;
    //for(k=0;k<j;k++)
    //printf("%c",temp2[k]);
    //printf("\n");
    scanTemp();
    printf("%.6lf\n",repolish.value[repolish.top2]);
    return 0;
} 
void scanInput(void)
{
	while(i<strlen(temp1))
	{
		if('0'<=temp1[i] && temp1[i]<='9')
		number1();
		else if('a'<=temp1[i] && temp1[i]<='z')
		alphabat1();
		else
		operator1(temp1[i]);
		i+=1;
	}
} 
void number1(void)
{
    while(('0'<=temp1[i] && temp1[i]<='9')||temp1[i]=='.')
    {
    	temp2[j]=temp1[i];
    	i+=1;
    	j+=1;
	}
	i-=1;
	temp2[j]=' ';
	j+=1;
}
void alphabat1(void)
{
	if(temp1[i]=='s')
    {
        i+=1;
        if(temp1[i]=='i')
        operator1('I');
        else if(temp1[i]=='q')
        operator1('Q');
    }else if (temp1[i]=='l')
    {
        i+=1;
        if(temp1[i]=='o')
        operator1('O');
        else if(temp1[i]=='n')
        operator1('N');
    }else if (temp1[i]=='c')
    operator1('N');
    else if (temp1[i]=='t')
    operator1('T');
    else if (temp1[i]=='e')
    operator1('E');
    while('a'<=temp1[i] && temp1[i]<='z')
    i+=1;
    i-=1;
}
void operator1(char ch)
{
	int k,chlevel,find=0;
	for(k=0;k<strlen(oper) && !find;k++)
	{
		if(ch==oper[k])
		chlevel=operlevel[k];
	}
	if(infix.top1==-1)
    {
        infix.top1+=1;
        infix.data[infix.top1]=ch;
        if(ch!='(')
        infix.level[infix.top1]=chlevel;
        else
        infix.level[infix.top1]=0;//左括号在栈内优先级为0 
    }else
    {
        if(chlevel>infix.level[infix.top1])//push 
        {
            infix.top1+=1;
            infix.data[infix.top1]=ch;
            if(ch!='(')
	        infix.level[infix.top1]=chlevel;
	        else
	        infix.level[infix.top1]=0;//左括号在栈内优先级为0 
        }else //pop
        {
        	int ifleft=0;
            while(infix.top1 >-1 && chlevel<=infix.level[infix.top1] && !ifleft)
            {
                if(infix.data[infix.top1]!='(')
				{
					temp2[j]=infix.data[infix.top1];
					infix.top1 -=1;
					j+=1; 
				}else
				{
					infix.top1 -=1;
					ifleft=1;
				}
            }
            if(ch!=')'&& ch!='=')
            {
                infix.top1+=1;
                infix.data[infix.top1]=ch;
                if(ch!='(')
		        infix.level[infix.top1]=chlevel;
		        else
		        infix.level[infix.top1]=0;//左括号在栈内优先级为0
            }
        }
    }
}
void scanTemp(void)
{
	j=0;
	while(j<strlen(temp2))
	{
		if('0'<=temp2[j] && temp2[j]<='9')
		number2();
		else
		operator2(temp2[j]);
		j+=1;
	}
} 
void number2(void)
{
	char str[100];
	int k=0;
    while(temp2[j]!=' ')
    {
    	str[k]=temp2[j];
    	k+=1;
    	j+=1;
	}
	str[k]='\0';
	repolish.top2+=1;
	repolish.value[repolish.top2]=atof(str);
	//printf("%.2lf\n",repolish.value[repolish.top2]);
}
void operator2(char ch)
{
    double st,nd;
    st=repolish.value[repolish.top2];
    nd=repolish.value[(repolish.top2)-1];
    if(ch=='+')
    {
    	repolish.top2-=1;
    	repolish.value[repolish.top2]=st+nd;
	}
    else if(ch=='-')
    {
    	repolish.top2-=1;
    	repolish.value[repolish.top2]=nd-st;
	}
    else if(ch=='*')
    {
    	repolish.top2-=1;
    	repolish.value[repolish.top2]=nd*st;
	}
    else if(ch=='/')
    {
    	repolish.top2-=1;
   		repolish.value[repolish.top2]=nd/st;
	}
    else if(ch=='^')
    {
    	repolish.top2-=1;
    	repolish.value[repolish.top2]=pow(nd,st);
	}
    else if(ch=='I')
    repolish.value[repolish.top2]=sin(st/180*M_PI);
    else if(ch=='C')
    repolish.value[repolish.top2]=cos(st/180*M_PI);
	else if(ch=='T')
    repolish.value[repolish.top2]=tan(st/180*M_PI);
    else if(ch=='G')
    repolish.value[repolish.top2]=log10(st);
    else if(ch=='N')
    repolish.value[repolish.top2]=log(st);
	else if(ch=='Q')
    repolish.value[repolish.top2]=sqrt(st);
    else if(ch=='E')
    repolish.value[repolish.top2]=exp(st);
}
