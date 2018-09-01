//Jeremiah Kalmus
//CPSC 3500
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

#include "Scheduler.h"
using namespace std;

int main(int argc, char* argv[])
{
	int num_tokens = 0;
	int quantum = 0;
	string algorithm = argv[2];
	Scheduler scheduler; //Creates a scheduler object

	//Checks for schedule algorithm 
	if (algorithm == "FCFS" ||
      algorithm == "SRTF")
	{
		//Throws error if # of args does not match
		//with schedule algorithm.
		if (argc != 3) {
			cerr << "Invalid command line: " << argv[2];
			cerr << " has improper number of arguments" << endl;
			return -1;
		}
		else if ((argc == 3) && (algorithm == "FCFS")){
			scheduler.FCFS(argv[1]);
		}
		else{
			scheduler.SRTF(argv[1]);
		}
	}
	//Checks for schedule algorithm 
	else if (algorithm == "RR"){
		//Throws error if # of args does not match
		//with schedule algorithm.
		if (argc != 4){
			cerr << "Invalid command line: " << argv[2];
			cerr << " has improper number of arguments" << endl;
			return -1;
		}
		//Checking each element to see if there are any character values
		else{
			for(unsigned int i = 0;i < strlen(argv[3]);i++){
				if(!isdigit(argv[3][i])){
					cerr << "Invalid time slice" << endl;
					return -1;
				}
			}
			//checks if time slice is greater than 0
			if(atoi(argv[3]) <= 0){
				cerr << "Invalid time slice" << endl;
				return -1;
			}
			quantum = atoi(argv[3]);
			scheduler.RR(argv[1], quantum);
		}
	}
	else if (argc < 3){
		cerr << "The command has too few arguments" << endl;
	}
	else if (argc > 4){
		cerr << "The command has too many arguments" << endl;
	}
	else{
		cerr << algorithm << " is not a valid scheduling algorithm" 
		<< endl;
	}
	
	return 0;
}
