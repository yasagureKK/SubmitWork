//=============================================================================
// �T�E���h�i���o�[���� [number_sound.h]
// Authore : ���� ��
//=============================================================================
#ifndef _NUMBER_SOUND_H_
#define _NUMBER_SOUND_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_NUMBER_SOUND_DIGIT (3)	// �Ȃ̍ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CNumberSound : public CScene
{
public:
	CNumberSound(int nPriority = PRIORITY_UI);			// �R���X�g���N�^
	~CNumberSound();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CNumberSound *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// ��������
	void SetNumber(int nNumber);						// �i���o�[�ݒ菈��

private:
	void PlaySound(int nNum);							// �T�E���h�Đ�����

	CNumber *m_apNumber[MAX_NUMBER_SOUND_DIGIT];		// �i���o�[�̃|�C���^
	int m_nSoundNumber;									// ���݂̐�
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXCOLOR m_col;									// �J���[
};

#endif	//_NUMBER_SOUND_H_