//=============================================================================
// �����L���O��� [ranking.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ranking.h"
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
float CRanking::m_fScroll = 0;
int CRanking::m_nRankingData[MAX_SCORE_DATA] = {};
CScore *CRanking::m_apScore[MAX_SCORE_DATA] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	// �ϐ��̃N���A
	m_fScroll = 0;
	m_nFadeCount = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_fScroll = 0;
	m_nFadeCount = 0;

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_RANKING);
	pSound->ControllVoice(CSound::SOUND_LABEL_RANKING, 0.4f);

	// �w�i
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UI�̐���  |
	//+------------+
	// �g�b�v
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_TOP);
	// ���ʖ��̘g
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_ONE);
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 1, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_TWO);
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 2, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_THREE);
	m_apUi[4] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 3, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_FOUR);
	m_apUi[5] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 4, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_FIVE);

	// ���O�̃��[�h�������ꂩ�̃Q�[����ʂ�������
	if (CManager::GetBeforeMode() == CManager::MODE_GAME || CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
	{
		// �����ރ{�^��
		m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
			D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_NEXT);

		// �{�^���͈Â����Ă���
		m_apUi[6]->SetTexture(0.0f, 0.0f, 1, 2);
	}
	// ���O�̃��[�h�����j���[��ʂ�������
	else if(CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		// ���ǂ�{�^��
		m_apUi[6] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
			D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_BACK);

		// �{�^���͈Â����Ă���
		m_apUi[6]->SetTexture(0.0f, 0.0f, 1, 2);
	}

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
	{
		D3DXCOLOR col;

		// 1�ʁ`3�ʂ̓J���[��ύX
		if (nCntRanking == 0)
		{
			col = D3DCOLOR_RGBA(219, 180, 0, 255);
		}
		else if (nCntRanking == 1)
		{
			col = D3DCOLOR_RGBA(190, 193, 195, 255);
		}
		else if (nCntRanking == 2)
		{
			col = D3DCOLOR_RGBA(196, 112, 34, 255);
		}
		// 4�ʁE5�ʂ̓J���[��ύX
		else
		{
			col = D3DCOLOR_RGBA(255, 200, 60, 255);
		}

		//�X�R�A�̐���
		m_apScore[nCntRanking] = CScore::Create(D3DXVECTOR3(RANKING_DATA_POS_X,
			RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * nCntRanking, 0.0f),
			D3DXVECTOR3(75.0f, 100.0f, 0.0f), col);
	}

	//���[�h����
	FILE *pFile;
	// �t�@�C�����J��
	pFile = fopen(SAVEFILENAME, "r");
	if (pFile != NULL)
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
		{
			// �����L���O�̃f�[�^��ϐ��ɕۑ�
			fscanf(pFile, "%d", &m_nRankingData[nCntRanking]);
		}
	}
	// �t�@�C�������
	fclose(pFile);

	//�X�R�A�̐ݒ�
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
	{
		m_apScore[nCntRanking]->SetScore(m_nRankingData[nCntRanking]);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	// �w�i�X�N���[���J�E���g�̍X�V
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
			// ���O�̃��[�h�������ꂩ�̃Q�[����ʂ�������
			if (CManager::GetBeforeMode() == CManager::MODE_GAME || CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				//***********************************************************************
				// ���ւ����� (Enter/Space �܂��� �p�b�hA�{�^��)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
				{
					// ���փ{�^�������点��
					m_apUi[6]->SetTexture(0.0f, 1.0f, 1, 2);

					// BGM�̒�~
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// ���j���[��ʂɑJ��
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			// ���O�̃��[�h�����j���[��ʂ�������
			else if (CManager::GetBeforeMode() == CManager::MODE_MENU)
			{
				//***********************************************************************
				// �O�ɂ��ǂ� (BackSpace �܂��� �p�b�hX�{�^��)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
				{
					// ���ǂ�{�^�������点��
					m_apUi[6]->SetTexture(0.0f, 1.0f, 1, 2);

					// BGM�̒�~
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// ���j���[��ʂɑJ��
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			// ���O�̃��[�h���^�C�g����ʂ�������
			else if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
			{
				// �J�ڗp�J�E���g�̍X�V
				m_nFadeCount++;

				//***********************************************************************
				// ���ւ����� (Enter/Space �܂��� �p�b�hA�{�^��)
				//***********************************************************************
				// �����Ԃ��o�����ꍇ�ɂ͎����I�ɑJ��
				if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
					|| m_nFadeCount >= RANKING_FADE_COUNT)
				{
					// BGM�̒�~
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// �^�C�g����ʂɑJ��
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}