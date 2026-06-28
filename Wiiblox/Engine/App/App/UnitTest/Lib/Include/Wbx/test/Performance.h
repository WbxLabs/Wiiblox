#pragma once

namespace WBX { namespace Test {
	class Memory
	{
		long initialBytes;
	public:
		Memory();
		long GetBytes();
	};
}}