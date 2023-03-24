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
		Dead lock: this problem happen must need at least tow lockers(mutex)
	 	Tow locker need to wait to each other unlock to lock so occur always waiting situation. 
	 */
	std::mutex myMutex1;//locker1
	std::mutex myMutex2;//locker2
	/*
	   The solution to deal with dead lock problem:
	   1.Keep these locker the same lock and unlock order when use.
	*/
public:
	void addMsg(){
		for(int i=0;i<100000;i++){
			cout<<"Run addMsg() to insert an element."<<i<<endl;
			myMutex1.lock();
			myMutex2.lock();
			msgRecvQ.push_back(i);
			myMutex2.unlock();
			myMutex1.unlock();
		}
	}
	
	bool protectGet(int & cmd){
		myMutex2.lock();//If the second lock go first here will cause dead lock.
		myMutex1.lock();
		myMutex2.lock();
		if(!msgRecvQ.empty()){
			cmd = msgRecvQ.front();
			msgRecvQ.pop_front();
			myMutex1.unlock();
			myMutex2.unlock();
			return true;
		}
		myMutex1.unlock();
		myMutex2.unlock();
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
