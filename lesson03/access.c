/**
 * @file access.c
 * 
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-27
 * 
 * @copyright Copyright (c) 2021
 * 
 *  #include<unistd.h>
 *  int access(const char *pathname,int mode);
 *      作用：判断某个文件时候有某个权限，判断文件是否存在
 *      参数：
 *          - pathname: 判断文件路径
 *          - mode: 
 *              R_OK: 判断是否有读权限
 *              W_OK：判断是否有写权限
 *              X_OK：判断是否有执行权限
 *              F_OK：判断文件是否存在
 *      返回值：成功返回0，失败返回-1
 */

#include <unistd.h>
#include <stdio.h>

int main()
{
    int ret = access("a.txt",F_OK);

    if(ret==-1)
    {
        perror("access");
    }
    else
    {
        printf("文件存在");
    }


    return 0;
}