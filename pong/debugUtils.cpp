#pragma once

#include <stdarg.h>
#include "Windows.h"
#include "debugUtils.h"

#ifdef _DEBUG
    #ifdef UNICODE
        #pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
    #else
        #pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
    #endif
#endif

/// <summary>
/// ����� ��� �Լ�(�����忡 �������� ����)
/// </summary>
/// <param name="pFormat"></param>
/// <param name=""></param>
void DebugPrint(const char* pFormat, ...)
{
    static char logBuf[8192];
    va_list args;
    va_start(args, pFormat);

    vsprintf_s(logBuf, pFormat, args);
    va_end(args);

    printf(logBuf);
    OutputDebugString(logBuf);
}

// ���� ���ڿ� ���� ���� ����Ʈ
// https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/va-arg-va-copy-va-end-va-start?view=msvc-160
