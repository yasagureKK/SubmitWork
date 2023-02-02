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
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CPause::m_nSelect = 0;
bool CPause::m_bPause = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pScene2D = NULL;
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
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_bPause = false;
	m_nSelect = 1;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	// �j������O�Ƀ|�[�Y��Ԃ�����
	m_bPause = false;

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	for (int nCntUi = 1; nCntUi < 4; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			// UI�̃e�N�X�`���̐ݒ�
			m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);

			if (nCntUi == m_nSelect)
			{
				D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
				// �T�C�Y�������傫������
				m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(UI_PAUSE_SELECT_SIZE_X + 40.0f, UI_PAUSE_SELECT_SIZE_Y + 10.0f, 0.0f));

				// ���点��
				m_apUi[nCntUi]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
				m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(UI_PAUSE_SELECT_SIZE_X, UI_PAUSE_SELECT_SIZE_Y, 0.0f));
			}
		}
	}

	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�p�b�h�擾����
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
			int nUp, nDown, nPause;

			if (nNum == 0)
			{
				nUp = DIK_W;
				nDown = DIK_S;
				nPause = DIK_P;
			}
			else if (nNum == 1)
			{
				nUp = DIK_UP;
				nDown = DIK_DOWN;
				nPause = NULL;
			}

			//***********************************************************************
			// �|�[�Y (P�L�[ �܂��� �p�b�hSTART�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(nPause) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
			{//���ʂ�������
				// 1�l�p�Q�[���Ȃ�
				if (CManager::GetMode() == CManager::MODE_GAME)
				{
					pSound->ControllVoice(CSound::SOUND_LABEL_GAME, 0.2f);
				}
				// 2�l�p�Q�[���Ȃ�
				else if (CManager::GetMode() == CManager::MODE_PVP)
				{
					pSound->ControllVoice(CSound::SOUND_LABEL_PVP, 0.2f);
				}
				// CPU�ΐ�Q�[���Ȃ�
				else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
				{
					pSound->ControllVoice(CSound::SOUND_LABEL_CPU, 0.2f);
				}

				// �|�[�Y���Ă��Ȃ��ꍇ�ɏ���
				if (m_bPause == false)
				{
					// ������true�ɂ���1�񂾂�����������
					m_bPause = true;

					//+------------+
					//|  UI�̐���  |
					//+------------+
					// ��ʂ��Â�����
					m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_UI);
					m_pScene2D->SetColor(D3DCOLOR_RGBA(0, 0, 0, 130));

					// �g�b�v
					m_apUi[0] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_PAUSE_TOP_POS_Y, 0.0f),
						D3DXVECTOR3(UI_PAUSE_TOP_SIZE_X, UI_PAUSE_TOP_SIZE_Y, 0.0f),
						PRIORITY_UI, CUi::UI_TYPE_PAUSE_TOP);

					// �I����
					m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_PAUSE_SELECT_POS_Y, 0.0f),
						D3DXVECTOR3(UI_PAUSE_SELECT_SIZE_X, UI_PAUSE_SELECT_SIZE_Y, 0.0f),
						PRIORITY_UI, CUi::UI_TYPE_PAUSE_CONTINUE);
					m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_PAUSE_SELECT_POS_Y + UI_PAUSE_SELECT_SIZE_Y, 0.0f),
						D3DXVECTOR3(UI_PAUSE_SELECT_SIZE_X, UI_PAUSE_SELECT_SIZE_Y, 0.0f),
						PRIORITY_UI, CUi::UI_TYPE_PAUSE_RETRY);
					m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_PAUSE_SELECT_POS_Y + UI_PAUSE_SELECT_SIZE_Y * 2, 0.0f),
						D3DXVECTOR3(UI_PAUSE_SELECT_SIZE_X, UI_PAUSE_SELECT_SIZE_Y, 0.0f),
						PRIORITY_UI, CUi::UI_TYPE_PAUSE_QUIT);

					for (int nCntUi = 1; nCntUi < 4; nCntUi++)
					{
						if (m_apUi[nCntUi] != NULL)
						{
							// UI�̃e�N�X�`���̐ݒ�
							m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);
						}
					}

				}
				// �|�[�Y���Ă���ꍇ�ɏ���
				else if (m_bPause == true)
				{
					CancelPause();
				}
			}

			// �|�[�Y���Ă�Ȃ瑀��\
			if (m_bPause == true)
			{
				//***********************************************************************
				// �I�� (1P:W/S�A2P:��/�� �܂��� �p�b�h���X�e�B�b�N)
				//***********************************************************************
				//��
				if (pInputKeyboard->GetTrigger(nUp) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNum) == true)
				{
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CURSOR);
					pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

					m_nSelect--;
				}
				//��
				else if (pInputKeyboard->GetTrigger(nDown) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNum) == true)
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

				//***********************************************************************
				// ���� (Enter/Space �܂��� A�{�^��)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
				{
					// m_nSelect�̒l�ŏ�����ς���
					switch (m_nSelect)
					{
						// �Â���
					case 1:
						// SE�̍Đ�
						pSound->Play(CSound::SOUND_LABEL_DECIDE);
						pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

						CancelPause();
						break;

						// ���Ȃ���
					case 2:
						// 1�l�p�Q�[����ʂȂ�
						if (CManager::GetMode() == CManager::MODE_GAME)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_GAME);
						}
						// 2�l�p�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_PVP)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_PVP);
						}
						// CPU�ΐ�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_CPU);
						}

						// SE�̍Đ�
						pSound->Play(CSound::SOUND_LABEL_DECIDE);
						pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

						// 1�l�p�Q�[����ʂȂ�
						if (CManager::GetMode() == CManager::MODE_GAME)
						{
							pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						// 2�l�p�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_PVP)
						{
							pFade->SetFade(CManager::MODE_PVP, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						// CPU�ΐ�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
						{
							pFade->SetFade(CManager::MODE_SOLO_CPU, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						break;

						// ��߂�
					case 3:
						// SE�̍Đ�
						pSound->Play(CSound::SOUND_LABEL_CANCEL);
						pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

						// 1�l�p�Q�[����ʂȂ�
						if (CManager::GetMode() == CManager::MODE_GAME)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_GAME);

							// �\�����j���[��ʂɑJ��
							pFade->SetFade(CManager::MODE_SOLO_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						// 2�l�p�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_PVP)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_PVP);

							// ���j���[��ʂɑJ��
							pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						// CPU�ΐ�Q�[����ʂȂ�
						else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
						{
							// BGM�̒�~
							pSound->Stop(CSound::SOUND_LABEL_CPU);

							// �\�����j���[��ʂɑJ��
							pFade->SetFade(CManager::MODE_SOLO_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
						}
						break;

					default:
						break;
					}
				}
			}
		}
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
CPause *CPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �C���X�^���X�̐���
	CPause *pPause = NULL;

	// �k���`�F�b�N
	if (pPause == NULL)
	{
		// �N���X�̐���
		pPause = new CPause;
		if (pPause != NULL)
		{
			// ����������
			pPause->Init(pos, size);
		}
	}

	return pPause;
}

//=============================================================================
// �|�[�Y�ݒ菈��
//=============================================================================
void CPause::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
// �|�[�Y�擾����
//=============================================================================
bool CPause::GetPause(void)
{
	return m_bPause;
}

//=============================================================================
// �|�[�Y�������̏���
//=============================================================================
void CPause::CancelPause(void)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// ������false�ɂ���1�񂾂�����������
	m_bPause = false;

	//���ʂ��グ��
	// 1�l�p�Q�[���Ȃ�
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		pSound->ControllVoice(CSound::SOUND_LABEL_GAME, 0.4f);
	}
	// 2�l�p�Q�[���Ȃ�
	else if (CManager::GetMode() == CManager::MODE_PVP)
	{
		pSound->ControllVoice(CSound::SOUND_LABEL_PVP, 0.4f);
	}
	// CPU�ΐ�Q�[���Ȃ�
	else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		pSound->ControllVoice(CSound::SOUND_LABEL_CPU, 0.4f);
	}

	// 2D�I�u�W�F�N�g�̔j��
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		delete m_pScene2D;
		m_pScene2D = NULL;
	}

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
}