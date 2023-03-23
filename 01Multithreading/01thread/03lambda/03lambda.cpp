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
	auto myLambdathread = []{
		std::cout<<"My third thread begin..."<<std::endl;
		std::cout<<"My third thread end..."<<std::endl;
	};
	std::thread t3(myLambdathread);
	t3.join();
	return 0;
}
