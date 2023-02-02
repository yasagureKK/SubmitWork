//=============================================================================
// �^�C�g����� [title.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "texture.h"
#include "scene2D.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
float CTitle::m_fScroll = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	// �ϐ��̃N���A
	m_fScroll = 0;
	m_nFadeCount = 0;
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	memset(&m_apUi, NULL, sizeof(m_apUi));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_fScroll = 0;
	m_nFadeCount = 0;

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_TITLE);
	pSound->ControllVoice(CSound::SOUND_LABEL_TITLE, 0.4f);

	// �w�i
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D[0]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_BG00));
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D[1]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_BG01));

	//+------------+
	//|  UI�̐���  |
	//+------------+
	// �^�C�g�����S(�㏸���铮�������邽�߁A��ʉ��ɐ���)
	m_apUi[0] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + TITLE_LOGO_POS_Y, 0.0f),
		D3DXVECTOR3(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_TITLE_LOGO);
	// �G���^�[����
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLE_ENTER_POS_Y, 0.0f),
		D3DXVECTOR3(TITLE_ENTER_SIZE_X, TITLE_ENTER_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_TITLE_ENTER);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �w�i�X�N���[���J�E���g�̍X�V
	m_fScroll += 0.001f;

	// �w�i���X�N���[��������
	m_pScene2D[0]->SetTexture(m_fScroll / 4, 1.0f, 1, 1);

	// �J�ڗp�J�E���g�̍X�V
	m_nFadeCount++;

	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade;
	pFade = CManager::GetFade();

	// �t�F�[�h���Ă��Ȃ����̂�
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			//***********************************************************************
			// ���ւ����� (Enter/Space �܂��� �p�b�hA�{�^��/X�{�^��/START�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
			{
				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_TITLE);
				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_TITLE_ENTER);
				pSound->ControllVoice(CSound::SOUND_LABEL_TITLE_ENTER, 0.4f);
				// ���j���[��ʂɑJ��
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				// ���O�̃��[�h��ݒ�
				CManager::SetBeforeMode(CManager::MODE_TITLE);
			}
		}

		// �����I�ɑJ��
		if (m_nFadeCount >= TITLE_FADE_COUNT)
		{
			// BGM�̒�~
			pSound->Stop(CSound::SOUND_LABEL_TITLE);

			// �����L���O��ʂɑJ��
			pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}