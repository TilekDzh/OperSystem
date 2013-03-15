#pragma once
#include "Pic.h"
#include "Memory.h"

namespace vm
{
	struct Registers
	{
		int eax,ebx,ecx,edx;

		unsigned int flags;

		unsigned int ip;
		unsigned sp;

		Registers();
	};

	class CPU
	{
		const static int MOV_EAX_OPCODE = 0x10;
		const static int MOV_EBX_OPCODE = 0x11;
		const static int MOV_ECX_OPCODE = 0x12;
		const static int MOV_EDX_OPCODE = 0x13;

		const static int INT = 0x20;
		const static int JUMP = 0x21;

		public:
			Registers registers;
			CPU(Memory &memory,Pic &pic);
			virtual ~CPU();

			void Step();

		private:
			Memory &memory;
			Pic &pic;
	};
}

