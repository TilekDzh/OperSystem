#ifndef REGISTER_H
#define REGISTER_H
class Registers
{
	int eax,ebx,ecx,edx;
	
	int esp,ebp,esi,edi;

	public:
		Registers(void);
		~Registers(void);
};
#endif

