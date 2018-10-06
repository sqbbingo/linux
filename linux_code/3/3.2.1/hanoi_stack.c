#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

struct node	//链式栈节点
{
	int data;
	struct node *next;
};

struct linked_stack //链式栈的管理结构体
{
        struct node *top;       //栈顶元素指针
        int size;       //当前栈元素总数
};

struct linked_stack *s1,*s2,*s3;//定义成全局变量时为了方便打印

bool stack_empty(struct linked_stack *s)        //判断栈是否为空
{
        return s->size == 0;
}

struct node *new_node(int data)//创建一个新的节点
{
        struct node *new = malloc(sizeof(struct node));
        if (new != NULL)
        {
                new->data = data;
                new->next = NULL;
        }
        return new;
};
//将新节点new压入栈s中
bool push(struct linked_stack *s,struct node *new)
{
        if (new == NULL)
                return false;
        
        new->next = s->top;
        s->top = new;
        s->size++;

        return true;
}
//从栈s中取出栈顶元素
bool pop(struct linked_stack *s,struct node **p)
{
        if (stack_empty(s))     
                return false;

        *p = s->top;
        s->top = s->top->next;
        (*p)->next = NULL;
        s->size--;

        return true;                
}

void show(struct linked_stack *s1,
        struct linked_stack *s2,
        struct linked_stack *s3)//纵向同时显示三个链栈数据
{
        int maxlen,len;

        maxlen = s1->size > s2->size ? s1->size:s2->size;
        maxlen = maxlen > s3->size ? maxlen : s3->size;
        len = maxlen;

        struct node *tmp1 = s1->top;
        struct node *tmp2 = s2->top;
        struct node *tmp3 = s3->top;

        int i;
        for (i = 0; i < maxlen; i++)    
        {
                if (tmp1 != NULL && len <= s1->size)
                {
                        printf("%d",tmp1->data);
                        tmp1 = tmp1->next;
                }
                printf("\t");

                if (tmp2 != NULL && len <= s2->size)
                {
                        printf("%d",tmp2->data);
                        tmp2 = tmp2->next;
                }
                printf("\t");

                if (tmp3 != NULL && len <= s3->size)
                {
                        printf("%d",tmp3->data);
                        tmp3 = tmp3->next;
                }
                printf("\n");
        
                len--;
        }
        printf("s1\ts2\ts3\n-------------\n");
}

void hanoi(int n,struct linked_stack *ss1,
        struct linked_stack *ss2,
        struct linked_stack *ss3)
{
        if(n <= 0)
                return;
        
        struct node *tmp;

        hanoi(n-1,ss1,ss3,ss2);//第一步：将n-1个汉诺塔从s1移到s3
        getchar();
        show(s1,s2,s3);
        pop(ss1,&tmp);//第二步：将最底层汉诺塔从s1移动到s2
        push(ss2,tmp);
        hanoi(n-1,ss3,ss2,ss1);//第三步：将n-1个汉诺塔从s3移动s2
}
//初始化一个空的链栈
struct linked_stack *init_stack(void)
{
        struct linked_stack *s;
        s = malloc(sizeof(struct linked_stack));//申请栈链管理结构体

        if(s != NULL)
        {
                s->top = NULL;
                s->size = 0;
        }
        return s;
}

int main(void)
{
        printf("how many hanois?");
        int hanois;
        scanf("%d",&hanois);

        s1 = init_stack();//初始化三个链栈，用来表示三根柱子
        s2 = init_stack();
        s3 = init_stack();

        int i;
        for (i = 0; i < hanois; i++)
        {
                struct node *new = new_node(hanois-i);
                push(s1,new);
        }

        hanoi(hanois,s1,s2,s3);//使用递归算法移动这些汉诺塔
        show(s1,s2,s3);//显示移动之后的汉诺塔形状

        return 0;
}