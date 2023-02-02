//=============================================================================
// �}�E�X���͏��� [mouse.h]
// Author : ���� ��
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CMouse : public CInput
{
public:
	// ���͏��
	typedef enum
	{
		MOUSE_INPUT_LEFT = 0,	// ���{�^��
		MOUSE_INPUT_RIGHT,		// �E�{�^��
		MOUSE_INPUT_WHEEL,		// �z�C�[��(�����{�^��)
		MOUSE_INPUT_MAX
	} MOUSE_INPUT;

	CMouse();										// �R���X�g���N�^
	~CMouse();										// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	bool GetPress(MOUSE_INPUT nType);				// ���͏�Ԃ̎擾����(�v���X)
	bool GetTrigger(MOUSE_INPUT nType);				// ���͏�Ԃ̎擾����(�g���K�[)

private:
	DIMOUSESTATE  m_aMouseState;					// �}�E�X�̏��
	DIMOUSESTATE  m_aMouseStateTrigger;				// �}�E�X�̏��(�g���K�[)
	bool m_bConnect;								// �ڑ����Ă��邩�ǂ���
	HWND m_hWnd;									// �ۑ��p�̃n���h��
};
#endif	// _MOUSE_H_