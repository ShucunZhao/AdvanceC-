#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>

/*
   std::condition_viriable:
   	It's a class which wait for one condition to meet, need to cooperate with mutex.
*/

using namespace std;

class msgOpt{
private:
	std::list<int> msgRecvQ;//Sotre the commands sent by player.
	std::mutex myMutex;//Create a mutex: lock() and unlock().
	std::condition_variable myCon;
public:
	void addMsg(){
		for(int i=0;i<100000;i++){
			cout<<"Run addMsg() to insert an element."<<i<<endl;
			std::unique_lock<std::mutex> Uni1();
			msgRecvQ.push_back(i);
			//myCon.notify_one();//We wake up the wait() thread here: after this the wait() in getMsg will work again(the stuck will pass)
			myCon.notify_all();
		}
	}
	/*	
	bool protectGet(int & cmd){
		myMutex.lock();
		if(!msgRecvQ.empty()){
			cmd = msgRecvQ.front();
			msgRecvQ.pop_front();
			myMutex.unlock();
			return true;
		}
		myMutex.unlock();
		return false;
	}
	*/
	void getMsg(){
		int cmd = 0;
		while(1){
			std::unique_lock<std::mutex> Uni1(myMutex);
			myCon.wait(Uni1, [this](){//One lambda expression is a obj(func)
				if(!msgRecvQ.empty()){
					return true;
				}
				return false;
			});//wait() is wait the return value from second parameter which is lambda here:
				//If return value is false: then wait() will unlock the mutex and stuck here until other thread call the notify_one() member func.
			//If return value is true this wait() will return and continue to run the following code.
			//And if wait() has not second parameter that only wait(Uni1): then then has the same effect as lambda return false above.
			//When other thread use notify_one() wake up this wait(), at this time the wait() will recover to work:
				//a)wait() will try to get mutex repeately, if cannot get mutex will also stuck, if get mutex successful will run b).
			    //b): b.1)if wait has second parameter(lambda), if lambda is false, wait() will unlock mutex again and stuck.
					//b.2)if lambda return true the wait() return and continue to go following code(The mutex is lock at this time).
				  //b.3)if has no second parameter, wait() return and continue...(Different from at first).
			cmd = msgRecvQ.front();
			msgRecvQ.pop_front();
			cout<<"getMsg run to pop one element.Thread id is:"<<std::this_thread::get_id()<<endl;
			Uni1.unlock();	
		}//end while

		/*
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
		*/
	}
};

int main(){
	msgOpt A;
	std::thread myGet(&msgOpt::getMsg, &A);//Use reference here to make sure the thread is using the same object in main thread.
	std::thread myAdd(&msgOpt::addMsg, &A);
	std::thread myGet2(&msgOpt::getMsg, &A);
	myGet.join();
	myAdd.join();
	myGet2.join();
	return 0;
}
