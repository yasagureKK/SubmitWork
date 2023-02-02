//=============================================================================
// 1�l�p�Q�[�����j���[��� [solo_menu.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "solo_menu.h"
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
float CSoloMenu::m_fScroll = 0;
int CSoloMenu::m_nSelect = 1;
int CSoloMenu::m_nDifficulty = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSoloMenu::CSoloMenu()
{
	// �ϐ��̃N���A
	m_fScroll = 0;
	m_pScene2D = NULL;
	//memset(&m_apUi, NULL, sizeof(m_apUi));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSoloMenu::~CSoloMenu()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSoloMenu::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_fScroll = 0;

	// ���j���[��ʂɖ߂�����I�����ŏ��ɖ߂�
	if (CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		m_nSelect = 1;
	}

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_SOLOMENU);
	pSound->ControllVoice(CSound::SOUND_LABEL_SOLOMENU, 0.4f);

	// �w�i
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UI�̐���  |
	//+------------+
	// �g�b�v
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_TOP);
	// CPU�Ƃ�������
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 3, SOLO_MENU_SELECT_POS_Y + 120.0f, 0.0f),
		D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_CPU);
	// �X�R�A�A�^�b�N
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 3, SOLO_MENU_SELECT_POS_Y + 120.0f * 2, 0.0f),
		D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_SCORE);
	// ���ǂ�{�^��
	m_apUi[3] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSoloMenu::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSoloMenu::Update(void)
{
	// �J�E���^�[�̍X�V
	m_fScroll += 0.001f;

	// �w�i���X�N���[��������
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

	for (int nCntUi = 1; nCntUi < 4; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			// UI�̃e�N�X�`���̐ݒ�
			m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);

			// �I������Ă����
			if (nCntUi == m_nSelect)
			{
				if (nCntUi != 3)
				{
					// �T�C�Y�������傫������
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X + 40.0f, SOLO_MENU_SELECT_SIZE_Y + 10.0f, 0.0f));
				}

				// ���点��
				m_apUi[nCntUi]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				if (nCntUi != 3)
				{
					// �T�C�Y��߂�
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f));
				}
			}
		}
	}

	// CPU�֘A��UI�𐶐�
	if (m_apUi[4] == NULL && m_nSelect == 1)
	{
		// CPU�̂悳(�����o��)
		m_apUi[4] = CUi::Create(D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_POS_X, SOLO_MENU_DIFFICULTY_00_POS_Y, 0.0f),
			D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_SIZE_X, SOLO_MENU_DIFFICULTY_00_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_DIFFICULTY_00);
		// CPU�̂悳(����)
		m_apUi[5] = CUi::Create(D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_POS_X + 37.0f, SOLO_MENU_DIFFICULTY_00_POS_Y + 40.0f, 0.0f),
			D3DXVECTOR3(260.0f, 90.0f, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_DIFFICULTY_01);
	}
	else if (m_apUi[4] != NULL && m_nSelect != 1)
	{
		for (int nCntUi = 4; nCntUi < 6; nCntUi++)
		{
			// UI�̔j��
			m_apUi[nCntUi]->Uninit();
			delete m_apUi[nCntUi];
			m_apUi[nCntUi] = NULL;
		}
	}

	if (m_apUi[5] != NULL)
	{
		// CPU�̋�����ύX
		m_apUi[5]->SetTexture(0, (float)m_nDifficulty, 1, 4);
	}

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
			int nUp, nDown, nLeft, nRight;

			if (nNum == 0)
			{
				nUp = DIK_W;
				nDown = DIK_S;
				nLeft = DIK_A;
				nRight = DIK_D;
			}
			else if (nNum == 1)
			{
				nUp = DIK_UP;
				nDown = DIK_DOWN;
				nLeft = DIK_LEFT;
				nRight = DIK_RIGHT;
			}

			//***********************************************************************
			// �I�� (1P:W/S�A2P:��/�� �܂��� �p�b�h���X�e�B�b�N/�\���L�[)
			//***********************************************************************
			//��
			if (pInputKeyboard->GetTrigger(nUp) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP, nNum) == true)
			{
				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSelect--;
			}
			//��
			else if (pInputKeyboard->GetTrigger(nDown) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nNum) == true)
			{
				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSelect++;
			}

			// �I�����̐���
			if (m_nSelect < 1)
			{
				m_nSelect = 3;
			}
			else if (m_nSelect > 3)
			{
				m_nSelect = 1;
			}

			if (m_nSelect == 1)
			{
				//***********************************************************************
				// �I�� (1P:A/D�A2P:��/�� �܂��� �p�b�h���X�e�B�b�N/�\���L�[)
				//***********************************************************************
				//��
				if (pInputKeyboard->GetTrigger(nLeft) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nNum) == true)
				{
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CURSOR);
					pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

					m_nDifficulty--;
				}
				//�E
				else if (pInputKeyboard->GetTrigger(nRight) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nNum) == true)
				{
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CURSOR);
					pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

					m_nDifficulty++;
				}

				// �I�����̐���
				if (m_nDifficulty < 0)
				{
					m_nDifficulty = 3;
				}
				else if (m_nDifficulty > 3)
				{
					m_nDifficulty = 0;
				}
			}

			//***********************************************************************
			// ���� (Enter/Space �܂��� �p�b�hA�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_SOLOMENU);

				// m_nSelect�̒l�Ńt�F�[�h���ύX
				switch (m_nSelect)
				{
				case 1:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// CPU�ΐ�Q�[����ʂɑJ��
					pFade->SetFade(CManager::MODE_SOLO_CPU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 2:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 1�l�p�Q�[����ʂɑJ��
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 3:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// ���j���[��ʂɑJ��
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				default:
					break;
				}
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// �߂� (BackSpace �܂��� �p�b�hX�{�^��)
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				m_nSelect = 3;

				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_SOLOMENU);

				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// ���j���[��ʂɑJ��
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSoloMenu::Draw(void)
{

}

//=============================================================================
// ��Փx�̎擾����
//=============================================================================
int CSoloMenu::GetDifficulty(void)
{
	return m_nDifficulty;
}