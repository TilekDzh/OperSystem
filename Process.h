#pragma once
#include "cpu.h"

namespace vm
{
	class Process
	{
	public:
		typedef short process_id;
		typedef std::vector<int>::size_type memory_position;
		typedef short stateProcess;

		// state == 1 -> Ready
		// state == 0 -> Running
		// state == -1 -> Blocked
		stateProcess state;
		memory_position lastPosition;
		memory_position currPosition;

		Registers registers;

		Process(process_id id,std::vector<int>::size_type lastPosition,std::vector<int>::size_type currentPosition, stateProcess state);
		~Process();
	private:
		process_id ID;

	};
}

