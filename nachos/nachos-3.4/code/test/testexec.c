/* ascii.c
 *	A simple program to print out printable ascii characters
 */

#include "syscall.h"

int main() {
	int id;
	id = Exec("./test/ascii");
	PrintString("Pass ascii");
	Halt();
	return 0;
}
