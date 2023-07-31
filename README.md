# timer
用rbtree，timewheel，minheap三种方式实现定时器

#通过使用不同的宏定义使用不同的方式组织定时器

#define ENABLE_RBTREE_TIMER 1 //采用红黑树组织定时器

#define ENABLE_MINHEAP_TIMER 1 /采用最小堆组织定时器

#define ENABLE_TIMERWHEEL_TIMER 1 //采用时间轮组织定时器
