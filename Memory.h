#pragma once
#include <vector>

namespace vm
{
	class Memory
	{
	public:

		typedef std::vector<int> ram_type;
		typedef ram_type::size_type ram_size_type;
		static const std::vector<int>::size_type RAM_SIZE = 0x10000;
		std::vector<int> ram;

		Memory();
		virtual ~Memory();

	};
}

