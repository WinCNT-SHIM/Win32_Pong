#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "pong_functions.h"
#include "fmod.h"

int FmodBGMSound = AddSoundFile("./Sound/BGM.mp3", true);

int main()
{
	BGMPlay(FmodBGMSound);

	InitGame();//�ʱ�ȭ

	GameMain();//��������

	return 0;
}