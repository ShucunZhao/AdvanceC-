#include <iostream>
#include <thread>
#include <map>
#include <chrono>

using namespace std::chrono_literals;

void func1(char symbol1){
	for(int i=0;i<200;i++){
		std::cout<<symbol1;
	}
}

void func2(){
	for(int i=0;i<200;i++){
		std::cout<<"-";
	}
}

void RefreshForecast(std::map<std::string, int> forecastM){
	while(1){
		for(auto & item : forecastM){
			item.second++;
			std::cout<<item.first<<" - "<<item.second<<std::endl;
		}	
		std::this_thread::sleep_for(2000ms);
	}
}

class T1{
public:
	void operator()(){
		std::cout<<"Begin func..."<<std::endl;
		std::cout<<"End fun..."<<std::endl;
	}
};

int main(){
	T1 * t1 = new T1();
	std::thread test1(*t1);
	test1.join();
	for(int i=0;i<5;i++){
		std::cout<<"This is the" <<i<<" time run in main thread."<<std::endl;
	}
	return 0;
}
