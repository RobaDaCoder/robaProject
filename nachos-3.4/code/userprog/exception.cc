// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

const int max_len = 11;
const int maxlen = 256;
// ./userprog/nachos –rs 1023 –x ./test/halt
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void IncreasePC(){
	// update Prev to Current
	int current = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, current);

	// update Current to Next
	int next = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, next);
	
	//update Next to next of Next
	machine->WriteRegister(NextPCReg, next + 4);
}

char* User2System(int virtAddr, int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	} while(i < len && oneChar != 0);
	return i;
}

void ReadIntFunc(){
	char number[max_len] = {0};
	int flag = 0;
	for (int i = 0; i < max_len; i++) {
		char c = 0;
		gSynchConsole->Read(&c,1);

		if (c >= '0' && c <= '9') number[i] = c;
		else if (i == 0 && c == '-') number[i] = c;
		else {
			if (c != 0)
				flag = 1;
			else break;}
	}
	long long res = 0;

	if (flag == 0){
		int i = (number[0] == '-');

		while (i < max_len && number[i] >= '0' && number[i] <= '9')
			res = res*10 + number[i++] - '0';

		res = (number[0] == '-') ? (-res) : res;

		machine->WriteRegister(2, (int)res);}
	else {
		printf("[Error] This is not an integer\n");
		res = 0;
		machine->WriteRegister(2, (int)res);}
}

void PrintIntFunc(){
	int n = machine->ReadRegister(4);

	char number[max_len] = {0};
	int tmp[max_len] = {0}, i = 0, j = 0;

	if (n < 0) {
		n = -n;
		number[i++] = '-';
	}

	do {
		tmp[j++] = n%10;
		n /= 10;
	} while(n);

	while (j) 
		number[i++] = '0' + (char)tmp[--j];

	gSynchConsole->Write(number,i);
	machine->WriteRegister(2, 0);	
}

void ReadStringFunc(){
	int buffer = machine->ReadRegister(4);
	int len = machine->ReadRegister(5);

	char *buf = NULL;

	if (len > 0) {
		buf = new char[len];
		if (buf == NULL) {
			char msg[] = "Not enough memory in system.\n";
			gSynchConsole->Write(msg, strlen(msg));
		}
		else memset(buf, 0, len);
	}

	if (buf != NULL) {
		gSynchConsole->Read(buf, len-1);
		int n = strlen(buf) + 1;
		System2User(buffer, n, buf);
		delete[] buf;
	}	    
	machine->WriteRegister(2, 0);
}

void PrintStringFunc(){
	int buffer = machine->ReadRegister(4), i = 0;

	char s[maxlen] = {0};

	while (i < maxlen) {
		int character = 0;
		machine->ReadMem(buffer+i, 1, &character);

		if (character == 0) break;

		s[i++] = (char)character;
	}
	
	gSynchConsole->Write(s,i);
	machine->WriteRegister(2, 0);
}

void ExceptionHandler(ExceptionType which)
{
    	int type = machine->ReadRegister(2);

    	switch(which){
		case NoException:{
	     		return;}
		case PageFaultException:{
			DEBUG('a',"[Page Fault Exc] No valid translation found Error.\n");
			printf("[Page Fault Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case ReadOnlyException:{
			DEBUG('a',"[Read Only Exc] No valid translation found Error.\n");
			printf("[Read Only Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case BusErrorException:{
			DEBUG('a',"[Bus Error Exc] No valid translation found Error.\n");
			printf("[Bus Error Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case AddressErrorException:{
			DEBUG('a',"[Address Error Exc] No valid translation found Error.\n");
			printf("[Address Error Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case OverflowException:{
			DEBUG('a',"[Overflow Exc] No valid translation found Error.\n");
			printf("[Overflow Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case IllegalInstrException:{
			DEBUG('a',"[Illegal Instruction Exc] No valid translation found Error.\n");
			printf("[Illegal Instruction Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case NumExceptionTypes:{
			DEBUG('a',"[Num Exc] No valid translation found Error.\n");
			printf("[Num Exc] No valid translation found Error.\n");
			interrupt->Halt();
			break;}
		case SyscallException:{
			switch(type){
				case SC_Halt:{
					DEBUG('a', "Shutdown, initiated by user program.\n");
					printf("\n[Noti] Halt, initiated by user program.\n");
					interrupt->Halt();
					break;}
				case SC_ReadInt:{
					ReadIntFunc();
					IncreasePC();
					break;}
				case SC_PrintInt:{
					PrintIntFunc();		
					IncreasePC();
					break;}
				case SC_ReadChar:
				{

					//Input: Khong co
					//Output: Duy nhat 1 ky tu (char)
					//Cong dung: Doc mot ky tu tu nguoi dung nhap
					int maxBytes = 255;
					char* buffer = new char[255];
					int numBytes = gSynchConsole->Read(buffer, maxBytes);

					if(numBytes > 1) //Neu nhap nhieu hon 1 ky tu thi khong hop le
					{
						printf("Chi duoc nhap duy nhat 1 ky tu!");
						DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
						machine->WriteRegister(2, 0);
					}
					else if(numBytes == 0) //Ky tu rong
					{
						printf("Ky tu rong!");
						DEBUG('a', "\nERROR: Ky tu rong!");
						machine->WriteRegister(2, 0);
					}
					else
					{
						//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
						char c = buffer[0];
						machine->WriteRegister(2, c);
					}

					delete buffer;
					IncreasePC(); // error system
					return;
					//break;
				}
				case SC_PrintChar:{
					char c = (char)machine->ReadRegister(4);

				    gSynchConsole->Write(&c,1);
				    machine->WriteRegister(2, 0);

					IncreasePC();
					break;}
				case SC_ReadString:{
					ReadStringFunc();
					IncreasePC();
					break;}
				case SC_PrintString:{
					PrintStringFunc();
					IncreasePC();
					break;}
				default:{
					IncreasePC();
					break;
				}
			}
		}
		break;
	
    }
}

