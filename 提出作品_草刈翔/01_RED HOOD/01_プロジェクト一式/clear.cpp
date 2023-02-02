//=============================================================================
// �N���A���� [clear.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "clear.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
#include "data_clear.h"
#include "keyboard.h"
#include "gamepad.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLEAR_FANFARE_COUNT		(30)			// �N���A�t�@���t�@�[���܂ł̎���
#define CLEAR_BG_COUNT			(90)			// �w�i�\���܂ł̎���
#define CLEAR_SCORE_COUNT		(120)			// �X�R�A�\���܂ł̎���
#define CLEAR_TRANS_COUNT		(200)			// ��ʑJ�ڂ܂ł̎���
#define CLEAR_TOP_POS_Y			(120.0f)		// �g�b�v������Y���W�ʒu
#define CLEAR_TOP_SIZE_X		(850.0f)		// �g�b�v�����̃T�C�YX
#define CLEAR_TOP_SIZE_Y		(170.0f)		// �g�b�v�����̃T�C�YY
#define CLEAR_SENTENCE_POS_X	(340.0f)		// �N���A������X���W�ʒu
#define CLEAR_SENTENCE_POS_Y	(240.0f)		// �N���A������Y���W�ʒu
#define CLEAR_SENTENCE_SIZE_X	(540.0f)		// �N���A�����̃T�C�YX
#define CLEAR_SENTENCE_SIZE_Y	(80.0f)			// �N���A�����̃T�C�YY
#define CLEAR_NUMBER_POS_X		(660.0f)		// �i���o�[��X���W�ʒu
#define CLEAR_NUMBER_SIZE_X		(60.0f)			// �i���o�[�̃T�C�YX
#define CLEAR_NUMBER_SIZE_Y		(120.0f)		// �i���o�[�̃T�C�YY
#define CLEAR_RANK_POS_X		(1100.0f)		// �����N��X���W�ʒu
#define CLEAR_RANK_SIZE_X		(240.0f)		// �����N�̃T�C�YX
#define CLEAR_RANK_SIZE_Y		(100.0f)		// �����N�̃T�C�YY
#define SAVEFILENAME "data/TEXT/clear_save.txt"	// �t�@�C���̃p�X

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CClear::CClear()
{
	// �ϐ��̃N���A
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	m_nAlpha = 0;
	m_nClearCount = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	m_nScore = 0;
	m_bDisplay = false;
	m_phase = CLEAR_FASE_NONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CClear::~CClear()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CClear::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_nAlpha = 0;
	m_nClearCount = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	m_nScore = 0;
	m_bDisplay = false;
	m_phase = CLEAR_FASE_NONE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CClear::Uninit(void)
{
	// UI�̏I������
	for (int nCntUI = 0; nCntUI < MAX_CLEAR_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �i���o�[�̔j��
	for (int nCntDigit = 0; nCntDigit < MAX_CLEAR_DIGIT; nCntDigit++)
	{
		for (int nCntUI = 0; nCntUI < MAX_CLEAR_SCORE_UI; nCntUI++)
		{
			if (m_apNumber[nCntDigit][nCntUI] != NULL)
			{
				m_apNumber[nCntDigit][nCntUI]->Uninit();
			}
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CClear::Update(void)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();
	
	//�t�F�[�h�擾����
	CFade *pFade = CManager::GetFade();

	// �J�E���g��i�߂�
	m_nClearCount++;

	// �摜�̃A���t�@�x��ύX���A�w�i�ƃg�b�v�������������\��������
	for (int nCntClear = 0; nCntClear < 2; nCntClear++)
	{
		if (m_apUI[nCntClear] != NULL)
		{
			if (nCntClear == 0)
			{
				m_nAlpha++;
			}

			int nAlpha = 8 * m_nAlpha;

			if (nAlpha >= 255)
			{
				nAlpha = 255;
			}
			m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, nAlpha));
		}
	}

	switch (m_phase)
	{
	case CLEAR_FASE_NONE:
		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_FANFARE_COUNT)
		{
			// BGM�̒�~
			pSound->Stop();

			// �Q�[���N���ASE�̍Đ�
			pSound->Play(16);
			pSound->ControllVoice(16, 0.6f);

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TOP;
		}
		break;

	case CLEAR_FASE_TOP:
		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_BG_COUNT)
		{
			// �w�i�E�g�b�v����
			m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 31, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, CLEAR_TOP_POS_Y), D3DXVECTOR2(CLEAR_TOP_SIZE_X, CLEAR_TOP_SIZE_Y), 32, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_SCORE;
		}
		break;

	case CLEAR_FASE_SCORE:
		// ���͂ɂ���ĕ\���𑁂߂�
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// �X�R�A�\��SE�̍Đ�
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// �X�R�A
			m_apUI[2] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[2]->SetTexture(0.0f, 0.0f, 1, 4);

			// �i���o�[�̐���
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				// �X�R�A���擾
				m_nScore = CManager::GetDataClear()->CDataClear::GetScore();

				m_apNumber[0][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[0][nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TIME;
		}
		break;

	case CLEAR_FASE_TIME:
		// ���͂ɂ���ĕ\���𑁂߂�
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// �X�R�A�\��SE�̍Đ�
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// �^�C���{�[�i�X
			m_apUI[3] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 100.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y),33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[3]->SetTexture(0.0f, 1.0f, 1, 4);

			// �^�C���{�[�i�X���擾
			m_nTimeBonus = CManager::GetDataClear()->CDataClear::GetTimeBonus();

			// �i���o�[�̐���
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[1][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 100.0f), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[1][nCntNumber]->SetTexture(float(m_nTimeBonus % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_LEVEL;
		}
		break;

	case CLEAR_FASE_LEVEL:
		// ���͂ɂ���ĕ\���𑁂߂�
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// �X�R�A�\��SE�̍Đ�
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// ���x���{�[�i�X
			m_apUI[4] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 200.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[4]->SetTexture(0.0f, 2.0f, 1, 4);

			// ���x���{�[�i�X���擾
			m_nLevelBonus = CManager::GetDataClear()->CDataClear::GetLevelBonus();

			// �i���o�[�̐���
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[2][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 200.0f),D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[2][nCntNumber]->SetTexture(float(m_nLevelBonus % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TOTAL;
		}
		break;

	case CLEAR_FASE_TOTAL:
		// ���͂ɂ���ĕ\���𑁂߂�
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// �X�R�A�\��SE�̍Đ�
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// �g�[�^���X�R�A
			m_apUI[5] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 300.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[5]->SetTexture(0.0f, 3.0f, 1, 4);

			// �X�R�A���擾
			m_nScore += m_nTimeBonus + m_nLevelBonus;

			// �i���o�[�̐���
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[3][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 300.0f), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));
				m_apNumber[3][nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_RANK;
		}
		break;

	case CLEAR_FASE_RANK:
		// �܂������N�̕\��������Ă��Ȃ��Ȃ�
		if (m_nClearCount < CLEAR_SCORE_COUNT)
		{
			// ���͂ɂ���ĕ\���𑁂߂�
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				m_nClearCount = CLEAR_SCORE_COUNT;
			}
		}

		// ���̎��Ԃ��o������
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// �e�N�X�`���̎��
			float fTexType = 0.0f;

			// �X�R�A��18000�_�ȏ��S�����N
			if (m_nScore >= CLEAR_RANK_S)
			{
				fTexType = 0.0f;

				// �����NS��SE���Đ�
				pSound->Play(32);
				pSound->ControllVoice(32, 0.7f);
			}
			// �X�R�A��13000�_�ȏ�A18000�_������A�����N
			else if (m_nScore >= CLEAR_RANK_A && m_nScore < CLEAR_RANK_S)
			{
				fTexType = 1.0f;

				// �����NA��SE���Đ�
				pSound->Play(33);
				pSound->ControllVoice(33, 0.8f);
			}
			// �X�R�A��6000�_�ȏ�A13000�_������B�����N
			else if (m_nScore >= CLEAR_RANK_B && m_nScore < CLEAR_RANK_A)
			{
				fTexType = 2.0f;

				// �����NB��SE���Đ�
				pSound->Play(34);
				pSound->ControllVoice(34, 0.8f);
			}
			// �X�R�A��6000�_������C�����N
			else if (m_nScore < CLEAR_RANK_B)
			{
				fTexType = 3.0f;

				// �����NC��SE���Đ�
				pSound->Play(35);
				pSound->ControllVoice(35, 0.8f);
			}

			// �����N
			m_apUI[6] = CUI::Create(D3DXVECTOR2(CLEAR_RANK_POS_X, CLEAR_SENTENCE_POS_Y + 300.0f),
				D3DXVECTOR2(CLEAR_RANK_SIZE_X, CLEAR_RANK_SIZE_Y), 30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apUI[6]->SetTexture(0.0f, fTexType, 1, 4);
		}
		else if (m_nClearCount == CLEAR_SCORE_COUNT + 50.0f)
		{
			m_nClearCount = 0;
			m_phase = CLEAR_FASE_FADE;
		}
		break;

	case CLEAR_FASE_FADE:
		// ���͂ɂ���ĕ\���𑁂߂�
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_TRANS_COUNT;
		}

		// ���̎��Ԃ��o���A�t�F�[�h���Ă��Ȃ��Ȃ�
		if (m_nClearCount >= CLEAR_TRANS_COUNT && pFade->GetFade() == CFade::FADE_NONE)
		{
			// �^�C�g����ʂɑJ��
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CClear::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CClear *CClear::Create(void)
{
	// �N���X�̐���
	CClear *pClear = NULL;
	pClear = new CClear;

	if (pClear != NULL)
	{
		// ����������
		pClear->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pClear;
}