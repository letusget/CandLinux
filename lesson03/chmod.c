/**
 * @file chmod.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * 
 *  #include <sys/stat.h>
 *  int chmod(const char *pathname, mode_t mode);
 *      参数：
 *          - pathname: 修改文件路径
 *          - mode: 需要修改的权限值，使用八进制数表示
 *      返回值：成功返回0，失败返回-1
 * 
 */
#include <sys/stat.h>   //需要包含的头文件
#include <stdio.h>
int main()
{
    //用三个八进制数 来表示权限     八进制数，以0 开头
    int ret=chmod("a.txt",0775);

    if(ret==-1)
    {
        perror("chmod");
        return -1;
    }
    else
    {
        printf("修改权限成功！！\n");
    }

    return 0;

}