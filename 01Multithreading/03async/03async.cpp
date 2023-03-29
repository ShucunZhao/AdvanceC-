#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>

using namespace std;

class B{
public:
	int myThread3(char In){
		cout<<"This is third thread... thread id is:"<<std::this_thread::get_id()<<endl;
		cout<<"The input of thread3 is: "<<In<<endl;
		cout<<"The third thread end... thread id is:"<<std::this_thread::get_id()<<endl;
	}
};

class A{
public:
	int myThread2(char In){
		cout<<"This is second thread... thread id is:"<<std::this_thread::get_id()<<endl;
		cout<<"The input of thread2 is: "<<In<<endl;
		cout<<"The second thread end... thread id is:"<<std::this_thread::get_id()<<endl;
	}

};

int myThread(){
	cout<<"myThread() start... thread id is: "<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(5000);//Wait 5s.
	std::this_thread::sleep_for(dura);
	cout<<"myThread() end... thread id is: "<<std::this_thread::get_id()<<endl;
	return 5;
}

int main(){
	/*
	  Use std::async and std::future create a background task and return a result:
	  std::async is a template for launching a asynchronous task and return a std::future obj.
	  Launch a async task means create a thread and start to run corresponding entry function automatically, it return a std::future obje.
	  This return result obj future of thread include the result of that thread, we can run the member func of future get() to obtain the result of thread which can omit the result of thread to return a global variable.
	  "future": offer a method to access(get) the async operation result in advance.(Which need't to wait for future to get.)
	  ----------------------------------------------------
	  We can pass extra parameters to std::async() whose type is launch for some special purpose:
	  a)std::launch::deferred: defer the entry function calling time to the point where the wait() or get() function of std::future is invoked. If the wait() or get() didn't use this deferral thread won't run.
	  b) std::launch::async: will create thread(has own thread id) when calling async() function.(And this is the default parameter of std::async()).
	*/
	A a;
	B b;
	char tmp1 = 'A';
	char tmp2 = 'B';
	cout<<"main start... thread id is: "<<std::this_thread::get_id()<<endl;
	std::future<int> result = std::async(myThread);
	std::future<int> result2 = std::async(std::launch::deferred, &A::myThread2, &a, tmp1);
	std::future<int> result3 = std::async(std::launch::async, &B::myThread3, &b, tmp2);
	cout<<"Something continue..."<<endl;
	cout<<"Something continue..."<<endl;
	cout<<"Something continue..."<<endl;
	cout<<"Something continue..."<<endl;
	cout<<"Something continue..."<<endl;
	int def;
	def = 0;
	cout<<result.get()<<endl;//This get function will wait for thread finish to get the return result(Stuck here.)
	cout<<"main end... thread id is: "<<std::this_thread::get_id()<<endl;
	cout<<result2.get()<<endl;
	cout<<result3.get()<<endl;
	return 0;
}
