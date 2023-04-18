#include "ThreadPool.hpp"
#include "Task.hpp"
#include <time.h>
#include <unistd.h>
using namespace std;

int main()
{
    // ThreadPool<CallTask> *threadpool = new ThreadPool<CallTask>;
    ThreadPool<CallTask>::getInstance()->run();

    srand((unsigned int)time(nullptr));
    // threadpool->run();
    while (true)
    {
        // int x=rand()%10;
        // int y=rand()%5;
        // char op=oper[rand()%oper.size()];
        // CallTask t(x,y,op,mymath);
        // threadpool->push(t);
        // cout<<"生成了任务"<<endl;
        int x, y;
        char op;
        std::cout << "请输入数据1# ";
        std::cin >> x;
        std::cout << "请输入数据2# ";
        std::cin >> y;
        std::cout << "请输入你要进行的运算#";
        std::cin >> op;
        CallTask t(x, y, op, mymath);
        std::cout << "你刚刚录入了一个任务: " << t.toSting() << ", 确认提交吗？[y/n]# ";
        char confirm;
        std::cin >> confirm;
        if (confirm == 'y')
            ThreadPool<CallTask>::getInstance()->push(t);
        sleep(1);
    }

    return 0;
}