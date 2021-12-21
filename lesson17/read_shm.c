#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

int main()
{
    //获取共享内存，key 值必须要相同，大小可以比写共享进行的小，但是不能大
    int shmid = shmget(100,0,IPC_CREAT);
    printf("shmid : %d\n",shmid);

    //和当前进程进行关联
    void* ptr = shmat(shmid,NULL,0);

    //读共享内存
    printf("%s\n",(char*)ptr);

    printf("按任意键继续\n");
    getchar();

    //解除关联
    shmdt(ptr);

    //删除内存
    shmctl(shmid,IPC_RMID,NULL);



    return 0;
}
