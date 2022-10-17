#include "Fmod/inc/fmod.hpp"
// 경로의 명시적 효과, 속성 창에 라이브러리 추가
#pragma	comment (lib, "Fmod/lib/fmodex_vc.lib")

using namespace FMOD;

#include "Fmod.h"

// STL 컨테이너
#include <map>

// FMOD 클래스
class FmodSound
{
	System* m_pSystem;
	int			m_Index;
	Channel* m_pBGChannel;
	float       m_vloum;

public:
	std::map<std::string, int >	m_CheckList;
	std::map<int, Sound* >		m_SoundList;

	FmodSound()
	{
		System_Create(&m_pSystem);
		// init(채널 최대설정값, 초기화 시점 , 추가로 넣을 보조 드라이버 );
		m_pSystem->init(10, FMOD_INIT_NORMAL, 0);

		m_Index = 0;
		m_vloum = 1.0f;
		m_pBGChannel = nullptr;
	}
	~FmodSound()
	{
		m_CheckList.clear();

		for (auto& Iter : m_SoundList)
		{
			Iter.second->release();
		}
		m_SoundList.clear();

		m_pSystem->release();
		m_pSystem->close();
	}

	// 사운드 추가 
	int	AddSoundFile(std::string _FullPath, bool _IsLoop)
	{
		auto Find = m_CheckList.find(_FullPath);

		if (Find != m_CheckList.end())
		{
			return Find->second;
		}

		Sound* pSound = nullptr;
		int		Mode = FMOD_HARDWARE | (_IsLoop ? FMOD_LOOP_NORMAL | FMOD_DEFAULT : FMOD_LOOP_OFF);

		// 사운드를 지정된 경로에서 두 번째 인자인 LOOP/ONCE 로 선택 해서 로딩
		m_pSystem->createSound(_FullPath.c_str(), Mode, 0, &pSound);

		if (pSound == nullptr)
		{
			return -1;
		}

		m_CheckList.insert(std::make_pair(_FullPath, m_Index));
		m_SoundList.insert(std::make_pair(m_Index, pSound));

		return m_Index++;
	}

	// 이펙트 플레이
	void EffectPlay(int _SoundNum)
	{
		auto Find = m_SoundList.find(_SoundNum);

		Channel* pChannel = nullptr;

		// playSound(채널그룹, sound, 일시정지? ,채널);
		m_pSystem->playSound(FMOD_CHANNEL_FREE, Find->second, 0, &pChannel);
	}

	// 배경음 플레이
	void BGMPlay(int _SoundNum)
	{
		auto Find = m_SoundList.find(_SoundNum);

		m_pSystem->playSound(FMOD_CHANNEL_REUSE, Find->second, 0, &m_pBGChannel);
	}

	// 배경음 멈춤
	void BGMStop()
	{
		if (m_pBGChannel != nullptr)
		{
			m_pBGChannel->stop();
		}
	}

	// 볼륨 다운
	void VolumeDown()
	{
		m_vloum -= 0.1f;
		if (m_vloum <= 0) m_vloum = 0;
		m_pBGChannel->setVolume(m_vloum);
	}

	// 볼륨 업
	void VolumeUp()
	{
		m_vloum += 0.1f;
		if (m_vloum >= 1.0) m_vloum = 1.0;
		m_pBGChannel->setVolume(m_vloum);
	}
};

static FmodSound g_SoundMgr;

// 전역 사운드 추가 함수
int	AddSoundFile(std::string _FullPath, bool _IsLoop)
{
	return g_SoundMgr.AddSoundFile(_FullPath, _IsLoop);
}

// 전역 이펙트 플레이 함수
void EffectPlay(int _SoundNum)
{
	g_SoundMgr.EffectPlay(_SoundNum);
}

// 전역 배경음 출력 함수
void BGMPlay(int _SoundNum)
{
	g_SoundMgr.BGMPlay(_SoundNum);
}

// 전역 볼륨 업
void VolumeUp()
{
	g_SoundMgr.VolumeUp();
}

// 전역 볼륨 다운
void VolumeDown()
{
	g_SoundMgr.VolumeDown();
}

// 전역 배경음 중지 함수
void BGMStop()
{
	g_SoundMgr.BGMStop();
}