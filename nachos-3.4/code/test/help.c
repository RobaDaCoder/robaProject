/* help.c
 *	A simple program to output name of each members
 */

#include "syscall.h"

int main()
{
    	PrintString("==================================================\n");
	PrintString("     OS - NachOS Project     \n");
	PrintString("\n");
	PrintString("1/ Thanh vien\n");
	PrintString("	21120157 - Le Pham Hoang Trung.\n");
	PrintString("	21120415 - Tran Ngoc Bao.\n");
	PrintString("	21120554 - Le Van Tan.\n");
	PrintString("	20120568 - Nguyen Duc Tai.\n");
	PrintString("	20120076 - Mai Vinh Hien.\n");
        PrintString("2/ Chuong trinh kha dung\n");
	PrintString("	int: Demo ReadInt va PrintInt.\n");
	PrintString("	char: Demo ReadChar va PrintChar.\n");
	PrintString("	string: Demo ReadString va ReadString.\n");
	PrintString("	help: In ra thong bao va ket thuc.\n");
	PrintString("	ascii: In ra bang ma ascii.\n");
	PrintString("	sort: Nguoi dung nhap vao 1 mang so nguyen\n");
	PrintString(" 	      Su dung bubble_sort de sap xep lai mang.\n");
	PrintString("==================================================\n");
}
