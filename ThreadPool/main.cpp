#include "ThreadPool.h"

void test(int val) {
    cout << val << '\t';
    cout << "threadID : " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main(){
    ThreadPool* pool = new ThreadPool(3);
    for (int i = 0; i < 10; i++) {
        pool->addTask(test, i);
    }
    delete pool;
    system("pause");
    return 0;
}