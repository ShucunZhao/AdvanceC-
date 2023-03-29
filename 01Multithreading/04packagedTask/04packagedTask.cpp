#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>

using namespace std;

int myThread(int In){
	cout<<In<<endl;
	cout<<"myThread() start... thread id is: "<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(5000);//Wait 5s.
	std::this_thread::sleep_for(dura);
	cout<<"myThread() end... thread id is: "<<std::this_thread::get_id()<<endl;
	return 5;
}

int main(){
	/*
		std::packaged_task: Use for packing task.
		It's a template whoes parameters is various objs, can use it to pack them for being the entry function of thread. 
	*/
	cout<<"main begin: thread id is "<<std::this_thread::get_id()<<endl;
	std::packaged_task<int(int)> mypt(myThread);//Use packaged_task to pack up myThread function.
	std::thread t1(std::ref(mypt), 1);
	t1.join();

	std::future<int> result = mypt.get_future();
	cout<<result.get()<<endl;
	cout<<"main end: thread id is "<<std::this_thread::get_id()<<endl;
	return 0;
}
