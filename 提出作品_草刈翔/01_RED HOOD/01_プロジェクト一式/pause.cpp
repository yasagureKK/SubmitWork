//=============================================================================
// �|�[�Y���� [pause.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_TOP_POS_X			(340.0f)	// �|�[�Y�g�b�v�̈ʒuX
#define PAUSE_TOP_POS_Y			(90.0f)		// �|�[�Y�g�b�v�̈ʒuY
#define PAUSE_TOP_SIZE_X		(640.0f)	// �|�[�Y�g�b�v�̃T�C�YX
#define PAUSE_TOP_SIZE_Y		(120.0f)	// �|�[�Y�g�b�v�̃T�C�YY
#define PAUSE_SELECT_POS_X		(400.0f)	// �I�����̈ʒuX
#define PAUSE_SELECT_POS_Y		(250.0f)	// �I�����̈ʒuY
#define PAUSE_SELECT_SIZE_X		(640.0f)	// �I�����̃T�C�YX
#define PAUSE_SELECT_SIZE_Y		(100.0f)	// �I�����̃T�C�YY
#define PAUSE_SENTENCE_POS_X	(360.0f)	// ���ڐ������̈ʒuX
#define PAUSE_SENTENCE_POS_Y	(660.0f)	// ���ڐ������̈ʒuY
#define PAUSE_SENTENCE_SIZE_X	(600.0f)	// ���ڐ������̃T�C�YX
#define PAUSE_SENTENCE_SIZE_Y	(90.0f)		// ���ڐ������̃T�C�YY
#define PAUSE_BACK_POS_X		(240.0f)	// �߂��X���W�ʒu
#define PAUSE_BACK_POS_Y		(660.0f)	// �߂��Y���W�ʒu
#define PAUSE_BACK_SIZE_X		(430.0f)	// �߂�̃T�C�YX
#define PAUSE_BACK_SIZE_Y		(80.0f)		// �߂�̃T�C�YY

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CPause::m_nSelect = 0;
bool CPause::m_bCanPause = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	memset(&m_apUi, NULL, sizeof(m_apUi));
	memset(&m_apSelectUi, NULL, sizeof(m_apSelectUi));
	m_bCanPause = false;
	m_bPause = false;
	m_nSelect = 0;
	m_Phase = PAUSE_PHASE_NORMAL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_bCanPause = true;
	m_bPause = false;
	m_nSelect = 0;
	m_Phase = PAUSE_PHASE_NORMAL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	// �|�[�Y��Ԃ�����
	CManager::SetPause(false);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();
	// �}�E�X�擾����
	CMouse *pMouse = CManager::GetMouse();

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade = CManager::GetFade();

	switch (m_Phase)
	{
	case PAUSE_PHASE_NORMAL:
		for (int nCntUi = 0; nCntUi < PAUSE_MAX_SELECT_UI; nCntUi++)
		{
			if (m_apSelectUi[nCntUi] != NULL)
			{
				// �I������Ă���UI��
				if (nCntUi == m_nSelect)
				{
					// �����x��0��
					m_apSelectUi[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

					// �T�C�Y�������傫������
					m_apSelectUi[nCntUi]->SetSize(D3DXVECTOR2(PAUSE_SELECT_SIZE_X + 40.0f, PAUSE_SELECT_SIZE_Y + 10.0f));
				}
				// �I������Ă��Ȃ�UI��
				else
				{
					// �����ɂ���
					m_apSelectUi[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));

					// �T�C�Y�����ɖ߂�
					m_apSelectUi[nCntUi]->SetSize(D3DXVECTOR2(PAUSE_SELECT_SIZE_X, PAUSE_SELECT_SIZE_Y));
				}

				// ���݂̑I�����ɍ��킹�ĕ���ς���
				m_apUi[2]->SetTexture(0, (float)m_nSelect, 1, 3);
			}
		}


		// �|�[�Y�ł���ꍇ���A�t�F�[�h���Ă��Ȃ��Ȃ�
		if (m_bCanPause == true && pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// �|�[�Y (1�L�[ �܂��� �p�b�hSTART�{�^��)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_1) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
			{
				// BGM�̉��ʂ�������
				pSound->ControllVoice(1, 0.2f);
				pSound->ControllVoice(2, 0.2f);

				// �}�l�[�W���[���A���̃N���X�̗����Ń|�[�Y���Ă��Ȃ��ꍇ�ɏ���
				if (CManager::GetPause() == false && m_bPause == false)
				{
					// UI�̐���
					UICreate(m_Phase, 0.0f);

					// �|�[�Y������Ԃɂ���
					CManager::SetPause(true);
					m_bPause = true;

				}
				// �|�[�Y���Ă���ꍇ�ɏ���
				else if (CManager::GetPause() == true && m_bPause == true)
				{
					// �|�[�Y��������
					CancelPause();
				}
			}

			// ���̃N���X���Ń|�[�Y���Ă�Ȃ瑀��\
			if (m_bPause == true)
			{
				//***********************************************************************
				// �I�� (�L�[�{�[�h�v/�r �܂��� �p�b�h���X�e�B�b�N)
				//***********************************************************************
				//��
				if (pKeyboard->GetTrigger(DIK_W) == true ||
					pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
				{
					// �I��SE�̍Đ�
					pSound->Play(27);
					pSound->ControllVoice(27, 0.4f);

					m_nSelect--;
				}
				//��
				else if (pKeyboard->GetTrigger(DIK_S) == true ||
					pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
				{
					// �I��SE�̍Đ�
					pSound->Play(27);
					pSound->ControllVoice(27, 0.4f);

					m_nSelect++;
				}

				// �I�����̐���
				if (m_nSelect < 0)
				{
					m_nSelect = PAUSE_MAX_SELECT_UI - 1;
				}
				else if (m_nSelect >= PAUSE_MAX_SELECT_UI)
				{
					m_nSelect = 0;
				}

				//***********************************************************************
				// ���� (Enter/Space �܂��� A�{�^��)
				//***********************************************************************
				if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
				{
					float fBackTex = 0.0f;
					// m_nSelect�̒l�ŏ�����ς���
					switch (m_nSelect)
					{
						// �Â���
					case 0:
						// ����SE�̍Đ�
						pSound->Play(28);
						pSound->ControllVoice(28, 0.4f);

						// �|�[�Y��������
						CancelPause();
						break;

						// �������
					case 1:
						// ����SE�̍Đ�
						pSound->Play(28);
						pSound->ControllVoice(28, 0.4f);

						// �t�F�[�Y�𑀍�K�C�h�ɂ���
						m_Phase = PAUSE_PHASE_GUIDE;

						if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
							pKeyboard->GetTrigger(DIK_RETURN) == true)
						{
							fBackTex = 0.0f;
						}
						else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
							pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
						{
							fBackTex = 1.0f;
						}

						// UI�̐���
						UICreate(m_Phase, fBackTex);

						break;

						// �I��
					case 2:
						// BGM�̒�~
						pSound->Stop();

						// �L�����Z��SE�̍Đ�
						pSound->Play(29);
						pSound->ControllVoice(29, 0.6f);

						// �^�C�g����ʂɑJ��
						pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
						break;

					default:
						break;
					}
				}
				//***********************************************************************
				// �L�����Z�� (Backspace �܂��� X�{�^��)
				//***********************************************************************
				else if (pKeyboard->GetTrigger(DIK_BACKSPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
				{
					// �L�����Z��SE�̍Đ�
					pSound->Play(29);
					pSound->ControllVoice(29, 0.6f);

					// �|�[�Y��������
					CancelPause();
				}
			}
		}

		break;

	case PAUSE_PHASE_GUIDE:
		//***********************************************************************
		// �߂� (�L�[�{�[�hBackspace �܂��� �p�b�hX�{�^�� �܂��� �}�E�X�E�N���b�N)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_BACK) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
			pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
		{
			if (m_apUi[3] != NULL && m_apUi[4] != NULL)
			{
				// �L�����Z��SE�̍Đ�
				pSound->Play(29);
				pSound->ControllVoice(29, 0.6f);

				// ����K�C�h������
				m_apUi[3]->Uninit();
				delete m_apUi[3];
				m_apUi[3] = NULL;
				m_apUi[4]->Uninit();
				delete m_apUi[4];
				m_apUi[4] = NULL;

				// �t�F�[�Y��ʏ�ɂ���
				m_Phase = PAUSE_PHASE_NORMAL;
			}
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CPause *CPause::Create(D3DXVECTOR3 pos)
{
	// �N���X�̐���
	CPause *pPause = NULL;
	pPause = new CPause;

	if (pPause != NULL)
	{
		// ����������
		pPause->Init(pos);
	}
	return pPause;
}

//=============================================================================
// UI��������
//=============================================================================
void CPause::UICreate(PAUSE_PHASE phase, float fTex)
{
	switch (phase)
	{
	case PAUSE_PHASE_NORMAL:
		// �|�[�Y�F�w�i
		m_apUi[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|�[�Y�F�g�b�v����
		m_apUi[1] = CUI::Create(D3DXVECTOR2(PAUSE_TOP_POS_X, PAUSE_TOP_POS_Y), D3DXVECTOR2(PAUSE_TOP_SIZE_X, PAUSE_TOP_SIZE_Y), 17, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|�[�Y�F���ڐ�����
		m_apUi[2] = CUI::Create(D3DXVECTOR2(PAUSE_SENTENCE_POS_X, PAUSE_SENTENCE_POS_Y), D3DXVECTOR2(PAUSE_SENTENCE_SIZE_X, PAUSE_SENTENCE_SIZE_Y), 21, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUi[2]->SetTexture(0, 0, 1, 3);

		// �|�[�Y�F�I����
		for (int nCntUI = 0; nCntUI < PAUSE_MAX_SELECT_UI; nCntUI++)
		{
			m_apSelectUi[nCntUI] = CUI::Create(D3DXVECTOR2(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + (PAUSE_SELECT_SIZE_Y * nCntUI)),
				D3DXVECTOR2(PAUSE_SELECT_SIZE_X, PAUSE_SELECT_SIZE_Y), 18 + nCntUI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apSelectUi[nCntUI]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));
		}
		break;

	case PAUSE_PHASE_GUIDE:
		// ����K�C�h�̐���
		m_apUi[3] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		m_apUi[4] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y),
			D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUi[4]->SetTexture(0.0f, fTex, 1, 2);
		break;

	default:
		break;
	}
}

//=============================================================================
// �|�[�Y�������̏���
//=============================================================================
void CPause::CancelPause(void)
{
	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �|�[�Y������
	CManager::SetPause(false);
	m_bPause = false;

	// BGM�̉��ʂ��グ��
	pSound->ControllVoice(1, 0.3f);
	pSound->ControllVoice(2, 0.3f);

	// UI�̔j��
	for (int nCntUi = 0; nCntUi < PAUSE_MAX_UI; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			m_apUi[nCntUi]->Uninit();
			delete m_apUi[nCntUi];
			m_apUi[nCntUi] = NULL;
		}
	}

	for (int nCntUi = 0; nCntUi < PAUSE_MAX_SELECT_UI; nCntUi++)
	{
		if (m_apSelectUi[nCntUi] != NULL)
		{
			m_apSelectUi[nCntUi]->Uninit();
			delete m_apSelectUi[nCntUi];
			m_apSelectUi[nCntUi] = NULL;
		}
	}

	// �I�����Ă���ԍ���������
	m_nSelect = 0;
}

//=============================================================================
// �|�[�Y�ł��邩�ݒ菈��
//=============================================================================
void CPause::SetPossiblePause(bool bPause)
{
	m_bCanPause = bPause;
}