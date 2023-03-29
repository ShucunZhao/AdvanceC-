#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>

using namespace std;

	/*
		std::shared_future:
		If use get() function within the template std::future class the data
		if future will transfer to another place after using, so cannot use get in other thread again.
		But std::shared_future will copy the data rather than transfer, so other place can also use get() to get the data.

	 */

int myThread(int In){
	cout<<In<<endl;
	cout<<"myThread() start... thread id is: "<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(2000);//Wait 5s.
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
	bool isValid = result.valid();
	cout<<"isValid: "<<isValid<<endl;
	//cout<<"result: "<<result.get()<<endl;
	std::shared_future<int> result_share(std::move(result));//After this line the result will be empty().
	isValid = result.valid();
	cout<<"isValid: "<<isValid<<endl;
	cout<<"result_share: "<<result_share.get()<<endl;
	auto test = result_share.get();
	cout<<"test: "<<test<<endl;//And we can get again here.
	cout<<"main end: thread id is "<<std::this_thread::get_id()<<endl;
	return 0;
}
