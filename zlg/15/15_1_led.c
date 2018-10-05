#include <stdio.h>			
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define TRIGGER			"trigger"
#define LED_PATH		"/sys/class/leds/"	//led灯文件位置
#define LED_STATUS		"brightness"
#define TRIGGER_NONE	"none"

int main(int argc, char **argv)
{
	char path[20],data[2];
	int fd,ret,flag;
	if(argv[1] == NULL){
		printf("usage:./led led_run");
		return 0;
	}
	
	printf("begin1\n");
	strcpy(path,LED_PATH);
	strcat(path,argv[1]);
	strcat(path,"/"TRIGGER);
	printf("begin2\n");
	printf("%s",path);
	
	fd=open(path,O_RDWR);  //打开trigger文件
	if(fd<0){
		perror("open1");
		return -1;
	}
	ret=write(fd,TRIGGER_NONE,strlen(TRIGGER_NONE));	//更改为用户控制
	if(ret<0){
		perror("write1");
		return -1;
	}
	close(fd);
	strcpy(path,LED_PATH);
	strcat(path,argv[1]);
	strcat(path,"/"LED_STATUS);
	fd=open(path,O_WRONLY);		//用户控制句柄

	if(fd<0){
		perror("open2");
		return -1;
	}
	for(;;)
	{
		data[0]=flag?'0':'1';
		ret=write(fd,data,1);
		if(ret<0){
			perror("write2");
			return -1;
		}
		flag =! flag;
		sleep(0.5);
	}

	return 0;
}