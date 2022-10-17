#include <math.h>
#include "GPEngine.h"
// TransparentBlt 함수를 사용하기 위해 선언
#pragma comment(lib, "Msimg32.lib")

#define PI  (double)3.14159265

// ==================== 전역 변수 선언 ====================
HINSTANCE   g_hInst;        // 인스턴스 핸들
HWND        g_hWnd;         // 윈도우의 핸들
HDC         g_hDCMain;      // 메인 DC(화면에 게임을 그리는 용도)
GameObject  backbuf;        // 백버퍼
int         g_width;     // 윈도우의 가로 길이
int         g_height;    // 윈도우의 세로 길이

/// <summary>
/// 그래픽스 모듈에 필요한 전역 변수들을 획득(윈도우, 그리기 화면을 1개로 설정)
/// </summary>
/// <param name="hInst">Win32프로그램의 시작 주소 값</param>
/// <param name="hWnd">Win32프로그램의 윈도우 핸들(식별자)</param>
/// <param name="hDC">윈도우DC의 핸들</param>
/// <param name="g_WinWidth">윈도우의 가로 길이</param>
/// <param name="g_WinHeight">윈도우의 세로 길이</param>
void InitializeGPEngine(HINSTANCE hInst, HWND hWnd, HDC hDC, int width, int height)
{
    g_hInst = hInst;
    g_hWnd = hWnd;
    g_hDCMain = hDC;
    g_width = width;
    g_height = height;

    // 백퍼버의 비트맵을 생성
    backbuf.hMemDC = CreateCompatibleDC(g_hDCMain);
    backbuf.hBitmap = CreateCompatibleBitmap(g_hDCMain, g_width, g_height);
    /// 메모리 DC의 색 지정
    PatBlt(backbuf.hMemDC, 0, 0, g_width, g_height, BLACKNESS);
    // 메모리 DC의 비트맵을 백버퍼의 비트맵으로 변경
    SelectObject(backbuf.hMemDC, backbuf.hBitmap);
    backbuf.width = g_width;
    backbuf.height = g_height;
}

/// <summary>
/// 백버퍼 메모리DC를 초기화 한다
/// </summary>
void ClearBackbuf()
{
    PatBlt(backbuf.hMemDC, 0, 0, g_width, g_height, BLACKNESS);
}

/// <summary>
/// 게임 오브젝트(비트맵)에 충돌 관련 정보를 설정한다
/// </summary>
/// <param name="gameObject">대상 게임 오브젝트</param>
/// <param name="collision">충돌 범위 정보</param>
void SetCollisionInfo(GameObject* gameObject, RECT collision)
{
    gameObject->collisionHit = collision;
}

/// <summary>
/// 리소스에서 게임 오브젝트(비트맵)를 로드한다
/// </summary>
/// <param name="bitmap">대상 비트맵의 정보</param>
/// <param name="resID">대상 리소스의 ID</param>
void LoadGameObject_FromResource(GameObject* gameObject, int resID)
{
    BITMAP tmpBitmap;

    // 메인 DC와 호환되는 메모리 DC 생성
    gameObject->hMemDC = CreateCompatibleDC(g_hDCMain);
    // 리소스에서 비트맵을 로드
    gameObject->hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(resID));
    // 메모리 DC의 비트맵을 리소스의 비트맵으로 변경
    SelectObject(gameObject->hMemDC, gameObject->hBitmap);

    // 게임 오브젝트에 부가 정보를 대입한다
    GetObject(gameObject->hBitmap, sizeof(BITMAP), &tmpBitmap);
    gameObject->width = tmpBitmap.bmWidth;
    gameObject->height = tmpBitmap.bmHeight;
    gameObject->keyColor = COLORKEY_PINK;
    // 비트맵 핸들 해제
    DeleteObject(gameObject->hBitmap);
}

/// <summary>
/// 파일에서 게임 오브젝트(비트맵)을 로드
/// </summary>
/// <param name="bitmap"></param>
/// <param name="filePath"></param>
void LoadGameObject_FromFile2(GameObject* gameObject, char* filePath)
{
    BITMAP tmpBitmap;

    // 메인 DC와 호환되는 메로리 DC 생성
    gameObject->hMemDC = CreateCompatibleDC(g_hDCMain);
    // 아이콘, 커서, 애니메이션 커서, 비트맵을 로드한다
    gameObject->hBitmap = (HBITMAP)LoadImage(
        g_hInst,        // 실행 파일(.exe)에 대한 핸들
        filePath,       // 로드할 이미지(파일 경로)
        IMAGE_BITMAP,   // 로드할 이미지 유형
        0, 0,           // 이미지의 픽셀 폭, 높이
        LR_LOADFROMFILE // filePath에 지정된 파일에서 stand-alone 이미지를 로드
    );

    // 메모리 DC의 비트맵을 리소스의 비트맵으로 변경
    SelectObject(gameObject->hMemDC, gameObject->hBitmap);

    // 게임 오브젝트에 부가 정보를 대입한다
    GetObject(gameObject->hBitmap, sizeof(BITMAP), &tmpBitmap);
    gameObject->width = tmpBitmap.bmWidth;
    gameObject->height = tmpBitmap.bmHeight;
    gameObject->keyColor = COLORKEY_PINK;

    DeleteObject(gameObject->hBitmap);
}

/// <summary>
/// 비트맵 하나를 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
void DrawBitmap(int posX, int posY, GameObject* gameObject)
{
    DrawBitmap(posX, posY, gameObject, 0, 0);
}

/// <summary>
/// 비트맵 하나를 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
/// <param name="srcX">표시할 비트맵 X범위</param>
/// <param name="srcY">표시할 비트맵 Y범위</param>
void DrawBitmap(int posX, int posY, GameObject* gameObject, int srcX, int srcY)
{
   BitBlt(backbuf.hMemDC,                       // 백버퍼
        posX, posY,                             // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭 넓이
        gameObject->hMemDC,                     // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        SRCCOPY                                 // 비트맵 색상(SRCCOPY: 사각형으로 복사)
    );
   // 충돌 관련 정보 설정
   RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
   SetCollisionInfo(gameObject, collision);
}

/// <summary>
/// 스프라이트를 하나 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
void DrawSprite(int posX, int posY, GameObject* gameObject)
{
    DrawSprite(posX, posY, gameObject, 0, 0);
}

/// <summary>
/// 스프라이트를 하나 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
/// <param name="srcX">표시할 비트맵 X범위</param>
/// <param name="srcY">표시할 비트맵 Y범위</param>
void DrawSprite(int posX, int posY, GameObject* gameObject, int srcX, int srcY)
{
    TransparentBlt(backbuf.hMemDC,              // 백버퍼
        posX, posY,                             // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭 넓이
        gameObject->hMemDC,                     // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        gameObject->width, gameObject->height,  // 비트맵의 폭 넓이
        gameObject->keyColor                    // 스프라이프할 색상
    );
    // 충돌 관련 정보 설정
    RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
    SetCollisionInfo(gameObject, collision);
}

/// <summary>
/// 확대, 축소한 스프라이트를 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
/// <param name="ratio">확대, 축소할 비율</param>
/// <param name="srcX">표시할 비트맵 X범위</param>
/// <param name="srcY">표시할 비트맵 Y범위</param>
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY)
{
    DrawStretchSprite(posX, posY, gameObject, ratioX, ratioY, 0, 0);
}

/// <summary>
/// 확대, 축소한 스프라이트를 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
/// <param name="ratio">확대, 축소할 비율</param>
/// <param name="srcX">표시할 비트맵 X범위</param>
/// <param name="srcY">표시할 비트맵 Y범위</param>
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY, int srcX, int srcY)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    
    // 임시 메모리DC 생성
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // 임시 메모리DC에 그릴 임시 비트맵 핸들 생성(그릴 비트맵 크기만큼)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);
    // 임시 메모리DC의 색 지정
    PatBlt(tmpHDC, 0, 0, g_width, g_height, BLACKNESS);
    // 임시 메모리DC의 비트맵을 임시 비트맵 핸들로 변경
    SelectObject(tmpHDC, tmpHbitmap);

    // 임시 버퍼에 스프라이트를 그리기
    TransparentBlt(tmpHDC,                      // 임시 버퍼
        0, 0,                                   // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭/높이
        gameObject->hMemDC,                     // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        gameObject->width, gameObject->height,  // 비트맵의 폭 넓이
        gameObject->keyColor                    // 스프라이프할 색상
    );

    int bmtWidth = (int)gameObject->width * ratioX;
    int bmtHeight = (int)gameObject->height * ratioY;

    // 임시 버퍼의 스프라이트를 확대, 축소하여 백버퍼에 그리기
    StretchBlt(backbuf.hMemDC,                  // 백버퍼
        posX, posY,                             // 비트맵을 그릴 좌상단 좌표
        bmtWidth, bmtHeight,                    // 비트맵을 그릴 폭, 높이(확대, 축소)
        tmpHDC,                                 // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        gameObject->width, gameObject->height,  // 비트맵의 폭 넓이
        SRCCOPY                                 // 비트맵 색상(SRCCOPY: 사각형으로 복사)
    );

    // 충돌 관련 정보 설정
    RECT collision;
    collision.left = posX;
    collision.top = posY;
    collision.right = posX + bmtWidth;
    collision.bottom = posY + bmtHeight;
    SetCollisionInfo(gameObject, collision);
    
    // 임시 버퍼의 핸들 해제
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
}


void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha)
{
    DrawAlphaSprite(posX, posY, gameObject, alpha, 0, 0);
}

/// <summary>
/// 알파블랜딩을 적용한 스프라이트를 그린다
/// </summary>
/// <param name="posX">비트맵을 그릴 X좌표</param>
/// <param name="posY">비트맵을 그릴 Y좌표</param>
/// <param name="gameObject">그릴 비트맵 정보</param>
/// <param name="alpha">알파 값(투명도)</param>
/// <param name="srcX">표시할 비트맵 X범위</param>
/// <param name="srcY">표시할 비트맵 Y범위</param>
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha, int srcX, int srcY)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    BITMAP tmpBitmap;
    BLENDFUNCTION bf;

    // 임시 메모리DC 생성
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // 임시 메모리DC에 그릴 임시 비트맵 핸들 생성(그릴 비트맵 크기만큼)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);
    // 임시 메모리DC의 색 지정(초기화)
    PatBlt(tmpHDC, 0, 0, g_width, g_height, BLACKNESS);
    // 임시 메모리DC의 비트맵을 임시 비트맵 핸들로 변경
    SelectObject(tmpHDC, tmpHbitmap);

    // 임시 버퍼에 스프라이트를 그리기
    TransparentBlt(tmpHDC,                      // 임시 버퍼
        0, 0,                                   // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭/높이
        gameObject->hMemDC,                     // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        gameObject->width, gameObject->height,  // 비트맵의 폭 넓이
        gameObject->keyColor                    // 스프라이프할 색상
    );

    // blend function에 알파 블렌드에 대한 값을 설정
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    // 비트맵이 해석되는 방식을 제어(32비트 비트맵: AC_SRC_ALPHA, 그 외: 0)
    bf.AlphaFormat = 0;
    // 전체 알파값을 조절하며, pre-multiplied에 반영된다.
    bf.SourceConstantAlpha = alpha;	

    // 알파블랜딩
    // 주의! Per-Pixel Alpha Blending을 하려면,
    /// 1. 알파채널이 있어야 함. (32비트)
    /// 2. Pre-Multiplied Alpha Image이어야 함.
    AlphaBlend(backbuf.hMemDC,                  // 백버퍼
        posX, posY,                             // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭, 높이(확대, 축소)
        tmpHDC,                                 // 복사될 비트맵을 가지고 있는 DC
        srcX, srcY,                             // 복사될 비트맵의 좌측 상단 좌표
        gameObject->width, gameObject->height,  // 비트맵의 폭 넓이
        bf                                      // blend function
    );

    // 충돌 관련 정보 설정
    RECT collision = { posX, posY, posX + gameObject->width, posY + gameObject->height };
    SetCollisionInfo(gameObject, collision);

    //// 임시 버퍼의 핸들 해제
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
}


void DrawRotateSpreite(int posX, int posY, GameObject* gameObject, int degree)
{
    HDC tmpHDC;
    HBITMAP tmpHbitmap;
    int oldGraphicsMode;

    // 임시 메모리DC 생성
    tmpHDC = CreateCompatibleDC(g_hDCMain);
    // 임시 메모리DC에 그릴 임시 비트맵 핸들 생성(그릴 비트맵 크기만큼)
    tmpHbitmap = CreateCompatibleBitmap(g_hDCMain, gameObject->width, gameObject->height);

    XFORM xForm = { 1, 0, 0, 1, 0, 0 };

    int tempDegree = 30;

    // 회전 행렬 적용
    xForm.eM11 = (float) cos(tempDegree * PI / 180);
    xForm.eM12 = (float) sin(tempDegree * PI / 180);
    xForm.eM21 = (float)-sin(tempDegree * PI / 180);
    xForm.eM22 = (float) cos(tempDegree * PI / 180);
    xForm.eDx = 0;
    xForm.eDy = 0;

    // 그래픽 모드 변경
    // oldGraphicsMode = SetGraphicsMode(backbuf.hMemDC, GM_ADVANCED);
    oldGraphicsMode = SetGraphicsMode(g_hDCMain, GM_ADVANCED);
    
    //SetWorldTransform(backbuf.hMemDC, &xForm);
    SetWorldTransform(g_hDCMain, &xForm);

    BitBlt(backbuf.hMemDC,                       // 백버퍼
        posX, posY,                             // 비트맵을 그릴 좌상단 좌표
        gameObject->width, gameObject->height,  // 비트맵을 그릴 폭 넓이
        gameObject->hMemDC,                     // 복사될 비트맵을 가지고 있는 DC
        0, 0,                             // 복사될 비트맵의 좌측 상단 좌표
        SRCCOPY                                 // 비트맵 색상(SRCCOPY: 사각형으로 복사)
    );

    //BitBlt(g_hDCMain,                       // 백버퍼
    //    posX, posY,                             // 비트맵을 그릴 좌상단 좌표
    //    gameObject->width, gameObject->height,  // 비트맵을 그릴 폭 넓이
    //    tmpHDC,                     // 복사될 비트맵을 가지고 있는 DC
    //    0, 0,                             // 복사될 비트맵의 좌측 상단 좌표
    //    SRCCOPY                                 // 비트맵 색상(SRCCOPY: 사각형으로 복사)
    //);

    // 임시 버퍼의 핸들 해제
    DeleteObject(tmpHbitmap);
    DeleteDC(tmpHDC);
    //SetGraphicsMode(backbuf.hMemDC, oldGraphicsMode);
    SetGraphicsMode(g_hDCMain, oldGraphicsMode);
}

/// <summary>
/// 메모리DC를 메인 DC에 Blit을 한다(백버퍼->화면)
/// </summary>
void DrawAllToScreen()
{
    BitBlt(g_hDCMain, 0, 0, g_width, g_height, backbuf.hMemDC, 0, 0, SRCCOPY);
    // 잔상 방지를 위해 백버퍼인 메모리 DC을 색 지정해서 초기화 시킴
    //ClearBackbuf();
}