#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>    //根据id 获取文件所有者 的头文件
#include <grp.h>    //获取文件所在组 的头文件
#include <time.h>    //将秒数 转化 为时间
#include <string.h>

//模拟实现 ls -l 命令
// -rw-r--r-- 1 root root    78 Nov 25 22:09 ls-l.c

//两个参数，一个是文件路径（文件名称），另一个是 命令行后的参数
int main(int argc,char * argv[])
{
    //检查输入参数是否正确，给出提示
    if(argc<2)
    {
        printf("%s filename\n",argv[0]);
        return -1;
    }

    //通过 stat 结构体获取文件各种信息
    struct stat st;     //存储文件信息的结构体
    int ret=stat(argv[1],&st);
    if(ret==-1)
    {
        //如果有错误就 输出错误信息
        perror("stat");
        return -1;
    }

    //获取文件类型和文件权限
    //使用一个数组来存储各个表示字符，方便表示状态
    char perms[11]={0}; //保存文件类型和文件权限的字符串

    //使用选择语句，通过 按位与 操作，得到文件权限信息
    switch (st.st_mode&S_IFMT)
    {
    case S_IFLNK:   //软链接 标识
        perms[0]='1';
        break;
    case S_IFDIR:   //目录 标识
        perms[0]='d';
        break;
    case S_IFREG:   //普通文件 标识
        perms[0]='-';
        break;
    case S_IFBLK:   //块设备 标识
        perms[0]='1';
        break;
    case S_IFCHR:   //字符设备 标识
        perms[0]='c';
        break;
    case S_IFSOCK:   //套接字 标识
        perms[0]='s';
        break;
    case S_IFIFO:   //管道文件 标识
        perms[0]='t';
        break;
    
    default:
        perms[0]='?';
        break;
    }

    // 判断文件权限

    //文件所有者的权限判断：与 权限标志位 做 与 操作，得到，如果为1，就有该权限，否则就是没有改权限
    perms[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    perms[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    perms[3] = st.st_mode & S_IXUSR ? 'x' : '-';
    //文件所在组 权限  is read group 的英文缩写
    perms[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    perms[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    perms[6] = st.st_mode & S_IXGRP ? 'x' : '-';
    //其他人 权限  is read other
    perms[7] = st.st_mode & S_IROTH ? 'r' : '-';
    perms[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    perms[9] = st.st_mode & S_IXOTH ? 'x' : '-';

    //获取硬链接数
    int linkNum=st.st_nlink;

    //文件所有者    从 uid 获取到所有者 名字
    char* fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char * fileGrp=getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //修改时间
    char* time= ctime(&st.st_mtime);
    //因为时间在格式化后 会 默认哟一个回车符，需要将其去掉
    char mtime[512]={0};
    strncpy(mtime,time,strlen(time)-1); //将time的内容拷贝到mtime中，并将最后的一个回车符去掉
    
    //将内容输出
    char buf[1024];
    sprintf(buf,"%s %d %s %s %ld %s %s",perms,linkNum,fileUser,fileGrp,fileSize,mtime,argv[1]);

    //输出该信息
    printf("%s\n",buf);

    return 0;
}