/*
    // 打开一个目录
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
        参数：
            - name: 需要打开的目录的名称
        返回值：
            DIR * 类型，理解为 目录流
            错误返回NULL


    // 读取目录中的数据
    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
    一次读取各个目录实体
        - 参数：dirp是 opendir 返回的结果
        - 返回值：
            struct dirent，代表读取到的文件的信息
            读取到了末尾或者失败了，返回NULL

    // 关闭目录
    #include <dirent.h>
    int closedir(DIR *dirp);

*/
#include <sys/types.h>
#include <dirent.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h> //使用该头文件，NULL 就不会显示未定义了
//#define NULL ((void *)0)

int getFileNum(const char * path);
//argc是命令行总的参数个数 argv[]是argc个参数，其中第0个参数是程序的全名
int main(int argc,char* argv[])
{
    // 没有参数
    if(argc<2)
    {
        //输出默认的第0个参数，即程序全名
        printf("%s path\n",argv[0]);
    }

    int num = getFileNum(argv[1]);
    printf("普通文件的个数为：%d \n",num);

    return 0;
}

//用于获取目录下 所有普通文件的个数
int getFileNum(const char * path)
{
    //打开目录
    DIR* dir=opendir(path);
    //如果该文件打开失败或者为空，就输出错误信息
    if(dir==NULL)
    {
        perror("opendir");
        exit(0);
    }

    struct dirent * ptr=readdir(dir);

    //记录普通文件的个数
    int toatl=0;

    //不断读入目录
    while((ptr=readdir(dir))!=NULL)
    {
        //获取名称
        char * dname=ptr->d_name;
        

        //忽略 . 和 .. 即忽略当前目录和父目录
        if(strcmp(dname,".")==0||strcmp(dname,"..")==0) //使用字符串比较函数 strcmp
        {
            //忽略这两个 目录
            continue;
        }

        //判断是否是普通文件 还是目录
        if(ptr->d_type==DT_DIR)
        {
            //是目录，就继续读取这个目录
            char newpath[256];
            //将新目录 格式化输出到newpath中
            sprintf(newpath,"%s/%s",path,dname);
            //递归这个过程
            toatl += getFileNum(newpath);

        }
        
        if(ptr->d_type==DT_REG)
        {
            //如果是普通文件，就 统计数量
            toatl++;
        }

    }

    //关闭目录
    closedir(dir);

    return toatl;
}
