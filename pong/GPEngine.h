#pragma once

#include <windows.h>
#include "GPDefine.h"

// �׷��Ƚ� ���� �ʱ�ȭ �Լ�
void InitializeGPEngine(HINSTANCE hInst, HWND hWnd, HDC hDC, int clientX, int clientY);

// ����� �޸�DC�� �ʱ�ȭ �Ѵ�
void ClearBackbuf();

// ���� ������Ʈ(��Ʈ��)�� �浹 ���� ������ �����Ѵ�
void SetCollisionInfo(GameObject* gameObject, RECT collision);

// ���ҽ����� ��Ʈ���� �ε��Ѵ�.
void LoadGameObject_FromResource(GameObject* gameObject, int resID);
// ���Ͽ��� ���� ������Ʈ(��Ʈ��)�� �ε��Ѵ�
void LoadGameObject_FromFile2(GameObject* gameObject, char* filePath);

// ��Ʈ���� �׸���
void DrawBitmap(int posX, int posY, GameObject* gameObject);
void DrawBitmap(int posX, int posY, GameObject* gameObject, int srcX, int srcY);

// ��������Ʈ�� �׸���
void DrawSprite(int posX, int posY, GameObject* gameObject);
void DrawSprite(int posX, int posY, GameObject* gameObject, int srcX, int srcY);

// Ȯ��, ����� ��������Ʈ�� �׸���
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY);
void DrawStretchSprite(int posX, int posY, GameObject* gameObject, float ratioX, float ratioY, int srcX, int srcY);

// ������ ������ ��������Ʈ�� �׸���
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha);
void DrawAlphaSprite(int posX, int posY, GameObject* gameObject, int alpha, int srcX, int srcY);

// ȸ���� ��������Ʈ�� �׸���
void DrawRotateSpreite(int posX, int posY, GameObject* gameObject, int degree);

// �޸�DC�� ���� DC�� Blit�� �Ѵ�(�����->ȭ��)
void DrawAllToScreen();