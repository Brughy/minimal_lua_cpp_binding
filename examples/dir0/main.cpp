#include <string>
#include <vector>
#include <iostream>
#include<bits/stdc++.h> 
#include "luabinding.h"
		
int main() {

	c2luabinding script("cfg.lua");
	std::cout<<"## state: "<<script.LUASTATE<<std::endl;
	
	// getting values:
	std::string command = script.get<std::string>("map.command");
	int posX = script.get<int>("map.pos.x");
	std::vector<int> v = script.getVector<int>("array");
	std::vector<std::string> vn = script.getVector<std::string>("names");

	std::cout<<"## command: "<<command<<std::endl;
	std::cout<<"## pos.X: "<<posX<<std::endl;    
	std::cout<<"## Contents of array:";
		for(std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
			std::cout<<*it<<",";
		}
		std::cout<<std::endl;
		
	std::cout<<"## Contents of array names:";
		for(std::vector<std::string>::iterator it = vn.begin(); it != vn.end(); it++) {
			std::cout<<*it<<",";
		}
		std::cout<<std::endl;		
    
    	// getting table keys:
	std::vector<std::string> keys = script.getTableKeys("map");
	std::cout<<"## Keys of [map] table:";
		for(std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++) { 
			std::cout<<*it<<",";
		}
		std::cout<<std::endl;

    	// run function:

	int a = 20;
	std::cout<<"## input: "<<a<<std::endl; 
	std::vector<int> result;
	std::vector<int> input = {20, 30};
	result = script.call<int, int>("add", input, 1);
	for(auto& it : result) { 
		std::cout<<"## result: "<<it<<std::endl;
	}
	result = script.call<int, int>("addN", input, 2);
	for(auto& it : result) { 
		std::cout<<"## result: "<<it<<std::endl;
	}
	
	std::vector<int> input0 = {20};
	result = script.call<int, int>("add", input0, 1);
	for(auto& it : result) { 
		std::cout<<"## result: "<<it<<std::endl;
	}	
	
	std::vector<int> doublei = {12};
	result = script.call<int, int>("double", doublei, 1);
	for(auto& it : result) { 
		std::cout<<"## result: "<<it<<std::endl;
	}		
	return 0;
		
	
}
