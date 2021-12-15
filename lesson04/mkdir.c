/*
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);   
    作用：创建一个目录
    参数：
        pathname：创建目录的路径
        mode：权限 三个八进制数
    返回值：
        成功返回0，失败返回-1

*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
int main()
{
    int ret=mkdir("aaa",0777);

    if(ret==-1)
    {
        perror("mkdir");
        return -1;
    }
    else
    {
        printf("创建目录成功！\n");
    }


    return 0;
}