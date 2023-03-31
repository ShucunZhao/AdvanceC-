#include <chrono>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <list>

using namespace std;

std::mutex myMutex;

class A{
private:
	List * Msg;
public:
	void entry(){
		for(int i=0;i<10000;i++){
			std::chrono::milliseconds timeout(100);
			if(myMutex.try_lock_for(timeout)){//Wait for 100ms to try to get the locker.
			//if(myMutex.try_lock_until(chrono::stead_clock::now()+timeout)){//try_lock_until usage.
				//if got the locker successfully this if judgement will meet.
				Msg.push_back(i);
				myMutex.unlock();
			}
			else{//When didn't get the locker:
				cout<<"Didn't get the locker."<<endl;
				std::chrono::microseconds sleep(100);
				std::this_thread::sleep_for(sleep);
			}
		}
	}
};

/*
   std::timed_mutex:
   	It's the mutex which has the time out function:
		Supply two others api:
		1.try_lock_for(): wait for a while - if get the locker or wait for more than a while, the process will go on, the parameter of this function is a period of time.
		2.try_lock_unti(): the parameter is a certain time in future, if the time didn't reach got the locker or reach the time but didn't get the locker the program also continue to run.
*/

int main(){

	return 0;
}
