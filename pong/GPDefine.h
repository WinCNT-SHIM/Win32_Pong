#pragma once

#include <windows.h>

// 칼라키(핑크) 설정
#define COLORKEY_PINK RGB(255,0,255)
#define COLORKEY_GREEN RGB(0, 255, 0)

typedef struct _tagGameObject
{
    // 기본 핸들
    HDC     hMemDC;     // 비트맵이 내장되는 메모리DC의 핸들
    HBITMAP hBitmap;    // 비트맵 리소스의 핸들

    // 비트맵 관련
    int width;          // 폭
    int height;         // 길이

    // 스프라이트 관련
    int keyColor;       // 키 칼라

    // 애니메이션 관련
    int centerPosX;     // 비트맵의 중심
    int centerPosY;     // (캐릭터의 발 위치 등등)
    int delayFrame;     // 애니메이션의 Frame

    // 게임 관련
    RECT collisionHit;  // 충돌 영역1(eg. 피격)
    RECT collisionAtk;  // 충돌 영역2(eg. 공격)
    int damage;         // 충격량
} GameObject;