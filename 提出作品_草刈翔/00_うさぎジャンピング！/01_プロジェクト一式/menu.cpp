//=============================================================================
// ���j���[��� [menu.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "menu.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CMenu::m_nSelect = 1;
float CMenu::m_fScroll = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMenu::CMenu()
{
	// �ϐ��̃N���A
	m_fScroll = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_apGuideUi = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMenu::~CMenu()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMenu::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_fScroll = 0;

	// �^�C�g����ʂɖ߂�����I�����ŏ��ɖ߂�
	if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
	{
		m_nSelect = 1;
	}

	// ���O�̃��[�h��ݒ�
	CManager::SetBeforeMode(CManager::MODE_MENU);

	// �T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_MENU);
	pSound->ControllVoice(CSound::SOUND_LABEL_MENU, 0.4f);

	// �w�i
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UI�̐���  |
	//+------------+
	// �g�b�v
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_TOP);
	// �`���[�g���A�����[�h
	m_apUi[1] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 2, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_TUTORIAL);
	// 1�l�p���[�h
	m_apUi[2] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 3, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_SINGLE);
	// 2�l�p���[�h
	m_apUi[3] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 4, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_PVP);
	// �����L���O���[�h
	m_apUi[4] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 5, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_RANKING);
	// �T�E���h�e�X�g���[�h
	m_apUi[5] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 6, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_SOUNDTEST);
	// ���ǂ�{�^��
	m_apUi[6] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMenu::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMenu::Update(void)
{
	// �J�E���^�[�̍X�V
	m_fScroll += 0.001f;

	// �w�i���X�N���[��������
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

	for (int nCntUi = 1; nCntUi < MENU_MAX_UI; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			// UI�̃e�N�X�`���̐ݒ�
			m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);

			// �I������Ă����
			if (nCntUi == m_nSelect)
			{
				if (nCntUi != 6)
				{
					// �T�C�Y�������傫������
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(MENU_SELECT_SIZE_X + 40.0f, MENU_SELECT_SIZE_Y + 10.0f, 0.0f));
				}

				// ���点��
				m_apUi[nCntUi]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				if (nCntUi != 6)
				{
					// �T�C�Y��߂�
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f));
				}
			}
		}
	}

	// ����UI�̉摜�؂�ւ�����
	GuideChange();

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
			int nUp, nDown;

			if (nNum == 0)
			{
				nUp = DIK_W;
				nDown = DIK_S;
			}
			else if (nNum == 1)
			{
				nUp = DIK_UP;
				nDown = DIK_DOWN;
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

				// ����UI���k������Ȃ�������
				if (m_apGuideUi != NULL)
				{
					// ����UI�̔j��
					m_apGuideUi->Uninit();
					delete m_apGuideUi;
					m_apGuideUi = NULL;
				}
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

				// ����UI���k������Ȃ�������
				if (m_apGuideUi != NULL)
				{
					// ����UI�̔j��
					m_apGuideUi->Uninit();
					delete m_apGuideUi;
					m_apGuideUi = NULL;
				}
			}

			// �I�����̐���
			if (m_nSelect < 1)
			{
				m_nSelect = 6;
			}
			else if (m_nSelect > 6)
			{
				m_nSelect = 1;
			}

			//***********************************************************************
			// ���� (Enter/Space �܂��� �p�b�hA�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_MENU);

				// m_nSelect�̒l�Ńt�F�[�h���ύX
				switch (m_nSelect)
				{
				case 1:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// ���O�̃��[�h�����j���[�ɐݒ�(�����̕\���̗L������ʂ��邽��)
					CManager::SetBeforeMode(CManager::MODE_MENU);

					// �`���[�g���A����ʂɑJ��
					pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 2:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 1�l�p���j���|��ʂɑJ��
					pFade->SetFade(CManager::MODE_SOLO_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 3:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 2�l�p�Q�[����ʂɑJ��
					pFade->SetFade(CManager::MODE_PVP, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 4:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// �����L���O��ʂɑJ��
					pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 5:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// �T�E���h�e�X�g��ʂɑJ��
					pFade->SetFade(CManager::MODE_SOUNDTEST, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 6:
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// �^�C�g����ʂɑJ��
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
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
				m_nSelect = 6;

				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_MENU);

				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// �^�C�g����ʂɑJ��
				pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMenu::Draw(void)
{

}

//=============================================================================
// ����UI�̉摜�؂�ւ�����
//=============================================================================
void CMenu::GuideChange(void)
{
	// �k���̎���������
	if (m_apGuideUi == NULL)
	{
		// �I�����Ă�ԍ��ŏ�����ς���
		switch (m_nSelect)
		{
			// ��񂵂イ���[�h
		case 1:
			// ����UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f),
				PRIORITY_UI, CUi::UI_TYPE_MENU_GUIDE_TUTORIAL);
			break;
			// �ЂƂ�悤���[�h
		case 2:
			// ����UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI, 
				CUi::UI_TYPE_MENU_GUIDE_SINGLE);
			break;
			// �ӂ���悤���[�h
		case 3:
			// ����UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_PVP);
			break;
			// �����L���O���[�h
		case 4:
			// ����UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_RANKING);
			break;
			// �T�E���h�e�X�g���[�h
		case 5:
			// ����UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_SOUNDTEST);
			break;
		default:
			break;
		}
	}
}