#pragma once
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include "pong_functions.h"
#include "fmod.h"
// �׷��� ���� ���
#include "GPDefine.h"
#include "GPEngine.h"
// ���ҽ�
#include "resource.h"
// ����׿� ��ƿ
#include "debugUtils.h"

// ==================== ���� ���� ���� ====================
//���� ���� ����
extern bool g_GameRun;
// ���� �޴� ����
extern char g_gamemenu;
// ���� ���� ����
bool isGameStart = false;
// FMOD(����) ���� ����
int FmodBGMSound = AddSoundFile("./Sound/BGM.mp3", true);
// �������� ũ�� ���� ����
const int           g_WinWidth  = 1480;   // �������� ����
const int           g_WinHeight =  999;   // �������� ����
// �������� �׸��� ���� ����
extern HINSTANCE    g_hInst;        // �ν��Ͻ� �ڵ�
extern HWND         g_hWnd;         // �������� �ڵ�
extern HDC          g_hDCMain;      // ���� DC(ȭ�鿡 ������ �׸��� �뵵)
extern GameObject   backbuf;        // �����
// ��Ʈ ���� ����
LOGFONT lf;
// ���� ������Ʈ(��Ʈ��) ���� ����
GameObject titleObject;
GameObject ballObject;
GameObject barObject1;
GameObject barObject2;
GameObject trophyObject;
GameObject gameBoardTopObject;
GameObject gameBoardBottomObject;
GameObject backgroundObject;

// ==================== �Լ� ���� ���� ====================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE   hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    TCHAR szAppName[] = _T("Win32API - PONG");
    HWND  hWnd;
    MSG   msg;
    WNDCLASS wndclass;
    
    // ������ Ŭ����
    wndclass.style = CS_HREDRAW | CS_VREDRAW;                       // ������ ��Ÿ�� �ɼ��� ����(OR ��Ʈ ����)
    wndclass.lpfnWndProc = WndProc;                                 // �����쿡�� �߻��Ǵ� �޽����� ó���ϴ� �Լ��� ����(�Լ� ������)
    wndclass.cbClsExtra = 0;                                        // ������ Ŭ�������� ����� ���� �޸𸮸� ����(Byte)
    wndclass.cbWndExtra = 0;                                        // ���� �����쿡�� ����� ���� �޸� ������ ����
    wndclass.hInstance = hInstance;                                 // ������ Ŭ������ ����ϴ� �ν��Ͻ� �ڵ�
    wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);          // ������ ��� ����
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 // ���콺 Ŀ�� ��� ����
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);   // ������ ��� ���� ����
    wndclass.lpszMenuName = NULL;                                   // ���α׷��� ����� �޴� ����
    wndclass.lpszClassName = szAppName;                             // ������ Ŭ������ �̸��� ����

    // ������ ���
    RegisterClass(&wndclass);

    // ������ ����
    hWnd = CreateWindow(szAppName,  // ������ Ŭ������ ����
        szAppName,                  // �������� ĸ�� ���ڿ�
        WS_OVERLAPPEDWINDOW,        // ������ �������� ��Ÿ���� ����(OR ��Ʈ ����)
        10, 10, g_WinWidth, g_WinHeight, // ������ �������� ��ġ�� ũ�� ����
        NULL,                       // �θ� ������(Ȥ�� ������ ������) ����
        NULL,                       // ��������, �˾� �������� ��� �޴��� �ڵ��� ����
        hInstance,                  // �� �����츦 �����ϴ� �ν��Ͻ� �ڵ��� ����(�ν��Ͻ� ���� �� �����쵵 ���� �ı���)
        NULL                        // MDI Ŭ���̾�Ʈ �����츦 ���� �� CLIENTCREATESTRUCT ����ü�� �����͸� ����
    );
    if (!hWnd) return FALSE;        // ������ ���� ���н� ����

    // ������ �����츦 ȭ�鿡 ����ϴ� �Լ�
    ShowWindow(hWnd, nCmdShow);
    //// �޽��� ť�� ������ �ʰ� ������ ���ν����� ��ٷ� WM_PAINT �޽����� ����
    //// �� �۾������� ��ȿ������ ������ WM_PAINT�� ������ �ʴ´�
    //UpdateWindow(hWnd);

    HDC hdc = GetDC(hWnd);
    
    // �׷��Ƚ� ��� �ʱ�ȭ
    // �������� �ڵ���� �׷��� ���� �Ѱ��ش�
    InitializeGPEngine(hInstance, hWnd, hdc, g_WinWidth, g_WinHeight);

    // ���ڿ� ����� �����ϰ� ����
    SetBkMode(backbuf.hMemDC, TRANSPARENT);
    SetTextColor(backbuf.hMemDC, RGB(204, 204, 204));
    // ��Ʈ ���� ����
    strcpy_s(lf.lfFaceName, "����");    // ��Ʈ ����
    //lf.lfWeight = 24;                   // ��Ʈ ��
    lf.lfHeight = 24;                   // ��Ʈ ����
    lf.lfPitchAndFamily = FIXED_PITCH;  // ���� ������ ���� �����ϰ� ����
    HFONT MyFont, OldFont;
    MyFont = CreateFontIndirect(&lf);
    OldFont = (HFONT)SelectObject(backbuf.hMemDC, MyFont);
    DeleteObject(MyFont);

    // ���� ���
    BGMPlay(FmodBGMSound);

    //���� �ʱ�ȭ
    InitGame();

    // ������ ���� ���� ���
    ResetScores();
    // ȭ��, ����� �ʱ�ȭ
    ClearBackbuf();
    DrawAllToScreen();

    // �޽��� ����   : idle time�� Ȱ���ϱ� ���� �޽��� ����
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // ========== �޽����� �߻����� ���� ��, ������ �����Ǵ� �κ� ==========
        else if(g_GameRun)
        {
            if (g_gamemenu == 22 ||
                g_gamemenu == 24)
            {
                GameMain(); //��������
            }
            else if (g_gamemenu == 20 && isGameStart == false)
            {
                isGameStart = !isGameStart;
                //���� �ʱ�ȭ
                InitGame();

                ClearBackbuf();
                DrawGameBoard();
                DrawWin32GameBoard();
                DrawBall();
                DrawStick1();
                DrawStick2();
                TextOut(backbuf.hMemDC, 684, 480, "GET READY", _tcslen("GET READY"));
                DrawAllToScreen();
                Sleep(1300);
            }
            else if (g_gamemenu == 20 && isGameStart == true)
            {
                GameMain(); //��������
            }
            else
            {
                // Ŀ�� �̵� ó��
                MoveCursor();
            }
        }
        else
        {
            return 0;
        }
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc, MemDC;
    PAINTSTRUCT ps;
    HBITMAP     MyBitmap, OldBitmap;
    BITMAP      BitmapInfo;

    switch (message)
    {
        case WM_CREATE:
        {
#ifdef _DEBUG
            DEBUG_PRINT("WM_CREATE\n");
#endif // _DEBUG
            // �����츦 �߾ӿ� ��ġ��Ű�� ���� ó��
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);        // ���÷����� ���� ȹ��
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);       // ���÷����� ���� ȹ��
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);                       // �������� ���� ���� ȹ��
            int clientWidth = clientRect.right - clientRect.left;   // �������� ���� ���� ���
            int clientHeight = clientRect.bottom - clientRect.top;  // �������� ���� ���� ���
            // �������� ũ��� ��ġ ����
            SetWindowPos(hWnd, NULL, 
                screenWidth  / 2 - clientWidth  / 2,    // �������� ���ο� �´� ��ǥ
                //screenHeight / 2 - clientHeight / 2,      // �������� ���ο� ��� ��ǥ
                (screenHeight / 2 - clientHeight / 2) - 50,// �������� ���ο� ��� ��ǥ
                clientWidth, clientHeight,              // �������� ���ο� ���� ����
                0
            );

            // �ִ�ȭ ��ư ��Ȱ��ȭ
            LONG Style;
            Style = GetWindowLong(hWnd, GWL_STYLE);
            Style &= ~WS_MAXIMIZEBOX; // ���� ( 1�� ���� )
            SetWindowLong(hWnd, GWL_STYLE, Style);
            SendMessage(hWnd, WM_NCPAINT, 1, 0);
            return 0;
        }

        case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            if (g_gamemenu == 0)
            {
                // �ʱ� ȭ�� �׸���
                ClearBackbuf();
                DrawLobby();
                DrawLobbyMenu();
                DrawCursor();
                DrawAllToScreen();
            }

#ifdef _DEBUG
            RECT thisRect;
            if (GetUpdateRect(hWnd, &thisRect, true))
            {
                DEBUG_PRINT("WM_PAINT Rect %d %d %d %d\n", thisRect.left, thisRect.top, thisRect.right, thisRect.bottom);
            }
            DEBUG_PRINT("WM_PAINT Rect %d %d %d %d\n", ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
#endif // _DEBUG
            EndPaint(hWnd, &ps);
            break;
        }

        case WM_SIZE:
        {
            // �������� ũ�⸦ ���� ��Ű�� ���� ó��
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);        // ���÷����� ���� ȹ��
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);       // ���÷����� ���� ȹ��
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);           // �������� ���� ��ǥ ȹ��

            // �������� ũ�⸦ ������Ŵ(�̵��� ����)
            SetWindowPos(hWnd, NULL,
                windowRect.left,    // �������� ���ο� �´� ��ǥ
                windowRect.top,    // �������� ���ο� ��� ��ǥ
                g_WinWidth, g_WinHeight,              // �������� ���ο� ���� ����
                SWP_SHOWWINDOW
            );
            break;
        }

        case WM_LBUTTONDOWN:
        {
#ifdef _DEBUG
            DEBUG_PRINT("WM_LBUTTONDOWN %d, %d\n", LOWORD(lParam), HIWORD(lParam));
#endif // _DEBUG
            break;
        }

        case WM_DESTROY:
        {
            ReleaseDC(hWnd, g_hDCMain);     // ���� hdc�� �����Ѵ�
            DeleteDC(backbuf.hMemDC);       // �� ���۸� �����Ѵ�
            DeleteObject(backbuf.hBitmap);  // �� ���� ��Ʈ���� �����Ѵ�
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}