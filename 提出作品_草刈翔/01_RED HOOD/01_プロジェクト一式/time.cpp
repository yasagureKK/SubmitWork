//=============================================================================
// �^�C������ [time.cpp]
// Author : ���� ��
//=============================================================================
#include "time.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUMBER_SIZE_X	(40)	// �i���o�[�̃T�C�YX
#define NUMBER_SIZE_Y	(80)	// �i���o�[�̃T�C�YY

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTime::CTime()
{
	// �ϐ��̃N���A
	m_nTime = 0;
	m_nNumber = 0;
	m_bTimeStop = false;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTime::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nTime = 0;
	m_nNumber = 0;
	
	// �^�C������
	m_apUI = CUI::Create(m_pos, m_size, 27, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �i���o�[�̐���
	for (int nCntTime = 0; nCntTime < MAX_TIME_DIGIT; nCntTime++)
	{
		m_apNumber[nCntTime] = CUI::Create(D3DXVECTOR2(m_pos.x + (m_size.x / 1.5f) + (nCntTime * (NUMBER_SIZE_X * 0.9f)), m_pos.y - (m_size.y * 0.1f)),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTime::Uninit(void)
{
	// UI�̏���
	m_apUI->Uninit();

	// �i���o�[�̔j��
	for (int nCntTime = 0; nCntTime < MAX_TIME_DIGIT; nCntTime++)
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
void CTime::Update(void)
{
	if (m_bTimeStop == false)
	{
		//�������Ԍ��炷
		m_nNumber++;
	}

	//60�t���[���o�����Ƃ�(��1�b��)
	if (m_nNumber != 0 && m_nNumber % 60 == 0)
	{
		if (m_nTime < MAX_TIME)
		{
			m_nTime++;
		}
	}

	for (int nCntNumber = 0; nCntNumber < MAX_TIME_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber]->SetTexture(float(m_nTime % (int)powf(10, MAX_TIME_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_TIME_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTime::Draw(void)
{
	m_apUI->Draw();

	for (int nCntScore = 0; nCntScore < MAX_TIME_DIGIT; nCntScore++)
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
CTime *CTime::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �N���X�̐���
	CTime *pTime = NULL;
	pTime = new CTime;

	if (pTime != NULL)
	{
		// ����������
		pTime->Init(pos, size);
	}
	return pTime;
}