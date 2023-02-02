//=============================================================================
// �^�C�g����� [title.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "ui.h"
#include "ranking.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CTitle::m_nSelect = 0;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_LOGO_POS_Y		(210.0f)	// �^�C�g�����S��Y���W�ʒu
#define TITLE_LOGO_SIZE_X		(640.0f)	// �^�C�g�����S�̃T�C�YX
#define TITLE_LOGO_SIZE_Y		(330.0f)	// �^�C�g�����S�̃T�C�YY
#define TITLE_PRESS_POS_Y		(520.0f)	// �v���X������Y���W�ʒu
#define TITLE_PRESS_SIZE_X		(860.0f)	// �v���X�����̃T�C�YX
#define TITLE_PRESS_SIZE_Y		(120.0f)	// �v���X�����̃T�C�YY
#define TITLE_START_POS_Y		(430.0f)	// �X�^�[�g������Y���W�ʒu
#define TITLE_START_SIZE_X		(570.0f)	// �X�^�[�g�����̃T�C�YX
#define TITLE_START_SIZE_Y		(90.0f)		// �X�^�[�g�����̃T�C�YY
#define TITLE_SELECT_SPACE		(80.0f)		// �I�����̊Ԋu
#define PAUSE_BACK_POS_X		(240.0f)	// �߂��X���W�ʒu
#define PAUSE_BACK_POS_Y		(660.0f)	// �߂��Y���W�ʒu
#define PAUSE_BACK_SIZE_X		(430.0f)	// �߂�̃T�C�YX
#define PAUSE_BACK_SIZE_Y		(80.0f)		// �߂�̃T�C�YY
#define PAUSE_PAGINATION_POS_X	(1020.0f)	// �y�[�W�����X���W�ʒu
#define PAUSE_PAGINATION_POS_Y	(660.0f)	// �y�[�W�����Y���W�ʒu
#define PAUSE_PAGINATION_SIZE_X (490.0f)	// �y�[�W����̃T�C�YX
#define PAUSE_PAGINATION_SIZE_Y (80.0f)		// �y�[�W����̃T�C�YY
#define PAUSE_HOWTO_TEXTURE		(41)		// �V�ѕ��̃e�N�X�`���ԍ�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	// �ϐ��̃N���A
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apSelectUI, NULL, sizeof(m_apSelectUI));
	m_pRanking = NULL;
	m_Phase = TITLE_PHASE_PRESS_START;
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
HRESULT CTitle::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_nSelect = 0;
	m_nHowtoTex = PAUSE_HOWTO_TEXTURE;
	m_Phase = TITLE_PHASE_PRESS_START;

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �^�C�g��BGM�̍Đ�
	pSound->Play(0);
	pSound->ControllVoice(0, 0.4f);

	// �w�i�̐���
	CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �^�C�g�����S�̐���
	CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_LOGO_POS_Y), D3DXVECTOR2(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y), 1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// UI�̐���
	UiCreate(m_Phase, false);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// UI�̔j��
	for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �I������UI�̔j��
	for (int nCntIcon = 0; nCntIcon < TITLE_MAX_SELECT; nCntIcon++)
	{
		if (m_apSelectUI[nCntIcon] != NULL)
		{
			m_apSelectUI[nCntIcon]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();
	// �Q�[���p�b�h�擾����
	CMouse *pMouse = CManager::GetMouse();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �t�F�[�h�擾����
	CFade *pFade = CManager::GetFade();

	// ���݂̃t�F�[�Y�ŏ�����ς���
	if (m_Phase == TITLE_PHASE_PRESS_START)
	{
		// �t�F�[�h���Ă��Ȃ����̂�
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// PressStart (�L�[�{�[�h�̉��炩�̃L�[ �܂��� �p�b�h�̉��炩�̑���)
			//***********************************************************************
			if (pKeyboard->GetAnyKey() == true ||
				pGamePad->GetAnyButton(0) == true)
			{
				// ����SE�̍Đ�
				pSound->Play(28);
				pSound->ControllVoice(28, 0.4f);

				// �t�F�[�Y�����ڑI���ɂ���
				m_Phase = TITLE_PHASE_MODE_SELECT;

				// UI�̐���
				UiCreate(m_Phase, false);
			}
		}
	}
	else if (m_Phase == TITLE_PHASE_MODE_SELECT)
	{
		for (int nCntUi = 0; nCntUi < TITLE_MAX_SELECT; nCntUi++)
		{
			if (m_apSelectUI[nCntUi] != NULL)
			{
				// �I������Ă���UI��
				if (nCntUi == m_nSelect)
				{
					// �����x��0��
					m_apSelectUI[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

					// �T�C�Y�������傫������
					D3DXVECTOR3 pos = m_apSelectUI[nCntUi]->GetPos();
					m_apSelectUI[nCntUi]->SetSize(D3DXVECTOR2(TITLE_START_SIZE_X + 40.0f, TITLE_START_SIZE_Y + 10.0f));

				}
				// �I������Ă��Ȃ�UI��
				else
				{
					// �����ɂ���
					m_apSelectUI[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));

					// �T�C�Y��߂�
					D3DXVECTOR3 pos = m_apSelectUI[nCntUi]->GetPos();
					m_apSelectUI[nCntUi]->SetSize(D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y));
				}
			}
		}
		// �t�F�[�h���Ă��Ȃ����̂�
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// �I�� (�L�[�{�[�h�v/�r �܂��� �p�b�h���X�e�B�b�N/�\���L�[)
			//***********************************************************************
			//��
			if (pKeyboard->GetTrigger(DIK_W) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true)
			{
				// �I��SE�̍Đ�
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);

				m_nSelect--;

			}
			//��
			else if (pKeyboard->GetTrigger(DIK_S) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN, 0) == true)
			{
				// �I��SE�̍Đ�
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);

				m_nSelect++;
			}

			// �I�����̐���
			if (m_nSelect < 0)
			{
				m_nSelect = TITLE_MAX_SELECT - 1;
			}
			else if (m_nSelect >= TITLE_MAX_SELECT)
			{
				m_nSelect = 0;
			}

			//***********************************************************************
			// ���� (�L�[�{�[�hEnter/Space �܂��� �p�b�hA/START�{�^��)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pKeyboard->GetTrigger(DIK_RETURN) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
			{
				// m_nSelect�̒l�Ńt�F�[�h���ύX
				switch (m_nSelect)
				{
				case 0:
					// �T�E���h�̒�~
					pSound->Stop();

					// ����SE�̍Đ�
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// �Q�[����ʂɑJ��
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 1:
					// ����SE�̍Đ�
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// �t�F�[�Y�𑀍�K�C�h�ɂ���
					m_Phase = TITLE_PHASE_HOWTO;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						// UI�̐���
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						// UI�̐���
						UiCreate(m_Phase, true);
					}
					break;

				case 2:
					// ����SE�̍Đ�
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// �t�F�[�Y�𑀍�K�C�h�ɂ���
					m_Phase = TITLE_PHASE_GUIDE;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						// UI�̐���
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						// UI�̐���
						UiCreate(m_Phase, true);
					}
					break;

				case 3:
					// ����SE�̍Đ�
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// �t�F�[�Y�������L���O�ɂ���
					m_Phase = TITLE_PHASE_RANKING;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						UiCreate(m_Phase, true);
					}

					break;

				default:
					break;
				}
			}

			//***********************************************************************
			// �߂� (�L�[�{�[�hBackspace �܂��� �p�b�hX�{�^�� �܂��� �}�E�X�E�N���b�N)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_BACK) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
				pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
			{
				// �L�����Z��SE�̍Đ�
				pSound->Play(29);
				pSound->ControllVoice(29, 0.6f);

				// �t�F�[�Y���X�^�[�g�҂��ɂ���
				m_Phase = TITLE_PHASE_PRESS_START;

				// UI�̐���
				UiCreate(m_Phase, false);
			}
		}
	}
	else if (m_Phase == TITLE_PHASE_HOWTO)
	{
		//***********************************************************************
		// �y�[�W���� (�L�[�{�[�h�`/�c �܂��� �p�b�h���X�e�B�b�N/�\���L�[)
		//***********************************************************************
		//��
		if (pKeyboard->GetTrigger(DIK_A) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, 0) == true)
		{
			// �e�N�X�`���ԍ������炷
			m_nHowtoTex--;

			if (m_nHowtoTex < PAUSE_HOWTO_TEXTURE)
			{
				m_nHowtoTex = PAUSE_HOWTO_TEXTURE;
			}
			else
			{
				// �I��SE�̍Đ�
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);
			}

			// �e�N�X�`����ݒ�
			m_apUI[0]->BindTexture(m_nHowtoTex);
		}
		//�E
		else if (pKeyboard->GetTrigger(DIK_D) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, 0) == true)
		{
			// �e�N�X�`���ԍ��𑝂₷
			m_nHowtoTex++;

			if (m_nHowtoTex >= PAUSE_HOWTO_TEXTURE + 3)
			{
				m_nHowtoTex = PAUSE_HOWTO_TEXTURE + 2;
			}
			else
			{
				// �I��SE�̍Đ�
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);
			}

			// �e�N�X�`����ݒ�
			m_apUI[0]->BindTexture(m_nHowtoTex);
		}
	}
	
	if (m_Phase == TITLE_PHASE_HOWTO || m_Phase == TITLE_PHASE_GUIDE || m_Phase == TITLE_PHASE_RANKING )
	{
		//***********************************************************************
		// �߂� (�L�[�{�[�hBackspace �܂��� �p�b�hX�{�^�� �܂��� �}�E�X�E�N���b�N)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_BACK) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
			pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
		{
			// �L�����Z��SE�̍Đ�
			pSound->Play(29);
			pSound->ControllVoice(29, 0.6f);

			for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
			{
				if (m_apUI[nCntUI] != NULL)
				{
					m_apUI[nCntUI]->Uninit();
					delete m_apUI[nCntUI];
					m_apUI[nCntUI] = NULL;
				}
			}

			if (m_pRanking != NULL)
			{
				m_pRanking->Uninit();
				delete m_pRanking;
				m_pRanking = NULL;
			}

			// �V�ѕ��̃e�N�X�`���ԍ������Z�b�g
			m_nHowtoTex = PAUSE_HOWTO_TEXTURE;

			// �t�F�[�Y�����ڑI���ɂ���
			m_Phase = TITLE_PHASE_MODE_SELECT;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CTitle *CTitle::Create(void)
{
	// �N���X�̐���
	CTitle *pTitle = NULL;
	pTitle = new CTitle;
	
	if (pTitle != NULL)
	{
		// ����������
		pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pTitle;
}

//=============================================================================
// UI��������
//=============================================================================
void CTitle::UiCreate(TITLE_PHASE phase, bool bPad)
{
	// �e�N�X�`���̎��
	int nTexType = 6;
	float fBackTex = 0.0f;

	// ��U�S�Ă�UI�������Ă���
	for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
			delete m_apUI[nCntUI];
			m_apUI[nCntUI] = NULL;
		}
	}

	switch (phase)
	{
	case TITLE_PHASE_PRESS_START:
		// �I������UI������
		for (int nCntSelectUI = 0; nCntSelectUI < TITLE_MAX_SELECT; nCntSelectUI++)
		{
			if (m_apSelectUI[nCntSelectUI] != NULL)
			{
				m_apSelectUI[nCntSelectUI]->Uninit();
				delete m_apSelectUI[nCntSelectUI];
				m_apSelectUI[nCntSelectUI] = NULL;
			}
		}

		// �^�C�g���F�v���X
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_PRESS_POS_Y),
			D3DXVECTOR2(TITLE_PRESS_SIZE_X, TITLE_PRESS_SIZE_Y), 2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_MODE_SELECT:
		// �I�����F�X�^�[�g����
		m_apSelectUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// �I�����F�V�ѕ�
		m_apSelectUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 40, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// �I�����F����K�C�h
		m_apSelectUI[2] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE * 2),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// �I�����F�����L���O
		m_apSelectUI[3] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE * 3),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_HOWTO:
		if (bPad == false)
		{
			fBackTex = 0.0f;
		}
		else
		{
			fBackTex = 1.0f;
		}
		// �V�ѕ��̐���
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 41, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y), D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1]->SetTexture(0.0f, fBackTex, 1, 2);
		m_apUI[2] = CUI::Create(D3DXVECTOR2(PAUSE_PAGINATION_POS_X, PAUSE_PAGINATION_POS_Y), D3DXVECTOR2(PAUSE_PAGINATION_SIZE_X, PAUSE_PAGINATION_SIZE_Y), 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_GUIDE:
		if (bPad == false)
		{
			nTexType = 6;
			fBackTex = 0.0f;
		}
		else
		{
			nTexType = 7;
			fBackTex = 1.0f;
		}

		// ����K�C�h�̐���
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), nTexType, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y), D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1]->SetTexture(0.0f, fBackTex, 1, 2);
		break;

	case TITLE_PHASE_RANKING:
		m_pRanking = CRanking::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), bPad);
		break;

	default:
		break;
	}
}