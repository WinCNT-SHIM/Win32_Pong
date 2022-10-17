#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<tchar.h>
#include "fmod.h"
#include "GPEngine.h"

// �ִ� ���ھ� ����
#define MAXSCORE 9
// ���� �̵����� ����
#define LEFTUP 1
#define LEFTDOWN 2
#define RIGHTDOWN 3
#define RIGHTUP 4
// �̵� �Ÿ��� �ȼ� ����(Win32API��)
#define MOVEPX_X 12
#define MOVEPX_Y 24
// ���� ũ�� ���� ����
#define STRETCHBAR (float)0.5625
// ������ ���� ��Ʈ�� ���濡 ���� ����
#define CHANGEBAR 3
#define CHANGDELTAALPAH 50;

// ==================== ���� ���� ���� ====================
// �������� �׸��� ���� ����
extern HINSTANCE    g_hInst;    // �ν��Ͻ� �ڵ�
extern HWND         g_hWnd;     // �������� �ڵ�
extern HDC          g_hDCMain;  // ���� DC(ȭ�鿡 ������ �׸��� �뵵)
extern GameObject   backbuf;    // �����
extern int          g_width;    // �������� ���� ����
extern int          g_height;   // �������� ���� ����
extern bool         isGameStart;
//���� ���� ����
bool g_GameRun = true;
// ���� ���� ����
int FmodEffectSound1 = AddSoundFile("./Sound/Effect1.mp3");
int FmodEffectSound2 = AddSoundFile("./Sound/Effect2.mp3");
int FmodEffectSound3 = AddSoundFile("./Sound/Effect3.mp3");
//Ŀ����ġ����
unsigned char CursorX, CursorY;             // �ֿܼ� ����
unsigned int Win32CursorX, Win32CursorY;    // Win32API�� ����
//�޴����
char g_gamemenu;//�޴� ����ġ�� ����
//���� ���� ����
char Score1, Score2;
//���� ��ǥ ��������
unsigned char Stick1X, Stick1Y, Stick2X, Stick2Y;   // �ֿܼ�
unsigned int Win32Stick1X, Win32Stick1Y;            // Win32API��
unsigned int Win32Stick2X, Win32Stick2Y;            // Win32API��
//�� ��ǥ ��������
unsigned char BallX, BallY = 0;             // �ֿܼ�
unsigned int Win32BallX, Win32BallY = 0;    // Win32API��
char BallDir;
bool isBall = false;
// ���� ������Ʈ(��Ʈ��) ���� ����
extern GameObject titleObject;
extern GameObject ballObject;
extern GameObject barObject1;
extern GameObject barObject2;
extern GameObject trophyObject;
extern GameObject gameBoardTopObject;
extern GameObject gameBoardBottomObject;
extern GameObject backgroundObject;
// ���� ���� ����
bool isOpacity = true;
int boardAlpah = 255;
int ballAlpah = 255;
// �� ��Ʈ�� �ִϸ��̼� ���� ����
int barAnimation = 1;
char* barPath1 = (char*)"./resource/Arkanoid_Vaus/Arkanoid_Vaus_";
char* barPath2 = (char*)".bmp";

// ����׿� ���� ����
#ifdef _DEBUG
// ����׿� Ű(Insert)�� ���ȴ��� Ȯ��
#define DEBUG_KEY_DOWN (GetAsyncKeyState(VK_INSERT) & 0x8000) ? 1 : 0
bool toggleDebugKey = false;
#endif // _DEBUG


// ==================== �Լ� ���� ��� ====================
//�ܼ��ڵ�
void gotoxy(short x, short y)
{
#ifdef _DEBUG
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif // _DEBUG
}
//����ȭ��ũ�� ����
void SetGameBoard()
{
#ifdef _DEBUG
    system("mode con cols=122 lines=40");
#endif // _DEBUG
}
//Ŀ�� �����
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
/// Ÿ��Ʋ(PONG) ���
/// </summary>
void DrawLobby()
{
    //DrawRotateSpreite(417, 117, &titleObject,45);
    DrawSprite(417, 117, &titleObject);
    TextOut(backbuf.hMemDC, 624, 480, "����  ����", 10);
    TextOut(backbuf.hMemDC, 624, 528, "����  ���", 10);
    TextOut(backbuf.hMemDC, 624, 576, "����", 4);

#ifdef _DEBUG
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    gotoxy(35, 5);  printf("�����    �����    ���        ��      ����");
    gotoxy(35, 6);  printf("��    ��  ���    ���  ���        ��    ��      ��");
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    gotoxy(35, 7);  printf("��    ��  ���    ���  ��  ��      ��  ��");
    gotoxy(35, 8);  printf("��    ��  ���    ���  ��  ��      ��  ��");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    gotoxy(35, 9);  printf("��    ��  ���    ���  ��    ��    ��  ��");
    gotoxy(35, 10); printf("�����  ���    ���  ��    ��    ��  ��      ����");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    gotoxy(35, 11); printf("���      ���    ���  ��      ��  ��  ��        ��");
    gotoxy(35, 12); printf("���      ���    ���  ��      ��  ��  ��        ��");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    gotoxy(35, 13); printf("���      ���    ���  ��        ���    ��      ��");
    gotoxy(35, 14); printf("���        �����    ��        ���      ����");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif // _DEBUG
}

/// <summary>
/// �κ� �޴� ���
/// </summary>
void DrawLobbyMenu()
{
    gotoxy(52, 20); printf("���� ����");
    gotoxy(52, 22); printf("���� ���");
    gotoxy(52, 24); printf("����");
}
/// <summary>
/// Ŀ�� �ʱ�ȭ
/// </summary>
void InitCursor()//Ŀ����ġ�ʱ�ȭ
{
    Win32CursorX = 575;
    Win32CursorY = 480;
    CursorX = 48;
    CursorY = 20;
}
/// <summary>
/// Ŀ�� ���
/// </summary>
void DrawCursor()//Ŀ�����
{
    TextOut(backbuf.hMemDC, Win32CursorX, Win32CursorY, "��", 2);
#ifdef _DEBUG
    gotoxy(CursorX, CursorY); printf("��");
#endif // _DEBUG

}
/// <summary>
/// Ŀ�� ����
/// </summary>
void EraseCursor()//Ŀ������
{
    TextOut(backbuf.hMemDC, Win32CursorX, Win32CursorY, "  ", 2);
#ifdef _DEBUG
    gotoxy(CursorX, CursorY); printf("  ");
#endif // _DEBUG
}
/// <summary>
/// Ŀ�� �̵�
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
/// ���� ��� �ȳ� ȭ��
/// </summary>
void DrawRuleAndKey()
{
    // Win32 ó��
    ClearBackbuf();
    LPCSTR str;
    str = "1P���۹�: W�� ����, S�� �Ʒ���";
    TextOut(backbuf.hMemDC, 575, 358, str, strlen(str));
    str = "2P���۹�: �谡 ����, �鰡 �Ʒ���";
    TextOut(backbuf.hMemDC, 575, 406, str, strlen(str));
    str = "��Ģ: ���� 9���� ���� ���� �¸�!";
    TextOut(backbuf.hMemDC, 575, 454, str, strlen(str));
    str = "G: �ٷ� ���� T: Ÿ��Ʋ��";
    TextOut(backbuf.hMemDC, 610, 717, str, strlen(str));

#ifdef _DEBUG
    // �ֿܼ� ó��
    gotoxy(48, 15); printf("1P���۹�: W�� ����, S�� �Ʒ���");
    gotoxy(48, 17); printf("2P���۹�: �谡 ����, �鰡 �Ʒ���");
    gotoxy(48, 19); printf("��Ģ: ���� 9���� ���� ���� �¸�!");
#endif // _DEBUG
}

/// <summary>
/// ���� ���� �� �޴��Լ�
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
/// ������ ���
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
                printf("��");
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
/// ������ ���(Win32API)
/// </summary>
void DrawWin32GameBoard()
{
    if (Score1 >= 3 && Score2 >= 3)
    {
        // ���� ���� ������ ���� ���ҽ�Ű�� �������� ���ڰŸ��� �Ѵ�
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

// ==================== ���� ���� ó�� ====================
/// <summary>
/// �� ���� �ʱ�ȭ
/// </summary>
void InitSticks()
{
    //��ƽ1
    Win32Stick1X = 48;
    Win32Stick1Y = 456;
    Stick1X = 4;
    Stick1Y = 20;
    //��ƽ2
    Win32Stick2X = 1392;
    Win32Stick2Y = 456;
    Stick2X = 116;
    Stick2Y = 20;
    return;
}

/// <summary>
/// ����1ǥ��
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
/// ����1����
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
/// ����2ǥ��
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
/// ����2����
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
/// ���� ����-Ű�Է�
/// </summary>
void MoveStick1()
{
    if (GetAsyncKeyState('W') && (Stick1Y > 7))
    {
        // Win32API��
        Win32Stick1Y -= MOVEPX_Y;
        
        EraseStick1();
        Stick1Y--;
        DrawStick1();
    }
    if (GetAsyncKeyState('S') && (Stick1Y < 37))
    {
        // Win32API��
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
        // Win32API��
        Win32Stick2Y -= MOVEPX_Y;

        EraseStick2();
        Stick2Y--;
        DrawStick2();
    }
    if (GetAsyncKeyState(VK_DOWN) && (Stick2Y < 37))
    {
        // Win32API��
        Win32Stick2Y += MOVEPX_Y;

        EraseStick2();
        Stick2Y++;
        DrawStick2();
    }
    return;
}

/// <summary>
/// �� �ʱ�ȭ
/// </summary>
void ResetBall()
{
    Win32BallX = 719;
    Win32BallY = 528;

    // �浹 ���� ���� ����
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
/// �� ǥ��
/// </summary>
void DrawBall()
{
    if (Score1 >= 3 && Score2 >= 3)
    {
        // ���� ���� ������ ���� ���ҽ�Ű�� �������� ���ڰŸ��� �Ѵ�
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
    printf("��");
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
/// �� ����
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
/// �浹(����1)
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
/// �浹(����2)
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
/// ���� �ʱ�ȭ
/// </summary>
void ResetScores()
{
    Score1 = 0;
    Score2 = 0;
    return;
}
/// <summary>
/// ���� ǥ��
/// </summary>
void ShowScore()
{
    //Win32API�� ���� ǥ��
    TCHAR  str[256] = _T("");
    wsprintf(str, _T("�÷��̾�1�� ����: %d"), Score1);
    TextOut(backbuf.hMemDC, 24, 72, str, _tcslen(str));

    wsprintf(str, _T("�÷��̾�2�� ����: %d"), Score2);
    TextOut(backbuf.hMemDC, 1200, 72, str, _tcslen(str));
#ifdef _DEBUG
    gotoxy(2, 3);
    printf("�÷��̾�1�� ����: %d", Score1);
    gotoxy(100, 3);
    printf("�÷��̾�2�� ����: %d", Score2);
#endif // _DEBUG
}
/// <summary>
/// �÷��̾�1����
/// </summary>
void Player1Score()
{
    EraseBall();
    Score1++;
    ResetBall();

    return;
}
/// <summary>
/// �÷��̾�2����
/// </summary>
void Player2Score()
{
    EraseBall();
    Score2++;
    ResetBall();

    return;
}

/// <summary>
/// Ʈ���Ǹ� �׸��� �Լ�
/// </summary>
void Drawtrophy()
{
    // Win32API��
    ClearBackbuf();
    DrawSprite(490, 119, &trophyObject);
#ifdef _DEBUG
    // �ֿܼ�
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    gotoxy(35, 5);   printf("                      �����");
    gotoxy(35, 6);   printf("                    �������");
    gotoxy(35, 7);   printf("        �����  ���������  �����");
    gotoxy(35, 8);   printf("      ��        �����������        ��");
    gotoxy(35, 9);   printf("      ��        �����������        ��");
    gotoxy(35, 10);  printf("      ��        �����������        ��");
    gotoxy(35, 11);  printf("        ��      �����������      ��");
    gotoxy(35, 12);  printf("          ��    �����������    ��");
    gotoxy(35, 13);  printf("            ��  �����������  ��");
    gotoxy(35, 14);  printf("              �������������");
    gotoxy(35, 15);  printf("                �����������");
    gotoxy(35, 16);  printf("                  ���������");
    gotoxy(35, 17);  printf("                    �������");
    gotoxy(35, 18);  printf("                      �����");
    gotoxy(35, 19);  printf("                      �����");
    gotoxy(35, 20);  printf("                      �����");
    gotoxy(35, 21);  printf("                  ���������");
    gotoxy(35, 22);  printf("              �������������");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif // _DEBUG
}

/// <summary>
/// �� �̵�, ���� �̵�, �浹, ������ ��� ��ģ ������ ���� �ݺ��� �Լ�
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
                // �÷��̾��� ���ھ MAX�� �������� ���� ���
                if (!(Score1 == MAXSCORE || Score2 == MAXSCORE))
                {
                    // ���� ������ �����ϸ� ���� ��Ʈ���� ����
                    if ((Score1 + Score2) > 1)
                    {
                        LoadGameObject_FromFile2(&ballObject, (char*)"./resource/ball.bmp");
                    }

                    //// ���� ������ �����ϸ� ���� ��Ʈ���� ����
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
                    // ���� ������ ������ ����� �׸���
                    if ((Score1 + Score2) > 0)
                    {
                        DrawStretchSprite(0, 6 * MOVEPX_Y, &backgroundObject, (float)2.4667, (float)1.8093);
                    }
                    ShowScore();
                    
                    // ������ õ��� �ٴ��� �׸���
                    DrawWin32GameBoard();
                    switch (BallDir)
                    {
                        case LEFTUP:
                            {
                                if (BallY == 6)
                                {
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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

                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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

                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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

                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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
                                    // ���� �浹 ���� ���� ����
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

                                    // ���� �浹 ���� ���� ����
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
                    //�����̵�
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
                // ����׿� Ű(Insert)�� ������ ���
                if (DEBUG_KEY_DOWN)
                {
                    toggleDebugKey = !toggleDebugKey;
                }
                // ��Ʈ�� ������ �ܰ����� �׸�
                if (toggleDebugKey)
                {
                    // ��Ʈ�� ��� ������ �ܰ������� ����ȭ
                    HPEN oldPen, newPen;
                    HBRUSH oldBrush, newBrush;
                    newPen = CreatePen(PS_SOLID, 1, RGB(255, 254, 0));
                    oldPen = (HPEN)SelectObject(backbuf.hMemDC, newPen);
                    newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    oldBrush = (HBRUSH)SelectObject(backbuf.hMemDC, newBrush);

                    // �� ������Ʈ�� ��Ʈ �ڽ�
                    RECT tmp = ballObject.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // �� ������Ʈ1�� ��Ʈ �ڽ�
                    tmp = barObject1.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // �� ������Ʈ2�� ��Ʈ �ڽ�
                    tmp = barObject2.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // õ�� ������Ʈ�� ��Ʈ �ڽ�
                    tmp = gameBoardTopObject.collisionHit;
                    Rectangle(backbuf.hMemDC, tmp.left, tmp.top, tmp.right, tmp.bottom);
                    // �ٴ� ������Ʈ�� ��Ʈ �ڽ�
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
                //����
                else
                {
                    LPCSTR str;
                    if (Score1 == MAXSCORE)
                    {
                        ClearBackbuf();
                        str = "�÷��̾�1 �¸�!";
                        TextOut(backbuf.hMemDC, 648, 648, str, strlen(str));
#ifdef _DEBUG
                        system("cls");
                        gotoxy(54, 27);
                        printf("�÷��̾� 1�¸�!\n");
#endif // _DEBUG
                        Drawtrophy();
                        //break;
                    }
                    else if (Score2 == MAXSCORE)
                    {
                        ClearBackbuf();
                        str = "�÷��̾�2 �¸�!";
                        TextOut(backbuf.hMemDC, 648, 648, str, strlen(str));
#ifdef _DEBUG
                        system("cls");
                        gotoxy(54, 27);
                        printf("�÷��̾� 2�¸�!\n");
#endif // _DEBUG
                        Drawtrophy();
                        //break;
                    }
                        
                    str = "G: �Ѱ��� �� T: Ÿ��Ʋ��";
                    TextOut(backbuf.hMemDC, 588, 720, str, strlen(str));
                    DrawAllToScreen();
#ifdef _DEBUG
                    gotoxy(49, 30); printf("G: �Ѱ��� �� T: Ÿ��Ʋ��");
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
                gotoxy(49, 30); printf("G: �ٷ� ���� T: Ÿ��Ʋ��");
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
/// ���� �ʱ�ȭ
/// </summary>
void InitGame()
{
    // ���� ���̺� �ʱ�ȭ
    srand((unsigned int)time(NULL));
#ifdef _DEBUG
    CursorView();   //Ŀ�� �����
    SetGameBoard(); //����ȭ��ũ�� ����
#endif // _DEBUG
    InitCursor();
    ResetBall();
    InitSticks();
    ResetScores();

    // ���� ������Ʈ(��Ʈ��) ���� �ε�
    LoadGameObject_FromFile2(&titleObject, (char*)"./resource/title.bmp");
    LoadGameObject_FromFile2(&ballObject, (char*)"./resource/ball_0.bmp");
    LoadGameObject_FromFile2(&barObject1, (char*)"./resource/Arkanoid_Vaus/Arkanoid_Vaus_0.bmp");
    LoadGameObject_FromFile2(&barObject2, (char*)"./resource/Arkanoid_Vaus_Lasers/Arkanoid_Vaus_Lasers_0.bmp");
    LoadGameObject_FromFile2(&trophyObject, (char*)"./resource/trophy.bmp");
    LoadGameObject_FromFile2(&gameBoardTopObject, (char*)"./resource/gameBoard.bmp");
    LoadGameObject_FromFile2(&gameBoardBottomObject, (char*)"./resource/gameBoard.bmp");
    LoadGameObject_FromFile2(&backgroundObject, (char*)"./resource/Starfield.bmp");
}