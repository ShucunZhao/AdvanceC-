#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <list>

/*
   One thread receive msg from player, another read.
   This program will crash because the pushing and poping will
   happen at the same time.
*/

using namespace std;

class msgOpt{
private:
	std::list<int> msgRecvQ;//Sotre the commands sent by player.

public:
	void addMsg(){
		for(int i=0;i<100000;i++){
			cout<<"Run addMsg() to insert an element."<<i<<endl;
			msgRecvQ.push_back(i);
		}
	}
	void getMsg(){
		for(int i=0;i<100000;i++){
			if(!msgRecvQ.empty()){
				int cmd = msgRecvQ.front();
				msgRecvQ.pop_front();
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
