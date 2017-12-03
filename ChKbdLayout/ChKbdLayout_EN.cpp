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


void startup(LPCTSTR lpApplicationName)
{

	/*keybd_event(VK_LWIN, 0, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY, 0);
	Sleep(10);
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
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);


	
	for (int i = 0; i <5; i++)
	{
		PostMessage(HWND_BROADCAST,
			WM_INPUTLANGCHANGEREQUEST,
			0,
			(LPARAM)LoadKeyboardLayout(L"0000409", 0)
		);

		Sleep(100);


	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	// Get the exit code.
	//bool result = GetExitCodeProcess(pi.hProcess, 0);

	// Close the handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	

	// Close process and thread handles. 
	//::WaitForSingleObject(pi.hProcess, INFINITE);
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}






int main(int argc, char* argv[])
{
	BOOL fResult, fResult2;
	//int aMouseInfo[3];    // Array for mouse information
	int currentLayout;
	int temp;
	DWORD QWERTY = 0x00000409;
	DWORD QWERTZ = 0x00000407;

						  // Get the current mouse speed.         
	//fResult = SystemParametersInfo(SPI_GETDEFAULTINPUTLANG, 0, &currentLayout, 0); ///SPI_GETMOUSE,   // Get mouse information
		
						// Double it.         
	//if (fResult)

	//{
	/*//cout << "0x" << std::hex << currentLayout << "\n";


		//cout << "current layout: ";
		//cout << currentLayout;
		cout << "\n";
		cout << "setting new layout to ";
		cout << "0x" << std::hex << QWERTY;
		cout << "\n";
		fResult2 = SystemParametersInfo(SPI_SETDEFAULTINPUTLANG, 0, &QWERTY, SPIF_SENDCHANGE);  //SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
		cout << fResult2;
		cout << "\n";
		//fResult = SystemParametersInfo(SPI_GETDEFAULTINPUTLANG, 0, &currentLayout, 0); ///SPI_GETMOUSE,   // Get mouse information
		//cout << "current layout: ";
		//cout << currentLayout << "\n";
		*/
		

		

		LPCWSTR eee = L"00000409";
		LPCWSTR ddd = L"00000407";

		cout << eee << "\n";

		HKL desiredhkl = LoadKeyboardLayout(eee, 0);
		cout << "desiredhkl: " << desiredhkl << "\n";

		//ShellExecute(NULL, L"open", L"c:\\windows\\system32\\notepad.exe", NULL, NULL, SW_SHOWDEFAULT);
		
		DWORD dwThreadID = GetCurrentThreadId();
		HKL currenthkl = GetKeyboardLayout(dwThreadID);
		cout << "currenthkl: " << currenthkl << "\n";

		
		
		
		
		HKL result = ActivateKeyboardLayout(desiredhkl, KLF_ACTIVATE);//KLF_SETFORPROCESS);

		/*HWND hwndFocused = GetFocus();

		HWND hwnd = FindWindowA("Notepad", NULL);

		if (hwnd)
		{
			cout << "found window" << "\n";
		}

		HWND hWndChild = ::FindWindowEx(hwnd, NULL, L"Edit", NULL);
		*/
		PostMessage(HWND_BROADCAST,
			WM_INPUTLANGCHANGEREQUEST,
			0,
			(LPARAM)desiredhkl
		);

		
		
		
		

		//ShowLastError(TEXT("ActivateKeyboardLayout"));
		
		//HKL result = ActivateKeyboardLayout(LoadKeyboardLayout(L"HKL_NEXT",0), KLF_ACTIVATE);
		cout << "result of setting desiredhkl: "<< result << "\n";
		currenthkl = GetKeyboardLayout(dwThreadID);
		cout << "new curenthkl: " << currenthkl << "\n";

		//cin >> temp;

	//}
	/*{
		aMouseInfo[2] = 2 * aMouseInfo[2];

		// Change the mouse speed to the new value.
		SystemParametersInfo(SPI_SETMOUSE,      // Set mouse information
			0,                 // Not used
			aMouseInfo,        // Mouse information
			SPIF_SENDCHANGE);  // Update Win.ini
	}*/



}


