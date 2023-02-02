//=============================================================================
// 武器UI処理 [weapon.cpp]
// Author : 草刈 翔
//=============================================================================
#include "weapon.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_CIRCLE_SIZE			(300)	// 円のUIのサイズ
#define MAX_WEAPON_ICON_SIZE	(200)	// 武器アイコンのサイズ
#define POS_DIFFERENCE			(150)	// 円の位置とアイコンの位置の差

//=============================================================================
// コンストラクタ
//=============================================================================
CWeapon::CWeapon()
{
	// 変数のクリア
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
// デストラクタ
//=============================================================================
CWeapon::~CWeapon()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWeapon::Init(D3DXVECTOR2 pos)
{
	// 変数の初期化
	m_pos = pos;
	m_fTiltCircle = 0.0f;
	m_fAddTilt = 0.0f;

	float fangle = sqrtf(POS_DIFFERENCE * POS_DIFFERENCE + POS_DIFFERENCE * POS_DIFFERENCE);
	m_fTiltIcon[0] = D3DX_PI / 4 * 3;
	m_fTiltIcon[1] = -(D3DX_PI / 4);
	m_posIcon[0] = D3DXVECTOR2(m_pos.x + sinf(m_fTiltIcon[0]) * fangle, m_pos.y + cosf(m_fTiltIcon[0]) * fangle);
	m_posIcon[1] = D3DXVECTOR2(m_pos.x + sinf(m_fTiltIcon[1]) * fangle, m_pos.y + cosf(m_fTiltIcon[1]) * fangle);

	// 武器UI：円
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(MAX_CIRCLE_SIZE, MAX_CIRCLE_SIZE), 24, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 武器UI：斧
	m_apUI[1] = CUI::Create(m_posIcon[0], D3DXVECTOR2(MAX_WEAPON_ICON_SIZE, MAX_WEAPON_ICON_SIZE), 25, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 武器UI：ボウガン
	m_apUI[2] = CUI::Create(m_posIcon[1], D3DXVECTOR2(MAX_WEAPON_ICON_SIZE, MAX_WEAPON_ICON_SIZE), 26, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWeapon::Uninit(void)
{
	// UIの終了処理
	for (int nCntUI = 0; nCntUI < MAX_WEAPON_UI_POLYGON; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWeapon::Update(void)
{
	//----------------------------
	// 円
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

	// 傾きを設定し、回転させる
	m_apUI[0]->SetTilt(m_fTiltCircle);

	//----------------------------
	// アイコン
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

	// アイコンUIの位置を設定
	m_fTiltIcon[0] = D3DX_PI / 4 * 3;
	m_apUI[1]->SetPos(D3DXVECTOR3(m_pos.x + sinf(m_fTiltIcon[0] + m_fAddTilt) * fangle, m_pos.y + cosf(m_fTiltIcon[0] + m_fAddTilt) * fangle, 0.0f));

	m_fTiltIcon[1] = -(D3DX_PI / 4);
	m_apUI[2]->SetPos(D3DXVECTOR3(m_pos.x + sinf(m_fTiltIcon[1] + m_fAddTilt) * fangle, m_pos.y + cosf(m_fTiltIcon[1] + m_fAddTilt) * fangle, 0.0f));
}

//=============================================================================
// 描画処理
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
// 生成処理
//=============================================================================
CWeapon *CWeapon::Create(D3DXVECTOR2 pos)
{
	// クラスの生成
	CWeapon *pWeapon = NULL;
	pWeapon = new CWeapon;

	if (pWeapon != NULL)
	{
		// 初期化処理
		pWeapon->Init(pos);
	}
	return pWeapon;
}

//=============================================================================
// UI変更処理
//=============================================================================
void CWeapon::SetUIChange(int nType)
{
	// 斧を右上に持ってくる
	if (nType == 0)
	{
		m_fObjectiveTilt = 0.0f;
	}
	// ボウガンを右上に持ってくる
	else if (nType == 1)
	{
		m_fObjectiveTilt = -D3DX_PI;
	}
}