#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <mutex>

using namespace std;

/*
	If we use lock the same mutex twice in one thread, the program will crash. For example, func1 and func2 below.
	So we introduce recursive_mutex here:
	std::mutex: exclusive mutex, when lock others cannot lock.
	recursive_mutex: it allow the same thread lock the mutex multiple times. 
 */

std::mutex myMutex;
std::recursive_mutex recurMutex;//The recursive mutex efficiency is worse than mutex.

class A{
public:
	void func1(){
		//std::lock_guard<std::mutex> myguard(myMutex);
		std::lock_guard<std::recursive_mutex> myguard(recurMutex);
		func2();//If we invoke func2 here the program will crash because lock mutex twice.
	}

	void func2(){
		//std::lock_guard<std::mutex> myguard(myMutex);
		std::lock_guard<std::recursive_mutex> myguard(recurMutex);
		/*.............
		  ............
		  */
	}
};


 

int main(){
	A a;
	std::thread myT(&A::func1, &a);
	myT.join();
	cout<<"This is main thread......"<<endl;
	return 0;
}
