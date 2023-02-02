//=============================================================================
// �^�C�g��UI�F�G���^�[�������쏈�� [control_ui_title_enter.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_ui_title_enter.h"
#include "manager.h"
#include "ui.h"
#include "scene.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlUiTitleEnter::CControlUiTitleEnter()
{
	// �ϐ��̃N���A
	m_nCount = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bAlphaUp = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlUiTitleEnter::~CControlUiTitleEnter()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlUiTitleEnter::Init(void)
{
	// �ϐ��̏�����
	m_nCount = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bAlphaUp = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlUiTitleEnter::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlUiTitleEnter::Update(CScene *pScene)
{
	// �����̃|�C���^��UI�N���X�̃|�C���^�ɃL���X�g
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;

	m_nCount++;

	// �A���t�@�l��������(�����ɂ���)
	if (m_bAlphaUp == false)
	{
		if (m_nCount % 2 == 0)
		{
			m_col.a += -0.05f;
		}

		if (m_col.a < 0)
		{
			m_nCount = 0;
			m_col.a = 0;
			m_bAlphaUp = true;
		}
	}
	// �A���t�@�l���グ��(�s�����ɂ���)
	else if (m_bAlphaUp == true)
	{
		if (m_nCount % 2 == 0)
		{
			m_col.a += 0.05f;
		}

		if (m_col.a > 1.0f && m_nCount >= 70)
		{
			m_nCount = 0;
			m_col.a = 1.0f;
			m_bAlphaUp = false;
		}
	}

	// m_fAlpha�̒l�𓧖��x�ɓ����
	pUi->SetColor(m_col);
}

//=============================================================================
// ��������
//=============================================================================
CControlUiTitleEnter *CControlUiTitleEnter::Create(void)
{
	// �C���X�^���X�̐���
	CControlUiTitleEnter *pControlUiTitleEnter = NULL;

	// �k���`�F�b�N
	if (pControlUiTitleEnter == NULL)
	{
		// �N���X�̐���
		pControlUiTitleEnter = new CControlUiTitleEnter;
		if (pControlUiTitleEnter != NULL)
		{
			// ����������
			pControlUiTitleEnter->Init();
		}
	}

	return pControlUiTitleEnter;
}