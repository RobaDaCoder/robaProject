#include "syscall.h"

int main()
{
	char character = ReadChar();
	PrintChar(character);
  	Halt();
	return 0;
}
