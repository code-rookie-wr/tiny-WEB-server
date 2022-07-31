#include "ThreadPool.h"

void test() {
    cout << "threadID : " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main(){
    ThreadPool pool(3);
    for (int i = 0; i < 10; i++) {
        pool.addTask(test);
    }
    pool.run();
    system("pause");
    return 0;
}