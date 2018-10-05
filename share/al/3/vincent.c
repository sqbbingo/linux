#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;

typedef struct node 					//1.设计节点
{
	datatype data;
	struct node *next;						//指向下一个相同类型节点
}node,*list;

list init_list(void) 					//2.初始化带头节点的空链表
{
	list head = malloc(sizeof(node));		//创建一个头节点
	if (head != NULL)
	{
		head->next = NULL;
	}
	return head;
}

list new_node(datatype n)
{
	list new = malloc(sizeof(node));

	if (new != NULL)
	{
		new->data = n;
		new->next =NULL;
	}

	return new;
}

void insert_list(list new, list head)		//3.插入节点
{
	if(new == NULL)								//检测新节点是否为空
		return;
	list p = head;

	while(p->next != NULL)						//将p指向最后一个节点
	{
		p = p->next;
	}

	new->next = p->next;						//第一步：新节点连接插入点后一个节点
	p->next = new;								//第二步：插入点前一个节点连接新节点
}

void show_list(list head)
{
	list tmp = head->next;

	while(tmp != NULL)
	{
		printf("%d\t", tmp->data );
		tmp = tmp->next;
	}
	printf("\n");
}

void revert(list head)
{
	list p = head->next;
	head->next = NULL;

	list tmp;
	while(p != NULL)
	{
		tmp = p->next;
		p->next = head->next;
		head->next = p;
	}
}

int main(void)
{
	list head = init_list();
	if(head == NULL)
	{
		printf("inin failed\n");
		exit(1);
	}

	int n;
	while(1)
	{
		int ret = scanf("%d",&n);
		if (ret != 1)
		{
			break;
		}
		//list new = new_node(n);
		//insert_list(new,head);
		revert(head);
		show_list(head);
	}
}
