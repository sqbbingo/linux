#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define NAMELEN 20

struct student//用来存放一个带有一定数据格式的学生节点
{
	char name[NAMELEN];
	char sex;
	int age;
	float stature;

	struct student *next;//用以形成链表
};

struct student *init_list(void)//初始化一个空链表
{
	struct student *head = malloc(sizeof(struct student));
	head->next = NULL;

	return head;
}

//将新节点new添加到链表head中
void add_studnet (struct student *head,struct student *new)
{
	struct student *tmp = head;

	while(tmp->next != NULL)
		tmp = tmp->next;

	tmp->next = new;
}

//显示链表中的所有节点
void show_student(struct student *head)
{
	struct student *tmp = head->next;

	while(tmp != NULL)
	{
		fprintf(stdout, "%-5s %c %d %.1f\n",
			tmp->name,tmp->sex,tmp->age,tmp->stature);

		tmp = tmp->next;
	}
}

int main(int argc, char const *argv[])
{
	FILE *fp = fopen("format_data","r");

	//创建一个用来保存学生节点的空链表
	struct student *head = init_list();

	int count = 0;
	while(1)
	{
		struct student *new = malloc(sizeof(struct student));

		//从文件fp中按照格式读取数据，并将之填充到new中
		if (fscanf(fp,"%s %c %d %f",\
			new->name,&(new->sex),\
			&(new->age),&(new->stature)) == EOF)
		{
			break;
		}

		//将新节点new加入到链表head中
		add_studnet(head,new);
		count++;
	}

	printf("%d students have ben added.\n", count);
	show_student(head);//打印所有的节点

	fclose(fp);//关闭文件指针，释放系统资源

	return 0;
}