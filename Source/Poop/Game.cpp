#define SAFE_DELETE(p) { if (p) { delete p; (p) = NULL; } }
#define GCC_ERROR(str) do { (void) sizeof(str); } while(0)
#define GCC_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)

#include "Game.h"
#include <Windows.h>
#include <direct.h>
#include <iostream>

Game::Game()
{
}

void Game::InitInstance()
{
}

bool IsOnlyInstance(LPCTSTR gameTitle)
{
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd)
		{
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree(drive, &diskfree);

	unsigned __int64 const neededClusters = diskSpaceNeeded / (diskfree.sectors_per_cluster * diskfree.bytes_per_sector);

	if (diskfree.avail_clusters < neededClusters)
	{
		GCC_ERROR("CheckStorage Failure: You are out of space.");
		return false;
	}
	return true;
}

bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	if (status.ullTotalPhys < physicalRAMNeeded)
	{
		GCC_ERROR("CheckMemory Failure: You are out of physical memory.");
		return false;
	}

	if (status.ullAvailVirtual < virtualRAMNeeded)
	{
		GCC_ERROR("CheckMemory Failure: You are out of virtual memory.");
		return false;
	}

	char *buff = GCC_NEW char[virtualRAMNeeded];
	if (buff)
	{
		delete[] buff;
	}
	else
	{
		GCC_ERROR("CheckMemory Failure: Not enough contiguous memory.");
		return false;
	}
	return true;
}

DWORD ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

	if (lError == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}
	return dwMHz;
}