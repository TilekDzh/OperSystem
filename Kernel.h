#pragma once
#include "Machine.h"
#include "Process.h"

namespace vm
{
	class Kernel
	{
	public:
		Machine machine;
		Kernel(char* type);
		std::vector<Process> vectorOfProcesses;
		void CreateProcess(std::string nameOfProcess);
	private:
		static const unsigned int MAX_CYCLES_BEFORE_PREEMPTION = 100;
		Process::process_id lastUsedProcessID;
		Memory::ram_type::size_type lastRamPosition;
		char* algorType;

		unsigned int cyclesPassedAfterPreemption;
		std::vector<Process>::size_type currentProcessIndex;
	};
}

