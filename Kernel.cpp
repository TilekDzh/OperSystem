#include "kernel.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

namespace vm
{
	
    Kernel::Kernel(Scheduler scheduler, std::vector<std::string> executables_paths,std::vector<int> prior)
        : machine(), processes(), priorities(),listPrior(prior), scheduler(scheduler),
          _last_issued_process_id(0),
          _last_ram_position(0),
          _current_process_index(0),
          _cycles_passed_after_preemption(0)
    {
		machine.memory.ram[0] = _free_pshysical_memory= 0;
		machine.memory.ram[1] = machine.memory.ram.size() - 2;

		int i = 0;
        std::for_each(executables_paths.begin(), executables_paths.end(), [&](const std::string &path) {
			CreateProcess(path,listPrior[i]);
			++i;
        });

       machine.pic.isr_0 = [&] ()
	   {
		   Switching();
	   };

	   machine.pic.isr_3 = [&] ()
	   {
		   Unloading();
	   };
       
	   machine.Start();
    }

	Kernel::~Kernel() {}

	void Kernel::Switching()
	{
		if(scheduler == RoundRobin)
		{
			if(processes.size() > 0){
					if( _cycles_passed_after_preemption > Kernel::_MAX_CYCLES_BEFORE_PREEMPTION) 
					{
						std::cout<<"Switching to Process"<< processes[_current_process_index].id<<std::endl;
						processes[_current_process_index].registers = machine.cpu.registers;

						_current_process_index = (_current_process_index + 1) % processes.size();

						machine.cpu.registers = processes[_current_process_index].registers;

						_cycles_passed_after_preemption = 0;
					}
					else
					{
						++_cycles_passed_after_preemption;
						if(_cycles_passed_after_preemption < Kernel::_MAX_CYCLES_BEFORE_PREEMPTION)
						{
							std::cout<<"Process "<<processes[_current_process_index].id<<std::endl;
						}
					}
				}
		}
		else if(scheduler == FirstComeFirstServed || scheduler == ShortestJob)
		{
			
		}
		else if(scheduler == Priority)
		{
			if(priorities.size() > 0)
			{
				std::cout<<"Process "<<priorities.top().id<<std::endl;
				priorities.top().registers = machine.cpu.registers;
				Process process = priorities.top();
				priorities.pop();
				process.priority--;
				if(priorities.size() > 0)
				{
					if(process.priority <= priorities.top().priority)
					{
						std::cout<<"Switching to process "<<priorities.top().id<<std::endl;
					}

				}

				priorities.push(process);
				machine.cpu.registers = priorities.top().registers;
			}
		}
	}

	void Kernel::Unloading()
	{
		if(scheduler == RoundRobin)
		{
			if(processes.size() > 0)
			{
				std::cout<<"Process "<<processes[_current_process_index].id<<" was unloaded"<<std::endl;
				processes.erase(processes.begin() + _current_process_index);
				if(processes.size() > 0) {
					if(processes.size() == 1)
						_current_process_index = 0;
					machine.cpu.registers = processes[_current_process_index].registers;
				}
			}
				
		}
		else if(scheduler == FirstComeFirstServed || scheduler == ShortestJob)
		{
			if(processes.size() > 0)
			{
				std::cout<<"Process "<<processes[_current_process_index].id<<" was unloaded"<<std::endl;
				processes.erase(processes.begin() + _current_process_index);
				if(processes.size() > 0)
				{
					machine.cpu.registers = processes[_current_process_index].registers;
				}
			}
		}
		else if(scheduler == Priority)
		{
			if(priorities.size() > 0)
				{
					std::cout<<"Process "<<priorities.top().id<<" was unloaded"<<std::endl;
					priorities.pop();
					if(priorities.size() > 0)
					{
						machine.cpu.registers = priorities.top().registers;
					}
				}
		}
	}

    void Kernel::CreateProcess(const std::string &name,int priority)
    {
        if (_last_issued_process_id == std::numeric_limits<Process::process_id_type>::max()) {
            std::cerr << "Kernel: failed to create a new process. The maximum number of processes has been reached." << std::endl;
        } else {
            std::ifstream input_stream(name, std::ios::in | std::ios::binary);
            if (!input_stream) {
                std::cerr << "Kernel: failed to open the program file." << std::endl;
            } else {
                Memory::ram_type ops;

                input_stream.seekg(0, std::ios::end);
                auto file_size = input_stream.tellg();
                input_stream.seekg(0, std::ios::beg);
                ops.resize(static_cast<Memory::ram_size_type>(file_size) / 4);

                input_stream.read(reinterpret_cast<char *>(&ops[0]), file_size);

                if (input_stream.bad()) {
                    std::cerr << "Kernel: failed to read the program file." << std::endl;
                } else {
                    std::copy(ops.begin(), ops.end(), (machine.memory.ram.begin() + _last_ram_position));

                    Process process(_last_issued_process_id++, _last_ram_position,
                                                               _last_ram_position + ops.size());
					process.priority = priority;

                    _last_ram_position += ops.size();

                    // ToDo: add the new process to an appropriate data structure
				    if(scheduler == ShortestJob)
					{
						if(processes.size() > 0)
						{
							if(process.sequential_instruction_count > processes[_current_process_index].sequential_instruction_count)
							{
								processes.push_back(process);
							}
							else
							{
								processes.push_front(process);
							}
						}
						else
						{
							processes.push_front(process);
						}
					}
					else if(scheduler != Priority)
					{
						processes.push_back(process);
					}
					else
					{
						priorities.push(process);
					}

                    // ToDo: process the data structure
                }
            }
        }
    }

	Memory::ram_size_type Kernel::AllocatePhysicalMemory(Memory::ram_size_type units)
	{
		//To Do videlit pamat' 
		//vozvrawaet index kuda zapisivaet dannie
	}

	void Kernel::FreephycialMemory(Memory::ram_size_type physical_memory_index)
	{
		Memory::ram_size_type previous_free_block_index = _free_pshysical_memory;
		Memory::ram_size_type current_block_index = physical_memory_index - 2;

		for(; !(current_block_index > previous_free_block_index && current_block_index < machine.memory.ram[previous_free_block_index]);
			previous_free_block_index =  machine.memory.ram[previous_free_block_index])	{

				if( previous_free_block_index >= machine.memory.ram[previous_free_block_index] && 
					((current_block_index > previous_free_block_index || current_block_index < machine.memory.ram[previous_free_block_index]))){
					break;
				}

		}

		if(current_block_index + machine.memory.ram[current_block_index + 1] == machine.memory.ram[previous_free_block_index])
		{
			machine.memory.ram[current_block_index] == machine.memory.ram[machine.memory.ram[previous_free_block_index]];
			machine.memory.ram[current_block_index + 1] +=  machine.memory.ram[machine.memory.ram[previous_free_block_index] + 1];
		}
		else
		{
			machine.memory.ram[current_block_index] = machine.memory.ram[previous_free_block_index];
		}

		if(previous_free_block_index + machine.memory.ram[previous_free_block_index + 1] == current_block_index)
		{
			machine.memory.ram[previous_free_block_index + 1] += machine.memory.ram[current_block_index + 1];
			machine.memory.ram[previous_free_block_index] = machine.memory.ram[current_block_index];
		}
		else
		{
			machine.memory.ram[previous_free_block_index] = current_block_index;
		}

		_free_pshysical_memory = previous_free_block_index;
		//  machine.memory.ram[current_block_index + 1]
		// previ or next and after it to merge
		//previous_free_block_index

	}
}
