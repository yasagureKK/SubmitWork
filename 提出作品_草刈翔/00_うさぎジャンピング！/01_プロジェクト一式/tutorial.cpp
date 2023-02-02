//=============================================================================
// �`���[�g���A����� [tutorial.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "tutorial.h"
#include "game.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "model_single.h"
#include "player.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_cylinder.h"
#include "score.h"
#include "jump_guide.h"
#include "tutorial_guide.h"
#include "stage_data.h"
#include "game_signboard.h"
#include "billboard_moon.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"
#include "control_ui_game_countdown.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CTutorial::m_bStop = false;
bool CTutorial::m_bExplanation = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{
	// �ϐ��̃N���A
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pPlayer = NULL;
	memset(&m_pMeshField, NULL, sizeof(m_pMeshField));
	m_pMeshSphere = NULL;
	memset(&m_pMeshCylinder, NULL, sizeof(m_pMeshCylinder));
	memset(&m_pMeshWall, NULL, sizeof(m_pMeshWall));
	memset(&m_pScore, NULL, sizeof(m_pScore));
	m_pJumpGuide = NULL;
	m_pTutorialGuide = NULL;
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	m_pTutorialPause = NULL;
	memset(&m_pStageData, NULL, sizeof(m_pStageData));
	memset(&m_pGameSignboard, NULL, sizeof(m_pGameSignboard));
	m_pBillboardMoon = NULL;
	m_phase = TUTORIAL_PHASE_NONE;
	m_nInterval = 0;
	m_nTime = 0;
	m_nPlayerCommandCount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_nInterval = 0;
	m_nTime = 0;

	// �O�̃��[�h�����j���[��ʂ�������
	if (CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		m_bExplanation = true;
	}
	else
	{
		m_bExplanation = false;
	}

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̍Đ�
	pSound->Play(CSound::SOUND_LABEL_TUTORIAL);
	pSound->ControllVoice(CSound::SOUND_LABEL_TUTORIAL, 0.4f);

	/*************************/
	/**	  �I�u�W�F�N�g�n	**/
	/*************************/
	//+------------------+
	//| �v���C���[�̐��� |
	//+------------------+
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);

	//+------------------------------------------------+
	//| ���b�V���t�B�[���h�̐����E�e�N�X�`���̊��蓖�� |
	//+------------------------------------------------+
	// ����
	m_pMeshField[0] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[1] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START + 2000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[2] = CMeshField::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[3] = CMeshField::Create(D3DXVECTOR3(3000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	// �y�G�X�^�[�g
	m_pMeshField[4] = CMeshField::Create(D3DXVECTOR3(-119.0f, 0.0f, 0.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshField::MESHFIELD_TYPE_NONE, CTexture::TEX_TYPE_FIELD01);
	m_pMeshField[4]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_FIELD01));
	// �y
	m_pMeshField[5] = CMeshField::Create(D3DXVECTOR3(980.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);
	m_pMeshField[6] = CMeshField::Create(D3DXVECTOR3(2980.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);

	//+------------------------------------------------+
	//| ���b�V���V�����_�[�̐����E�e�N�X�`���̊��蓖�� |
	//+------------------------------------------------+
	//��PNG�摜�̎d�l��A�O������`�悳����
	// ��̐F
	m_pMeshCylinder[0] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(6500.0f, 10000.0f, 6500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 8,
		D3DCOLOR_RGBA(180, 240, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_BACK_COLOR, CTexture::TEX_TYPE_CLOUD00);
	// �_
	m_pMeshCylinder[1] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(6000.0f, 4000.0f, 6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
		D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD00);
	m_pMeshCylinder[2] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5000.0f, 3500.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
		D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.001f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD00);

	//+----------------------------------------------+
	//| ���b�V���E�H�[���̐����E�e�N�X�`���̊��蓖�� |
	//+----------------------------------------------+
	// �R
	m_pMeshWall[0] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -450.0f, 600.0f), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
	m_pMeshWall[1] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 20000.0f, -450.0f, 600.0f), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
	// �R
	m_pMeshWall[2] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -350.0f, 500.0f), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
	m_pMeshWall[3] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 15000.0f, -350.0f, 500.0f), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
	// �X
	m_pMeshWall[4] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -10.0f, 200.0f), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);
	m_pMeshWall[5] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 6400.0f, -10.0f, 200.0f), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);

	//+------------+
	//| �Ŕ̐��� |
	//+------------+
	for (int nCntGameObj = 0; nCntGameObj < MAX_PLAYER_NUM; nCntGameObj++)
	{
		if (m_pGameSignboard[nCntGameObj] == NULL)
		{
			m_pGameSignboard[nCntGameObj] = CGameSignboard::Create(D3DXVECTOR3(nCntGameObj * 2000.0f, 0.0f, 80.0f), D3DXVECTOR3(2000.0f, 0.0f, 0.0f), 0);
		}
	}

	//+------------------------+
	//| �w�i�I�u�W�F�N�g�̐��� |
	//+------------------------+
	for (int nCntStageData = 0; nCntStageData < MAX_PLAYER_NUM; nCntStageData++)
	{
		if (m_pStageData[nCntStageData] == NULL)
		{
			m_pStageData[nCntStageData] = CStageData::Create(D3DXVECTOR3(GAME_PLAYER_START + nCntStageData * 1200.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 0);
		}
	}

	//+----------------------+
	//| �r���{�[�h(��)�̐��� |
	//+----------------------+
	m_pBillboardMoon = CBillboardMoon::Create(D3DXVECTOR3(GAME_PLAYER_START, 5000.0f, 2000.0f), D3DXVECTOR3(1200.0f, 1200.0f, 0.0f), 0);

	/*************************/
	/**			UI�n		**/
	/*************************/
	//+--------------+
	//| �X�R�A�̐��� |
	//+--------------+
	// �v���C���[�̃X�R�A
	m_pScore[0] = CScore::Create(D3DXVECTOR3(GAME_UI_SCORE_POS_X, GAME_UI_SCORE_POS_Y, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pScore[0]->SetNum(0);

	// �X�R�A����
	m_apUi[0] = CUi::Create(D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_SIZE_X, GAME_UI_SCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_SCORE);

	// ���O�̃��[�h�����j���[�܂��̓`���[�g���A���̏ꍇ�̂�
	if (CManager::GetBeforeMode() == CManager::MODE_MENU || CManager::GetBeforeMode() == CManager::MODE_TUTORIAL)
	{
		// �I���̕���
		m_apUi[1] = CUi::Create(D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_POS_X, TUTORIAL_UI_SKIP_SENTENCE_POS_Y, 0.0f),
			D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_SIZE_X, TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_TUTORIAL_END);

		// �������̏ꍇ�̂�
		if (m_bExplanation == true && CManager::GetBeforeMode())
		{
			// �X�L�b�v�̕���
			m_apUi[2] = CUi::Create(D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_POS_X, TUTORIAL_UI_SKIP_SENTENCE_POS_Y + 60.0f, 0.0f),
				D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_SIZE_X, TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y, 0.0f),
				PRIORITY_UI, CUi::UI_TYPE_TUTORIAL_SKIP);
		}
	}

	//+--------------+
	//| �K�C�h�̐��� |
	//+--------------+
	m_pJumpGuide = CJumpGuide::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 70.0f, GAME_UI_JUMP_GUIDE_POS_Y, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));

	//+----------------------------+
	//| �`���[�g���A���K�C�h�̐��� |
	//+----------------------------+
	m_pTutorialGuide = CTutorialGuide::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// �O�̃��[�h���`���[�g���A����������
	if (CManager::GetBeforeMode() == CManager::MODE_TUTORIAL)
	{
		// �y�[�W9��\�������Ă���
		m_pTutorialGuide->SetPage(10);
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �I�����ɐ������I�t�ɂ��Ă���
	m_bExplanation = false;

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
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

	// ��������n�߂�Ȃ�
	if (m_bExplanation == true)
	{
		// �t�F�[�h���Ă��Ȃ��Ȃ�
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			// �^�C�����C��(�����͊֐��̒�)
			TimeLine();
		}
	}
	else if (m_bExplanation == false)
	{
		// �t�F�C�Y����(�����͊֐��̒�)
		Phase();
	}

	// �t�F�[�h���Ă��Ȃ����̂ݑ���ł���
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			int nEnter, nBack;

			if (nNum == 0)
			{
				nEnter = DIK_RETURN;
				nBack = DIK_BACK;
			}
			else if (nNum == 1)
			{
				// �G���[����̂����A�ϐ����g��Ȃ��ꍇ�ɂ�0�����蓖�ĂĂ���
				nEnter = 0;
				nBack = 0;
			}

			// ���O�̃��[�h���^�C�g���̎�
			if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
			{
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
				{
					// BGM�̒�~
					pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

					// �^�C�g����ʂɑJ��
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			else
			{
				//***********************************************************************
				// �X�L�b�v (Enter �܂��� �p�b�hSTART�{�^��)
				//***********************************************************************
				//���������̏ꍇ�̂ݓ��͂ł���
				if (m_bExplanation == true)
				{
					if (pInputKeyboard->GetTrigger(nEnter) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
					{
						// BGM�̒�~
						pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

						// ���O�̃��[�h���`���[�g���A���ɐݒ�
						CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

						// �`���[�g���A����ʂɑJ��
						pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
					}
				}

				//***********************************************************************
				// �I�� (BackSpace �܂��� �p�b�hBACK�{�^��)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(nBack) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_BACK, nNum) == true)
				{
					// BGM�̒�~
					pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

					// ���O�̃��[�h�����j���[�ɐݒ�
					CManager::SetBeforeMode(CManager::MODE_MENU);

					// ���j���[��ʂɑJ��
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// �t�F�C�Y�ݒ菈��
//=============================================================================
void CTutorial::SetPhase(TUTORIAL_PHASE phase)
{
	m_phase = phase;
}

//=============================================================================
// �t�F�C�Y�擾����
//=============================================================================
CTutorial::TUTORIAL_PHASE CTutorial::GetPhase(void)
{
	return m_phase;
}

//=============================================================================
// ���������ǂ����擾����
//=============================================================================
bool CTutorial::GetExplanation(void)
{
	return m_bExplanation;
}

//=============================================================================
// �^�C�����C���Ǘ�����
//=============================================================================
void CTutorial::TimeLine(void)
{
	//�t�F�[�h�擾����
	CFade *pFade;
	pFade = CManager::GetFade();

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// �J�E���g�𑝂₷
	m_nTime++;

	// ���ݐ������Ă���y�[�W�����擾
	int nPage = CTutorialGuide::GetPage();

	//++++++++++++++++++++++++//
	//   �����̃y�[�W���X�V   //
	//++++++++++++++++++++++++//
	if (m_nTime % 240 == 0)
	{
		if (nPage < CTexture::GUIDE_TEX_TYPE_MAX - 1)
		{
			// �y�[�W��i�߂�
			m_pTutorialGuide->AdvancePage();
		}
		else if (nPage == CTexture::GUIDE_TEX_TYPE_MAX - 1)
		{
			// �y�[�W��i�߁A���b�Z�[�W�E�B���h�E������
			m_pTutorialGuide->AddPageAndDestroyWindow();
		}
	}

	if (m_nTime >= 240 * 10)
	{
		// ���O�̃��[�h�����j���[��ʂ̎�
		if (CManager::GetBeforeMode() == CManager::MODE_MENU)
		{
			// ���O�̃��[�h���`���[�g���A���ɐݒ�
			CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

			// �`���[�g���A����ʂɑJ��
			pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
		// ���O�̃��[�h���^�C�g����ʂ̎�
		else if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
		{
			// BGM�̒�~
			pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

			// �^�C�g����ʂɑJ��
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}

	//++++++++++++++++++++++++++++//
	//   �v���C���[�ɓ������w��   //
	//++++++++++++++++++++++++++++//
	// �y�[�W�ԍ���3�ɂȂ�����
	if (nPage == 3)
	{
		// �v���C���[�𓮂�����悤�ɂ���
		m_pPlayer->SetStop(false);
	}

	// �y�[�W�ԍ���3�`4�̊�
	if (nPage >= 3 && nPage <= 4)
	{
		m_nPlayerCommandCount++;

		if (m_nPlayerCommandCount >= 15)
		{
			// �_�b�V��������
			m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_DASH);

			m_nPlayerCommandCount = 0;
		}
	}

	if (nPage == 5 && m_nTime % 118 == 0)
	{
		// �W�����v������
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_JUMP);
	}

	// �y�[�W�ԍ���6�ɂȂ�����
	if (nPage == 6 && m_nTime % 10 == 0)
	{
		// 2�i�W�����v������
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_DOUBLE_JUMP);
	}
	
	if ((nPage == 6 && m_nTime % 7 == 0) || (nPage == 7 && m_nTime % 7 == 0))
	{
		// �z�o�����O������
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_HOVERING);
	}

	// �v�����I�������v���C���[���~�߂�
	if (m_pPlayer->GetLandFinish() == true)
	{
		// �v���C���[�𓮂����Ȃ��悤�ɂ���
		m_pPlayer->SetStop(true);
	}
}

//=============================================================================
// �t�F�[�Y����
//=============================================================================
void CTutorial::Phase(void)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade;
	pFade = CManager::GetFade();

	//�t�F�[�Y�ɂ�鏈������
	switch (m_phase)
	{
	case TUTORIAL_PHASE_NONE:
		// ���K�����t�F�[�h������Ȃ��Ȃ�
		if (m_bExplanation == false && pFade->GetFade() == CFade::FADE_NONE)
		{
			//+----------------------+
			//| �J�E���g�_�E���̐��� |
			//+----------------------+
			m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(GAME_UI_COUNTDOWN_SIZE_X, GAME_UI_COUNTDOWN_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_COUNTDOWN);

			// �J�E���g�_�E�����n�߂�
			m_phase = TUTORIAL_PHASE_COUNTDOWN;
		}
		break;

	case TUTORIAL_PHASE_COUNTDOWN:
		if (m_nInterval == 0)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);
		}

		//�J�E���g�𑝂₷
		m_nInterval++;

		if (m_nInterval == 30)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);

			CControlUiGameCountdown::SetCount(2);
		}
		else if (m_nInterval == 60)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);

			CControlUiGameCountdown::SetCount(1);
		}
		else if (m_nInterval == 90)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_START);
			pSound->ControllVoice(CSound::SOUND_LABEL_START, 0.4f);

			CControlUiGameCountdown::SetCount(0);
			// �t�F�[�Y��؂�ւ�
			m_phase = TUTORIAL_PHASE_ACTION;
			m_nInterval = 0;
		}
		break;

	case TUTORIAL_PHASE_ACTION:
		// �v���C���[�𓮂�����悤�ɂ���
		m_pPlayer->SetStop(false);

		//�J�E���g�𑝂₷
		m_nInterval++;

		// �����o������
		if (m_nInterval >= 60)
		{
			// �J�E���g�_�E����UI������
			if (m_apUi[3] != NULL)
			{
				m_apUi[3]->Uninit();
				delete m_apUi[3];
				m_apUi[3] = NULL;
			}
		}
		if (m_pPlayer->GetLandFinish() == true)
		{
			m_phase = TUTORIAL_PHASE_ACTION_END;
			m_nInterval = 0;
		}
		break;

	case TUTORIAL_PHASE_ACTION_END:
		// �v���C���[�𓮂����Ȃ��悤�ɂ���
		m_pPlayer->SetStop(true);

		//�J�E���g�𑝂₷
		m_nInterval++;

		if (m_nInterval >= 50)
		{
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				// BGM�̒�~
				pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

				// ���O�̃��[�h���`���[�g���A���ɐݒ�
				CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

				// �`���[�g���A����ʂɑJ�ڂ��ă��g���C
				pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
		break;

	default:
		break;
	}
}