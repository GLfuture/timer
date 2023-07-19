//#define ENABLE_RBTREE_TIMER 1
//#define ENABLE_MINHEAP_TIMER 1
#define ENABLE_TIMERWHEEL_TIMER 1
#include "timermanager.hpp"
#include <sys/epoll.h>
using namespace std;
void Print(const char *msg)
{
    printf("%s\n",msg);
}
using namespace std::chrono;

int main()
{
#if ENABLE_RBTREE_TIMER | ENABLE_MINHEAP_TIMER
    TimerManager manager;
    const char *test1="ONCE : 20ms";
    const char *test2="ONCE : 3ms";
    const char *test3="CIRCLE : 10ms";
    const char *test4="CIRCLE : 2ms";
    std::function<void()> func1=bind(Print,test1);
    std::function<void()> func2=bind(Print,test2);
    std::function<void()> func3=bind(Print,test3);
    std::function<void()> func4=bind(Print,test4);
    manager.Add_Timer(1,20,Timer::TYPE_ONCE,func1);
    manager.Add_Timer(2,3,Timer::TYPE_ONCE,func2);
    Timer_Ptr ptr1 = manager.Add_Timer(3,10,Timer::TYPE_CIRCLE,func3);
    Timer_Ptr ptr2 = manager.Add_Timer(4,2,Timer::TYPE_CIRCLE,func4);
    manager.Create_Timerfd();
    int epfd=epoll_create(1);
    epoll_event event,events[64];
    event.data.fd=manager.Get_Timerfd();
    event.events=EPOLLIN | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,manager.Get_Timerfd(),&event);
    int i=0;
    while(1)
    {
        if(i<20) manager.Update_Timerfd();
        int nready = epoll_wait(epfd,events,64,-1);
        if(i==10) manager.Del_Timer(ptr1);
        if(i==14) manager.Del_Timer(ptr2);
        printf("%d ONCE\n",i);
        manager.Tick();
        i++;
    }
#elif ENABLE_TIMERWHEEL_TIMER
    const char *test1="ONCE : 20ms";
    const char *test3="CIRCLE : 10ms";
    std::function<void()> func3=bind(Print,test3);
    std::function<void()> func1=bind(Print,test1);
    TimerManager manager(5,256,10);
    manager.Start();
    for(int i=0;i<100;i++)
    {
        manager.Add_Timer(1,func1,50);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
#endif
    return 0;
}