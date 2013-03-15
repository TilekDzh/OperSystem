#include "Machine.h"

namespace vm
{
	Machine::Machine(void):memory(),pic(),cpu(memory,pic){}

	Machine::~Machine(void)
	{
	}

	void Machine::Start()
	{
		for(;;){
			cpu.Step();

			//Generate the Timer Interrupt
			pic.irq_0();
		}
	}
}
