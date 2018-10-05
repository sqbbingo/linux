#include <stdio.h>			
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define LED1_PATH		"/sys/class/gpio/gpio68/value"	//led灯文件位置
#define LED2_PATH		"/sys/class/gpio/gpio69/value"
#define LED3_PATH		"/sys/class/gpio/gpio70/value"

#define	RCK_H			{ret1=write(fd1,"1",1);if(ret1<0){perror("write1 err");return -1;}}
#define	RCK_L			{ret1=write(fd1,"0",1);if(ret1<0){perror("write1 err");return -1;}}
#define	DIN_H			{ret2=write(fd2,"1",1);if(ret2<0){perror("write2 err");return -1;}}
#define	DIN_L			{ret2=write(fd2,"0",1);if(ret2<0){perror("write2 err");return -1;}}
#define	CLK_H			{ret3=write(fd3,"1",1);if(ret3<0){perror("write3 err");return -1;}}
#define	CLK_L			{ret3=write(fd3,"0",1);if(ret3<0){perror("write3 err");return -1;}}

/*显示数值和位选值对照表 0 1 2 3 4 5 6 7 8 9 */
unsigned char led_value_table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
int fd1,fd2,fd3,ret1,ret2,ret3,flag,i;

int display(int num,int duan)
{
	int wei;
	wei = led_value_table[num];
	//RCK_H;
	//sleep(1);
	RCK_L;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(wei & 0x80)
			{ DIN_H;}
		else
			{ DIN_L;}	
		CLK_H;
		wei = wei << 1;
	}
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(duan == (8-i))
			{	DIN_H;}
		else
			{	DIN_L;}
		CLK_H;
	}
	RCK_H;

	return 0;
}

int main(int argc, char **argv)
{
	char path[20],data[2];
	
	
	printf("begin1\n");
	
	
	fd1=open(LED1_PATH,O_WRONLY);		//用户控制句柄
	fd2=open(LED2_PATH,O_WRONLY);
	fd3=open(LED3_PATH,O_WRONLY);

	if((fd1<0)|(fd2<0)|(fd3<0)){
		perror("led err");
		return -1;
	}
	for(;;)
	{
		
		display(4,1);
		display(3,2);
		display(2,3);
		display(1,4);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	return 0;
}