//=============================================================================
// �T�E���h���� [sound.h]
// Author : ���� ��
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �T�E���h�N���X
//*****************************************************************************
class CSound
{
public:
	// �T�E���h�t�@�C��
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,		// BGM�F�^�C�g��
		SOUND_LABEL_MENU,			// BGM�F���j���[
		SOUND_LABEL_TUTORIAL,		// BGM�F��񂵂イ���[�h
		SOUND_LABEL_SOLOMENU,		// BGM�F�ЂƂ�悤���j���[
		SOUND_LABEL_CPU,			// BGM�FCPU��������
		SOUND_LABEL_GAME,			// BGM�F�X�R�A�A�^�b�N
		SOUND_LABEL_PVP,			// BGM�F�ӂ���悤���[�h
		SOUND_LABEL_RESULT,			// BGM�F���U���g
		SOUND_LABEL_RANKING,		// BGM�F�����L���O
		SOUND_LABEL_SOUNDTEST,		// BGM�F�T�E���h�e�X�g
		SOUND_LABEL_TRAILER,		// BGM�F����pBGM
		SOUND_LABEL_TITLE_ENTER,	//  SE�F�^�C�g���G���^�[
		SOUND_LABEL_CURSOR,			//  SE�F�J�[�\��
		SOUND_LABEL_DECIDE,			//  SE�F����
		SOUND_LABEL_CANCEL,			//  SE�F�L�����Z��
		SOUND_LABEL_ACCELERATION,	//  SE�F����
		SOUND_LABEL_JUMP,			//  SE�F�W�����v
		SOUND_LABEL_LANDING,		//  SE�F���n
		SOUND_LABEL_SPIN,			//  SE�F��]
		SOUND_LABEL_RISE,			//  SE�F�㏸
		SOUND_LABEL_COUNTDOWN,		//  SE�F�J�E���g�_�E��
		SOUND_LABEL_START,			//  SE�F�X�^�[�g
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();												// �R���X�g���N�^
	~CSound();												// �f�X�g���N�^
	HRESULT Init(HWND hWnd);								// ����������
	void Uninit(void);										// �I������
	HRESULT Play(SOUND_LABEL label);						// �Z�O�����g�Đ�(��~)
	void Stop(SOUND_LABEL label);							// ��~
	void Stop(void);										// ��~
	void ControllVoice(SOUND_LABEL label, float fVolume)	// ���ʒ���(���x�����w�肵�A1.0����{�̉��ʂƂ��Ē���)
	{
		m_apSourceVoice[label]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char *filename;		// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	static PARAM m_aParam[SOUND_LABEL_MAX];							// �e���f�ނ̃p�����[�^
};

#endif	// _SOUND_H_