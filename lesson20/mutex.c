/*
互斥量的类型 pthread_mutex_t
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
    功能：初始化互斥量
    参数：
        mutex: 需要初始化的互斥量
        attr: 互斥量的相关属性，一般设置为NULL
    restrict: C语言的修饰符，被其修饰的指针，就无法被另外一个指针操作
int pthread_mutex_destroy(pthread_mutex_t *mutex);
    功能：释放互斥量资源
int pthread_mutex_lock(pthread_mutex_t *mutex);
    功能：加锁  
    阻塞执行：如果有一个线程加锁了，其他线程就只能阻塞等待
int pthread_mutex_trylock(pthread_mutex_t *mutex);
    功能：尝试加锁，如果加锁失败，不会阻塞，可以直接返回
int pthread_mutex_unlock(pthread_mutex_t *mutex);
    功能：解锁
*/