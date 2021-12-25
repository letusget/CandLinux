/*
网络通信时，需要将主机字节序 转换为 网络字节序（大端），另外一段 在获取到数据后，根据情况将网络字节序转换为主机字节序

*/
#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    //htons  转换端口
    unsigned short a = 0x0102;
    printf("主机端口: %x\n",a);
    unsigned short b = htons(a);    //主机字节序-> 网络字节序
    printf("网络端口: %x\n",b);

    printf("=======================\n");

    //htonl  转换 IP 
    char buf[4]={192,168,1,100};
    printf("主机IP: 192,168,1,100\n");
    int num=*(int *)buf;    //先将 字符指针，转换为 int 型指针，然后在解引用，得到数字
    int sum = htonl(num);   //主机字节序 -> 网络字节序

    unsigned char* p=(char*)&sum;
    printf("网络IP: %d %d %d %d\n",*p,*(p+1),*(p+2),*(p+3));

    return 0;
}