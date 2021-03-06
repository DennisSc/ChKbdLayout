// ChKbdLayout_RunProc2.cpp : Defines the entry point for the console application.
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

DWORD    gPidToFind = 0;
HWND    gTargetWindowHwnd = NULL;





BOOL CALLBACK myWNDENUMPROC(HWND hwCurHwnd, LPARAM lpMylp)
{
	DWORD dwCurPid = 0;
	

	

	GetWindowThreadProcessId(hwCurHwnd, &dwCurPid);

	if (dwCurPid == gPidToFind )
	{
		
		gTargetWindowHwnd = hwCurHwnd;
		return FALSE;
	}

	return TRUE;
}


// starts a separate process, then broadcasts WM_INPUTLANGCHANGEREQUEST several times to all windows and tries to set EN-US kbd layout
void startup(LPCTSTR lpApplicationName, LPWSTR lpApplicationCmdLine)
{
	// one possibility: manually send keystrokes

	/*keybd_event(VK_LWIN, 0, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY, 0);
	Sleep(10);

	// don't forget to release the keys :)
	keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LWIN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	Sleep(10);*/

	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(lpApplicationName,   // the path
		lpApplicationCmdLine,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);



	
	Sleep(250);

	gPidToFind = pi.dwProcessId;
	EnumWindows(myWNDENUMPROC, NULL);
	//Now "gTargetWindowHwnd" variable will have the window handle(HWND)
	
	cout << "Sending WM_INPUTLANGCHANGEREQUEST message to window with HWND " << gTargetWindowHwnd;

	LPCWSTR eee = L"00000409";
	HKL desiredhkl = LoadKeyboardLayout(eee, 0);
	
	DWORD dwThreadID = GetCurrentThreadId();
	HKL oldhkl = GetKeyboardLayout(dwThreadID);

	ActivateKeyboardLayout(desiredhkl, KLF_ACTIVATE);

	for (int i = 0; i <1; i++)
	{
		PostMessage(gTargetWindowHwnd,
			WM_INPUTLANGCHANGEREQUEST,
			0,
			(LPARAM)LoadKeyboardLayout(eee, 0)
		);

		
	}

	

	
	cout << "\nWaiting for thread to finish...";
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	
	ActivateKeyboardLayout(oldhkl, KLF_ACTIVATE);

	for (int i = 0; i <1; i++)
	{
		PostMessage(HWND_BROADCAST,
			WM_INPUTLANGCHANGEREQUEST,
			0,
			(LPARAM)oldhkl
		);

		Sleep(10);


	}

	// Get the exit code.
	//bool result = GetExitCodeProcess(pi.hProcess, 0);

	// Close the handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	

}




int main(int argc, char* argv[])
{
	
	
	



	if (argv[1] != NULL)
	{
		
			//convert argv[1] to LPTSTR
			const char* commandlinearg = argv[1];
			size_t newsize = strlen(commandlinearg) + 1;
			wchar_t * wcstring = new wchar_t[newsize];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wcstring, newsize, commandlinearg, _TRUNCATE);
			LPTSTR szPathToExe = _tcsdup(wcstring);

			
			if (argv[2] != NULL)
			{
				
				//convert argv[2] to LPWSTR
				wchar_t wtext[500];
				mbstowcs(wtext, argv[2], strlen(argv[2]) + 1);//Plus null
				LPWSTR szCmdline = wtext;

				cout << "Starting new thread running \"" << commandlinearg << argv[2] <<"\"\n";
				startup(szPathToExe, szCmdline);
			}
			else 
			{
				cout << "Starting new thread running \"" << commandlinearg << "\"\n";
				startup(szPathToExe, NULL);
			}

			

		
	}


	

}


