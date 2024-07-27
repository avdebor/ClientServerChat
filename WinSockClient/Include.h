#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>
#include <string>
#pragma comment(lib, "ws2_32.lib")

//elements needed for colors in sonsole and other cosmetical features
#include<conio.h>
#include <Windows.h>
#include<stdio.h>
#include<stdio.h>

enum ColorConsole {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cayan = 3,
	Red = 4,
	Magenta = 5,
	lightGray = 7,
	White = 15,
	Yellow = 14,
	LightRed = 12,
};