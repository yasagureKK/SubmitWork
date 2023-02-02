//=============================================================================
// ライフ処理 [life.cpp]
// Author : 草刈 翔
//=============================================================================
#include "life.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUMBER_SIZE_X	(20)	// ナンバーのサイズX
#define NUMBER_SIZE_Y	(30)	// ナンバーのサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CLife::CLife()
{
	// 変数のクリア
	m_fLife = 0.0f;
	m_fMaxLife = 0.0f;
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = 0.0f;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_emptyPos = D3DXVECTOR2(0.0f, 0.0f);
	m_emptySize = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
}

//=============================================================================
// デストラクタ
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type)
{
	// 変数の初期化
	m_pos = pos;
	m_size = D3DXVECTOR2(size.x, size.y);
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = m_size.x;
	m_emptyPos = D3DXVECTOR2(m_pos.x + (m_size.x / 2), m_pos.y);
	m_emptySize = D3DXVECTOR2(0.0f, m_size.y);

	int nLifeCaseTex = 0, LifeBarTex = 0;
	float fCaseSizeX = 0.0f, fCaseSizeY = 0;
	if (type == LIFE_TYPE_PLAYER)
	{
		nLifeCaseTex = 11;
		LifeBarTex = 9;
		fCaseSizeX = 30.0f;
		fCaseSizeY = 15.0f;
	}
	else if (type == LIFE_TYPE_BOSS)
	{
		nLifeCaseTex = 12;
		LifeBarTex = 10;
		fCaseSizeX = 60.0f;
		fCaseSizeY = 20.0f;
	}

	// ライフ(枠)
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(m_size.x + fCaseSizeX, m_size.y + fCaseSizeY), nLifeCaseTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ライフ(ゲージ)
	m_apUI[1] = CUI::Create(m_pos, m_size, LifeBarTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ライフ(空部分)
	m_apUI[2] = CUI::Create(m_emptyPos, m_emptySize, 13, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	// UIの終了処理
	for (int nCntUI = 0; nCntUI < MAX_LIFE_POLYGON; nCntUI++)
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
void CLife::Update(void)
{
	// ライフバーの更新
	SetLifeBar();
}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_POLYGON; nCntLife++)
	{
		if (m_apUI[nCntLife] != NULL)
		{
			m_apUI[nCntLife]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CLife *CLife::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type)
{
	// クラスの生成
	CLife *pLife = NULL;
	pLife = new CLife;

	if (pLife != NULL)
	{
		// 初期化処理
		pLife->Init(pos, size, type);
	}
	return pLife;
}

//=============================================================================
// ライフバーの設定処理
//=============================================================================
void CLife::SetLifeBar(void)
{
	// 画像のサイズとライフの最大値から割合の値を出す
	float fRatio = m_fMaxSize / m_fMaxLife;

	// <割合×減ったライフ>で目的のサイズを出す
	m_fObjectiveSize = fRatio * (m_fMaxLife - m_fLife);

	// ライフが0になったら
	if (m_fLife <= 0)
	{
		// サイズをすぐに変更し、ゲージを消す
		m_emptySize.x = m_fObjectiveSize;
		m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
	}

	// 目的のサイズと空部分のサイズが違うなら
	if (m_fObjectiveSize != m_emptySize.x)
	{
		// 目的のサイズが空部分のサイズより大きい場合
		if (m_fObjectiveSize > m_emptySize.x)
		{
			// 空部分のサイズを増やす
			m_emptySize.x += 5.0f;

			if (m_emptySize.x > m_fObjectiveSize)
			{
				m_emptySize.x = m_fObjectiveSize;
			}
		}

		if (m_fObjectiveSize < m_emptySize.x)
		{
			// 空部分のサイズを減らす
			m_emptySize.x -= 5.0f;

			if (m_emptySize.x < m_fObjectiveSize)
			{
				m_emptySize.x = m_fObjectiveSize;
			}
		}

		// 空部分のサイズを変更
 		m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
	}
}