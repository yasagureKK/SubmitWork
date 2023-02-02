//=============================================================================
// ����UI���� [weapon.cpp]
// Author : ���� ��
//=============================================================================
#include "weapon.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_CIRCLE_SIZE			(300)	// �~��UI�̃T�C�Y
#define MAX_WEAPON_ICON_SIZE	(200)	// ����A�C�R���̃T�C�Y
#define POS_DIFFERENCE			(150)	// �~�̈ʒu�ƃA�C�R���̈ʒu�̍�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWeapon::CWeapon()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	m_fTiltCircle = 0.0f;
	for (int nCntIcon = 0; nCntIcon < MAX_WEAPON_ICON; nCntIcon++)
	{
		m_posIcon[nCntIcon] = D3DXVECTOR2(0.0f, 0.0f);
		m_fTiltIcon[nCntIcon] = 0.0f;
	}
	m_fAddTilt = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWeapon::~CWeapon()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWeapon::Init(D3DXVECTOR2 pos)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_fTiltCircle = 0.0f;
	m_fAddTilt = 0.0f;

	float fangle = sqrtf(POS_DIFFERENCE * POS_DIFFERENCE + POS_DIFFERENCE * POS_DIFFERENCE);
	m_fTiltIcon[0] = D3DX_PI / 4 * 3;
	m_fTiltIcon[1] = -(D3DX_PI / 4);
	m_posIcon[0] = D3DXVECTOR2(m_pos.x + sinf(m_fTiltIcon[0]) * fangle, m_pos.y + cosf(m_fTiltIcon[0]) * fangle);
	m_posIcon[1] = D3DXVECTOR2(m_pos.x + sinf(m_fTiltIcon[1]) * fangle, m_pos.y + cosf(m_fTiltIcon[1]) * fangle);

	// ����UI�F�~
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(MAX_CIRCLE_SIZE, MAX_CIRCLE_SIZE), 24, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ����UI�F��
	m_apUI[1] = CUI::Create(m_posIcon[0], D3DXVECTOR2(MAX_WEAPON_ICON_SIZE, MAX_WEAPON_ICON_SIZE), 25, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ����UI�F�{�E�K��
	m_apUI[2] = CUI::Create(m_posIcon[1], D3DXVECTOR2(MAX_WEAPON_ICON_SIZE, MAX_WEAPON_ICON_SIZE), 26, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWeapon::Uninit(void)
{
	// UI�̏I������
	for (int nCntUI = 0; nCntUI < MAX_WEAPON_UI_POLYGON; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWeapon::Update(void)
{
	//----------------------------
	// �~
	//----------------------------
	m_fTiltCircle -= 0.01f;

	if (m_fTiltCircle > D3DX_PI)
	{
		m_fTiltCircle -= D3DX_PI * 2.0f;
	}
	else if (m_fTiltCircle < -D3DX_PI)
	{
		m_fTiltCircle += D3DX_PI * 2.0f;
	}

	// �X����ݒ肵�A��]������
	m_apUI[0]->SetTilt(m_fTiltCircle);

	//----------------------------
	// �A�C�R��
	//----------------------------
	float fangle = sqrtf(POS_DIFFERENCE * POS_DIFFERENCE + POS_DIFFERENCE * POS_DIFFERENCE);

	if (m_fObjectiveTilt != m_fAddTilt)
	{
		m_fAddTilt -= 0.5f;

		if (m_fObjectiveTilt < 0)
		{
			if (m_fAddTilt <= m_fObjectiveTilt)
			{
				m_fAddTilt = m_fObjectiveTilt;
			}

			if (m_fAddTilt < -D3DX_PI)
			{
				m_fAddTilt += D3DX_PI * 2.0f;
			}
		}
		else if (m_fObjectiveTilt >= 0)
		{
			if (m_fAddTilt < -D3DX_PI)
			{
				m_fAddTilt += D3DX_PI * 2.0f;
			}

			if (m_fAddTilt <= 0)
			{
				m_fAddTilt = m_fObjectiveTilt;
			}
		}
	}

	// �A�C�R��UI�̈ʒu��ݒ�
	m_fTiltIcon[0] = D3DX_PI / 4 * 3;
	m_apUI[1]->SetPos(D3DXVECTOR3(m_pos.x + sinf(m_fTiltIcon[0] + m_fAddTilt) * fangle, m_pos.y + cosf(m_fTiltIcon[0] + m_fAddTilt) * fangle, 0.0f));

	m_fTiltIcon[1] = -(D3DX_PI / 4);
	m_apUI[2]->SetPos(D3DXVECTOR3(m_pos.x + sinf(m_fTiltIcon[1] + m_fAddTilt) * fangle, m_pos.y + cosf(m_fTiltIcon[1] + m_fAddTilt) * fangle, 0.0f));
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWeapon::Draw(void)
{
	for (int nCntLevel = 0; nCntLevel < MAX_WEAPON_UI_POLYGON; nCntLevel++)
	{
		if (m_apUI[nCntLevel] != NULL)
		{
			m_apUI[nCntLevel]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CWeapon *CWeapon::Create(D3DXVECTOR2 pos)
{
	// �N���X�̐���
	CWeapon *pWeapon = NULL;
	pWeapon = new CWeapon;

	if (pWeapon != NULL)
	{
		// ����������
		pWeapon->Init(pos);
	}
	return pWeapon;
}

//=============================================================================
// UI�ύX����
//=============================================================================
void CWeapon::SetUIChange(int nType)
{
	// �����E��Ɏ����Ă���
	if (nType == 0)
	{
		m_fObjectiveTilt = 0.0f;
	}
	// �{�E�K�����E��Ɏ����Ă���
	else if (nType == 1)
	{
		m_fObjectiveTilt = -D3DX_PI;
	}
}