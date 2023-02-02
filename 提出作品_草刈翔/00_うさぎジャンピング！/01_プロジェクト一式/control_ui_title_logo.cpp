//=============================================================================
// �^�C�g�����S�R���g���[������ [control_title_logo.cpp]
// Author : KUSAKARI KAKERU
//=============================================================================
#include "control_ui_title_logo.h"
#include "manager.h"
#include "ui.h"
#include "title.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlUiTitleLogo::CControlUiTitleLogo()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlUiTitleLogo::~CControlUiTitleLogo()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlUiTitleLogo::Init(void)
{
	m_fMaxMove = 1.5f;
	m_bOnce = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlUiTitleLogo::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlUiTitleLogo::	Update(CScene *pScene)
{
	// �����̃|�C���^��UI�N���X�̃|�C���^�ɃL���X�g
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;

	//�I�u�W�F�N�g�̈ʒu�E�T�C�Y���擾
	D3DXVECTOR3 pos;
	pos = pUi->GetPos();
	D3DXVECTOR3 size;
	size = pUi->GetSize();

	if (m_bOnce == false && pos.y >= SCREEN_HEIGHT)
	{
		m_move.y -= TITLE_LOGO_RISE;
		m_bOnce = true;
	}
	else if (m_move.y >= 0.0f && pos.y >= TITLE_LOGO_POS_Y)
	{
		m_move.y = 0.0f;
		m_move.y -= TITLE_LOGO_RISE_SECOND;
	}
	else
	{
		//��ɉ��ɉ�����
		m_move.y += TITLE_LOGO_GRAVITY;
	}

	// �ʒu��T�C�Y�̐ݒ�
	pUi->SetPos(pos, size);
}

//=============================================================================
// ��������
//=============================================================================
CControlUiTitleLogo *CControlUiTitleLogo::Create()
{
	//�C���X�^���X�̐���
	CControlUiTitleLogo *pControlTitleLogo = NULL;

	if (pControlTitleLogo == NULL)
	{
		pControlTitleLogo = new CControlUiTitleLogo;

		if (pControlTitleLogo != NULL)
		{
			pControlTitleLogo->Init();
		}
	}

	return pControlTitleLogo;
}