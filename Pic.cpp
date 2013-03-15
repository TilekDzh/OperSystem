#include "Pic.h"

namespace vm
{
	Pic::Pic():irq_0([]() {}),
			 irq_1([]() {}),
			 irq_2([]() {}),

			 isr_0([]() {}),
			 isr_1([]() {}),
			 isr_2([]() {}){}


	Pic::~Pic(){}
}
