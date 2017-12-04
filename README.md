# ChKbdLayout
Command line tool(s) to change win10 keyboard layout

Set of small tools to change windows 10 keyboard layout to EN-US locale (useful for example when setting BitLocker unlock passwords on non-US keyboards, since the pre-boot unlock screen always uses US keyboard layout)

Uses various win32 API functions to read/set the keyboard layout.

There are three executables generated when compiling. Two of them ("ChKbdLayout_EN.exe" and "ChKbdLayout_DE.exe") simply set the layout to EN-US and DE-DE and then exit.
The third one, "ChKbdLayout_RunProc.exe" accepts a path to another executable as argument. When launched it starts that executable in s separate thread, sets the keyboard layout to EN-US and monitors the thread for completion.
When the external executable has ended, the keyboard layout is switched back to the original state (whatever it was before).


