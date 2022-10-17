#pragma once
#include<stdio.h>
#include<windows.h>
#include<time.h>

void InitGame();//���� �ʱ�ȭ

void SetGameBoard();//����ȭ��ũ�� ����
void CursorView();//Ŀ�� �����

//���� �������� ����, �Լ�
extern bool g_GameRun;//���� ���� ����
void DrawLobby();//�κ�ȭ�����
extern char g_gamemenu;//�޴� ����ġ�� ����
void DrawLobbyMenu();//�κ�޴����
extern unsigned char CursorX, CursorY;//Ŀ����ġ����
void InitCursor();//Ŀ����ġ�ʱ�ȭ
void DrawCursor();//Ŀ�����
void EraseCursor();//Ŀ������
void MoveCursor();//Ŀ���̵�
void DrawRuleAndKey();//���� ��� �ȳ� ȭ��
void EndMenuTrigger();//���� ���� �� �޴��Լ�

void DrawGameBoard();//������ ���
void DrawWin32GameBoard();//������ ���(Win32API)

extern unsigned char Stick1X, Stick1Y, Stick2X, Stick2Y;//���� ��ǥ ����
void InitSticks();//�� ���� �ʱ�ȭ

void DrawStick1();//����1ǥ��
void EraseStick1();//����1����
void MoveStick1();//����1�̵�

void DrawStick2();//����2ǥ��
void EraseStick2();//����2����
void MoveStick2();//����2�̵�


extern unsigned char BallX, BallY, BallDir;//����ǥ����,�� ���⺯��
void ResetBall();//���ʱ�ȭ
void DrawBall();//��ǥ��
void EraseBall();//������

//�浹
bool CollStick1();
bool CollStick2();

extern char Score1, Score2;//���� ���� ����

void ResetScores();//���� �ʱ�ȭ
void ShowScore();//����ǥ��
void Player1Score();//�÷��̾�1����
void Player2Score();//�÷��̾�2����

//����
//void EndGame();
void Drawtrophy();//Ʈ����


void GameMain();//���� ������ �ϳ��� �ݺ���
