// NtProcessDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <winternl.h>

typedef NTSTATUS(NTAPI* QUERYINFORMATIONPROCESS)(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformstionClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

int main()
{
	STARTUPINFO startupInfo = {0};
	PROCESS_INFORMATION processInformation = {0};

	BOOL bSuccess = CreateProcess(
		TEXT("C:\\Windows\\notepad.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startupInfo, &processInformation
	);

	if (bSuccess)
	{
		printf("Process started.\n");
		printf("Process ID:\t%d\n",processInformation.dwProcessId);

		PROCESS_BASIC_INFORMATION pbi;
		ULONG uLength = 0;

		HMODULE hDll = LoadLibrary(TEXT("C:\\Windows\\System32\\ntdll.dll"));

		if (hDll)
		{
			QUERYINFORMATIONPROCESS QueryInformationProcess = (QUERYINFORMATIONPROCESS)GetProcAddress(hDll, "NtQueryInformationProcess");
		    
			if (QueryInformationProcess)
			{
				NTSTATUS ntStatus = QueryInformationProcess(
				    processInformation.hProcess,
					PROCESSINFOCLASS::ProcessBasicInformation,
					&pbi,
					sizeof(pbi),
					&uLength
				);
				if (NT_SUCCESS(ntStatus))
				{
					printf("Process ID(from PCB):\t%d\n",pbi.UniqueProcessId);
				}
				else
				{
					printf("Can not open PCB!\n");
					printf("Error code:\t%d\n", GetLastError());
				}
			}
			else
			{
				printf("Can not get NtQueryInformationProcess function!\n");
				printf("Error Code:\t%d\n", GetLastError());
			}
			FreeLibrary(hDll);
		}
		else
		{
			printf("Can not load ntdll.dll!\n");
			printf("Error Code:\t%d\n", GetLastError());
		}
	}
	else
	{
		printf("Can not start Process!\n");
		printf("Error Code:\t%d\n", GetLastError());
	}
    return 0;
}

