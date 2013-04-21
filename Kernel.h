#ifndef KERNEL_H
#define KERNEL_H

#include <deque>
#include <queue>
#include <string>

#include "machine.h"
#include "process.h"

namespace vm
{
    class Kernel
    {
    public:
        enum Scheduler
        {
            FirstComeFirstServed,
            ShortestJob,
            RoundRobin,
            Priority
        };

        typedef std::deque<Process> process_list_type;
        typedef std::priority_queue<Process> process_priorities_type;
		typedef std::vector<int> listOfPriorities;

        Machine machine;

        process_list_type processes;
        process_priorities_type priorities;
		listOfPriorities listPrior;

        Scheduler scheduler;
		void Switching();
		void Unloading();
		void FreephycialMemory(Memory::ram_size_type physical_memory_index);
		Memory::ram_size_type AllocatePhysicalMemory(Memory::ram_size_type units);

        Kernel(Scheduler scheduler, std::vector<std::string> executables_paths,std::vector<int> prior);
        virtual ~Kernel();

        void CreateProcess(const std::string &name,int priority);

    private:
        static const unsigned int _MAX_CYCLES_BEFORE_PREEMPTION = 2;

        Process::process_id_type _last_issued_process_id;
        Memory::ram_type::size_type _last_ram_position;
		Memory::ram_size_type _free_pshysical_memory;

        unsigned int _cycles_passed_after_preemption;
        process_list_type::size_type _current_process_index;
    };
}

#endif
