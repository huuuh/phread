#include<pthread.h>
//要用到的函数
//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
/*
函数的说明：

        POSIX thread 简称 pthread
        pthread_t 是 unsigned long int 类型的变量，用来表示线程的ID

    函数参数：
        thread(输出参数)，由pthread_create在线程创建成功后返回的线程句柄，该句柄在后续操作线程的API中用于标志该新建的线程；
        start_routine(输入参数)，新建线程的入口函数；
        arg(输入参数)，传递给新线程入口函数的参数；
        attr(输入参数)，指定新建线程的属性，如线程栈大小等；如果值为NULL，表示使用系统默认属性。

    函数返回值：
        成功，返回0；
        失败，返回相关错误码。

    需要注意：
        主线程，这是一个进程的初始线程，其入口函数为main函数。
        新线程的运行时机，一个线程被创建之后有可能不会被马上执行，甚至，在创建它的线程结束后还没被执行；也有可能新线程在当前线程从pthread_create前就已经在运行，甚至，在pthread_create前从当前线程返回前新线程就已经执行完毕
*/





/*

在新线程被创建后，便有了一个在其所在进程内(线程依附于进程而存在)唯一的标识符，由pthread_t表示，称为线程ID。一个线程可以调用以下接口获取其ID：

include <pthread.h>
pthread_t pthread_self(void);

    pthread_self直接返回调用线程的ID。

判断两个线程ID的大小是没有任何意义的，但有时可能需要判断两个给定的线程ID是否相等，使用以下接口：

include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2);
*/







/*
* 文件名： thread_sample1.c
* 描述：演示线程基本操作
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*子线程1入口函数*/
void *thread_routine1(void *arg)
{
    fprintf(stdout, "thread1: hello world!\n");
    sleep(1);
    /*子线程1在此退出*/
    return NULL;
}

/*子线程2入口函数*/
void *thread_routine2(void *arg)
{

    fprintf(stdout, "thread2: I'm running...\n");
    pthread_t main_thread = (pthread_t)arg;

    /*分离自我，不能再被连接*/
    pthread_detach(pthread_self());

    /*判断主线程ID与子线程2ID是否相等*/
    if (!pthread_equal(main_thread, pthread_self())) {
        fprintf(stdout, "thread2: main thread id is not equal thread2\n");
    }

    /*等待主线程终止*/
    pthread_join(main_thread, NULL);
    fprintf(stdout, "thread2: main thread exit!\n");

    fprintf(stdout, "thread2: exit!\n");
    fprintf(stdout, "thread2: process exit!\n");
    /*子线程2在此终止，进程退出*/
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    /*创建子线程1*/
    pthread_t t1;
    if (pthread_create(&t1, NULL, thread_routine1, NULL)!=0) {
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }
    /*等待子线程1终止*/
    pthread_join(t1, NULL);
    fprintf(stdout, "main thread: thread1 terminated!\n\n");

    /*创建子线程2，并将主线程ID传递给子线程2*/
    pthread_t t2;
    if (pthread_create(&t2, NULL, thread_routine2, (void *)pthread_self())!=0) {
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }

    fprintf(stdout, "main thread: sleeping...\n");
    sleep(3);
    /*主线程使用pthread_exit函数终止，进程继续存在*/
    fprintf(stdout, "main thread: exit!\n");
    pthread_exit(NULL);    

    fprintf(stdout, "main thread: never reach here!\n");
    return 0;
}
