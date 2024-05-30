#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#define ll long long
typedef unsigned long long ull;
using namespace std;
using namespace std::chrono;

std::binary_semaphore signal_to_producer{1}, signal_to_consumer{0}; 
int buff_size = 5;
int buff[5];

void producer() {
    while (1) {
        signal_to_producer.acquire();
        cout << "Produced = ";
        for (int i = 0; i < buff_size; i++) {
            buff[i] = i * i;
            cout << buff[i] << " " << flush;
            this_thread::sleep_for(milliseconds(200));
        }
        cout << endl;
        signal_to_consumer.release();
    }
}

void consumer() {
    while (1) {
        signal_to_consumer.acquire();
        cout << "Consumed = ";
        for (int i = buff_size - 1; i >= 0; i--) {
            cout << buff[i] << " " << flush;
            buff[i] = 0;
            this_thread::sleep_for(milliseconds(200));
        }
        cout << endl << endl;
        signal_to_producer.release();
    }
}

void solve() {
    thread producer_thread(producer);
    thread consumer_thread(consumer);
    cout << "Got the signal \n";
    producer_thread.join();
    consumer_thread.join();
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
