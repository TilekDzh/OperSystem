#pragma once

#include "cpu.h"
#include "Memory.h"
#include "Pic.h"

namespace vm
{
	class Machine
	{
	public:
		Memory memory;
		Pic pic;
		CPU cpu;
		Machine();
		virtual ~Machine();
		void Start();
	};
}

