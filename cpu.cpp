#include "cpu.h"
namespace vm
{
	Registers::Registers():eax(0),ebx(0),ecx(0),edx(0),flags(0),ip(0),sp(0){}

	CPU::CPU(Memory &memory,Pic &pic):memory(memory),pic(pic){}

	CPU::~CPU(){}

	void CPU::Step()
	{
		int instruction = memory.ram[registers.ip];
		int data = memory.ram[registers.ip];

		if(instruction == MOV_EAX_OPCODE)
		{
			registers.eax = data;
			registers.ip += 2;
		}
		else if(instruction == MOV_EBX_OPCODE)
		{
			registers.ebx = data;
			registers.ip += 2;
		}
		else if(instruction == MOV_ECX_OPCODE)
		{
			registers.ecx = data;
			registers.ip += 2;
		}
		else if(instruction == MOV_EDX_OPCODE)
		{
			registers.edx = data;
			registers.ip += 2;
		}
		else if(instruction == INT)
		{
			pic.isr_1();

			registers.ip += 2;
		}
		else if(instruction == JUMP)
		{
			registers.ip += data;
		}
	}
}
