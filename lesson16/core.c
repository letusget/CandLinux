#include <stdio.h>
#include <string.h>

int main()
{
    //指针未初始化
    char * buf;
    strcpy(buf,"hello!");

    return 0;
}