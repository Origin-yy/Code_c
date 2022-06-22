/*
互斥量
奇数号哲学家，先左后右.
偶数号哲学家，先右后左
*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

pthread_mutex_t chopsticks[5];

void my_error(const char *err_string,int line,int errnumber);
void* philosopher(void *s);
void pick_two(int i);
void free_two(int i);

int main(void)
{
    int r;
    pthread_t tid[5];
    for(int i=0; i<5; i++)
    {
        r = pthread_mutex_init(&chopsticks[i],NULL);
        if(r != 0 )
            my_error("pthrerad_mutex_init",__LINE__,r);
    }

    for(int i=0; i<5; i++)
    {
        r = pthread_create(&tid[i],NULL,philosopher,(void*)(long)i);
        if(r != 0)
            my_error("pthread_create",__LINE__,r);
    }

    for(int i=0; i<5; i++)
    {
        r = pthread_join(tid[i],NULL);
        if(r != 0)
            my_error("pthread_join",__LINE__,r);
    }

    for(int i=0; i<5; i++)
    {
        r = pthread_mutex_destroy(&chopsticks[i]);
        if(r != 0)
            my_error("pthread_destroy",__LINE__,r);
    }
    return 0;
}

//哲学家线程
void* philosopher(void *s)
{
    long i = (long)s;
    while(1)
    {
        //思考...
        pick_two(i);       //饿了，拿左右筷子
        //吃饭...
        free_two(i);       //吃完，放左右筷子
    }
}
//编号为i的哲学家拿左右筷子
void  pick_two(int i) 
{
    int r = 0;
    if(i%2 == 1)
    {
        r = pthread_mutex_lock(&chopsticks[i]);
        if(r != 0)
            my_error("pthread_mutex_lock",__LINE__,r);
        printf("哲学家%d拿起左筷子%d.\n",i,i);

        r = pthread_mutex_lock(&chopsticks[(i+1)%5]);
        if(r != 0)
            my_error("pthread_mutex_lock",__LINE__,r);
        printf("哲学家%d拿起右筷子%d,开始吃饭\n",i,(i+1)%5);
    }
    else
    {
        r = pthread_mutex_lock(&chopsticks[(i+1)%5]);
        if(r != 0)
            my_error("pthread_mutex_lock",__LINE__,r);
        printf("哲学家%d拿起右筷子%d,开始吃饭\n",i,(i+1)%5);

        r = pthread_mutex_lock(&chopsticks[i]);
        if(r != 0)
            my_error("pthread_mutex_lock",__LINE__,r);
        printf("哲学家%d拿起左筷子%d.\n",i,i);
    }
}
//编号为i的哲学家释放左右筷子
void free_two(int i)
{
    int r = 0;

    r = pthread_mutex_unlock(&chopsticks[i]);
    if(r != 0)
        my_error("pthread_mutex_unlock",__LINE__,r);
    printf("哲学家%d放下左筷子%d.\n",i,i);

    r = pthread_mutex_unlock(&chopsticks[(i+1)%5]);
    if(r != 0)
        my_error("pthread_mutex_unlock",__LINE__,r);
    printf("哲学家%d放下右筷子%d,开始思考.\n",i,(i+1)%5);
}
//函數
void my_error(const char *err_string,int line,int errnumber)
{
    fprintf(stderr,"line:%d %s %s\n",line, err_string, strerror(errnumber));
    exit(1);
}