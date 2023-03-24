#include <iostream>
#include <thread>
#include <map>
#include <vector>

using namespace std;

void Entry(int num){
	cout<<"Thread id: "<<num<<" begin"<<endl;
	//////////
	cout<<"Thread id: "<<num<<" end"<<endl;
	return;
}

int main(){
	vector<thread> myThread;
	for(int i=0;i<10;i++){
		myThread.push_back(thread(Entry, i));//This ten threads order is mess.
	}
	for(auto it = myThread.begin();it!=myThread.end();++it){
		it->join();
	}
	cout<<"This is main thread......"<<endl;
	return 0;
}
