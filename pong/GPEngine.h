#pragma once

#include <windows.h>
#include "GPDefine.h"

// 그래픽스 변수 초기화 함수
void InitializeGPEngine(HINSTANCE hInst, HWND hWnd, HDC hDC, int clientX, int clientY);

// 백버퍼 메모리DC를 초기화 한다
void ClearBackbuf();

// 게임 오브젝트(비트맵)에 충돌 관련 정보를 설정한다
void SetCollisionInfo(GameObject* gameObject, RECT collision);

// 리소스에서 비트맵을 로드한다.
void LoadGameObject_FromResource(GameObject* gameObject, int resID);
// 파일에서 게임 오브젝트(비트맵)을 로드한다
void LoadGameObject_FromFile2(GameObject* gameObject, char* filePath);

// 비트맵을 그린다
void DrawBitmap(int posX, int posY, GameObject* gameObject);
void DrawBitmap(int posX, int posY, GameObject* gameObject, int srcX, int srcY);

// 스프라이트를 그린다
void DrawSprite(int posX, int posY, GameObject* gameObject);
void DrawSprite(int posX, int posY, GameObject* gameObject, int srcX, int srcY);

// 확대, 축소한 스프라이트를 그린다
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY);
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY, int srcX, int srcY);

// 투명도를 설정한 스프라이트를 그린다
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha);
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha, int srcX, int srcY);

// 회전한 스프라이트를 그린다
void DrawRotateSpreite(int posX, int posY, GameObject* gameObject, int degree);

// 메모리DC를 메인 DC에 Blit을 한다(백버퍼->화면)
void DrawAllToScreen();