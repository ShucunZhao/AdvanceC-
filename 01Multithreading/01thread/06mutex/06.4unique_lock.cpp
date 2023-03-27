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
		Use unique_lock to replace lock_guard:
		(Generally use lock_guard is enough)
		
	*/
	std::mutex myMutex1;//Locker1
	std::mutex myMutex2;//Locker2

public:
	void addMsg(){
		for(int i=0;i<100000;i++){
			cout<<"Run addMsg() to insert an element."<<i<<endl;
			std::unique_lock<std::mutex> ul1(myMutex1);
			//std::lock(myMutex1, myMutex2);
			msgRecvQ.push_back(i);
			//myMutex1.unlock();
			//myMutex2.unlock();
		}
		return;
	}
	
	bool protectGet(int & cmd){
		//myMutex.lock();
		//std::lock(myMutex1, myMutex2);
		//Also can ues lock_guard to help us unlock automatically to
		//replace the mutiple unlock operations, remember to add a 
		//adopt_lock parameter.
		std::unique_lock<std::mutex> ul2(myMutex2);
		//std::lock_guard<std::mutex> g2(myMutex2, adopt_lock);
		if(!msgRecvQ.empty()){
			cmd = msgRecvQ.front();
			msgRecvQ.pop_front();
			//myMutex.unlock();
			//myMutex1.unlock();
			//myMutex2.unlock();
			return true;
		}
		//myMutex.unlock();
		//myMutex1.unlock();
		//myMutex2.unlock();
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
