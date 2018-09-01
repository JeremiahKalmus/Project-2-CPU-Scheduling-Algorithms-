//Jeremiah Kalmus
//CPSC 3500
//Scheduler Class
#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
#include <vector>
#include <iomanip>


#include "Scheduler.h"
using namespace std;

int Scheduler::file_size(string file_name)
{
	string line;
	int counter = 0;
	
	ifstream infile;
	infile.open(file_name);
	if (infile.fail()) {
		cerr << "Could not open text file" << endl;
		exit(-1);
	}
	//counting how many processes
	while(getline(infile, line)){
		counter++;
	}
	infile.close();
	return counter;
}

void Scheduler::file_process(string file_name, int counter, schedule_data *structPtr)
{
	string line;
	ifstream infile;
	infile.open(file_name);
	if (infile.fail()) {
		cerr << "Could not open text file" << endl;
		exit(-1);
	}
	//inputting file data into appropriate subfields in 
	//an array of structs.
	for(int i = 0;i < counter;i++){
		getline(infile, line);
		istringstream iss(line);
		iss >> structPtr[i].process_num;
		iss >> structPtr[i].arrival_time;
		iss >> structPtr[i].burst;
		structPtr[i].remaining_burst = structPtr[i].burst;
	}
	infile.close();
	return;
}
void Scheduler::FCFS(string file_name)
{
	int counter = file_size(file_name);
	schedule_data *structPtr = new schedule_data[counter]; 
	file_process(file_name, counter, structPtr);
	
	float total_idle = 0;
	float total_wait = 0;
	float sum_arrival = 0;
	float sum_busy = 0;
	float finish = 0;
	int start = 0;
	int end = 0;
	bool same = false;

	arrivalSort(counter, structPtr);
	float clock = 0;
	for(int i = 0;i < counter;i++){
		//if clock has reached arrival time
		if(structPtr[i].arrival_time <= clock){
			if(same == true){
				same = false;
				end = clock;
			}
			sum_arrival = sum_arrival + structPtr[i].arrival_time;
			sum_busy = sum_busy + structPtr[i].burst;
			finish = finish + (clock + structPtr[i].burst);
			cout << "Time " << clock << " Process "
				 << structPtr[i].process_num << endl;
			clock = clock + structPtr[i].burst;
		}
		//idling
		else{
			if(same == false){
				same = true;
				start = clock;
			}
			cout << "Time " << clock << " Idle" << endl;
			clock = structPtr[i].arrival_time;
			i--;
		}
		total_idle = total_idle + (end - start);
		start = 0;
		end = 0;
	}
	cout << sum_arrival << " " << sum_busy << endl;
	total_wait = finish - sum_arrival - sum_busy;
	delete[] structPtr;
	stats(clock, total_idle, total_wait, counter);
}
void Scheduler::SRTF(string file_name)
{
	int counter = file_size(file_name);
	schedule_data *structPtr = new schedule_data[counter]; 
	file_process(file_name, counter, structPtr);
	
	int clock = 0;
	int proc_count = 0; 
	int proc_done = 0;
	float total_busy = 0;
	float sum_arrival = 0;
	float sum_busy = 0;
	float sum_finish = 0;
	float total_idle = 0;
	float total_wait = 0;
	schedule_data on_cpu;

	
	const auto compare = [](const schedule_data lhs, const schedule_data rhs){
		if(lhs.remaining_burst == rhs.remaining_burst){
			return (lhs.process_num > rhs.process_num);
		}
		else{
			return (lhs.remaining_burst > rhs.remaining_burst);
		}
	};
	//Creation of priority queue.
	priority_queue<schedule_data, vector<schedule_data>,
									decltype(compare)> q(compare);
	//sorts based on arrival time
	arrivalSort(counter, structPtr);
	while(proc_done < counter){
		//inputs values into the queue only when the clock has
		//reached a processes arrival time
		while((structPtr[proc_count].arrival_time <= clock) && (proc_count < counter))
		{
			if(q.empty()){
				on_cpu.process_num = -1;
			}
			//used for computing statistics
			total_busy = total_busy + structPtr[proc_count].burst;
			sum_arrival = sum_arrival + structPtr[proc_count].arrival_time;
			sum_busy = sum_busy + structPtr[proc_count].burst;
			//value enters into the queue
			q.push(structPtr[proc_count]);
			proc_count++;
		}

		if(!q.empty()){
			//if process is different than the last one worked on
			if(on_cpu.process_num != q.top().process_num){
				on_cpu = q.top();
				cout << "Time " << clock << " Process "
				 << q.top().process_num << endl;
			}
			//if process is completed
			else if(q.top().remaining_burst == 0){
				sum_finish = sum_finish + clock;
				q.pop();
				proc_done++;
			}
			else{
				on_cpu.remaining_burst--;
				q.pop();
				q.push(on_cpu);
				clock++;
			}
		}
		//idling
		else if(q.empty()){
			//if idling but previous but last clock cycle was not
			if(on_cpu.process_num != -1){
				on_cpu.process_num = -1;
				cout << "Time " << clock << " Idle" << endl;
			}
			clock++;
		}
		total_idle = clock - total_busy;
//cout << on_cpu.process_num << " " << q.top().process_num << " " << clock << endl;
	}
	total_wait = sum_finish - sum_arrival - sum_busy;
	delete[] structPtr;
	stats(clock, total_idle, total_wait, counter);	
}
void Scheduler::RR(string file_name, int time_slice)
{
	int clock = 0;
	int proc_done = 0;
	int proc_count = 0;
	int time_count = 0;
	int curr_proc = 0;
	float total_busy = 0;
	float sum_arrival = 0;
	float sum_busy = 0;
	float sum_finish = 0;
	float total_idle = 0;
	float total_wait = 0;
	schedule_data on_cpu;
	
	int counter = file_size(file_name);
	schedule_data *structPtr = new schedule_data[counter]; 
	file_process(file_name, counter, structPtr);
	
	//creation of queue
	queue<schedule_data> q;
	//sorts based on arrival time
	arrivalSort(counter, structPtr);
	while(proc_done < counter){
		//inputs values into the queue only when the clock has
		//reached a processes arrival time
		while((structPtr[proc_count].arrival_time <= clock) && 
			(proc_count < counter)){
				
			if(q.empty()){
				on_cpu.process_num = -1;
			}
			//used for computing statistics
			total_busy = total_busy + structPtr[proc_count].burst;
			sum_arrival = sum_arrival + structPtr[proc_count].arrival_time;
			sum_busy = sum_busy + structPtr[proc_count].burst;
			
			//enters into the queue
			q.push(structPtr[proc_count]);
			proc_count++;
		}
		if(!q.empty()){
			//if process is different than the last one worked on
			if(on_cpu.process_num != q.front().process_num){
				on_cpu = q.front();
				cout << "Time " << clock << " Process "
				<< q.front().process_num << endl;
			}
			//if a process is complete
			else if(q.front().remaining_burst == 0){
				sum_finish = sum_finish + clock;
				q.pop();
				proc_done++;
				time_count = 0;
			}
			//if a process has run for the time slice amount
			else if(time_count == time_slice){
				q.pop();
				q.push(on_cpu);
				time_count = 0;
			}
			else{
				time_count++;
				on_cpu.remaining_burst--;
				q.front() = on_cpu;
				clock++;
			}
		}
		//empty queue 'idling'
		else if(q.empty()){
			if(on_cpu.process_num != -1){
			on_cpu.process_num = -1;
			time_count = 0;
			cout << "Time " << clock << " Idle" << endl;
			}
			clock++;
		}
			
	}
	total_idle = clock - total_busy;
	total_wait = sum_finish - sum_arrival - sum_busy;
	
	delete[] structPtr;
	stats(clock, total_idle, total_wait, counter);	
}
//sorts array of structs in order based on arrival time
void Scheduler::arrivalSort(int n, schedule_data *structPtr)
{
	schedule_data temp;
	for(int i = 0;i < (n-1);i++){
		if((structPtr[i].arrival_time > structPtr[i+1].arrival_time)||
		  ((structPtr[i].arrival_time == structPtr[i+1].arrival_time)&&
				(structPtr[i].process_num > structPtr[i+1].process_num))){
			temp = structPtr[i];
			structPtr[i] = structPtr[i+1];
			structPtr[i+1] = temp;
			i = 0;
		}
	}
}	
//prints statistics
void Scheduler::stats(float total_t, float total_idle, float total_wait, int proc_num)
{
	cout << fixed;
	cout << "CPU Utilization: " << setprecision(0) << 
			100*((total_t - total_idle)/total_t) << "%" << endl;
	cout << "Average waiting time: " << setprecision(2) << 
			(total_wait/proc_num) << endl;
	cout << "Throughput: " << setprecision(2) << (proc_num/total_t) << endl;
}