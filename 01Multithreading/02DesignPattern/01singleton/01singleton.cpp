#include<cstdlib>
#include<iostream>

using namespace std;

class MySig{

private:
	MySig(){}

private:
	static MySig * myInstance;//Static member variable.
	int val = 99;
public:
	static MySig * getInstance(){
		if(myInstance==NULL){
			myInstance = new MySig();
			static GC gc;
		}
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

int main(){
	//MySig A;
	//Mysig B;
	MySig * prtA = MySig::getInstance();//Create a object and return the pointer of this obj.
	MySig * prtB = MySig::getInstance();
	prtA->printVal();
	prtA->changeVal(5);
	prtA->printVal();
	prtB->changeVal(100);
	prtA->printVal();
	return 0;
}
