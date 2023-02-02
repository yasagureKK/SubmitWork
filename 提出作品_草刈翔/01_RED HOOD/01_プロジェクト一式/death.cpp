//=============================================================================
// ���S���� [death.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "death.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DEATH_DISPLAY_COUNT		 (50)		// �摜�\���܂ł̎���
#define DEATH_TRANS_COUNT		 (180)		// ��ʑJ�ڂ܂ł̎���
#define UI_GAMEOVER_SIZE_X		 (700.0f)	// �Q�[���I�[�o�[�����̃T�C�YX
#define UI_GAMEOVER_SIZE_Y		 (180.0f)	// �Q�[���I�[�o�[�����̃T�C�YY

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDeath::CDeath()
{
	// �ϐ��̃N���A
	memset(&m_apUI, NULL, sizeof(m_apUI));
	m_nTrans = 0;
	m_bDisplay = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDeath::~CDeath()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDeath::Init(void)
{
	// �ϐ��̏�����
	m_nTrans = 0;
	m_bDisplay = false;

	// �w�i
	m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 22, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	// ����
	m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(UI_GAMEOVER_SIZE_X, UI_GAMEOVER_SIZE_Y), 23, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDeath::Uninit(void)
{
	// UI�̏I������
	for (int nCntUI = 0; nCntUI < DEATH_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDeath::Update(void)
{
	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade = CManager::GetFade();

	// �J�E���g��i�߂�
	m_nTrans++;

	// �摜��\�����Ă��Ȃ���
	if (m_bDisplay == false)
	{
		// ���̎��Ԃ��o������
		if (m_nTrans >= DEATH_DISPLAY_COUNT)
		{
			// �T�E���h�̒�~
			pSound->Stop();

			// �Q�[���I�[�o�[SE�̍Đ�
			pSound->Play(15);
			pSound->ControllVoice(15, 0.6f);

			m_nTrans = 0;
			m_bDisplay = true;
		}
	}
	else if (m_bDisplay == true)
	{
		// �摜�̃A���t�@�x��ύX���A�摜���������\��������
		for (int nCntDeath = 0; nCntDeath < DEATH_MAX_UI; nCntDeath++)
		{
			int nAlpha = 2 * m_nTrans;

			if (nAlpha >= 255)
			{
				nAlpha = 255;
			}

			m_apUI[nCntDeath]->SetColor(D3DCOLOR_RGBA(255, 255, 255, nAlpha));
		}

		// ���̎��Ԃ��o���A�t�F�[�h���Ă��Ȃ��Ȃ�
		if (m_nTrans >= DEATH_TRANS_COUNT && pFade->GetFade() == CFade::FADE_NONE)
		{
			// �^�C�g����ʂɑJ��
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDeath::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CDeath *CDeath::Create(void)
{	
	// �N���X�̐���
	CDeath *pDeath = NULL;
	pDeath = new CDeath;

	if (pDeath != NULL)
	{
		// ����������
		pDeath->Init();
	}
	return pDeath;
}