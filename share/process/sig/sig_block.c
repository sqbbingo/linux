#include <stdio.h>
#include <signal.h>

void sighandler(int sig)
{
	printf("[%d]:catch%d.\n",getpid(),sig );
}

int main(int argc, char const *argv[])
{
	pid_t x = fork();

	if (x > 0 )//父进程
	{
		signal(SIGINT,sighandler);//设置SIGINT的响应函数

		sigset_t sigmask;
		sigemptyset(&sigmask);
		sigaddset(&sigmask,SIGINT);//将SIGINT添加到信号集中

#ifdef TEST
		printf("[%d]:block SIGINT...\n",getpid());
		sigprocmask(SIG_BLOCK,&sigmask,NULL);//设置阻塞
#endif
		sleep(5);//睡眠5秒，信号在此期间到来
#ifdef TEST
		printf("[%d]:unblock SIGINT...\n",getpid());
		sigprocmask(SIG_UNBLOCK,&sigmask,NULL);//解除阻塞
#endif
		wait(NULL);//让子进程先退出，从而正确显示Shell命令提示
	}

	if (x == 0)
	{
		sleep(1);//睡眠1秒钟，保证父进程做好准备工作
		if (kill(getpid(),SIGINT) == 0)//父进程发送信号SIGINT
		{
			printf("[%d]:SIGINT has been sended!d\n",
			getpid());
		}
	}
	return 0;
}