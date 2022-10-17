#pragma once
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include "pong_functions.h"
#include "fmod.h"
// 그래픽 관련 모듈
#include "GPDefine.h"
#include "GPEngine.h"
// 리소스
#include "resource.h"
// 디버그용 유틸
#include "debugUtils.h"

// ==================== 전역 변수 선언 ====================
//게임 실행 변수
extern bool g_GameRun;
// 게임 메뉴 변수
extern char g_gamemenu;
// 게임 시작 변수
bool isGameStart = false;
// FMOD(사운드) 관련 변수
int FmodBGMSound = AddSoundFile("./Sound/BGM.mp3", true);
// 윈도우의 크기 관련 변수
const int           g_WinWidth  = 1480;   // 윈도우의 가로
const int           g_WinHeight =  999;   // 윈도우의 세로
// 윈도우의 그리기 관련 변수
extern HINSTANCE    g_hInst;        // 인스턴스 핸들
extern HWND         g_hWnd;         // 윈도우의 핸들
extern HDC          g_hDCMain;      // 메인 DC(화면에 게임을 그리는 용도)
extern GameObject   backbuf;        // 백버퍼
// 폰트 관련 변수
LOGFONT lf;
// 게임 오브젝트(비트맵) 관련 변수
GameObject titleObject;
GameObject ballObject;
GameObject barObject1;
GameObject barObject2;
GameObject trophyObject;
GameObject gameBoardTopObject;
GameObject gameBoardBottomObject;
GameObject backgroundObject;

// ==================== 함수 전방 선언 ====================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE   hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    TCHAR szAppName[] = _T("Win32API - PONG");
    HWND  hWnd;
    MSG   msg;
    WNDCLASS wndclass;
    
    // 윈도우 클래스
    wndclass.style = CS_HREDRAW | CS_VREDRAW;                       // 윈도우 스타일 옵션을 지정(OR 비트 연산)
    wndclass.lpfnWndProc = WndProc;                                 // 윈도우에서 발생되는 메시지를 처리하는 함수를 지정(함수 포인터)
    wndclass.cbClsExtra = 0;                                        // 윈도우 클래스에서 사용할 여분 메모리를 지정(Byte)
    wndclass.cbWndExtra = 0;                                        // 개별 윈도우에서 사용할 여분 메모리 영역을 지정
    wndclass.hInstance = hInstance;                                 // 윈도우 클래스가 사용하는 인스턴스 핸들
    wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);          // 아이콘 모양 지정
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 // 마우스 커서 모양 지정
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);   // 윈도우 배경 색상 지정
    wndclass.lpszMenuName = NULL;                                   // 프로그램이 사용할 메뉴 지정
    wndclass.lpszClassName = szAppName;                             // 윈도우 클래스의 이름을 지정

    // 윈도우 등록
    RegisterClass(&wndclass);

    // 윈도우 생성
    hWnd = CreateWindow(szAppName,  // 윈도우 클래스를 지정
        szAppName,                  // 윈도우의 캡션 문자열
        WS_OVERLAPPEDWINDOW,        // 생성될 윈도우의 스타일을 지정(OR 비트 연산)
        10, 10, g_WinWidth, g_WinHeight, // 생성될 윈도우의 위치와 크기 조절
        NULL,                       // 부모 윈도우(혹은 소유주 윈도우) 지정
        NULL,                       // 오버랩드, 팝업 윈도우인 경우 메뉴의 핸들을 지정
        hInstance,                  // 이 윈도우를 생성하는 인스턴스 핸들을 지정(인스턴스 종료 시 윈도우도 같이 파괴됨)
        NULL                        // MDI 클라이언트 윈도우를 만들 때 CLIENTCREATESTRUCT 구조체의 포인터를 설정
    );
    if (!hWnd) return FALSE;        // 윈도우 생성 실패시 종료

    // 생성된 윈도우를 화면에 출력하는 함수
    ShowWindow(hWnd, nCmdShow);
    //// 메시지 큐를 통하지 않고 윈도우 프로시저로 곧바로 WM_PAINT 메시지를 전달
    //// 단 작업영역에 무효영역이 없으면 WM_PAINT를 보내지 않는다
    //UpdateWindow(hWnd);

    HDC hdc = GetDC(hWnd);
    
    // 그래픽스 모듈 초기화
    // 윈도우의 핸들들을 그래픽 모듈로 넘겨준다
    InitializeGPEngine(hInstance, hWnd, hdc, g_WinWidth, g_WinHeight);

    // 문자열 배경은 투명하게 설정
    SetBkMode(backbuf.hMemDC, TRANSPARENT);
    SetTextColor(backbuf.hMemDC, RGB(204, 204, 204));
    // 폰트 관련 설정
    strcpy_s(lf.lfFaceName, "굴림");    // 폰트 설정
    //lf.lfWeight = 24;                   // 폰트 폭
    lf.lfHeight = 24;                   // 폰트 높이
    lf.lfPitchAndFamily = FIXED_PITCH;  // 개별 글자의 폭을 일정하게 설정
    HFONT MyFont, OldFont;
    MyFont = CreateFontIndirect(&lf);
    OldFont = (HFONT)SelectObject(backbuf.hMemDC, MyFont);
    DeleteObject(MyFont);

    // 음악 재생
    BGMPlay(FmodBGMSound);

    //게임 초기화
    InitGame();

    // 게임이 실행 중일 경우
    ResetScores();
    // 화면, 백버퍼 초기화
    ClearBackbuf();
    DrawAllToScreen();

    // 메시지 루프   : idle time을 활용하기 위한 메시지 루프
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // ========== 메시지가 발생하지 않을 시, 게임이 구동되는 부분 ==========
        else if(g_GameRun)
        {
            if (g_gamemenu == 22 ||
                g_gamemenu == 24)
            {
                GameMain(); //게임진행
            }
            else if (g_gamemenu == 20 && isGameStart == false)
            {
                isGameStart = !isGameStart;
                //게임 초기화
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
                GameMain(); //게임진행
            }
            else
            {
                // 커서 이동 처리
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
            // 윈도우를 중앙에 위치시키기 위한 처리
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);        // 디스플레이의 가로 획득
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);       // 디스플레이의 세로 획득
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);                       // 윈도우의 가로 세로 획득
            int clientWidth = clientRect.right - clientRect.left;   // 윈도우의 가로 길이 계산
            int clientHeight = clientRect.bottom - clientRect.top;  // 윈도우의 세로 길이 계산
            // 윈도우의 크기와 위치 변경
            SetWindowPos(hWnd, NULL, 
                screenWidth  / 2 - clientWidth  / 2,    // 윈도우의 새로운 좌단 좌표
                //screenHeight / 2 - clientHeight / 2,      // 윈도우의 새로운 상단 좌표
                (screenHeight / 2 - clientHeight / 2) - 50,// 윈도우의 새로운 상단 좌표
                clientWidth, clientHeight,              // 윈도우의 새로운 폭과 높이
                0
            );

            // 최대화 버튼 비활성화
            LONG Style;
            Style = GetWindowLong(hWnd, GWL_STYLE);
            Style &= ~WS_MAXIMIZEBOX; // 반전 ( 1의 보수 )
            SetWindowLong(hWnd, GWL_STYLE, Style);
            SendMessage(hWnd, WM_NCPAINT, 1, 0);
            return 0;
        }

        case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            if (g_gamemenu == 0)
            {
                // 초기 화면 그리기
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
            // 윈도우의 크기를 고정 시키기 위한 처리
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);        // 디스플레이의 가로 획득
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);       // 디스플레이의 세로 획득
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);           // 윈도우의 현재 좌표 획득

            // 윈도우의 크기를 고정시킴(이동은 가능)
            SetWindowPos(hWnd, NULL,
                windowRect.left,    // 윈도우의 새로운 좌단 좌표
                windowRect.top,    // 윈도우의 새로운 상단 좌표
                g_WinWidth, g_WinHeight,              // 윈도우의 새로운 폭과 높이
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
            ReleaseDC(hWnd, g_hDCMain);     // 전역 hdc를 해제한다
            DeleteDC(backbuf.hMemDC);       // 백 버퍼를 해제한다
            DeleteObject(backbuf.hBitmap);  // 백 버퍼 비트맵을 해제한다
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}