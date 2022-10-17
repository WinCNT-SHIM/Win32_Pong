#pragma once
#include<stdio.h>
#include<windows.h>
#include<time.h>

void InitGame();//게임 초기화

void SetGameBoard();//게임화면크기 설정
void CursorView();//커서 지우기

//게임 구조관련 변수, 함수
extern bool g_GameRun;//게임 전원 변수
void DrawLobby();//로비화면출력
extern char g_gamemenu;//메뉴 스위치용 변수
void DrawLobbyMenu();//로비메뉴출력
extern unsigned char CursorX, CursorY;//커서위치변수
void InitCursor();//커서위치초기화
void DrawCursor();//커서출력
void EraseCursor();//커서삭제
void MoveCursor();//커서이동
void DrawRuleAndKey();//게임 방법 안내 화면
void EndMenuTrigger();//게임 종료 후 메뉴함수

void DrawGameBoard();//게임판 출력
void DrawWin32GameBoard();//게임판 출력(Win32API)

extern unsigned char Stick1X, Stick1Y, Stick2X, Stick2Y;//막대 좌표 변수
void InitSticks();//전 막대 초기화

void DrawStick1();//막대1표시
void EraseStick1();//막대1삭제
void MoveStick1();//막대1이동

void DrawStick2();//막대2표시
void EraseStick2();//막대2삭제
void MoveStick2();//막대2이동


extern unsigned char BallX, BallY, BallDir;//공좌표변수,공 방향변수
void ResetBall();//공초기화
void DrawBall();//공표시
void EraseBall();//공삭제

//충돌
bool CollStick1();
bool CollStick2();

extern char Score1, Score2;//점수 관련 변수

void ResetScores();//점수 초기화
void ShowScore();//점수표시
void Player1Score();//플레이어1득점
void Player2Score();//플레이어2득점

//엔딩
//void EndGame();
void Drawtrophy();//트로피


void GameMain();//전부 통합한 하나의 반복문
