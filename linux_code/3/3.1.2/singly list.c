#include "commonheader.h"	//自定义的通用头文件

#define SIZE 20

typedef int	datatype;

typedef struct node //设计节点
{
	datatype	data;
	struct node *	next;
} listnode, *singly_list;

singly_list init_list(void)//建立带有头节点的空链表
{
	singly_list mylist = malloc(sizeof(listnode));//创建一个头结点
	if (mylist != NULL)
	{
		mylist->next = NULL;
	}

	return mylist;
}

bool is_empty(singly_list list)//判断链表list是否为空
{
	return list->next == NULL;
}

singly_list new_node(datatype data, singly_list next) //创建新节点
{
	singly_list new = malloc(sizeof(listnode));

	if (new != NULL)
	{
		new->data = data;
		new->next = next;
	}

	return new;
}

void insert_node(singly_list p, singly_list new) //往节点p后面插入新节点
{
	if (p  == NULL || new == NULL)
		return;

	new->next = p->next;	//第一步
	p->next = new;	//e第二步
}

bool remove_node(singly_list mylist, singly_list delete) //删除节点
{
	if (is_empty(mylist))
		return false;

	singly_list p = mylist;
	while (p != NULL && p->next != delete)	//第一步
	{
		p = p->next;
	}

	if (p == NULL)
		return false;

	p->next = delete->next;	//第二步
	delete->next = NULL;	//第三步

	return true;
}
//移动节点
void move_node(singly_list mylist, singly_list p, singly_list anchor)
{
	if (mylist == NULL || p == NULL || anchor == NULL)
		return;

	remove_node(mylist, p);
	insert_node(anchor, p);
}

void show(singly_list list)//显示链表
{
	if (is_empty(list))
		return;

	singly_list p = list->next;

	int i = 0;
	while (p != NULL)
	{

		printf("%s%d", i == 0 ? "" : "-->", p->data);
		p = p->next;
		i++;
	}

	printf("\n");
}

singly_list find_node(singly_list mylist, datatype data) //查找节点
{
	if (is_empty(mylist))
		return NULL;

	singly_list p;

	for (p = mylist->next; p != NULL; p = p->next)
	{
		if (p -> data == data)	//如果找到指定的数据，则返回其指针
			break;
	}

	return p;
}

int parse(char buf[SIZE], int number[2]) //分析用户的输入数据
{
	if (!strcmp(buf, "\n")) //如果用户直接输入回车，则返回0
		return 0;

	int count = 1;
	char *p, delim[] = ",";

	p = strtok(buf, delim);
	number[0] = atoi(p);//获取用户输入的第一个数据

	p = strtok(NULL, delim);
	if (p != NULL)
	{
		number[1] = atoi(p);//获取用户输入的第二个数据
		count++;
	}

	return count; //返回用户输入数据的个数
}

int main(void)
{
	singly_list mylist = init_list();

	int ret, number[2];
	char buf[SIZE];

	while (1)
	{
		bzero(buf, SIZE);
		fgets(buf, SIZE, stdin);
		ret = parse(buf, number);


		if (ret == 0) //如果用户直接输入回车，则什么都不干
		{
			continue;
		}


		if (ret == 1 && number[0] > 0) //若输入一个正数则插入节点
		{
			singly_list new = new_node(number[0], NULL);
			insert_node(mylist, new);
			show(mylist);
		}


		else if (ret == 1 && number[0] < 0) //若输入一个负数则删除节点
		{
			singly_list delete = find_node(mylist, -number[0]);
			if (delete == NULL)
			{
				printf("%d is NOT found.\n", -number[0]);
				show(mylist);
				continue;
			}

			remove_node(mylist, delete);
			show(mylist);
		}

		else if (ret == 2) //若输入两个数据，则执行移动
		{
			singly_list pa = find_node(mylist, number[0]);
			singly_list pb = find_node(mylist, number[1]);

			if (pa == NULL || pb == NULL)
			{
				printf("node does NOT exit.\n");
				show(mylist);
				continue;
			}

			move_node(mylist, pa, pb);
			show(mylist);
		}
		else
			break;
	}
	return 0;
}