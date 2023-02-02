//=============================================================================
// �T�E���h���� [sound.h]
// Author : ���� ��
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SOUND (64)

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
	CSound();										// �R���X�g���N�^
	~CSound();										// �f�X�g���N�^
	HRESULT Init(HWND hWnd);						// ����������
	void Uninit(void);								// �I������
	HRESULT Play(int nLabel);						// �Z�O�����g�Đ�(��~)
	void Stop(int nLabel);							// ��~
	void Stop(void);								// ��~
	void ControllVoice(int nLabel, float fVolume)	// ���ʒ�������
	{
		m_apSourceVoice[nLabel]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char filename[128];	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;		// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[MAX_SOUND];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[MAX_SOUND];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[MAX_SOUND];					// �I�[�f�B�I�f�[�^�T�C�Y
	static PARAM m_aParam[MAX_SOUND];						// �e���f�ނ̃p�����[�^
};
#endif	// _SOUND_H_