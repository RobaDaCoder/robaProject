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
#define MaxFileLength 32

//#define PCReg 34
//#define NextPCReg 35
//#define PrevPCReg 36
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

char* User2System(int virtAddr,int limit)
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

int System2User(int virtAddr,int len,char* buffer)
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
					const int maxlen = 11;
    					char num_string[maxlen] = {0};
    					long long ret = 0;
    					for (int i = 0; i < maxlen; i++) {
        					char c = 0;
        					gSynchConsole->Read(&c,1);
        					if (c >= '0' && c <= '9') num_string[i] = c;
        					else if (i == 0 && c == '-') num_string[i] = c;
        					else {
							break;
						};
    					}
    					int i = (num_string[0] == '-') ? 1 : 0;
    					while (i < maxlen && num_string[i] >= '0' && num_string[i] <= '9')
        					ret = ret*10 + num_string[i++] - '0';
    					ret = (num_string[0] == '-') ? (-ret) : ret;
    					machine->WriteRegister(2, (int)ret);
					IncreasePC();
					break;}
				case SC_PrintInt:{
					int n = machine->ReadRegister(4);
    					/*int: [-2147483648 , 2147483647] --> max length = 11*/
    					const int maxlen = 11;
    					char num_string[maxlen] = {0};
    					int tmp[maxlen] = {0}, i = 0, j = 0;
    					if (n < 0) {
        					n = -n;
						num_string[i++] = '-';
					 }
				    	do {
						tmp[j++] = n%10;
						n /= 10;
				    	} while (n);
				    	while (j) 
						num_string[i++] = '0' + (char)tmp[--j];
				    	gSynchConsole->Write(num_string,i);
				    	machine->WriteRegister(2, 0);				
					IncreasePC();
					break;}
				case SC_ReadChar:{
					char c = 0;
					gSynchConsole->Read(&c,1);
					machine->WriteRegister(2, (int)c);
					IncreasePC();
					break;}
				case SC_PrintChar:{
					char c = (char)machine->ReadRegister(4);
				    	gSynchConsole->Write(&c,1);
				    	machine->WriteRegister(2, 0);
					IncreasePC();
					break;}
				case SC_ReadString:{
					int buffer = machine->ReadRegister(4);
				    	int length = machine->ReadRegister(5);
				    	char *buf = NULL;
				    	if (length > 0) {
						buf = new char[length];
						if (buf == NULL) {
					    		char msg[] = "Not enough memory in system.\n";
					    		gSynchConsole->Write(msg,strlen(msg));
						}
						else
					    		memset(buf, 0, length);
				    	}
					if (buf != NULL) {
						/*make sure string is null terminated*/
						gSynchConsole->Read(buf,length-1);
						int n = strlen(buf)+1;
						System2User(buffer, n, buf);
						delete[] buf;
					}	    
					machine->WriteRegister(2, 0);
					IncreasePC();
					break;}
				case SC_PrintString:{
					int buffer = machine->ReadRegister(4), i = 0;
					/*limit the length of strings to print both null and non-null terminated strings*/
					const int maxlen = 256;
					char s[maxlen] = {0};
					while (i < maxlen) {
						int oneChar = 0;
						machine->ReadMem(buffer+i, 1, &oneChar);
						if (oneChar == 0) break;
						s[i++] = (char)oneChar;
					}
					gSynchConsole->Write(s,i);
					machine->WriteRegister(2, 0);
					IncreasePC();
					break;}
				case SC_Exec:
					{
					// Input: vi tri int
					// Output: Fail return -1, Success: return id cua thread dang chay
					// SpaceId Exec(char *name);
					int virtAddr;
					virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel

					if(name == NULL)
					{
						DEBUG('a', "\n Not enough memory in System");
						printf("\n Not enough memory in System");
						machine->WriteRegister(2, -1);
						//IncreasePC();
						return;
					}
					OpenFile *oFile = fileSystem->Open(name);
					if (oFile == NULL)
					{
						printf("\nExec:: Can't open this file.");
						machine->WriteRegister(2,-1);
						IncreasePC();
						return;
					}

					delete oFile;

					// Return child process id
					int id = pTab->ExecUpdate(name); 
					machine->WriteRegister(2,id);

					delete[] name;	
					IncreasePC();
					return;
					}


				default:{
					IncreasePC();}
				break;
			}
		}
		break;
	
    }
}
