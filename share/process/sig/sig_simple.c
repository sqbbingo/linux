#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void f(int sig)
{
	printf("catched a signal:%d\n", sig);
}

int main(int argc, char const *argv[])
{
	signal(SIGHUP,SIG_IGN);//1、设置SIGHUP响应动作为：忽略
	signal(SIGINT,SIG_DFL);//2、设置SIGINT响应动作为：缺省
	signal(SIGQUIT,f);//3、设置SIGQOUIT啥啥响应动作为：执行函数f()

	printf("[%d]:I am waitting for some signal...\n",
				getpid() );
	pause();//暂停进程，静静等待信号的到来。。。
	
	return 0;
}