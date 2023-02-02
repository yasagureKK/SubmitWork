//=============================================================================
// �T�E���h�e�X�g��� [soundtest.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "soundtest.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"
#include "number_sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CSoundTest::m_nSelect = 0;
float CSoundTest::m_fScroll = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSoundTest::CSoundTest()
{
	// �ϐ��̃N���A
	m_fScroll = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pSoundNumber = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSoundTest::~CSoundTest()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSoundTest::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_nSelect = 1;
	m_fScroll = 0;

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_SOUNDTEST);
	pSound->ControllVoice(CSound::SOUND_LABEL_SOUNDTEST, 0.4f);

	// �w�i
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	// �g�b�v
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_TOP);
	// ���ǂ�{�^��
	m_apUi[1] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);
	// �Đ��{�^��
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_PLAY);
	// ���C��
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SOUNDTEST_UI_MAIN_SIZE_X, SOUNDTEST_UI_MAIN_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_MAIN);

	// �T�E���h�i���o�[
	m_pSoundNumber = CNumberSound::Create(D3DXVECTOR3(SOUNDTEST_UI_NUMBER_POS_X, SOUNDTEST_UI_NUMBER_POS_Y, 0.0f), D3DXVECTOR3(SOUNDTEST_UI_NUMBER_SIZE_X, SOUNDTEST_UI_NUMBER_SIZE_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));

	// �{�^���͈Â����Ă���
	m_apUi[1]->SetTexture(0.0f, 0.0f, 1, 2);
	m_apUi[2]->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSoundTest::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSoundTest::Update(void)
{
	// �J�E���^�[�̍X�V
	m_fScroll += 0.001f;

	// �w�i���X�N���[��������
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

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

	// �t�F�[�h���Ă��Ȃ����̂ݑ���ł���
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			//***********************************************************************
			// �߂� (BackSpace �܂��� �p�b�hX�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				// ���点��
				m_apUi[1]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGM�̒�~
				pSound->Stop();

				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// ���j���[��ʂɑJ��
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
			}

			//***********************************************************************
			// ���� (Enter/Space �܂��� �p�b�hA�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetPress(DIK_RETURN) == true ||
				pInputKeyboard->GetPress(DIK_SPACE) == true ||
				pInputPad->GetButtonPress(XINPUT_GAMEPAD_A, 0) == true ||
				pInputPad->GetButtonPress(XINPUT_GAMEPAD_A, 1) == true)
			{
				// ���点��
				m_apUi[2]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				// �Â�����
				m_apUi[2]->SetTexture(0.0f, 0.0f, 1, 2);
			}
		}
	}
}

//================================================
// �`�揈��
//================================================
void CSoundTest::Draw(void)
{

}