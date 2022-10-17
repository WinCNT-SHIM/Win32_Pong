#pragma once
#include <stdio.h>

#define DEBUG_PRINT DebugPrint

// �Լ��� ���� ����
void DebugPrint(const char* pFormat, ...);

#ifdef _DEBUG
    #define FILE_PRINT() printfprintf("[%s:%d]\n", __FILE__, __LINE__)
    #define DEBUG_PRINT DebugPrint
#else
    #define FILE_PRINT() __noop
    #define DEBUG_PRINT __noop
#endif // _DEBUG


// ���� ����Ʈ
// https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160
//__func__, __line__
