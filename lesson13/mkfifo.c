/*
创建 FIFO 文件
可以通过shell命令，也可以通过 系统函数调用
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
    参数：
        pathname: 管道名称
        mode: 文件权限，与open的参数一致，读 写 操作,使用八进制数表示
    返回值：
        成功返回0，如果失败返回-1，并设置错误号

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    
    //判断文件是否存在
    int ret = access("fifo1",F_OK);
    if(ret==-1)
    {
        printf("管道不存在，创建管道\n");

        //创建管道文件
        ret = mkfifo("fifo1",0664);
        if(ret==-1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    



    return 0;
}