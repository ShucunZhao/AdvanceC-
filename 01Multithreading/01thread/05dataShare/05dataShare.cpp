#include <iostream>
#include <thread>
#include <map>
#include <vector>

using namespace std;

vector<int> dataRO = {1,2,3};//Read only: it's safe, don't need other protection operation.
vector<int> dataRW = {1,2,3}//Read and write: if you do nothing in code it will cause crash, the easy to protect is not allow to write when reading and not allow to read when writing.

void Entry(int num){
	/*
	cout<<"Thread id: "<<num<<" begin"<<endl;
	//////////
	cout<<"Thread id: "<<num<<" end"<<endl;
	*/
	cout<<"ID: "<<std::this_thread::get_id()<<" , Val:"<<dataRO[0]<<dataRO[1]<<dataRO[2]<<endl;
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
