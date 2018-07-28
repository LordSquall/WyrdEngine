#include "ConsoleLogger.h"

#include <windows.h>
#include <stdio.h>
#include <cstdarg>
#include <string>
#include <iostream>

using namespace OrisisEngine;

ConsoleLogger::ConsoleLogger()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
}

void ConsoleLogger::LogDebug(string msg)
{
	cout << msg << endl;
}

void ConsoleLogger::LogInfo(string msg)
{
	cout << msg << endl;
}

void ConsoleLogger::LogWarning(string msg)
{
	cout << msg << endl;
}

void ConsoleLogger::LogError(string msg)
{
	cout << msg << endl;
}