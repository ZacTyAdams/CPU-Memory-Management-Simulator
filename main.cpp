using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "mem.h"
#include "generator.h"

int main(){

	vector<processTuple*> processList = generateProcesses();

	//main loop
	while(1){

		//handle meta-functionality for project, execution is split into project portions
		char input[5];
		cout << "Which portion of the project would you like to run? [1-3, -1 to quit]" << endl;
		cin >> input;
		int selection = atoi(input);

		//check for user quitting
		if(selection == -1) break;
		//project portion 1
		else if(selection == 1){
			//declare variables for part 1
			int cyclesElapsed = 0;
			vector<processTuple*> executionList;
			int processCntr = 0, processesCompleted = 0;
			//declare clock variable to take note of when execution began
			clock_t start = clock();
			//main portion 1 execution loop - simulate cycles passing
			while(1){
				if(processesCompleted == processList.size()) break;
				//introduce new process
				if(cyclesElapsed % 50 == 0 && processCntr < processList.size()){
					cout << "New Process: P" << processList[processCntr]->getPid() << ": Cycles = " << processList[processCntr]->getTime() << " Mem = " << processList[processCntr]->getMem() << endl; 
					//copy process data over to new queue so we can re-use original
					processTuple* tempProcess = new processTuple(processList[processCntr]->getPid(), processList[processCntr]->getTime(), processList[processCntr]->getMem());

					//use process memory pointer to point to a chunk allocated by malloc,
					//based on the processes' memory parameter
					cout << "Calling 'malloc(" << tempProcess->getMem() << ")' for P" << processList[processCntr]->getPid() << endl;
					tempProcess->memPtr = malloc(tempProcess->getMem());
					tempProcess->setMStatus(1);

					//add tempProcess to list of processes that have been allocated mem and are running
					executionList.push_back(tempProcess);

					//prepare for next process
					processCntr++;
				}
				//update running and allocated process cycle values
				if(executionList.size() > 0){
					int j = 0;
					//decrease all running process cycle counts by 1
					for(j = 0; j < executionList.size(); j++){
						//if cycles are complete...
						if(executionList[j]->getTime() == 0 && executionList[j]->getMStatus() == true){
							cout << "P" << executionList[j]->getPid() << " completed execution. Calling free() on memory pointer." << endl;
							//call free on the processes memory pointer
							free(executionList[j]->memPtr);
							//update processes memory allocation status
							executionList[j]->setMStatus(0);
							//update counter of completed processes
							processesCompleted++;
						}
						//decrease cycle count by 1
						executionList[j]->decreaseCycle();
					}
				}
				cyclesElapsed++;
			}
			//once we're done executing, take note of time again
			clock_t finish = clock();
			//compare two timestamps to calculate amount of time elapsed during execution
			double executionTime = double(finish - start) / CLOCKS_PER_SEC;
			cout << "All processes have completed execution and freed memory pointers." << endl;
			cout << "The total time taken to do so was " << executionTime << " seconds." << endl << endl;
		}
		else if(selection == 2){
			//initialize a chunk of memory to use as our custom memory pool
			char* space = (char*)malloc(10000000);
			//initialize memory functions to use the space we just set up
			initMemPool(space, 10000000);
			//declare variables for part 2
			int cyclesElapsed = 0;
			vector<processTuple*> executionList;
			int processCntr = 0, processesCompleted = 0;
			//declare clock variable to take note of when execution began
			clock_t start = clock();
			clock_t finish;
			//main portion 1 execution loop - simulate cycles passing
			while(1){
				if(processesCompleted == processList.size()){
					//once we're done executing, take note of time again
					finish = clock();
					break;
				}
				//introduce new process
				if(cyclesElapsed % 50 == 0 && processCntr < processList.size()){
					cout << "New Process: P" << processList[processCntr]->getPid() << ": Cycles = " << processList[processCntr]->getTime() << " Mem = " << processList[processCntr]->getMem() << endl; 
					//copy process data over to new queue so we can re-use original
					processTuple* tempProcess = new processTuple(processList[processCntr]->getPid(), processList[processCntr]->getTime(), processList[processCntr]->getMem());

					//use process memory pointer to point to a chunk allocated by malloc,
					//based on the processes' memory parameter
					cout << "Calling 'my_malloc(" << tempProcess->getMem() << ")' for P" << processList[processCntr]->getPid() << endl;
					//since for this part of the project, we're safe to assume the combined memory load will never
					//surpass our 10mb limit, I've add the division by 10 to ensure I don't over-allocate
					//Portion 3 is there to deal with this.
					tempProcess->memPtr = my_malloc(tempProcess->getMem()/10);
					tempProcess->setMStatus(1);

					//add tempProcess to list of processes that have been allocated mem and are running
					executionList.push_back(tempProcess);

					//prepare for next process
					processCntr++;
				}
				//update running and allocated process cycle values
				if(executionList.size() > 0){
					int j = 0;
					//decrease all running process cycle counts by 1
					for(j = 0; j < executionList.size(); j++){
						//if cycles are complete...
						if(executionList[j]->getTime() == 0 && executionList[j]->getMStatus() == true){
							cout << "P" << executionList[j]->getPid() << " completed execution. Calling my_free() on memory pointer." << endl;
							//call free on the processes memory pointer
							my_free(executionList[j]->memPtr);
							//update processes memory allocation status
							executionList[j]->setMStatus(0);
							//update counter of completed processes
							processesCompleted++;
						}
						//decrease cycle count by 1
						executionList[j]->decreaseCycle();
					}
				}
				cyclesElapsed++;
			}
			//compare two timestamps to calculate amount of time elapsed during execution
			double executionTime = double(finish - start) / CLOCKS_PER_SEC;
			cout << "All processes have completed execution and freed memory pointers." << endl;
			cout << "The total time taken to do so was " << executionTime << " seconds." << endl << endl;
		}
		//project portion 3
		else if(selection == 3){
			//new round of i/o to determing max memory in example
			cout << "Max memory at 50% or 10%? [50/10]" << endl;
			int input2 = 0;
			cin >> input2;
			int preTotal = 0;
			int h = 0;
			//gather total mem needed by all generated processes
			for(h=0;h<processList.size();h++){
				preTotal += processList[h]->getMem();
			}
			//based on the input received, either make max mem 10 or 50 percent of the total
			int memTotal;
			int memUsed = 0;
			//half of total
			if(input2 == 50){
				memTotal = (preTotal / 2) / 10;
			}
			//10 percent of total
			else if(input2 == 10){
				memTotal = (preTotal / 10) / 10;
			}
			else{
				cout << "Input was not recognized. Aborting." << endl;
				continue;
			}
			//initialize a chunk of memory to use as our custom memory pool
			char* space = (char*)malloc(memTotal);
			//initialize memory functions to use the space we just set up
			initMemPool(space, memTotal);
			//declare variables for part 2
			int cyclesElapsed = 0;
			vector<processTuple*> executionList;
			vector<processTuple*> waitList;
			int processCntr = 0, processesCompleted = 0;
			//declare clock variable to take note of when execution began
			clock_t start = clock();
			clock_t finish;
			//main portion 1 execution loop - simulate cycles passing
			while(1){
				if(processesCompleted == processList.size()){
					//once we're done executing, take note of time again
					finish = clock();
					break;
				}
				//introduce new process
				if(cyclesElapsed % 50 == 0 && processCntr < processList.size()){
					cout << "New Process: P" << processList[processCntr]->getPid() << ": Cycles = " << processList[processCntr]->getTime() << " Mem = " << processList[processCntr]->getMem()/10 << endl; 
					//copy process data over to new queue so we can re-use original
					processTuple* tempProcess = new processTuple(processList[processCntr]->getPid(), processList[processCntr]->getTime(), processList[processCntr]->getMem());

					if((tempProcess->getMem()/10 + memUsed) < memTotal){
						cout << "Calling 'my_malloc(" << tempProcess->getMem()/10 << ")' for P" << processList[processCntr]->getPid() << endl;
						//add tempProcess to list of processes that have been allocated mem and are running
						executionList.push_back(tempProcess);
						memUsed += tempProcess->getMem()/10;
						//using real memory values for this portion of the project
						tempProcess->memPtr = my_malloc(tempProcess->getMem()/10);
						tempProcess->setMStatus(1);
					}
					else{
						cout << "Adding P" << processList[processCntr]->getPid() << " to wait list due to memory shortage." << endl;
						//add tempProcess to list of processes that have been allocated mem and are running
						waitList.push_back(tempProcess);
						//processesCompleted++;
					}
					//prepare for next process
					processCntr++;
				}
				//update running and allocated process cycle values
				if(executionList.size() > 0){
					int j = 0;
					//decrease all running process cycle counts by 1
					for(j = 0; j < executionList.size(); j++){
						//if cycles are complete...
						if(executionList[j]->getTime() == 0 && executionList[j]->getMStatus() == true){
							cout << "P" << executionList[j]->getPid() << " completed execution. Calling my_free() on memory pointer." << endl;
							//call free on the processes memory pointer
							if(executionList[j]->memPtr != NULL)
								my_free(executionList[j]->memPtr);
							//update processes memory allocation status
							executionList[j]->setMStatus(0);
							//update memory tracker
							memUsed -= executionList[j]->getMem()/10;
							//update counter of completed processes
							processesCompleted++;
						}
						//decrease cycle count by 1
						executionList[j]->decreaseCycle();
					}
				}
				//handle wait list 
				if(waitList.size() > 0 && memUsed < ((memTotal / 4) * 3)){
					int k = 0;
					for(k = 0; k < waitList.size(); k++){
						if((waitList[k]->getMem()/10 + memUsed) < memTotal){
							processTuple* tempProcess = new processTuple(waitList[k]->getPid(), waitList[k]->getTime(), waitList[k]->getMem());
							cout << "Calling 'my_malloc(" << tempProcess->getMem()/10 << ")' for P" << tempProcess->getPid() << endl;
							memUsed += tempProcess->getMem()/10;
							//using real memory values for this portion of the project
							tempProcess->memPtr = my_malloc(tempProcess->getMem()/10);
							tempProcess->setMStatus(1);
							//add tempProcess to list of processes that have been allocated mem and are running
							executionList.push_back(tempProcess);
							waitList.erase(waitList.begin() + (k));
						}
					}
				}
				cyclesElapsed++;
			}
			//compare two timestamps to calculate amount of time elapsed during execution
			double executionTime = double(finish - start) / CLOCKS_PER_SEC;
			cout << "All processes have completed execution and freed memory pointers." << endl;
			cout << "The total time taken to do so was " << executionTime << " seconds." << endl << endl;
		}
		else{
			cout << "Unrecognized input." << endl;
		}
	}	
	return 0;
}
