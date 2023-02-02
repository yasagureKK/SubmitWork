//=============================================================================
// �X�R�A���� [score.cpp]
// Author : ���� ��
//=============================================================================
#include "score.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "data_clear.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUMBER_SIZE_X	(30)	// �i���o�[�̃T�C�YX
#define NUMBER_SIZE_Y	(60)	// �i���o�[�̃T�C�YY

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore()
{
	// �ϐ��̃N���A
	m_nScore = 0;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nScore = 0;

	// �X�R�A����
	m_apUI = CUI::Create(m_pos, m_size, 37, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �i���o�[�̐���
	for (int nCntTime = 0; nCntTime < MAX_SCORE_DIGIT; nCntTime++)
	{
		m_apNumber[nCntTime] = CUI::Create(D3DXVECTOR2(m_pos.x + (m_size.x / 1.8f) + (nCntTime * (NUMBER_SIZE_X * 0.9f)), m_pos.y),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	// UI�̏���
	m_apUI->Uninit();

	// �i���o�[�̔j��
	for (int nCntTime = 0; nCntTime < MAX_SCORE_DIGIT; nCntTime++)
	{
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{
	m_nScore = CManager::GetDataClear()->GetScore();

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_SCORE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	m_apUI->Draw();

	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CScore *CScore::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �N���X�̐���
	CScore *pTime = NULL;
	pTime = new CScore;

	if (pTime != NULL)
	{
		// ����������
		pTime->Init(pos, size);
	}
	return pTime;
}