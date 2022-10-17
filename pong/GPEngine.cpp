#include <math.h>
#include "GPEngine.h"
// TransparentBlt �Լ��� ����ϱ� ���� ����
#pragma comment(lib, "Msimg32.lib")

#define PI  (double)3.14159265

// ==================== ���� ���� ���� ====================
HINSTANCE   g_hInst;        // �ν��Ͻ� �ڵ�
HWND        g_hWnd;         // �������� �ڵ�
HDC         g_hDCMain;      // ���� DC(ȭ�鿡 ������ �׸��� �뵵)
GameObject  backbuf;        // �����
int         g_width;     // �������� ���� ����
int         g_height;    // �������� ���� ����

/// <summary>
/// �׷��Ƚ� ��⿡ �ʿ��� ���� �������� ȹ��(������, �׸��� ȭ���� 1���� ����)
/// </summary>
/// <param name="hInst">Win32���α׷��� ���� �ּ� ��</param>
/// <param name="hWnd">Win32���α׷��� ������ �ڵ�(�ĺ���)</param>
/// <param name="hDC">������DC�� �ڵ�</param>
/// <param name="g_WinWidth">�������� ���� ����</param>
/// <param name="g_WinHeight">�������� ���� ����</param>
void InitializeGPEngine(HINSTANCE hInst, HWND hWnd, HDC hDC, int width, int height)
{
    g_hInst = hInst;
    g_hWnd = hWnd;
    g_hDCMain = hDC;
    g_width = width;
    g_height = height;

    // ���۹��� ��Ʈ���� ����
    backbuf.hMemDC = CreateCompatibleDC(g_hDCMain);
    backbuf.hBitmap = CreateCompatibleBitmap(g_hDCMain, g_width, g_height);
    /// �޸� DC�� �� ����
    PatBlt(backbuf.hMemDC, 0, 0, g_width, g_height, BLACKNESS);
    // �޸� DC�� ��Ʈ���� ������� ��Ʈ������ ����
    SelectObject(backbuf.hMemDC, backbuf.hBitmap);
    backbuf.width = g_width;
    backbuf.height = g_height;
}

/// <summary>
/// ����� �޸�DC�� �ʱ�ȭ �Ѵ�
/// </summary>
void ClearBackbuf()
{
    PatBlt(backbuf.hMemDC, 0, 0, g_width, g_height, BLACKNESS);
}

/// <summary>
/// ���� ������Ʈ(��Ʈ��)�� �浹 ���� ������ �����Ѵ�
/// </summary>
/// <param name="gameObject">��� ���� ������Ʈ</param>
/// <param name="collision">�浹 ���� ����</param>
void SetCollisionInfo(GameObject* gameObject, RECT collision)
{
    gameObject->collisionHit = collision;
}

/// <summary>
/// ���ҽ����� ���� ������Ʈ(��Ʈ��)�� �ε��Ѵ�
/// </summary>
/// <param name="bitmap">��� ��Ʈ���� ����</param>
/// <param name="resID">��� ���ҽ��� ID</param>
void LoadGameObject_FromResource(GameObject* gameObject, int resID)
{
    BITMAP tmpBitmap;

    // ���� DC�� ȣȯ�Ǵ� �޸� DC ����
    gameObject->hMemDC = CreateCompatibleDC(g_hDCMain);
    // ���ҽ����� ��Ʈ���� �ε�
    gameObject->hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(resID));
    // �޸� DC�� ��Ʈ���� ���ҽ��� ��Ʈ������ ����
    SelectObject(gameObject->hMemDC, gameObject->hBitmap);

    // ���� ������Ʈ�� �ΰ� ������ �����Ѵ�
    GetObject(gameObject->hBitmap, sizeof(BITMAP), &tmpBitmap);
    gameObject->width = tmpBitmap.bmWidth;
    gameObject->height = tmpBitmap.bmHeight;
    gameObject->keyColor = COLORKEY_PINK;
    // ��Ʈ�� �ڵ� ����
    DeleteObject(gameObject->hBitmap);
}

/// <summary>
/// ���Ͽ��� ���� ������Ʈ(��Ʈ��)�� �ε�
/// </summary>
/// <param name="bitmap"></param>
/// <param name="filePath"></param>
void LoadGameObject_FromFile2(GameObject* gameObject, char* filePath)
{
    BITMAP tmpBitmap;

    // ���� DC�� ȣȯ�Ǵ� �޷θ� DC ����
    gameObject->hMemDC = CreateCompatibleDC(g_hDCMain);
    // ������, Ŀ��, �ִϸ��̼� Ŀ��, ��Ʈ���� �ε��Ѵ�
    gameObject->hBitmap = (HBITMAP)LoadImage(
        g_hInst,        // ���� ����(.exe)�� ���� �ڵ�
        filePath,       // �ε��� �̹���(���� ���)
        IMAGE_BITMAP,   // �ε��� �̹��� ����
        0, 0,           // �̹����� �ȼ� ��, ����
        LR_LOADFROMFILE // filePath�� ������ ���Ͽ��� stand-alone �̹����� �ε�
    );

    // �޸� DC�� ��Ʈ���� ���ҽ��� ��Ʈ������ ����
    SelectObject(gameObject->hMemDC, gameObject->hBitmap);

    // ���� ������Ʈ�� �ΰ� ������ �����Ѵ�
    GetObject(gameObject->hBitmap, sizeof(BITMAP), &tmpBitmap);
    gameObject->width = tmpBitmap.bmWidth;
    gameObject->height = tmpBitmap.bmHeight;
    gameObject->keyColor = COLORKEY_PINK;

    DeleteObject(gameObject->hBitmap);
}

/// <summary>
/// ��Ʈ�� �ϳ��� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
void DrawBitmap(int posX, int posY, GameObject* gameObject)
{
    DrawBitmap(posX, posY, gameObject, 0, 0);
}

/// <summary>
/// ��Ʈ�� �ϳ��� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
/// <param name="srcX">ǥ���� ��Ʈ�� X����</param>
/// <param name="srcY">ǥ���� ��Ʈ�� Y����</param>
void DrawBitmap(int posX, int posY, GameObject* gameObject, int srcX, int srcY)
{
   BitBlt(backbuf.hMemDC,                       // �����
        posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� �� ����
        gameObject->hMemDC,                     // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        SRCCOPY                                 // ��Ʈ�� ����(SRCCOPY: �簢������ ����)
    );
   // �浹 ���� ���� ����
   RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
   SetCollisionInfo(gameObject, collision);
}

/// <summary>
/// ��������Ʈ�� �ϳ� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
void DrawSprite(int posX, int posY, GameObject* gameObject)
{
    DrawSprite(posX, posY, gameObject, 0, 0);
}

/// <summary>
/// ��������Ʈ�� �ϳ� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
/// <param name="srcX">ǥ���� ��Ʈ�� X����</param>
/// <param name="srcY">ǥ���� ��Ʈ�� Y����</param>
void DrawSprite(int posX, int posY, GameObject* gameObject, int srcX, int srcY)
{
    TransparentBlt(backbuf.hMemDC,              // �����
        posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� �� ����
        gameObject->hMemDC,                     // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �� ����
        gameObject->keyColor                    // ������������ ����
    );
    // �浹 ���� ���� ����
    RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
    SetCollisionInfo(gameObject, collision);
}

/// <summary>
/// Ȯ��, ����� ��������Ʈ�� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
/// <param name="ratio">Ȯ��, ����� ����</param>
/// <param name="srcX">ǥ���� ��Ʈ�� X����</param>
/// <param name="srcY">ǥ���� ��Ʈ�� Y����</param>
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY)
{
    DrawStretchSprite(posX, posY, gameObject, ratioX, ratioY, 0, 0);
}

/// <summary>
/// Ȯ��, ����� ��������Ʈ�� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
/// <param name="ratio">Ȯ��, ����� ����</param>
/// <param name="srcX">ǥ���� ��Ʈ�� X����</param>
/// <param name="srcY">ǥ���� ��Ʈ�� Y����</param>
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY, int srcX, int srcY)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    
    // �ӽ� �޸�DC ����
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // �ӽ� �޸�DC�� �׸� �ӽ� ��Ʈ�� �ڵ� ����(�׸� ��Ʈ�� ũ�⸸ŭ)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);
    // �ӽ� �޸�DC�� �� ����
    PatBlt(tmpHDC, 0, 0, g_width, g_height, BLACKNESS);
    // �ӽ� �޸�DC�� ��Ʈ���� �ӽ� ��Ʈ�� �ڵ�� ����
    SelectObject(tmpHDC, tmpHbitmap);

    // �ӽ� ���ۿ� ��������Ʈ�� �׸���
    TransparentBlt(tmpHDC,                      // �ӽ� ����
        0, 0,                                   // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� ��/����
        gameObject->hMemDC,                     // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �� ����
        gameObject->keyColor                    // ������������ ����
    );

    int bmtWidth = (int)gameObject->width * ratioX;
    int bmtHeight = (int)gameObject->height * ratioY;

    // �ӽ� ������ ��������Ʈ�� Ȯ��, ����Ͽ� ����ۿ� �׸���
    StretchBlt(backbuf.hMemDC,                  // �����
        posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
        bmtWidth, bmtHeight,                    // ��Ʈ���� �׸� ��, ����(Ȯ��, ���)
        tmpHDC,                                 // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �� ����
        SRCCOPY                                 // ��Ʈ�� ����(SRCCOPY: �簢������ ����)
    );

    // �浹 ���� ���� ����
    RECT collision;
    collision.left = posX;
    collision.top = posY;
    collision.right = posX + bmtWidth;
    collision.bottom = posY + bmtHeight;
    SetCollisionInfo(gameObject, collision);
    
    // �ӽ� ������ �ڵ� ����
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
}


void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha)
{
    DrawAlphaSprite(posX, posY, gameObject, alpha, 0, 0);
}

/// <summary>
/// ���ĺ����� ������ ��������Ʈ�� �׸���
/// </summary>
/// <param name="posX">��Ʈ���� �׸� X��ǥ</param>
/// <param name="posY">��Ʈ���� �׸� Y��ǥ</param>
/// <param name="gameObject">�׸� ��Ʈ�� ����</param>
/// <param name="alpha">���� ��(����)</param>
/// <param name="srcX">ǥ���� ��Ʈ�� X����</param>
/// <param name="srcY">ǥ���� ��Ʈ�� Y����</param>
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha, int srcX, int srcY)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    BITMAP tmpBitmap;
    BLENDFUNCTION bf;

    // �ӽ� �޸�DC ����
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // �ӽ� �޸�DC�� �׸� �ӽ� ��Ʈ�� �ڵ� ����(�׸� ��Ʈ�� ũ�⸸ŭ)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);
    // �ӽ� �޸�DC�� �� ����(�ʱ�ȭ)
    PatBlt(tmpHDC, 0, 0, g_width, g_height, BLACKNESS);
    // �ӽ� �޸�DC�� ��Ʈ���� �ӽ� ��Ʈ�� �ڵ�� ����
    SelectObject(tmpHDC, tmpHbitmap);

    // �ӽ� ���ۿ� ��������Ʈ�� �׸���
    TransparentBlt(tmpHDC,                      // �ӽ� ����
        0, 0,                                   // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� ��/����
        gameObject->hMemDC,                     // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �� ����
        gameObject->keyColor                    // ������������ ����
    );

    // blend function�� ���� ���忡 ���� ���� ����
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    // ��Ʈ���� �ؼ��Ǵ� ����� ����(32��Ʈ ��Ʈ��: AC_SRC_ALPHA, �� ��: 0)
    bf.AlphaFormat = 0;
    // ��ü ���İ��� �����ϸ�, pre-multiplied�� �ݿ��ȴ�.
    bf.SourceConstantAlpha = alpha;	

    // ���ĺ���
    // ����! Per-Pixel Alpha Blending�� �Ϸ���,
    /// 1. ����ä���� �־�� ��. (32��Ʈ)
    /// 2. Pre-Multiplied Alpha Image�̾�� ��.
    AlphaBlend(backbuf.hMemDC,                  // �����
        posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� ��, ����(Ȯ��, ���)
        tmpHDC,                                 // ����� ��Ʈ���� ������ �ִ� DC
        srcX, srcY,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �� ����
        bf                                      // blend function
    );

    // �浹 ���� ���� ����
    RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
    SetCollisionInfo(gameObject, collision);

    //// �ӽ� ������ �ڵ� ����
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
}


void DrawRotateSpreite(int posX, int posY, GameObject* gameObject, int degree)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    int oldGraphicsMode;

    // �ӽ� �޸�DC ����
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // �ӽ� �޸�DC�� �׸� �ӽ� ��Ʈ�� �ڵ� ����(�׸� ��Ʈ�� ũ�⸸ŭ)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);

    XFORM xForm = { 1, 0, 0, 1, 0, 0 };

    int tempDegree = 30;

    // ȸ�� ��� ����
    xForm.eM11 = (float) cos(tempDegree * PI / 180);
    xForm.eM12 = (float) sin(tempDegree * PI / 180);
    xForm.eM21 = (float)-sin(tempDegree * PI / 180);
    xForm.eM22 = (float) cos(tempDegree * PI / 180);
    xForm.eDx = 0;
    xForm.eDy = 0;

    // �׷��� ��� ����
    // oldGraphicsMode = SetGraphicsMode(backbuf.hMemDC, GM_ADVANCED);
    oldGraphicsMode = SetGraphicsMode(g_hDCMain, GM_ADVANCED);
    
    //SetWorldTransform(backbuf.hMemDC, &xForm);
    SetWorldTransform(g_hDCMain, &xForm);

    BitBlt(backbuf.hMemDC,                       // �����
        posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
        gameObject->width, gameObject->height,  // ��Ʈ���� �׸� �� ����
        gameObject->hMemDC,                     // ����� ��Ʈ���� ������ �ִ� DC
        0, 0,                             // ����� ��Ʈ���� ���� ��� ��ǥ
        SRCCOPY                                 // ��Ʈ�� ����(SRCCOPY: �簢������ ����)
    );

    //BitBlt(g_hDCMain,                       // �����
    //    posX, posY,                             // ��Ʈ���� �׸� �»�� ��ǥ
    //    gameObject->width, gameObject->height,  // ��Ʈ���� �׸� �� ����
    //    tmpHDC,                     // ����� ��Ʈ���� ������ �ִ� DC
    //    0, 0,                             // ����� ��Ʈ���� ���� ��� ��ǥ
    //    SRCCOPY                                 // ��Ʈ�� ����(SRCCOPY: �簢������ ����)
    //);

    // �ӽ� ������ �ڵ� ����
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
    //SetGraphicsMode(backbuf.hMemDC, oldGraphicsMode);
    SetGraphicsMode(g_hDCMain, oldGraphicsMode);
}

/// <summary>
/// �޸�DC�� ���� DC�� Blit�� �Ѵ�(�����->ȭ��)
/// </summary>
void DrawAllToScreen()
{
    BitBlt(g_hDCMain, 0, 0, g_width, g_height, backbuf.hMemDC, 0, 0, SRCCOPY);
    // �ܻ� ������ ���� ������� �޸� DC�� �� �����ؼ� �ʱ�ȭ ��Ŵ
    //ClearBackbuf();
}