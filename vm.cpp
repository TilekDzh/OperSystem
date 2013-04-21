#include <vector>
#include <algorithm>
#include <iostream>

#include "kernel.h"

int main(int argc, char *argv[])
{
    if (argc > 2) {
        std::string arg(argv[1]);

        vm::Kernel::Scheduler scheduler;
        if (arg == "/scheduler:fcfs") {
            scheduler = vm::Kernel::Scheduler::FirstComeFirstServed;
        } else if (arg == "/scheduler:sj") {
            scheduler = vm::Kernel::Scheduler::ShortestJob;
        } else if (arg == "/scheduler:rr") {
            scheduler = vm::Kernel::Scheduler::RoundRobin;
        } else if (arg == "/scheduler:priority") {
            scheduler = vm::Kernel::Scheduler::Priority;
        }

        std::vector<std::string> processes;
		std::vector<int> priorities;
        for (int i = 2; i < argc; ++i) {
			if(i%2 == 0)
			{
				processes.push_back(std::string(argv[i]));
			}
			else
			{
				char *ch = argv[i];
				priorities.push_back(*ch - '0');
			}
        }

        vm::Kernel kernel(scheduler, processes,priorities);
    }

    return 0;
}
