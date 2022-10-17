#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<tchar.h>
#include "fmod.h"
#include "GPEngine.h"

// 최대 스코어 정의
#define MAXSCORE 9
// 공의 이동방향 정의
#define LEFTUP 1
#define LEFTDOWN 2
#define RIGHTDOWN 3
#define RIGHTUP 4
// 이동 거리의 픽셀 정의(Win32API용)
#define MOVEPX_X 12
#define MOVEPX_Y 24
// 바의 크기 조절 정의
#define STRETCHBAR (float)0.5625
// 점수에 따른 비트맵 변경에 대한 정의
#define CHANGEBAR 3
#define CHANGDELTAALPAH 50;

// ==================== 전역 변수 선언 ====================
// 윈도우의 그리기 관련 변수
extern HINSTANCE    g_hInst;    // 인스턴스 핸들
extern HWND         g_hWnd;     // 윈도우의 핸들
extern HDC          g_hDCMain;  // 메인 DC(화면에 게임을 그리는 용도)
extern GameObject   backbuf;    // 백버퍼
extern int          g_width;    // 윈도우의 가로 길이
extern int          g_height;   // 윈도우의 세로 길이
extern bool         isGameStart;
//게임 실행 변수
bool g_GameRun = true;
// 사운드 전역 변수
int FmodEffectSound1 = AddSoundFile("./Sound/Effect1.mp3");
int FmodEffectSound2 = AddSoundFile("./Sound/Effect2.mp3");
int FmodEffectSound3 = AddSoundFile("./Sound/Effect3.mp3");
//커서위치변수
unsigned char CursorX, CursorY;             // 콘솔용 변수
unsigned int Win32CursorX, Win32CursorY;    // Win32API용 변수
//메뉴출력
char g_gamemenu;//메뉴 스위치용 변수
//점수 관련 변수
char Score1, Score2;
//막대 좌표 전역변수
unsigned char Stick1X, Stick1Y, Stick2X, Stick2Y;   // 콘솔용
unsigned int Win32Stick1X, Win32Stick1Y;            // Win32API용
unsigned int Win32Stick2X, Win32Stick2Y;            // Win32API용
//공 좌표 전역변수
unsigned char BallX, BallY = 0;             // 콘솔용
unsigned int Win32BallX, Win32BallY = 0;    // Win32API용
char BallDir;
bool isBall = false;
// 게임 오브젝트(비트맵) 관련 변수
extern GameObject titleObject;
extern GameObject ballObject;
extern GameObject barObject1;
extern GameObject barObject2;
extern GameObject trophyObject;
extern GameObject gameBoardTopObject;
extern GameObject gameBoardBottomObject;
extern GameObject backgroundObject;
// 투명도 조절 변수
bool isOpacity = true;
int boardAlpah = 255;
int ballAlpah = 255;
// 바 비트맵 애니메이션 조절 변수
int barAnimation = 1;
char* barPath1 = (char*)"./resource/Arkanoid_Vaus/Arkanoid_Vaus_";
char* barPath2 = (char*)".bmp";

// 디버그용 전역 변수
#ifdef _DEBUG
// 디버그용 키(Insert)가 눌렸는지 확인
#define DEBUG_KEY_DOWN (GetAsyncKeyState(VK_INSERT) & 0x8000) ? 1 : 0
bool toggleDebugKey = false;
#endif // _DEBUG


// ==================== 함수 관련 기술 ====================
//콘솔핸들
void gotoxy(short x, short y)
{
#ifdef _DEBUG
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif // _DEBUG
}
//게임화면크기 설정
void SetGameBoard()
{
#ifdef _DEBUG
    system("mode con cols=122 lines=40");
#endif // _DEBUG
}
//커서 지우기
void CursorView()
{
#ifdef _DEBUG
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    return;
#endif // _DEBUG
}

/// <summary>
/// 타이틀(PONG) 출력
/// </summary>
void DrawLobby()
{
    //DrawRotateSpreite(417, 117, &titleObject,45);
    DrawSprite(417, 117, &titleObject);
    TextOut(backbuf.hMemDC, 624, 480, "게임  시작", 10);
    TextOut(backbuf.hMemDC, 624, 528, "게임  방법", 10);
    TextOut(backbuf.hMemDC, 624, 576, "종료", 4);

#ifdef _DEBUG
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    gotoxy(35, 5);  printf("■■■■    ■■■■    ■■        ■      ■■■");
    gotoxy(35, 6);  printf("■    ■  ■■    ■■  ■■        ■    ■      ■");
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    gotoxy(35, 7);  printf("■    ■  ■■    ■■  ■  ■      ■  ■");
    gotoxy(35, 8);  printf("■    ■  ■■    ■■  ■  ■      ■  ■");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    gotoxy(35, 9);  printf("■    ■  ■■    ■■  ■    ■    ■  ■");
    gotoxy(35, 10); printf("■■■■  ■■    ■■  ■    ■    ■  ■      ■■■");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    gotoxy(35, 11); printf("■■      ■■    ■■  ■      ■  ■  ■        ■");
    gotoxy(35, 12); printf("■■      ■■    ■■  ■      ■  ■  ■        ■");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    gotoxy(35, 13); printf("■■      ■■    ■■  ■        ■■    ■      ■");
    gotoxy(35, 14); printf("■■        ■■■■    ■        ■■      ■■■");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif // _DEBUG
}

/// <summary>
/// 로비 메뉴 출력
/// </summary>
void DrawLobbyMenu()
{
    gotoxy(52, 20); printf("게임 시작");
    gotoxy(52, 22); printf("게임 방법");
    gotoxy(52, 24); printf("종료");
}
/// <summary>
/// 커서 초기화
/// </summary>
void InitCursor()//커서위치초기화
{
    Win32CursorX = 575;
    Win32CursorY = 480;
    CursorX = 48;
    CursorY = 20;
}
/// <summary>
/// 커서 출력
/// </summary>
void DrawCursor()//커서출력
{
    TextOut(backbuf.hMemDC, Win32CursorX, Win32CursorY, "▶", 2);
#ifdef _DEBUG
    gotoxy(CursorX, CursorY); printf("▶");
#endif // _DEBUG

}
/// <summary>
/// 커서 삭제
/// </summary>
void EraseCursor()//커서삭제
{
    TextOut(backbuf.hMemDC, Win32CursorX, Win32CursorY, "  ", 2);
#ifdef _DEBUG
    gotoxy(CursorX, CursorY); printf("  ");
#endif // _DEBUG
}
/// <summary>
/// 커서 이동
/// </summary>
void MoveCursor()
{
    {
        if (GetAsyncKeyState(VK_UP) & 1 && (CursorY > 20))
        {
            ClearBackbuf();
            EraseCursor();
            Win32CursorY -= 48;
            CursorY -= 2;
            DrawLobby();
            DrawCursor();
            DrawAllToScreen();
        }
        else if (GetAsyncKeyState(VK_DOWN) & 1 && (CursorY < 24))
        {
            ClearBackbuf();
            EraseCursor();
            Win32CursorY += 48;
            CursorY += 2;
            DrawLobby();
            DrawCursor();
            DrawAllToScreen();
        }
        else if (GetAsyncKeyState(VK_RETURN))
        {
            g_gamemenu = CursorY;
        }
        else
        {
            ClearBackbuf();
            EraseCursor();
            DrawLobby();
            DrawCursor();
            DrawAllToScreen();
        }
    }
}

/// <summary>
/// 게임 방법 안내 화면
/// </summary>
void DrawRuleAndKey()
{
    // Win32 처리
    ClearBackbuf();
    LPCSTR str;
    str = "1P조작법: W가 위로, S가 아래로";
    TextOut(backbuf.hMemDC, 575, 358, str, strlen(str));
    str = "2P조작법: ↑가 위로, ↓가 아래로";
    TextOut(backbuf.hMemDC, 575, 406, str, strlen(str));
    str = "규칙: 먼저 9점을 따는 쪽이 승리!";
    TextOut(backbuf.hMemDC, 575, 454, str, strlen(str));
    str = "G: 바로 시작 T: 타이틀로";
    TextOut(backbuf.hMemDC, 610, 717, str, strlen(str));

#ifdef _DEBUG
    // 콘솔용 처리
    gotoxy(48, 15); printf("1P조작법: W가 위로, S가 아래로");
    gotoxy(48, 17); printf("2P조작법: ↑가 위로, ↓가 아래로");
    gotoxy(48, 19); printf("규칙: 먼저 9점을 따는 쪽이 승리!");
#endif // _DEBUG
}

/// <summary>
/// 게임 종료 후 메뉴함수
/// </summary>
void EndMenuTrigger()
{
    {
        //if (GetAsyncKeyState('G') & 0x8000)
        if (GetKeyState('G') & 0x8000)
        {
            g_gamemenu = 20;
            isGameStart = false;
        }
        //else if (GetAsyncKeyState('T') & 0x8000)
        else if (GetKeyState('T') & 0x8000)
        {
            g_gamemenu = 0;
            isGameStart = false;
        }
    }
}

/// <summary>
/// 게임판 출력
/// </summary>
void DrawGameBoard()
{
#ifdef _DEBUG
    char x, y = 0;
    char Gameboard[40][61] = { 0, };
    for (y = 0; y < 40; y++)
    {
        for (x = 0; x < 61; x++)
        {
            if ((y == 5) || (y == 39))
            {
                printf("■");
            }
            else
            {
                printf(" ");
            }
        }
        if (y != 39)
        {
            printf("\n");
        }
    }
    return;
#endif // _DEBUG
}

/// <summary>
/// 게임판 출력(Win32API)
/// </summary>
void DrawWin32GameBoard()
{
    if (Score1 >= 3 && Score2 >= 3)
    {
        // 알파 값을 일정한 값을 감소시키나 증가시켜 깜박거리게 한다
        if (isOpacity)
        {
            boardAlpah = boardAlpah - CHANGDELTAALPAH;
        }
        else
        {
            boardAlpah = boardAlpah + CHANGDELTAALPAH;
        }

        if (boardAlpah <= 0)
        {
            isOpacity = !isOpacity;
            boardAlpah = 0;
        }
        else if (boardAlpah >= 255)
        {
            isOpacity = !isOpacity;
            boardAlpah = 255;
        }

        DrawAlphaSprite(0, 5 * MOVEPX_Y, &gameBoardTopObject, boardAlpah);
        DrawAlphaSprite(0, 39 * MOVEPX_Y, &gameBoardTopObject, boardAlpah);
    }
    else
    {
        DrawSprite(0, 5 * MOVEPX_Y, &gameBoardTopObject);
        DrawSprite(0, 39 * MOVEPX_Y, &gameBoardBottomObject);
    }
}

// ==================== 막대 관련 처리 ====================
/// <summary>
/// 전 막대 초기화
/// </summary>
void InitSticks()
{
    //스틱1
    Win32Stick1X = 48;
    Win32Stick1Y = 456;
    Stick1X = 4;
    Stick1Y = 20;
    //스틱2
    Win32Stick2X = 1392;
    Win32Stick2Y = 456;
    Stick2X = 116;
    Stick2Y = 20;
    return;
}

/// <summary>
/// 막대1표현
/// </summary>
void DrawStick1()
{
    if (Score1 >= CHANGEBAR)
    {
        DrawStretchSprite(Win32Stick1X, Win32Stick1Y, &barObject1, STRETCHBAR, STRETCHBAR);
    }
    else
    {
        DrawSprite(Win32Stick1X, Win32Stick1Y, &barObject1);
    }
#ifdef _DEBUG
    gotoxy(Stick1X, Stick1Y - 1);
    printf("l");
    gotoxy(Stick1X, Stick1Y);
    printf("l");
    gotoxy(Stick1X, Stick1Y + 1);
    printf("l");
    return;
#endif // _DEBUG
}

/// <summary>
/// 막대1삭제
/// </summary>
void EraseStick1()
{
#ifdef _DEBUG
    gotoxy(Stick1X, Stick1Y - 1);
    printf(" ");
    gotoxy(Stick1X, Stick1Y);
    printf(" ");
    gotoxy(Stick1X, Stick1Y + 1);
    printf(" ");
    return;
#endif // _DEBUG
}

/// <summary>
/// 막대2표현
/// </summary>
void DrawStick2()
{
    if (Score2 >= CHANGEBAR)
    {
        DrawStretchSprite(Win32Stick2X, Win32Stick2Y, &barObject2, STRETCHBAR, STRETCHBAR);
    }
    else
    {
        DrawSprite(Win32Stick2X, Win32Stick2Y, &barObject2);
    }

#ifdef _DEBUG
    gotoxy(Stick2X, Stick2Y - 1);
    printf("l");
    gotoxy(Stick2X, Stick2Y);
    printf("l");
    gotoxy(Stick2X, Stick2Y + 1);
    printf("l");
    return;
#endif // _DEBUG
}

/// <summary>
/// 막대2삭제
/// </summary>
void EraseStick2()
{
#ifdef _DEBUG
    gotoxy(Stick2X, Stick2Y - 1);
    printf(" ");
    gotoxy(Stick2X, Stick2Y);
    printf(" ");
    gotoxy(Stick2X, Stick2Y + 1);
    printf(" ");
    return;
#endif // _DEBUG
}

/// <summary>
/// 막대 조작-키입력
/// </summary>
void MoveStick1()
{
    if (GetAsyncKeyState('W') && (Stick1Y > 7))
    {
        // Win32API용
        Win32Stick1Y -= MOVEPX_Y;
        
        EraseStick1();
        Stick1Y--;
        DrawStick1();
    }
    if (GetAsyncKeyState('S') && (Stick1Y < 37))
    {
        // Win32API용
        Win32Stick1Y += MOVEPX_Y;

        EraseStick1();
        Stick1Y++;
        DrawStick1();
    }
    return;
}
void MoveStick2()
{
    if (GetAsyncKeyState(VK_UP) && (Stick2Y > 7))
    {
        // Win32API용
        Win32Stick2Y -= MOVEPX_Y;

        EraseStick2();
        Stick2Y--;
        DrawStick2();
    }
    if (GetAsyncKeyState(VK_DOWN) && (Stick2Y < 37))
    {
        // Win32API용
        Win32Stick2Y += MOVEPX_Y;

        EraseStick2();
        Stick2Y++;
        DrawStick2();
    }
    return;
}

/// <summary>
/// 공 초기화
/// </summary>
void ResetBall()
{
    Win32BallX = 719;
    Win32BallY = 528;

    // 충돌 관련 정보 설정
    RECT temp;
    temp.left   = Win32BallX;
    temp.top    = Win32BallY;
    temp.right  = Win32BallX + ballObject.width;
    temp.bottom = Win32BallY + ballObject.height;
    SetCollisionInfo(&ballObject, temp);

    BallX = 60;
    BallY = 22;
    BallDir = rand() % 4 + 1;
    return;
}

/// <summary>
/// 공 표현
/// </summary>
void DrawBall()
{
    if (Score1 >= 3 && Score2 >= 3)
    {
        // 알파 값을 일정한 값을 감소시키나 증가시켜 깜박거리게 한다
        if (isOpacity)
        {
            boardAlpah = boardAlpah - CHANGDELTAALPAH;
        }
        else
        {
            boardAlpah = boardAlpah + CHANGDELTAALPAH;
        }

        if (boardAlpah <= 0)
        {
            isOpacity = !isOpacity;
            boardAlpah = 0;
        }
        else if (boardAlpah >= 255)
        {
            isOpacity = !isOpacity;
            boardAlpah = 255;
        }
        DrawAlphaSprite(Win32BallX, Win32BallY, &ballObject, boardAlpah);
    }
    else
    {
            DrawSprite(Win32BallX, Win32BallY, &ballObject);
    }
#ifdef _DEBUG
    gotoxy((int)BallX, BallY);
    printf("●");
#endif // _DEBUG
    if (Score1 >= 4 && Score2 >= 4)
    {
        Sleep(1);
    }
    else
    {
        Sleep(30);
    }

    return;
}

/// <summary>
/// 공 삭제
/// </summary>
void EraseBall()
{
#ifdef _DEBUG
    gotoxy(BallX, BallY);
    printf("  ");
#endif // _DEBUG
    return;
}

/// <summary>
/// 충돌(막대1)
/// </summary>
/// <returns></returns>
bool CollStick1()
{
    if ((BallX == Stick1X + 1 && BallY == Stick1Y) ||
        (BallX == Stick1X + 1 && BallY == Stick1Y - 1) ||
        (BallX == Stick1X + 1 && BallY == Stick1Y + 1) ||
        (BallX == Stick1X && BallY == Stick1Y) ||
        (BallX == Stick1X && BallY == Stick1Y - 1) ||
        (BallX == Stick1X && BallY == Stick1Y + 1)
        )
    {
        EffectPlay(FmodEffectSound1);
        return true;
    }
    else
        return false;
}
/// <summary>
/// 충돌(막대2)
/// </summary>
/// <returns></returns>
bool CollStick2()
{
    if ((BallX == Stick2X - 1 && BallY == Stick2Y) ||
        (BallX == Stick2X - 1 && BallY == Stick2Y - 1) ||
        (BallX == Stick2X - 1 && BallY == Stick2Y + 1) ||
        (BallX == Stick2X && BallY == Stick2Y) ||
        (BallX == Stick2X && BallY == Stick2Y - 1) ||
        (BallX == Stick2X && BallY == Stick2Y + 1)
        )
    {
        EffectPlay(FmodEffectSound1);
        return true;
    }
    else
        return false;
}

/// <summary>
/// 점수 초기화
/// </summary>
void ResetScores()
{
    Score1 = 0;
    Score2 = 0;
    return;
}
/// <summary>
/// 점수 표시
/// </summary>
void ShowScore()
{
    //Win32API용 점수 표시
    TCHAR  str[256] = _T("");
    wsprintf(str, _T("플레이어1의 점수: %d"), Score1);
    TextOut(backbuf.hMemDC, 24, 72, str, _tcslen(str));

    wsprintf(str, _T("플레이어2의 점수: %d"), Score2);
    TextOut(backbuf.hMemDC, 1200, 72, str, _tcslen(str));
#ifdef _DEBUG
    gotoxy(2, 3);
    printf("플레이어1의 점수: %d", Score1);
    gotoxy(100, 3);
    printf("플레이어2의 점수: %d", Score2);
#endif // _DEBUG
}
/// <summary>
/// 플레이어1득점
/// </summary>
void Player1Score()
{
    EraseBall();
    Score1++;
    ResetBall();

    return;
}
/// <summary>
/// 플레이어2득점
/// </summary>
void Player2Score()
{
    EraseBall();
    Score2++;
    ResetBall();

    return;
}

/// <summary>
/// 트로피를 그리는 함수
/// </summary>
void Drawtrophy()
{
    // Win32API용
    ClearBackbuf();
    DrawSprite(490, 119, &trophyObject);
#ifdef _DEBUG
    // 콘솔용
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    gotoxy(35, 5);   printf("                      ■■■■");
    gotoxy(35, 6);   printf("                    ■■■■■■");
    gotoxy(35, 7);   printf("        ■■■■  ■■■■■■■■  ■■■■");
    gotoxy(35, 8);   printf("      ■        ■■■■■■■■■■        ■");
    gotoxy(35, 9);   printf("      ■        ■■■■■■■■■■        ■");
    gotoxy(35, 10);  printf("      ■        ■■■■■■■■■■        ■");
    gotoxy(35, 11);  printf("        ■      ■■■■■■■■■■      ■");
    gotoxy(35, 12);  printf("          ■    ■■■■■■■■■■    ■");
    gotoxy(35, 13);  printf("            ■  ■■■■■■■■■■  ■");
    gotoxy(35, 14);  printf("              ■■■■■■■■■■■■");
    gotoxy(35, 15);  printf("                ■■■■■■■■■■");
    gotoxy(35, 16);  printf("                  ■■■■■■■■");
    gotoxy(35, 17);  printf("                    ■■■■■■");
    gotoxy(35, 18);  printf("                      ■■■■");
    gotoxy(35, 19);  printf("                      ■■■■");
    gotoxy(35, 20);  printf("                      ■■■■");
    gotoxy(35, 21);  printf("                  ■■■■■■■■");
    gotoxy(35, 22);  printf("              ■■■■■■■■■■■■");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif // _DEBUG
}

/// <summary>
/// 공 이동, 막대 이동, 충돌, 득점을 모두 합친 게임의 메인 반복문 함수
/// </summary>
void GameMain()
{
#ifdef DEBUG
    system("cls");
#endif // DEBUG

    switch (g_gamemenu)
    {
        case 20:
            {
                // 플레이어의 스코어가 MAX에 도달하지 않은 경우
                if (!(Score1 == MAXSCORE || Score2 == MAXSCORE))
                {
                    // 일정 점수에 도달하면 볼의 비트맵을 변경
                    if ((Score1 + Score2) > 1)
                    {
                        LoadGameObject_FromFile2(&ballObject, (char*)"./resource/ball.bmp");
                    }

                    //// 일정 점수에 도달하면 바의 비트맵을 변경
                    //char* tempChar = (char*)"";
                    //sprintf_s(tempChar, sizeof(tempChar), "%d", barAnimation);
                    
                    if (Score1 == CHANGEBAR)
                    {
                        //LoadGameObject_FromFile2(&barObject1, strcat_s(strcat_s(barPath1, tempChar), barPath2));
                        LoadGameObject_FromFile2(&barObject1, (char*)"./resource/Arkanoid_Vaus/Arkanoid_Vaus_2.bmp");
                    }
                    if (Score2 == CHANGEBAR)
                    {
                        //LoadGameObject_FromFile2(&barObject2, (char*)strcat(strcat(barPath1, tempChar), barPath2));
                        LoadGameObject_FromFile2(&barObject2, (char*)"./resource/Arkanoid_Vaus_Lasers/Arkanoid_Vaus_Lasers_2.bmp");
                    }


                    ClearBackbuf();
                    // 일정 점수를 넘으면 배경을 그린다
                    if ((Score1 + Score2) > 0)
                    {
                        DrawStretchSprite(0, 6 * MOVEPX_Y, &backgroundObject, (float)2.4667, (float)1.8093);
                    }
                    ShowScore();
                    
                    // 게임의 천장과 바닥을 그린다
                    DrawWin32GameBoard();
                    switch (BallDir)
                    {
                        case LEFTUP:
                            {
                                if (BallY == 6)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);
                                    BallDir = LEFTDOWN;
                                    break;
                                }
                                else if (CollStick1() == true)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = RIGHTUP;
                                    break;
                                }
                                else if (BallX == 2)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    EffectPlay(FmodEffectSound3);
                                    Player2Score();
                                }
                                else {
                                    Win32BallX -= MOVEPX_X;
                                    Win32BallY -= MOVEPX_Y;

                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);

                                    EraseBall();
                                    BallX--;
                                    BallY--;
                                    DrawBall();
                                    break;
                                }
                            }
                        case LEFTDOWN:
                            {
                                if (CollStick1() == true)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = RIGHTDOWN;
                                    break;
                                }
                                else if (BallX == 2)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    EffectPlay(FmodEffectSound3);
                                    Player2Score();
                                }
                                else if (BallY == 38)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = LEFTUP;
                                    break;
                                }
                                else {
                                    Win32BallX -= MOVEPX_X;
                                    Win32BallY += MOVEPX_Y;

                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);

                                    EraseBall();
                                    BallX--;
                                    BallY++;
                                    DrawBall();
                                    break;
                                }
                            }
                        case  RIGHTDOWN:
                            {
                                if (BallY == 38)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = RIGHTUP;
                                    break;
                                }
                                else if (CollStick2() == true)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = LEFTDOWN;
                                    break;
                                }
                                else if (BallX == 120)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    EffectPlay(FmodEffectSound2);
                                    Player1Score();
                                }
                                else {
                                    Win32BallX += MOVEPX_X;
                                    Win32BallY += MOVEPX_Y;

                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);

                                    EraseBall();
                                    BallX++;
                                    BallY++;
                                    DrawBall();
                                    break;
                                }
                            }
                        case  RIGHTUP:
                            {
                                if (CollStick2() == true)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = LEFTUP;
                                    break;
                                }
                                else if (BallX == 120)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    EffectPlay(FmodEffectSound2);
                                    Player1Score();
                                }
                                else if (BallY == 6)
                                {
                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);
                                    DrawSprite(Win32BallX, Win32BallY, &ballObject);

                                    BallDir = RIGHTDOWN;
                                    break;
                                }
                                else
                                {
                                    Win32BallX += MOVEPX_X;
                                    Win32BallY -= MOVEPX_Y;

                                    // 볼의 충돌 관련 정보 설정
                                    RECT temp;
                                    temp.left = Win32BallX;
                                    temp.top = Win32BallY;
                                    temp.right = Win32BallX + ballObject.width;
                                    temp.bottom = Win32BallY + ballObject.height;
                                    SetCollisionInfo(&ballObject, temp);

                                    EraseBall();
                                    BallX++;
                                    BallY--;
                                    DrawBall();
                                    break;
                                }
                            }
                    }
                    //막대이동
                    MoveStick1();
                    MoveStick2();
                    if (Score1 >= CHANGEBAR)
                    {
                        DrawStretchSprite(Win32Stick1X, Win32Stick1Y, &barObject1, STRETCHBAR, STRETCHBAR);
                    }
                    else
                    {
                        DrawSprite(Win32Stick1X, Win32Stick1Y, &barObject1);
                    }
                    if (Score2 >= CHANGEBAR)
                    {
                        DrawStretchSprite(Win32Stick2X, Win32Stick2Y, &barObject2, STRETCHBAR, STRETCHBAR);
                    }
                    else
                    {
                        DrawSprite(Win32Stick2X, Win32Stick2Y, &barObject2);
                    }
#ifdef _DEBUG
                // 디버그용 키(Insert)가 눌렸을 경우
                if (DEBUG_KEY_DOWN)
                {
                    toggleDebugKey = !toggleDebugKey;
                }
                // 비트맵 범위의 외곽선을 그림
                if (toggleDebugKey)
                {
                    // 비트맵 출력 범위를 외곽선으로 가시화
                    HPEN oldPen, newPen;
                    HBRUSH oldBrush, newBrush;
                    newPen = CreatePen(PS_SOLID, 1, RGB(255, 254, 0));
                    oldPen = (HPEN)SelectObject(backbuf.hMemDC, newPen);
                    newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    oldBrush = (HBRUSH)SelectObject(backbuf.hMemDC, newBrush);

                    // 볼 오브젝트의 히트 박스
                    RECT tmp = ballObject.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // 바 오브젝트1의 히트 박스
                    tmp = barObject1.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // 바 오브젝트2의 히트 박스
                    tmp = barObject2.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // 천장 오브젝트의 히트 박스
                    tmp = gameBoardTopObject.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // 바닥 오브젝트의 히트 박스
                    tmp = gameBoardBottomObject.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);

                    DeleteObject(SelectObject(g_hDCMain, oldPen));
                    DeleteObject(SelectObject(g_hDCMain, newPen));
                    DeleteObject(SelectObject(g_hDCMain, oldPen));
                    DeleteObject(SelectObject(g_hDCMain, newBrush));
                }
#endif // _DEBUG
                DrawAllToScreen();
                }
                //엔딩
                else
                {
                    LPCSTR str;
                    if (Score1 == MAXSCORE)
                    {
                        ClearBackbuf();
                        str = "플레이어1 승리!";
                        TextOut(backbuf.hMemDC, 648, 648, str, strlen(str));
#ifdef _DEBUG
                        system("cls");
                        gotoxy(54, 27);
                        printf("플레이어 1승리!\n");
#endif // _DEBUG
                        Drawtrophy();
                        //break;
                    }
                    else if (Score2 == MAXSCORE)
                    {
                        ClearBackbuf();
                        str = "플레이어2 승리!";
                        TextOut(backbuf.hMemDC, 648, 648, str, strlen(str));
#ifdef _DEBUG
                        system("cls");
                        gotoxy(54, 27);
                        printf("플레이어 2승리!\n");
#endif // _DEBUG
                        Drawtrophy();
                        //break;
                    }
                        
                    str = "G: 한게임 더 T: 타이틀로";
                    TextOut(backbuf.hMemDC, 588, 720, str, strlen(str));
                    DrawAllToScreen();
#ifdef _DEBUG
                    gotoxy(49, 30); printf("G: 한게임 더 T: 타이틀로");
#endif // _DEBUG
                    EndMenuTrigger();
                }
                break;
            }
        case 22:
            {
                DrawRuleAndKey();
                DrawAllToScreen();
#ifdef _DEBUG
                gotoxy(49, 30); printf("G: 바로 시작 T: 타이틀로");
#endif // _DEBUG
                EndMenuTrigger();
                break;
            }
        case 24:
            {
                g_GameRun = false;
                break;
            }
    }
}

/// <summary>
/// 게임 초기화
/// </summary>
void InitGame()
{
    // 난수 테이블 초기화
    srand((unsigned int)time(NULL));
#ifdef _DEBUG
    CursorView();   //커서 지우기
    SetGameBoard(); //게임화면크기 설정
#endif // _DEBUG
    InitCursor();
    ResetBall();
    InitSticks();
    ResetScores();

    // 게임 오브젝트(비트맵) 관련 로드
    LoadGameObject_FromFile2(&titleObject, (char*)"./resource/title.bmp");
    LoadGameObject_FromFile2(&ballObject, (char*)"./resource/ball_0.bmp");
    LoadGameObject_FromFile2(&barObject1, (char*)"./resource/Arkanoid_Vaus/Arkanoid_Vaus_0.bmp");
    LoadGameObject_FromFile2(&barObject2, (char*)"./resource/Arkanoid_Vaus_Lasers/Arkanoid_Vaus_Lasers_0.bmp");
    LoadGameObject_FromFile2(&trophyObject, (char*)"./resource/trophy.bmp");
    LoadGameObject_FromFile2(&gameBoardTopObject, (char*)"./resource/gameBoard.bmp");
    LoadGameObject_FromFile2(&gameBoardBottomObject, (char*)"./resource/gameBoard.bmp");
    LoadGameObject_FromFile2(&backgroundObject, (char*)"./resource/Starfield.bmp");
}