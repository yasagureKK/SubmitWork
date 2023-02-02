//=============================================================================
// �X�^�[�gUI���� [start.cpp]
// Author : ���� ��
//=============================================================================
#include "start.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUIStart::CUIStart()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_expansionSize = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	m_bStartMoment = false;
	m_nWaitTime = 0;
	m_fAlpha = 0.0f;
	m_fExpansion = 0.0f;
	m_phase = START_PHASE_READY;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUIStart::~CUIStart()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUIStart::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_bStartMoment = false;
	m_nWaitTime = 0;
	m_fAlpha = 0.0f;
	m_fExpansion = 1.5f;
	m_phase = START_PHASE_READY;
	m_expansionSize = m_size * m_fExpansion;

	// Ready?�`Go!��UI
	m_apUI = CUI::Create(m_pos, m_expansionSize, 39, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUIStart::Uninit(void)
{
	// UI�̏I������
	if (m_apUI != NULL)
	{
		m_apUI->Uninit();
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUIStart::Update(void)
{
	m_bStartMoment = false;

	// �t�F�[�h�擾����
	CFade *pFade = CManager::GetFade();

	// �t�F�[�h���Ă��Ȃ����̂�
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		switch (m_phase)
		{
		case START_PHASE_READY:
			// �T�C�Y�E�J���[�̐ݒ�
			m_expansionSize = m_size * m_fExpansion;
			m_apUI->SetSize(m_expansionSize);
			m_apUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

			// �g�嗦�̕ύX
			m_fExpansion -= 0.5f / 30;

			if (m_fExpansion <= 1.0f)
			{
				m_fExpansion = 1.0f;
			}

			// �A���t�@�x�̕ύX
			m_fAlpha += 1.0f / 30;

			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;
				m_nWaitTime++;
			}

			// 1�b�o������
			if (m_nWaitTime >= 60)
			{
				// �摜�̃e�N�X�`�����W��؂�ւ���
				m_apUI->SetTexture(0.0f, 1.0f, 1, 2);

				// �X�^�[�g�����u�Ԃɂ���
				m_bStartMoment = true;

				// �t�F�[�Y��؂�ւ���s
				m_phase = START_PHASE_GO;
			}
			break;

		case START_PHASE_GO:
			// �T�C�Y�E�J���[�̐ݒ�
			m_expansionSize = m_size * m_fExpansion;
			m_apUI->SetSize(m_expansionSize);
			m_apUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

			// �T�C�Y�̕ύX
			m_fExpansion += 0.5f / 60;

			if (m_fExpansion >= 1.5f)
			{
				m_fExpansion = 1.5f;
			}

			// �A���t�@�x�̕ύX
			m_fAlpha -= 1.0f / 60;

			// �A���t�@�x��0�ȉ��ɂȂ�����
			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;

				// �I������
				Uninit();
			}
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUIStart::Draw(void)
{
	
}

//=============================================================================
// ��������
//=============================================================================
CUIStart *CUIStart::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �N���X�̐���
	CUIStart *pUIStart = NULL;
	pUIStart = new CUIStart;

	if (pUIStart != NULL)
	{
		// ����������
		pUIStart->Init(pos, size);
	}
	return pUIStart;
}