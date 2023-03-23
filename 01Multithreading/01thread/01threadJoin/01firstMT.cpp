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

int main(){
	std::map<std::string, int> forecastM = {
		{"New York",15},
		{"Mumbai", 20},
		{"Berlin", 18}
	};
	std::thread bgWorker(RefreshForecast, forecastM);
	for(int i=0;i<5;i++){
		std::cout<<"This is the" <<i<<" time run in main thread."<<std::endl;
	}
	bgWorker.join();
	/*
	//func1();
	//func2();
	std::thread worker1(func1, '*');
	std::thread worker2(func2);
	worker1.join();
	worker2.join();
	std::cout<<std::endl;
	//system("pause>nul");
	*/
	return 0;
}
