#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
//#include <atomic>

using namespace std;

/*
int myThread(int In){
	cout<<In<<endl;
	cout<<"myThread() start... thread id is: "<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(2000);//Wait 5s.
	std::this_thread::sleep_for(dura);
	cout<<"myThread() end... thread id is: "<<std::this_thread::get_id()<<endl;
	return 5;
}
*/

//We can verify the statement below by this global value:
int mycout = 0;
std::atomic<int> mycout2(0);//Use atomic type instead.
//std::mutex myMutex;


void mythread(){
	for(int i=0;i<10000000;i++){
		//myMutex.lock();//If add the lock the result will be back to normal.
		mycout++;
		mycout2++;//It's a atomic type so that it won't be disturb.
		//myMutex.unlock();
	}
	return;
}


int main(){
	/*
		std::atomic: Use for encapsulating one type of variable.
		1.mutex - use for protecting share data in multithreading:
		lock --> operate share data --> unlock
		2.There are two threads, one is read another is write:
		//1)Read thread:	
		int tmp = atom//This is data that need to share with multiple threads
		//2)Write thread:
		atom = 6;
		In real process it will read some tmporary value because the operation system need to deal with this step by multiple step.
		We can treat atom operation as the method to achieve concurrency without mutex locker, it's a part that would not be disturb or break. Generally, the mutex is used for a segment code but atom operation is used for only one variable.
		Why it's called atom operation is because it's a indivisible operation which will only didn't operation or finished status.
	*/
	std::thread myObj1(mythread);  
	std::thread myObj2(mythread);  
	myObj1.join();
	myObj2.join();
	cout<<"Two thread finished, the final result of mycout is:"<<mycout<<endl;//It should be 20000000 here but got the wrong answer because it's not safe to read and write at the same time.
	cout<<"Two thread finished, the final result of mycout is:"<<mycout2<<endl;
	return 0;
}
