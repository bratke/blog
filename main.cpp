#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

const int COUNT = 10;

int main(int argc, char** argv)
{
	atomic_bool f1{false}, f2{false};

	thread t1([&]() {
		for(int i = 0; i < COUNT;)
		{
			f1.store(true, memory_order_relaxed);
			if(f2.load(memory_order_acquire) == false)
			{
				cout << "T1 in critical section" << endl;
				f1.store(false, memory_order_relaxed);
				++i;
			}
			else
			{
				f1.store(false, memory_order_relaxed);
				this_thread::yield();
			}
		}
	});

	thread t2([&]() {
		for(int i = 0; i < COUNT;)
		{
			f2.store(true, memory_order_relaxed);
			if(f1.load(memory_order_acquire) == false)
			{
				cout << "T2 in critical section" << endl;
				f2.store(false, memory_order_relaxed);
				++i;
			}
			else
			{
				f2.store(false, memory_order_relaxed);
				this_thread::yield();
			}
		}
	});

	t1.join();
	t2.join();

	return 1;
}
