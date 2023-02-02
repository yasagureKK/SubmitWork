//=============================================================================
// ���U���g��� [result.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "billboard.h"
#include "player.h"
#include "motion_player.h"
#include "score.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	// �ϐ��̃N���A
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pBillboard = NULL;
	memset(&m_pPlayer, NULL, sizeof(m_pPlayer));
	memset(&m_pScore, NULL, sizeof(m_pScore));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_RESULT);
	pSound->ControllVoice(CSound::SOUND_LABEL_RESULT, 0.4f);

	// �w�i
	m_pBillboard = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 230.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CTexture::TEX_TYPE_RESULT_BG, PRIORITY_NONE);
	
	//+------------+
	//|  UI�̐���  |
	//+------------+
	// ���փ{�^��
	m_apUi[0] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_NEXT);

	// ���g���C�{�^��
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_RETRY);

	// ��
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_BAND_POS_Y, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, RESULT_UI_BAND_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_BAND);

	// �g�b�v����
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(RESULT_UI_TOP_SIZE_X, RESULT_UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_TOP);

	// ���O�̃��[�h��1�l�p�Q�[����������
	if (CManager::GetBeforeMode() == CManager::MODE_GAME)
	{
		// ����
		m_apUi[4] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_POSITION_Y, 0.0f),
			D3DXVECTOR3(RESULT_UI_SIZE_X, RESULT_UI_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_RESULT_SENTENCE);

		// �X�R�A�̐���
		m_pScore[0] = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 300.0f, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pScore[0]->SetNum(0);

		// �v���C���[
		m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);
		m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_WIN);
	}
	// ���O�̃��[�h��2�l�p�Q�[���܂���CPU�ΐ�Q�[����������
	if (CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
	{
		// �X�R�A��ۑ����邽�߂̕ϐ�
		int n1PScore = 0, n2PScore = 0;

		for (int nCntPlayer = 0; nCntPlayer < RESULT_MAX_PLAYER; nCntPlayer++)
		{
			// �X�R�A�̐���
			m_pScore[nCntPlayer] = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 3 * (nCntPlayer + 1) - 250.0f + (nCntPlayer * 140.0f), 300.0f, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
			m_pScore[nCntPlayer]->SetNum(nCntPlayer);
		}

		// ���ꂼ��̃X�R�A��ۑ�
		n1PScore = m_pScore[0]->GetResultScore(0);
		n2PScore = m_pScore[1]->GetResultScore(1);

		// �v���C���[
		m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);
		m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1, true);

		// 1P�����̎�
		if (n1PScore > n2PScore)
		{
			// 1P�E2P(/CPU)�̕\��
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			// �u�����I�v�̕�����\��
			m_apUi[7] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X + RESULT_UI_1P_SIZE_X / 1.5f, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_WIN_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_WIN);

			// ���ꂼ��Ƀ��[�V������ݒ�
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_WIN);
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_LOSE);
		}
		// 2P�����̎�
		else if (n2PScore > n1PScore)
		{
			// 1P�E2P�̕\��
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			
			// �u�����I�v�̕�����\��
			m_apUi[7] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X + RESULT_UI_1P_SIZE_X / 1.5f, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_WIN_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_WIN);

			// ���ꂼ��Ƀ��[�V������ݒ�
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_WIN);
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_LOSE);
		}
		// ���������̎�
		else if (n1PScore == n2PScore)
		{
			// 1P�E2P�̕\��
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			
			// �u�Ђ��킯�c�v�̕�����\��
			m_apUi[7] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_DRAW_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_DRAW);

			// �����ɕ������[�V������ݒ�
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_LOSE);
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_LOSE);
		}
	}

	// �{�^���͈Â����Ă���
	m_apUi[0]->SetTexture(0.0f, 0.0f, 1, 2);
	m_apUi[1]->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
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
			// ���ւ����� (Enter/Space �܂��� �p�b�hA�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// �{�^�������点��
				m_apUi[0]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_RESULT);

				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_DECIDE);
				pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

				// �����L���O��ʂɑJ��
				pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
			}

			//***********************************************************************
			// ���g���C (BackSpace �܂��� �p�b�hX�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				// �{�^�������点��
				m_apUi[1]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_RESULT);

				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_DECIDE);
				pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

				//�ĂуQ�[����ʂɑJ��
				// ���O�̃��[�h��1�l�p�Q�[����������
				if (CManager::GetBeforeMode() == CManager::MODE_GAME)
				{
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
				// ���O�̃��[�h��2�l�p�Q�[����������
				else if (CManager::GetBeforeMode() == CManager::MODE_PVP)
				{
					pFade->SetFade(CManager::MODE_PVP, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
				// ���O�̃��[�h��CPU�ΐ�Q�[����������
				else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
				{
					pFade->SetFade(CManager::MODE_SOLO_CPU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}