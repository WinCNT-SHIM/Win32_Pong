#include "Fmod/inc/fmod.hpp"
// ����� ����� ȿ��, �Ӽ� â�� ���̺귯�� �߰�
#pragma	comment (lib, "Fmod/lib/fmodex_vc.lib")

using namespace FMOD;

#include "Fmod.h"

// STL �����̳�
#include <map>

// FMOD Ŭ����
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
		// init(ä�� �ִ뼳����, �ʱ�ȭ ���� , �߰��� ���� ���� ����̹� );
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

	// ���� �߰� 
	int	AddSoundFile(std::string _FullPath, bool _IsLoop)
	{
		auto Find = m_CheckList.find(_FullPath);

		if (Find != m_CheckList.end())
		{
			return Find->second;
		}

		Sound* pSound = nullptr;
		int		Mode = FMOD_HARDWARE | (_IsLoop ? FMOD_LOOP_NORMAL | FMOD_DEFAULT : FMOD_LOOP_OFF);

		// ���带 ������ ��ο��� �� ��° ������ LOOP/ONCE �� ���� �ؼ� �ε�
		m_pSystem->createSound(_FullPath.c_str(), Mode, 0, &pSound);

		if (pSound == nullptr)
		{
			return -1;
		}

		m_CheckList.insert(std::make_pair(_FullPath, m_Index));
		m_SoundList.insert(std::make_pair(m_Index, pSound));

		return m_Index++;
	}

	// ����Ʈ �÷���
	void EffectPlay(int _SoundNum)
	{
		auto Find = m_SoundList.find(_SoundNum);

		Channel* pChannel = nullptr;

		// playSound(ä�α׷�, sound, �Ͻ�����? ,ä��);
		m_pSystem->playSound(FMOD_CHANNEL_FREE, Find->second, 0, &pChannel);
	}

	// ����� �÷���
	void BGMPlay(int _SoundNum)
	{
		auto Find = m_SoundList.find(_SoundNum);

		m_pSystem->playSound(FMOD_CHANNEL_REUSE, Find->second, 0, &m_pBGChannel);
	}

	// ����� ����
	void BGMStop()
	{
		if (m_pBGChannel != nullptr)
		{
			m_pBGChannel->stop();
		}
	}

	// ���� �ٿ�
	void VolumeDown()
	{
		m_vloum -= 0.1f;
		if (m_vloum <= 0) m_vloum = 0;
		m_pBGChannel->setVolume(m_vloum);
	}

	// ���� ��
	void VolumeUp()
	{
		m_vloum += 0.1f;
		if (m_vloum >= 1.0) m_vloum = 1.0;
		m_pBGChannel->setVolume(m_vloum);
	}
};

static FmodSound g_SoundMgr;

// ���� ���� �߰� �Լ�
int	AddSoundFile(std::string _FullPath, bool _IsLoop)
{
	return g_SoundMgr.AddSoundFile(_FullPath, _IsLoop);
}

// ���� ����Ʈ �÷��� �Լ�
void EffectPlay(int _SoundNum)
{
	g_SoundMgr.EffectPlay(_SoundNum);
}

// ���� ����� ��� �Լ�
void BGMPlay(int _SoundNum)
{
	g_SoundMgr.BGMPlay(_SoundNum);
}

// ���� ���� ��
void VolumeUp()
{
	g_SoundMgr.VolumeUp();
}

// ���� ���� �ٿ�
void VolumeDown()
{
	g_SoundMgr.VolumeDown();
}

// ���� ����� ���� �Լ�
void BGMStop()
{
	g_SoundMgr.BGMStop();
}