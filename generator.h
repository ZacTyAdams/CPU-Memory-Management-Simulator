#ifndef GENERATOR_GUARD
#define GENERATOR_GUARD

using namespace std;
#include <iostream>
#include <cstring>
#include <random>
#include <chrono>
#include <unistd.h>
#include <vector>

//class declaration for process
class processTuple{
	public:
		processTuple(int, unsigned long long, int);
		void printTuple();
		unsigned long long getTime() const;
		int getMem() const;
		int getPid();
		void decreaseCycle();
		bool getMStatus();
		void setMStatus(int);
		void *memPtr;
	private:
		int processID;
		unsigned long long cpuCycles;
		int memPrint;
		int isMalloced;


};

//generate process list
vector<processTuple*> generateProcesses();

//custom comparator, description in cpp
class timeCmp{
	public:
	   bool operator() ( const processTuple* p1, const processTuple* p2 ) const;
   private:

};

#endif