#pragma once

#include <string>

int		AddSoundFile(std::string _FullPath, bool IsLoop = false);
void	EffectPlay(int _SoundNum);
void	BGMPlay(int _SoundNum);
void	VolumeUp();
void	VolumeDown();
void	BGMStop();