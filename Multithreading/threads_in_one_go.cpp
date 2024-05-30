/*  * 
* 1. What do you understand by thread and give one example in C++?

ANSWER
0. In every application there is a default thread which is main(), in side this we create other threads.
1. A thread is also known as lightweight process. Idea is achieve parallelism by dividing a process into multiple threads. 
   For example:
   (a) The browser has multiple tabs that can be different threads. 
   (b) MS Word must be using multiple threads, one thread to format the text, another thread to process inputs (spell checker)
   (c) Visual Studio code editor would be using threading for auto completing the code. (Intellicence)

? WAYS TO CREATE THREADS IN C++11
1. Function Pointers
2. Lambda Functions
3. Functors
4. Member Functions
5. Static Member functions

If we create multiple threads at the same time, it doesn't guarantee which one will start first 


* JOIN NOTES
0. Once a thread is started we wait for this thread to finish by calling join() function on thread object.
1. Double join will result into program termination.
2. If needed we should check thread is joinable before joining. ( using joinable() function)

*DETACH NOTES
0. This is used to detach newly created thread from the parent thread.
1. Always check before detaching a thread that it is joinable otherwise we may end up double detaching and 
   double detach() will result into program termination.
2. If we have detached thread and main function is returning then the detached thread execution is suspended.

NOTES:
Either join() or detach() should be called on thread object, otherwise during thread object�s destructor it will 
terminate the program. Because inside destructor it checks if thread is still joinable? if yes then it terminates the program.

* Mutex: Mutual Exclusion

RACE CONDITION:
0. Race condition is a situation where two or more threads/process happens to change a common data at the same time.
1. If there is a race condition then we have to protect it and the protected section is  called critical section/region.

MUTEX:
0. Mutex is used to avoid race condition.
1. We use lock() , unlock() on mutex to avoid race condition.

*  try_lock 
0. try_lock() Tries to lock the mutex. Returns immediately. On successful lock acquisition returns true otherwise returns false.
1. If try_lock() is not able to lock mutex, then it doesn't get blocked that's why it is called non-blocking.
2. If try_lock is called again by the same thread which owns the mutex, the behaviour is undefined.
   It is a dead lock situation with undefined behaviour. (if you want to be able to lock the same mutex by same thread more than one time the go for recursive_mutex)

There are so many try_lock function
1. std::try_lock
2. std::mutex::try_lock
3. std::shared_lock::try_lock
4. std::timed_mutex::try_lock
5. std::unique_lock::try_lock
6. std::shared_mutex::try_lock
7. std::recursive_mutex::try_lock
8. std::shared_timed_mutex::try_lock
9. std::recursive_timed_mutex::try_lock

* try_lock 
0. std::try_lock() tries to lock all the mutex passed in it one by one in given order.
1. On success this function returns -1 otherwise it will return 0-based mutex index number which it could not lock.
2. If it fails to lock any of the mutex then it will release all the mutex it locked before.
3. If a call to try_lock results in an exception, unlock is called for any locked objects before re-throwing.

*Timed Mutex
few points to remember about  timed mutex is as follows:
0. std::timed_mutex is blocked till timeout_time or the lock is acquired and returns true if success 
   otherwise false.
1. Member Function:
   a. lock 
   b. try_lock
   c. try_lock_for    ---\ These two functions makes it different from mutex.
   d. try_lock_until  ---/ 
   e. unlock

EXAMPLE: try_lock_for();
Waits until specified timeout_duration has elapsed or the lock is acquired, whichever comes first.
On successful lock acquisition returns true, otherwise returns false.

* Recursive Mutux 
Few points to remember about recursive mutex is as follows:
0. It is same as mutex but, Same thread can lock one mutex multiple times using recursive_mutex.
1. If thread T1 first call lock/try_lock on recursive mutex m1, then m1 is locked by T1, now 
   as T1 is running in recursion T1 can call lock/try_lock any number of times there is no issue.
2. But if T1 have acquired 10 times lock/try_lock on mutex m1 then thread T1 will have to unlock
   it 10 times otherwise no other thread will be able to lock mutex m1.
   It means recursive_mutex keeps count how many times it was locked so that many times it should be unlocked.
3. How many time we can lock recursive_mutex is not defined but when that number reaches and if we were calling
   lock() it will return std::system_error OR if we were calling try_lock() then it will return false.

BOTTOM LINE:
0. It is similar to mutex but have extra facility that it can be locked multiple time by same thread.
1. If we can avoid recursive_mutex then we should because it brings overhead to the system.
2. It can be used in loops also.

* lock_guard
0. It is very light weight wrapper for owning mutex on scoped basis.
1. It acquires mutex lock the moment you create the object of lock_guard.
2. It automatically removes the lock while goes out of scope.
3. You can not explicitly unlock the lock_guard.
4. You can not copy lock_guard.

* unique lock 
1. The class unique_lock is a mutex ownership wrapper.
2. It Allows:
    a. Can Have Different Locking Strategies
    b. time-constrained attempts at locking (try_lock_for, try_lock_until)
    c. recursive locking
    d. transfer of lock ownership (move not copy)
    e. condition variables. (See this in coming videos)

Locking Strategies
   TYPE           EFFECTS(S)
1. defer_lock  do not acquire ownership of the mutex.
2. try_to_lock  try to acquire ownership of the mutex without blocking.
3. adopt_lock  assume the calling thread already has ownership of the mutex.

* condition variables 
? used for synchronization of two or more threads
1. Condition variables allow us to synchronise threads via notifications.
   a. notify_one();
   b. notify_all();
2. You need mutex to use condition variable
3. Condition variable is used to synchronise two or more threads.
4. Best use case of condition variable is Producer/Consumer problem.
5. Condition variables can be used for two purposes:
    a. Notify other threads
    b. Wait for some condition


* Thread OR Process Synchronisation" 
1.0 Thread Or Process synchronize to access critical section.
2.0 Critical section is one or collection of program statements which should be executed by only one thread or process at a time.

* std::lock(m1, m2, m3, m4);
1. All arguments are locked via a sequence of calls to lock(),  try_lock(), or unlock() on each argument.
2. Order of locking is not defined (it will try to lock provided mutex in any order and ensure that
    there is no deadlock).
3. It is a blocking call.

[Example:0] -- No deadlock.
    Thread 1                    Thread 2
    std::lock(m1,m2);           std::lock(m1,m2);

[Example:1] -- No deadlock. 

    Thread 1                    Thread 2
    std::lock(m1, m2);          std::lock(m2, m1);

[Example:2] -- No deadlock. 

    Thread 1                    Thread 2
    std::lock(m1, m2, m3, m4);  std::lock(m3, m4);
                                std::lock(m1, m2);

[Example:3] -- Yes, the below can deadlock. 

    Thread 1                    Thread 2
    std::lock(m1,m2);           std::lock(m3,m4);
    std::lock(m3,m4);           std::lock(m1,m2);


*std::promise and std::future and how they work together to synchronise threads.
Basically std::promise is sent to the called thread and once the value is ready we set that value in promise object, now at calling thread side we get that value using std::future object which was created using std::promise object before sending it to the called thread. And this is how we receive value from one thread to another in synchronisation.

*async
* three different launch policy for creating task using std::async are as follows:
1. std::launch::async
2. std::launch::deffered // Os is overloaded, without a thread , kind of a blocking call 
3. std::launch::async | std::launch::deffered // based on

With the help of above policies std::async decides to create task as asynchronous or synchronous. 
? HOW IT WORKS:
1. It automatically creates a thread (Or picks from internal thread pool) and a promise object for us.
2. Then passes the std::promise object to thread function and returns the associated std::future object.
3. When our passed argument function exits then its value will be set in this promise object, so eventually return value will be available in std::future object.
! return values to thread

? SIDE NOTES:
1. We can send functor and lambda function as callback to std::async, it will work the same.

* Producer Consumer OR Bounded Buffer Problem
THE PROBLEM STATEMENT:
 1. Producer will produce and consumer will consume with synchronisation of a common buffer.
 2. Until producer thread produces any data consumer thread can't consume.
 3. Threads will use condition_variable to notify each other.
 4. We need mutex if we use condition_variable because CV waits on mutex.
 5. This is one of the example of producer consumer there are many.

PRODUCER thread steps:
 1. lock mutex, if success then go ahead otherwise wait for mutex to get free.
 2. check if buffer is full and if it is full then unlock mutex and sleep, if not then go ahead and produce.
 3. insert item in buffer.
 4. unlock mutex. 
 5. notify consumer.

? CONSUMER thread steps:
 1. lock mutex, if success then go ahead otherwise wait for mutex to get free.
 2. check if buffer is empty and if it is, then unlock the mutex and sleep, if not then go ahead and consume.
 3. consume item from buffer.
 4. unlock mutex.
 5. notify producer.

! IMP:
 Producer and Consumer have to notify each other upon completion of their job.

? What Is SLEEP :
0. “I’m done with my time-slice, and please don’t give me another one for at least n milliseconds.”  The OS doesn’t even try to schedule the sleeping thread until requested time has passed.
1. It will keep the lock and sleep.
2. Sleep is directly to thread, it is a thread function.

? What Is WAIT :
0. “I’m done with my time-slice. Don’t give me another time-slice until someone calls notify().” As with sleep(), the OS won’t even try to schedule your task unless someone calls notify() (or one of a few other wakeup scenarios occurs).
1. It releases the lock and wait.
2. Wait is on condition variable, it is like there is a condition variable in a thread and wait is applied to that CV but it ends up putting thread in waiting state.


! static variables are not thread safe

* If we want to safeguard a resource from multiple access of threads then go with mutex
*If we want to synchronize threads , then go with semaphores 

*Scoped Lock
A scoped_lock in C++ is a mechanism provided by the C++ Standard Library's threading support (such as the mutex header) for managing mutual exclusion in multi-threaded applications. Here's a summary of scoped_lock:

Purpose: The purpose of a scoped_lock is to acquire ownership of one or more mutexes for a duration of a scope, ensuring that the mutexes are locked when the scoped_lock object is constructed and automatically released when it goes out of scope.

Usage: It is typically used in situations where multiple threads need to access shared resources concurrently, and the access to these resources must be synchronized to prevent data races and ensure thread safety.

RAII (Resource Acquisition Is Initialization): Scoped_lock follows the RAII principle, which ties the lifetime of a resource (in this case, mutex ownership) to the lifetime of an object. When a scoped_lock object is created, it acquires the lock on the associated mutex(es), and when it goes out of scope (e.g., at the end of a function or block), it automatically releases the lock(s), even in the presence of exceptions.

Constructor: The constructor of scoped_lock takes one or more mutexes as arguments and locks them in a deadlock-safe manner. If any of the mutexes are already locked by another thread, the constructor blocks until it can acquire ownership of all the mutexes without deadlocking.

Ownership: A scoped_lock object owns the lock on the associated mutex(es) until it is destroyed, meaning that the thread that owns the scoped_lock can safely access the protected resources. Other threads attempting to lock the same mutex(es) will be blocked until the scoped_lock releases the lock(s).

Preventing Deadlocks: Scoped_lock is designed to help prevent deadlocks by acquiring locks in a consistent and deadlock-safe order. It's important to acquire locks in the same order across all threads to avoid potential deadlocks.

Overall, scoped_lock provides a convenient and safe way to manage mutex ownership and ensure mutual exclusion in multi-threaded C++ applications, helping to prevent data races and synchronization issues.

*/

#include <iostream>
#include <thread>
#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
#define ll long long
typedef unsigned long long ull;
using namespace std;
using namespace std::chrono;



ull oddSum=0;
ull evenSum=0;
void findOdd(ull start,ull end){
	for(ull i=start;i<=end;i++){
		if((i&1)==1){
			oddSum+=i;
		}
	}
}
void findEven(ull start,ull end){
	for(ull i=start ;i<=end ;i++){
		if((i&1)==1){
			evenSum+=i;
		}
	}
}

void fun(int x){
	while(x-- > 0){
		cout<<x<<endl;
	}
}
// functor (function object)
/* class Base{
public:
	void operator()(int x){
		while(x-- >0){
			cout<<x<<endl;
		}
	}
}; */
// 4. Non-static member function
/* class Base{
public:
	void run(int x){
		while(x-->0){
			cout<<x<<endl;
		}
	}
}; */

// 5 static member fucntion
/* class Base{
public:
	static void run(int x){
		while(x-- > 0){
			cout<<x<<endl;
		}
	}
}; */

void run(int count){
	while(count-->0){
		cout<<"Cpp"<<endl;
	}
	//this_thread::sleep_for(chrono::seconds(3));
	cout<<"thread finsihed"<<endl;
}
// mutex
int myAmount =0;
mutex m;
void addMoney(){
	m.lock();
	++myAmount; // critical section 
	m.unlock();
}
//try_lock
int counter=0;
mutex m1;
void increaseTheCounter() {
	for(int i=0;i<100000;i++){
		if(m1.try_lock()){
			counter++;
			m1.unlock();
		}
	}
}
//try_lock 
mutex m2,m3;
int X=0,Y=0;
void doSomeWorkForSeconds(int seconds) { this_thread::sleep_for(std::chrono::seconds(seconds)); }
void incrementXY(int& XorY, mutex& m, const char* desc) {
    for(int i=0; i<5; ++i){
        m.lock();
            ++XorY;
        cout << desc << XorY << '\n';
        m.unlock();
        doSomeWorkForSeconds(1);
    }
} // while this thread sleep , consumeXY will fill the bucket 
void consumeXY () {
    int useCount = 5;
    int XplusY = 0;
    while(1){
        int lockResult = try_lock(m1,m2);
        if(lockResult == -1){ // lock successful
            if(X!=0 && Y!=0){
                --useCount;
                XplusY+=X+Y; // bucket example
                X = 0;
                Y = 0;
                cout << "XplusY " << XplusY << '\n';
            }
            m1.unlock();
            m2.unlock();
            if(useCount == 0) break;
        }
    }
}

// * Timed Mutex 
int myAmount2=0;
timed_mutex m4;
void increment(int i) {
    if(m4.try_lock_for(std::chrono::seconds(2))){  // wait for unlock
        ++myAmount2;
        this_thread::sleep_for (seconds(1));
        cout << "Thread " << i << " Entered" << endl;
        m4.unlock();
    }else{
        cout << "Thread " << i << " Couldn't Enter" << endl;
    }
}

// Recursive Mutex
int buffer=0;
recursive_mutex m5;
void recursion(char c,int loopFor){
	if(loopFor<0){
		return ;
	}
	m5.lock();
	cout<<"ThreadID: "<<c<<" "<<buffer++<<endl;
	recursion(c,--loopFor);
	m5.unlock();
	cout<<"Unlocked by thread "<<c<<endl;
}

// * lock guard  
mutex m6;
int buffer1=0;
void task(const char* threadNumber, int loopFor){
	lock_guard<mutex>lock(m6);
	for(int i=0;i<loopFor;i++){
		buffer1++;
		cout<<threadNumber<<buffer1<<endl;
	}
}

// * unique lock
mutex m7;
int buffer2=0;
void task1(const char* threadNumber, int loopFor){
	unique_lock<mutex>lock(m7,defer_lock); // does not call lock on mutex m7
	for(int i=0;i<loopFor;i++){
		buffer2++;
		cout<<threadNumber<<buffer2<<endl;
	}
}

//* conditional variable
mutex m8;
long balance =0 ;
condition_variable cv;
void addMoney1(int money){
	lock_guard<mutex> lg(m8);
	balance+=money;
	cout<<"Amount added current balance"<<balance<<endl;
	cv.notify_one();
}

void withdrawMoney(int money){
	unique_lock<mutex> ul(m8); // waiting here , sleeping thread
	cv.wait(ul,[]{ return (balance!=0)? true:false; }); // wait releases the mutex , when condition is met , the mutex is reacquired
	if(balance >= money){
		balance-=money;
		cout<<"Amount deducted : "<<money<<endl;
	}else{
		cout<<"Amount cannot be detucted ,Current Balance is less than "<<money<<endl;
	}
	cout<<"Current Balance is: "<<balance<<endl;
}


//* Thread synchronization
// ! Only one thread should access the critical section 
mutex m9;
int bankBalance=0;
void addMoney2(int money){
	m9.lock();
	bankBalance+=money; // critical section
	m9.unlock();
}

//* Promise and Future 
void findOddSum(promise<ull> && OddSumPromise,ull start,ull end){
	ull Oddsum=0;
	for(ull i=0;i<=end;i++){
		if(i&1){
			Oddsum+=i;
		}
	}
	OddSumPromise.set_value(Oddsum);
}

// * async 
//? return the values
ull findOdd2(ull start,ull end){
	ull res=0;
	for(ull i=0;i<=end;i++){
		if(i&1){
			res+=i;
		}
	}
	return res;
}

// * producer and consumer problem
mutex mu;
const unsigned int maxBufferSize = 5;
deque<int> bufferqueue;
condition_variable cond;
bool done = false; // Flag to indicate when producer is done

void producer(int val) {
    while (val) {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, []() { return bufferqueue.size() < maxBufferSize; });
        bufferqueue.push_back(val);
        cout << "Produced: " << val << endl;
        val--;
        locker.unlock();
        cond.notify_one();
    }
    // Indicate that production is done
    {
        lock_guard<mutex> locker(mu);
        done = true;
    }
    cond.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, []() { return !bufferqueue.empty() || done; });
        if (!bufferqueue.empty()) {
            int val = bufferqueue.back();
            bufferqueue.pop_back();
            cout << "Consumed: " << val << endl;
        } else if (done) {
            break; // Exit loop if producer is done and buffer is empty
        }
        locker.unlock();
        cond.notify_one();
    }
}





void solve()
{
	ull start=0,end=1900000000;
	auto startTime = high_resolution_clock::now();
	thread t1(findEven,start,end);
	thread t2(findOdd,start,end);
	t1.join(); 
	t2.join();
	// findOdd(start,end);
	// findEven(start,end);
	auto stopTime = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stopTime-startTime);
	cout<<"Oddsum:"<<oddSum<<endl;
	cout<<"Evensum:"<<evenSum<<endl;
	cout<<"Sec:"<<duration.count()/1000000<<endl;
	cout<<endl;

	//Function pointers
	thread t3(fun,3);
	t3.join();


	// Lamda function
	// thread t4([](int x){
	// 	while(x-- > 0){
	// 		cout<<x<<endl;
	// 	}
	// },1);
	// t4.join();

	// functor (function object)
	// thread t5((Base()),10);
	// t5.join();

	// 4. Non-static member function
	// Base b;
	// thread t6(&Base::run,&b,1);
	// t6.join();

	//5. static member function
	// thread t7(&Base::run,1);
	// t7.join();
	

	thread t11(run,5);
	cout<<"main()"<<endl;
	t11.join();
	// Double join will result into program termination.
	// terminate called after throwing an instance of 'std::system_error'
	if(t11.joinable()){
		t11.join();
	}
	if(t11.joinable()){
		t11.detach();
	}
	// Always check before detaching a thread that it is joinable otherwise we may end up double detaching and double detach() will result into program termination.
	cout<<"main() after"<<endl;
	//this_thread::sleep_for(chrono::seconds(3));
	// if waited ,thread will run 

	cout<<"\n\n\n";

	// Mutual Exclusion 
	//Race condition is a situation where two or more threads/process happens to change a common data at the same time.
	cout<<"          Mutex \n";

    thread t12(addMoney);
	thread t13(addMoney);
	t12.join();
	t13.join();
	cout<<myAmount<<endl;

    cout<<endl<<"            Mutex trylock"<<endl;
	// Mutex trylock

	thread t14(increaseTheCounter);
	thread t15(increaseTheCounter);

	t14.join(); 
	t15.join();
	cout<<"Counter could increase upto"<<counter<<endl;

	// * gives  different answer everytime because t14 will be busy incrementing the counter and while incrementing the lock may get thransferred to t15 as the function unlocks it and busy in incrementing i

	thread t16(incrementXY, ref(X), ref(m2), "X ");
    thread t17(incrementXY, ref(Y), ref(m3), "Y ");
    thread t18(consumeXY);
    t16.join();
    t17.join();
    t18.join();
	
	cout<<endl<<"           Timed Mutex \n\n";
	thread t19(increment,1);
	thread t20(increment,2);
	t19.join(); 
	t20.join();
	cout<<myAmount2<<endl;

	cout<<"            Recursive Mutex"<<endl;
	thread t21(recursion,'1',3);
	thread t22(recursion,'2',3);
	t21.join();
	t22.join();

	// * lock guard 
	cout<<"\n\n       Lock guard \n\n";
	thread t23(task,"T1 ",10);
	thread t24(task,"T2 ",10);
	t23.join();
	t24.join();

	//* unique lock (defer_lock , try_to_lock , adopt_lock)
    cout<<"\n\n       Unique lock \n\n";
	thread t25(task1,"T1 ",10);
	thread t26(task1,"T2 ",10);
	t25.join();
	t26.join();

	// * conditional variable 
	// ? used for synchronization of threads 
	cout<<"\n\n         Conditional variable \n\n";
	thread t27(withdrawMoney,500); // ! 600 --> Amount insufficient  
	thread t28(addMoney1,500); 
	t27.join();
	t28.join();

	// * Deadlock
	// ? Give me , I give you back
	// *maintain the order of lock and unlock because if you change the order both thread lock each other requirements , leaving infinite run

	// * Thread Synchronization 
    // ? very very rarely both thread may change value at the same time resulting in bank balance 200 or 100 .
	cout<<"\n\n            Thread Synchronization\n\n";
	thread t29(addMoney2,100);
	thread t30(addMoney2,200); 
	t29.join();
	t30.join();
	cout<<"Final bank balance : "<<bankBalance<<endl;

	//* Promise and Future
	cout<<"\n\n            Promise and Future\n\n";
	promise<ull> Oddsum;
	future<ull>Oddfuture = Oddsum.get_future();
	cout<<"Thread created "<<endl;
	thread t31(findOddSum,move(Oddsum),start,end);
	cout<<"Waiting for result"<<endl;
	cout<<"Oddsum: "<<Oddfuture.get()<<endl;
	cout<<"Completed"<<endl;
	t31.join();

	// * async
    cout<<"\n\n            Async\n\n";
	cout<<"ThreadID"<<this_thread::get_id()<<endl;
	future<ull> Oddsum2 = async(launch::deferred,findOdd2,start,end); // launch ::async means new thread will be created , deferred will not create a new thread
	cout<<"Waiting for result"<<endl;
	cout<<"Oddsum: "<<Oddsum2.get()<<endl;
	cout<<"Completed"<<endl;

	// * Producer consumer 
	cout<<"\n\n            Producer consumer\n\n";
	thread t32(producer,10);
	thread t33(consumer);
	t32.join();
	t33.join();
   

}


// <--------------------I/O------------->

int main(){
  ios::sync_with_stdio(0);
  cin.tie(0);
  srand(chrono::high_resolution_clock::now().time_since_epoch().count());
#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
#endif
  int t=1;
  //cin>>t;
  while(t--){
    solve();
  }
  return 0;
}