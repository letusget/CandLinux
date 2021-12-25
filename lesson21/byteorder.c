/*
字节序：字节在内存中的存储顺序
小端字节序：数据的低位存储在内存的低位地址
大端字节序：数据的低位存储在内存的高位地址
*/

//通过代码实现检测当前主机的字节序
#include <stdio.h>

int main()
{
    union 
    {
        short value;    //2字节
        char bytes[sizeof(short)];  //char[2]
        /* data */
    }test;

    //正好两个字节01 和 02
    test.value=0x0102;
    //查看低位地址存储的是高位还是低位，如果是01，就是低位地址存储的高位--> 大端存储
    //如果是02，就是低位地址存储的低位--> 小端存储
    if((test.bytes[0]==1)&&(test.bytes[1]==2))
    {
        printf("大端字节序\n");
    }
    else if((test.bytes[0]==2)&&(test.bytes[1]==1))
    {
        printf("小端字节序\n");
    }
    else
    {
        printf("位置字节序，出现错误\n");
    }
    

    return 0;
}