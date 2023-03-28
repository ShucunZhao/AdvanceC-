#include<cstdlib>
#include<iostream>
#include<thread>
#include<mutex>

using namespace std;
/*
   Sigleton class data share:
   1.Create obj in main:
   		Before creating other threads and other thread only read the data in singleton obj.
	2.If we create the singleton obj in other thread:
		We need to mutex to protect create at the same time.
	3.We can use call_once to make one func only be called once even if in multiple threads, and it's more effective, need to cooperate with std::once_flag.
 */

//std::mutex resMutex;
std::once_flag ifCalled;

class MySig{

	static void createInstanceOnce(){
			myInstance = new MySig();
			static GC gc;
	}

private:
	MySig(){}

private:
	static MySig * myInstance;//Static member variable.
	int val = 99;
public:
	static MySig * getInstance(){
		/*
		//if(myInstance==NULL){
			//std::unique_lock<std::mutex> myMutex(resMutex);
		if(myInstance==NULL){
			myInstance = new MySig();
			static GC gc;
		}
		//}
		*/
		std::call_once(ifCalled, createInstanceOnce);//Work like mutex locker here.
		return myInstance;
	}
	void func(){
		cout<<"Testing."<<endl;
	}
	void changeVal(int In){
		val = In;
	}
	void printVal(){
		cout<<"val:"<<val<<endl;
	}
	class GC{
	public:
		~GC(){
			if(MySig::myInstance!=NULL){
				delete myInstance;
				myInstance = NULL;
			}
		}
	};
};

MySig * MySig::myInstance = NULL;//Initialize the static member var.

//Thread entry func:
void mythread(){
	cout<<"My thread start..."<<endl;
	MySig * ptrA = MySig::getInstance();//Maybe something wrong here.
	cout<<"My thread end..."<<endl;
	return;
}

int main(){
	std::thread	T1(mythread);//Though these two threads use the same entry but they are two different thread, so it will have two path to execute the getInstance(), which will cause the address of MySig will change mutiple times. 
	std::thread T2(mythread);
	T1.join();
	T2.join();
	return 0;
}
