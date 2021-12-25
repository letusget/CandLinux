#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    //创建一个 IP字符串  点分十进制字符串
    char buf[]="192.168.1.4";

    unsigned int num=0;
    //将点分十进制的IP 字符串 转换为 网络字节序的整数
    inet_pton(AF_INET,buf,&num);
    //输出转换完的值,一位一位的输出
    unsigned char* p=(unsigned char *)&num;
    //转换为10 进制输出了
    printf("%d %d %d %d \n",*p,*(p+1),*(p+2),*(p+3));

    printf("===========================\n");
    //将网络字节序的IP地址转换为点分十进制的IP字符串
    char ip[16]="";
    const char* str = inet_ntop(AF_INET,&num,ip,16);
    printf("str: %s\n",str);
    printf("ip: %s\n",ip);
    printf("%d\n",ip==str);

    return 0;
}