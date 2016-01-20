using namespace std;
#include <iostream>
#include <cstring>
#include <random>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <math.h> 
#include <iomanip>  
#include "generator.h"

//constructor for the process class
processTuple::processTuple(int inPid, unsigned long long inCycles, int inMem){
	this->processID = inPid;
	this->cpuCycles = inCycles;
	this->memPrint = inMem;
	this->isMalloced = 0;
}

//function to print the process parameters
void processTuple::printTuple(){
	cout << "Process ID: " << this->processID << "\t  Clock Cycles: " << setprecision(0) << this->cpuCycles << "\tMemory Footprint: " << setprecision(4) << this->memPrint << endl;
}

//pid getter
int processTuple::getPid(){
	return this->processID;
}

//cycles getter
unsigned long long processTuple::getTime() const{
	return this->cpuCycles;
}

//mem getter
int processTuple::getMem() const{
	return this->memPrint;
}

void processTuple::decreaseCycle(){
	this->cpuCycles--;
}

bool processTuple::getMStatus(){
	if (this->isMalloced == 0)
	{
		return false;
	}
	else return true;
}

void processTuple::setMStatus(int input){
	if(input == 1){
		this->isMalloced = 1;		
	}
	else this->isMalloced = 0;
}

//function to generate a list full of random processes, parameters described within
vector<processTuple*> generateProcesses(){
	//variable to determine how many processes are created
	int k = 50, i = 0;

	//use clock namespace to create a seed for random engine based on current time
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	//create random generation engine, seed it with pre-made seed
	default_random_engine randomGen(seed);
	srand(seed);
	//create a normal dist for the memory, mean = 0.5MB, std dev = .25MB
	normal_distribution<double> memDist(500000, 1000000);

	//create a vector based on the process class to hold a list of process elements
	vector<processTuple*> processList;

	//loop by K defined above, use distributions to create random numbers
	//use random numbers to define new process parameters, add process to vector
	while(i<k){
		//using a random distribution to select clock cycles to try and get a more varied selection
		unsigned long long number = rand() % (unsigned long long)(10*pow(10,6)) + (unsigned long long)(10*pow(10,4));
	
		//use distribution to create memory value
		int mem = (int)memDist(randomGen);
		//make sure memory generated is less than or equal to 8, and greater than or equal to 0.00025 (0.25MB)
		if((mem >= 10000) && (mem <= 1000000)){
			//add process to list
			processTuple* temp = new processTuple(i, number, mem);
			processList.push_back(temp);
			i++;
		}
	}

	//return the created list of processes
	return processList;
}

//custom comparator used with vectors to sort processes by their clock cyles
bool timeCmp::operator() ( const processTuple* p1, const processTuple* p2 ) const{
     return p1->getTime() < p2->getTime();
}