#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#define ll long long
typedef unsigned long long ull;
using namespace std;
using namespace std::chrono;

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        workers.resize(numThreads);
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(
                [this] {
                    for (;;) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(queueMutex);
                            condition.wait(lock, [this]{ return stop || !tasks.empty(); });
                            if (stop && tasks.empty())
                                return;
                            task = std::move(tasks.front()); // Extract task from list.
                            tasks.pop(); // Remove task from list after extracting it.
                        } // Unlock mutex so another thread can accept the tasks.
                        task(); // Run The Task
                    }
                }
            );
        }
    }
    template<class F>
    void enqueue(F&& task) {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::forward<F>(task));
        lock.unlock();
        condition.notify_one();
    }

    void dequeue() {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { return !tasks.empty(); });
        auto task = std::move(tasks.front());
        tasks.pop();
        lock.unlock();
        worker(std::thread(worker));
    }
private:
  vector<thread> workers;
  queue<function<void()>>tasks;
  mutex queueMutex;
  condition_variable condition;
  bool stop;

};




string get_thread_id(){
    auto myid = this_thread::get_id();
    stringstream ss;
    ss<<myid;
    string str = ss.str();
    return str;
}

void solve() {
    ThreadPool pool(4);
    cout<<"Thread Pool Created\n";
    cout<<"Enqueue (Assign) some tasks\n";
    for(int i=0;i<8;i++){
        pool.enqueue([i]{
            printf("Task %d %s exceeded by thread \n",i,get_thread_id(),c_str());
            this_thread::sleep_for(seconds(1));
        });
    }

}

// <--------------------I/O------------->

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    srand(chrono::high_resolution_clock::now().time_since_epoch().count());
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}