#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>

using namespace std;

void myThread(std::promise<int> & tmp, int tar){
	tar++;
	tar*=10;
	std::chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	int rst = tar;
	tmp.set_value(rst);//Save result to tmp.
}

void myThread2(std::future<int> & tmp){	
	auto result = tmp.get();
	cout<<"myThread2 result = "<<result<<endl;

}

int main(){
	/*
		std::promise:
		we can assign value to it in a thread and get this value in other thread.
	 */
	std::promise<int> myprom;
	std::thread t1(myThread, std::ref(myprom), 180);
	t1.join();

	//Get the reuslt:
	std::future<int> fu1 = myprom.get_future();
	//auto result = fu1.get();
	//cout<<"result = "<<result<<endl;
	
	std::thread t2(myThread2, std::ref(fu1));
	t2.join();

	cout<<"main end: thread id is "<<std::this_thread::get_id()<<endl;
	return 0;
}
