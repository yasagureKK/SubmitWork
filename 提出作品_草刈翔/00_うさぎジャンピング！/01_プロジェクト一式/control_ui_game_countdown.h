//=============================================================================
// �Q�[��UI�F�J�E���g�_�E�����쏈�� [control_ui_game_countdown.h]
// Author : ���� ��
//=============================================================================
#ifndef _CONTROL_UI_GAME_COUNTDOWN_H_
#define _CONTROL_UI_GAME_COUNTDOWN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CControlUiGameCountdown : public CControl
{
public:
	CControlUiGameCountdown();						// �R���X�g���N�^
	~CControlUiGameCountdown();						// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(CScene *pScene);					// �X�V����
	static CControlUiGameCountdown *Create(void);	// ��������
	static void SetCount(int nCount);				// �J�E���g�ݒ菈��

private:
	static int m_nCount;							// �J�E���g
	D3DXCOLOR m_col;								// �J���[
};

#endif	//_CONTROL_UI_GAME_COUNTDOWN_H_