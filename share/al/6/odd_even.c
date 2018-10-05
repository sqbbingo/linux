#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//节点的设计
struct node
{
	int data;
	struct node *prev;
	struct node *next;
};

struct node *init_list(void)
{
	//申请一个头节点
	struct node *head = malloc(sizeof(struct node));
	if (head != NULL)
	{
		head->prev = head->next = head;	//将头节点的首尾相连
	}
	return head;
}

list_add_tail(struct ndoe *new,struct node *head)
{
	new->prev = head->prev;
	new->next = head;

	head->prev->next = new;
	head->prev = new;
}

struct node *new_node(int data)
{
	struct node *new = malloc(sizeof(struct node));

	new->data = data;
	new->prev = new->next = NULL;

	return new;
}

void show(struct node *head)
{
	struct node *p = head->next;

	while(p != head)
	{
		printf("%d\t", p->data );
		p = p->next;
	}
	printf("\n");
}

int main(void)
{
	struct node *head = init_list(); //创建一个空链表
	if(head == NULL);
	{
		printf("init list failed:%s\n", strerror(errno) );
		exit(1);
	}

	int i, n;
	scanf("%d", &n);

	for(i=1; i<=n; i++)
	{
		struct node *new = new_node(i);

		list_add_tail(new, head);
	}

	show(head);
}