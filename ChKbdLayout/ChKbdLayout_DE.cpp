// ChKbdLayout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream> 
using namespace std;
#include <iomanip>
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <Winuser.h>
#pragma comment(lib, "user32.lib")  
#define  _CRT_NONSTDC_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#define NDEBUG

#pragma warning(suppress : 4996)








int main(int argc, char* argv[])
{
	
	LPCWSTR eee = L"00000409";
	LPCWSTR ddd = L"00000407";

	cout << eee << "\n";

	HKL desiredhkl = LoadKeyboardLayout(ddd, 0);
	cout << "desiredhkl: " << desiredhkl << "\n";

	//ShellExecute(NULL, L"open", L"c:\\windows\\system32\\notepad.exe", NULL, NULL, SW_SHOWDEFAULT);

	DWORD dwThreadID = GetCurrentThreadId();
		HKL currenthkl = GetKeyboardLayout(dwThreadID);
	cout << "currenthkl: " << currenthkl << "\n";





	HKL result = ActivateKeyboardLayout(desiredhkl, KLF_ACTIVATE);
	PostMessage(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)desiredhkl);

	
	cout << "result of setting desiredhkl: " << result << "\n";
	currenthkl = GetKeyboardLayout(dwThreadID);
	cout << "new curenthkl: " << currenthkl << "\n";



}


