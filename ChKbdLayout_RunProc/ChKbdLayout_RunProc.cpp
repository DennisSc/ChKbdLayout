// ChKbdLayout_RunProc.cpp : Defines the entry point for the console application.
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



void ShowLastError(LPTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s last error message %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("return code"), MB_OK);

}


std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
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


	

	

	cout << "Repeatedly sending WM_INPUTLANGCHANGEREQUEST broadcast message to all windows: ";
	Sleep(100);



	for (int i = 0; i <5; i++)
	{
		PostMessage(HWND_BROADCAST,
			WM_INPUTLANGCHANGEREQUEST,
			0,
			(LPARAM)LoadKeyboardLayout(L"0000409", 0)
		);

		Sleep(100);
		cout << "* ";

	}

	

	cout << "\nWaiting for thread to finish...\n";

	WaitForSingleObject(pi.hProcess, INFINITE);

	// Get the exit code.
	//bool result = GetExitCodeProcess(pi.hProcess, 0);

	// Close the handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	

}




int main(int argc, char* argv[])
{
	
	
	LPCWSTR eee = L"00000409";
	
	//cout << eee << "\n";

	HKL desiredhkl = LoadKeyboardLayout(eee, 0);
	cout << "desiredhkl: " << desiredhkl << "\n";

	
	DWORD dwThreadID = GetCurrentThreadId();
	HKL oldhkl = GetKeyboardLayout(dwThreadID);
	cout << "oldhkl: " << oldhkl << "\n";



		
	/*HWND hwndFocused = GetFocus();

	HWND hwnd = FindWindowA("Notepad", NULL);

	if (hwnd)
	{
		cout << "found window" << "\n";
	}

	HWND hWndChild = ::FindWindowEx(hwnd, NULL, L"Edit", NULL);
	*/

	//ShowLastError(TEXT("ActivateKeyboardLayout"));
	

	//HKL result = ActivateKeyboardLayout(LoadKeyboardLayout(L"HKL_NEXT",0), KLF_ACTIVATE);
	HKL result = ActivateKeyboardLayout(desiredhkl, KLF_ACTIVATE);//KLF_SETFORPROCESS);
	cout << "Result of \"ActivateKeyboardLayout(desiredhkl, KLF_ACTIVATE)\" (returns previous hkl if successful): " << result << "\n";
	
	HKL newhkl = GetKeyboardLayout(dwThreadID);
	cout << "newhkl: " << newhkl << "\n";




	
	//set kbd layout globally

	PostMessage(HWND_BROADCAST,
		WM_INPUTLANGCHANGEREQUEST,
		0,
		(LPARAM)desiredhkl
	);



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


	
	
	cout << "Reverting to original layout \"" << oldhkl << "\".\n";
	
	result = ActivateKeyboardLayout(oldhkl, KLF_ACTIVATE);//KLF_SETFORPROCESS);
	cout << "Result of \"ActivateKeyboardLayout(oldhkl, KLF_ACTIVATE)\" (returns previous hkl if successful): " << result << "\n";


	// set kbd layout globally
	PostMessage(HWND_BROADCAST,
		WM_INPUTLANGCHANGEREQUEST,
		0,
		(LPARAM)oldhkl
	);
	


	//end of file
	

}


