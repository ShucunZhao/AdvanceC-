#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <list>
#include <mutex>

/*
   One thread receive msg from player, another read.
   This program will crash because the pushing and poping will
   happen at the same time.
*/

using namespace std;

class msgOpt{
private:
	std::list<int> msgRecvQ;//Sotre the commands sent by player.
	/*
		Protection: when operation data we need to lock the it(
		Lock,Handle,Unlock), others want to handle it must wait
		for unlock.

		Mutex: A locker, mitiple threads try to use member func
		lock() to lock but only one thread can be locked successfully,
		if succeeded, lock() func will return, else it will keep trying
		and stuck here.
	*/
	std::mutex myMutex;//Create a mutex: lock() and unlock().
	//lock and unlock occur with pairs.
public:
	void addMsg(){
		for(int i=0;i<100000;i++){
			cout<<"Run addMsg() to insert an element."<<i<<endl;
			myMutex.lock();
			msgRecvQ.push_back(i);
			myMutex.unlock();
		}
	}
	
	bool protectGet(int & cmd){
		std::lock_guard<std::mutex> guard(myMutex);//This class lock_guard
		//write the lock() in constructor and unlock in destrcutor. 
		//myMutex.lock();
		if(!msgRecvQ.empty()){
			cmd = msgRecvQ.front();
			msgRecvQ.pop_front();
			myMutex.unlock();
			return true;
		}
		//myMutex.unlock();
		return false;
	}

	void getMsg(){
		int cmd = 0;
		for(int i=0;i<100000;i++){
			if(protectGet(cmd)){
				cout<<"getMsg() func run to get one element."<<cmd<<endl;
				////Handling msg...///
			}
			else{
				cout<<"The msg queue is empty now!"<<i<<endl;
			}
		}
		cout<<"End of getting msg."<<endl;
	}
};

int main(){
	msgOpt A;
	std::thread myGet(&msgOpt::getMsg, &A);//Use reference here to make sure the thread is using the same object in main thread.
	std::thread myAdd(&msgOpt::addMsg, &A);
	myGet.join();
	myAdd.join();
	return 0;
}
