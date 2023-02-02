//=============================================================================
// �^�C�g��UI�F�G���^�[�������쏈�� [control_ui_title_enter.h]
// Author : ���� ��
//=============================================================================
#ifndef _CONTROL_UI_TITLE_ENTER_H_
#define _CONTROL_UI_TITLE_ENTER_H_

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
class CControlUiTitleEnter : public CControl
{
public:
	CControlUiTitleEnter();						// �R���X�g���N�^
	~CControlUiTitleEnter();					// �f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(CScene *pScene);				// �X�V����
	static CControlUiTitleEnter *Create(void);	// ��������

private:
	int m_nCount;								// �J�E���g
	D3DXCOLOR m_col;							// �J���[
	bool m_bAlphaUp;							// �A���t�@�x���グ�邩�ǂ���
};

#endif	//_CONTROL_UI_TITLE_ENTER_H_