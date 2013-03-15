#include "Kernel.h"
#include<iostream>
#include<fstream>

namespace vm
{
	Kernel::Kernel(char* type):machine(),vectorOfProcesses(),lastUsedProcessID(0),lastRamPosition(0),currentProcessIndex(0),cyclesPassedAfterPreemption(0),algorType(type)
	{
		machine.pic.irq_0 = [](){

			std::cout << "Caught the hardware interruption 1 " << std::endl;
		};

		CreateProcess("change_register.vmexe");
		CreateProcess("change_register_in_loop.vmexe");
		CreateProcess("Change_register.vmexe");

		machine.pic.isr_0 = [&] () {
			if(algorType == "Round")
			{
				if(cyclesPassedAfterPreemption > Kernel::MAX_CYCLES_BEFORE_PREEMPTION) 
				{
					vectorOfProcesses[currentProcessIndex].registers = machine.cpu.registers;

					currentProcessIndex = (currentProcessIndex + 1) % vectorOfProcesses.size();

					machine.cpu.registers = vectorOfProcesses[currentProcessIndex].registers;

					cyclesPassedAfterPreemption = 0;
				}
				else
				{
					cyclesPassedAfterPreemption++;
				}
			}
			
		};

		machine.pic.isr_1 = [](){
			
		};

		machine.pic.isr_2 = [&](){
			
		};

	}

	void Kernel::CreateProcess(std::string name)
	{
		if(lastUsedProcessID == std::numeric_limits<Process::process_id>::max())
		{
			std::cerr<<"End"<<std::endl;
		}
		else
		{
			std::ifstream input_stream(name,std::ios::in|std::ios::binary);

			if(!input_stream)
			{
				std::cerr<<"Error with opening a file"<<std::endl;
			}
			else
			{
				Memory::ram_type ops;
				
				input_stream.open("test.bin",std::ios::binary);
				input_stream.seekg(0,std::ios::end);
				auto file_size = input_stream.tellg();
				input_stream.seekg(0,std::ios::beg);
				
				ops.resize(file_size/4);
				
				input_stream.read(reinterpret_cast<char *> (&ops[0]),file_size);

				if(input_stream.bad())
				{
					std::cerr<<"Error with inputing"<<std::endl;
				}
				else
				{
					std::copy(ops.begin(),ops.end(),(machine.memory.ram.begin() + lastRamPosition));
					Process process = Process::Process(lastUsedProcessID,lastRamPosition,lastRamPosition + ops.size(),1);

					lastRamPosition += ops.size();
					vectorOfProcesses.push_back(process);
				}
			}
		}
	}
}