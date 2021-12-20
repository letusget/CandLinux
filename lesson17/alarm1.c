/**
 * 该案例实现计算1s 电脑可以处理的数据个数
 */
#include <stdio.h>
#include <unistd.h>

int main()
{
    alarm(1);
    int i=0;
    while (1)
    {
        printf("%i\n",i++);
    }
    

    return 0;
}