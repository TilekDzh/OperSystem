#include "Process.h"


namespace vm
{
	Process::Process(process_id id,std::vector<int>::size_type lastP,std::vector<int>::size_type currP, stateProcess State):
						ID(id),lastPosition(lastP),currPosition(currP),state(State)
	{}


	Process::~Process()
	{
	}
}
