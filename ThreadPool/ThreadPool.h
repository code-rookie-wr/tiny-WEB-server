#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<future>
#include<memory>
#include<vector>
#include<queue>
#include<atomic>
#include<chrono>

using namespace std;

//最大可用线程数
const int MAX_THREADS = 9;

class ThreadPool {
public:
    //线程池构造函数，传入初始线程数，默认为1
    ThreadPool(int num = 1);
    //线程池析构函数
    ~ThreadPool();
    //添加任务到任务队列
    template <class F, class...Args>
    void addTask(F&& f, Args&& ...args);

private:
    //工作线程
    vector<thread> workers;
    //任务队列
    queue<function<void()>> tasks;
    //队列互斥锁
    mutex mtx;
    //条件变量
    condition_variable cond;
    //停止标志
    atomic<bool> stop;
};

ThreadPool::ThreadPool(int num) :stop(false) {
    //判断输入默认线程是否合理(1-9)之间
    if (num <= 0 || num > MAX_THREADS) {
        cerr << "Input threadNum is invalid !" << endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        exit(1);
    }

    for (int i = 0; i < num; i++) {
        cout << "Creating the " << i+1 << "th thread"  << endl;
        //将工作函数添加进线程中
        workers.emplace_back([this]() {
            while (true) {
                //函数模板用于接收队头任务
                function<void()> task;
                {
                    //加锁
                    unique_lock<mutex> lock(this->mtx);
                    //当stop为fasle或队列为空时，线程处于等待状态
                    this->cond.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty();
                        });
                    //当stop为true且队列为空时，函数返回
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    //取出队头任务
                    task = this->tasks.front();
                    //任务出队
                    this->tasks.pop();
                }
                //执行当前任务
                task();
            }
        });
    }
}

template <class F, class...Args>
void ThreadPool::addTask(F&& f, Args&& ...args) {
    //使用可变参模板结合bind实现参数数量自定义
    auto task = bind(forward<F>(f), forward<Args>(args)...);
    {
        //加锁
        unique_lock<mutex> lock(mtx);
        //将新任务添加进任务队列
        tasks.emplace(task);
    }
    //队列中有了任务，唤醒等待线程
    this->cond.notify_one();
}

ThreadPool::~ThreadPool() {
    //停止标志置为true
    stop = true;

    //唤醒所有工作线程，直到队列任务执行完
    this->cond.notify_all();

    //主线程等待子线程执行
    for (auto& th : workers) {
        th.join();
    }
}

#endif

