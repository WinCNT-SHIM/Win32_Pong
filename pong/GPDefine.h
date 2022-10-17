#pragma once

#include <windows.h>

// Į��Ű(��ũ) ����
#define COLORKEY_PINK RGB(255,0,255)
#define COLORKEY_GREEN RGB(0, 255, 0)

typedef struct _tagGameObject
{
    // �⺻ �ڵ�
    HDC     hMemDC;     // ��Ʈ���� ����Ǵ� �޸�DC�� �ڵ�
    HBITMAP hBitmap;    // ��Ʈ�� ���ҽ��� �ڵ�

    // ��Ʈ�� ����
    int width;          // ��
    int height;         // ����

    // ��������Ʈ ����
    int keyColor;       // Ű Į��

    // �ִϸ��̼� ����
    int centerPosX;     // ��Ʈ���� �߽�
    int centerPosY;     // (ĳ������ �� ��ġ ���)
    int delayFrame;     // �ִϸ��̼��� Frame

    // ���� ����
    RECT collisionHit;  // �浹 ����1(eg. �ǰ�)
    RECT collisionAtk;  // �浹 ����2(eg. ����)
    int damage;         // ��ݷ�
} GameObject;