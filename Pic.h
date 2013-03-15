#pragma once

#include <functional>
namespace vm
{
	class Pic
	{
	public:
		typedef std::function<void()> isr_type;
		
		//Hardware Interrupts
		isr_type irq_0; //Timer
		isr_type irq_1;
		isr_type irq_2;

		//Software Interrupts
		isr_type isr_0;
		isr_type isr_1;
		isr_type isr_2;

		Pic();
		~Pic();
	};
}

