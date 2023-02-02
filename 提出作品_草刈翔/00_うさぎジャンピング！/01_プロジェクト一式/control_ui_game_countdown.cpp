//=============================================================================
// �Q�[��UI�F�J�E���g�_�E�����쏈�� [control_ui_game_countdown.h]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_ui_game_countdown.h"
#include "manager.h"
#include "ui.h"
#include "scene.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CControlUiGameCountdown::m_nCount = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlUiGameCountdown::CControlUiGameCountdown()
{
	// �ϐ��̃N���A
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlUiGameCountdown::~CControlUiGameCountdown()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlUiGameCountdown::Init(void)
{
	// �ϐ��̏�����
	m_nCount = 3;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlUiGameCountdown::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlUiGameCountdown::Update(CScene *pScene)
{
	// �����̃|�C���^��UI�N���X�̃|�C���^�ɃL���X�g
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;
	
	switch (m_nCount)
	{
	case 3:
		pUi->SetTexture(0.0f, 0.0f, 1, 4);
		break;

	case 2:
		pUi->SetTexture(0.0f, 1.0f, 1, 4);
		break;

	case 1:
		pUi->SetTexture(0.0f, 2.0f, 1, 4);
		break;

	case 0:
		pUi->SetTexture(0.0f, 3.0f, 1, 4);
		break;

	default:
		break;
	}
}

//=============================================================================
// ��������
//=============================================================================
CControlUiGameCountdown *CControlUiGameCountdown::Create(void)
{
	// �C���X�^���X�̐���
	CControlUiGameCountdown *pControlUiGameCountdown = NULL;

	// �k���`�F�b�N
	if (pControlUiGameCountdown == NULL)
	{
		// �N���X�̐���
		pControlUiGameCountdown = new CControlUiGameCountdown;
		if (pControlUiGameCountdown != NULL)
		{
			// ����������
			pControlUiGameCountdown->Init();
		}
	}

	return pControlUiGameCountdown;
}

//=============================================================================
// �J�E���g�ݒ菈��
//=============================================================================
void CControlUiGameCountdown::SetCount(int nCount)
{
	m_nCount = nCount;
}