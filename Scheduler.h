//Jeremiah Kalmus
//CPSC 3500
//Scheduler Header File

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
using namespace std;

class Scheduler {
	
		
	struct schedule_data
	{
		int process_num;
		int arrival_time;
		int burst;
		int remaining_burst;
	
		void operator=(const schedule_data &a){
			process_num = a.process_num;
			arrival_time = a.arrival_time;
			burst = a.burst;
			remaining_burst = a.remaining_burst;
		}
	}; 
	public:
	
	int file_size(string file_name);
	
	 //If file successfully opens, then the data within is
	 //processed and stored in 
	void file_process(string file_name, int counter, schedule_data *structPtr);
	
	void FCFS(string file_name);
	
	void SRTF(string file_name);
	
	void RR(string file_name, int time_slice);
	
	private:
	//Sorts processes on their arrival time, with the earliest
	//in the front and the latest in the back.
	void arrivalSort(int n, schedule_data *structPtr);
	
	void stats(float, float, float, int);

	
};

#endif