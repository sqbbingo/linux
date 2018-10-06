#include "commonheader.h"

struct sequent_stack	//栈的管理结构体
{
	int *stack;	//用stack指向一块连续的内存来存储栈元素
	int size;	//size保存了该顺序栈的总大小
	int top;	//用top来指示栈顶元素的偏移量
};

struct sequent_stack *init_stack(int size)	//参数size表明空栈的初始化大小
{
	struct sequent_stack *s;
	s = malloc(sizeof(struct sequent_stack));//申请栈管理结构体

	if(s != NULL)
	{
		s->stack = calloc(size,sizeof(int));	//申请栈空间，并由stack指向
		s->size = size;
		s->top = -1;	//将栈顶偏移量置为-1，代表空栈
	}
	return s;
}

bool stack_full(struct sequent_stack *s)
{
	return s->top >= s->size-1;	//判断栈是否已满
}

bool push(struct sequent_stack *s,int data)	//压栈
{
	if(stack_full(s))	//如果栈已满，则出错返回
		return false;

	s->top++;
	s->stack[s->top] = data;
	return true;
}

bool stack_empty(struct sequent_stack *s)
{
	return s->top == -1;	//判断是否为空
}

//出栈
bool pop(struct sequent_stack *s,int *p)	//p指向存放栈顶元素的内存
{
	if(stack_empty(s))	//如果栈为空，则出错返回
		return false;
	*p = s->stack[s->top];
	s->top--;
	return true;
}

int main(void)
{
	struct sequent_stack *s;
	s = init_stack(10);	//初始化一个具有10个元素空间的顺序栈

	int n;
	scanf("%d",&n);	//让用户输入一个需要转换的十进制数

	while(n > 0)
	{
		push(s,n%8);	//使用短除法统统压栈
		n /= 8;
	}
	
	int m;
	while(!stack_empty(s))	//只要栈不为空，就继续循环
	{
		pop(s,&m);	//出栈并打印出来
		printf("%d",m);
	}
	printf("\n");
	
	return 0;
}
