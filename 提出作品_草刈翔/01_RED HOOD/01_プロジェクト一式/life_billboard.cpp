//=============================================================================
// ビルボードのライフ処理 [life_billboard.cpp]
// Author : 草刈 翔
//=============================================================================
#include "life_billboard.h"
#include "ui.h"
#include "manager.h"
#include "billboard.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLifeBillboard::CLifeBillboard()
{
	// 変数のクリア
	m_fLife = 0;
	m_fMaxLife = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_emptySize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(&m_apBillboard, NULL, sizeof(m_apBillboard));
}

//=============================================================================
// デストラクタ
//=============================================================================
CLifeBillboard::~CLifeBillboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLifeBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = D3DXVECTOR3(pos.x, pos.y + size.y * 3.0f, pos.z);
	m_size = size;
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = m_size.x;
	m_emptySize = D3DXVECTOR3(0.0f, m_size.y, 0.0f);

	SetPos(m_pos);
	SetSize(m_size);

	// ライフ(枠)
	m_apBillboard[0] = CBillboard::Create(m_pos, D3DXVECTOR3(size.x + 15.0f, size.y + 5.0f, size.z), 7, false);

	// ライフ(ゲージ)
	m_apBillboard[1] = CBillboard::Create(m_pos, m_size, 8, false);

	//  ライフ(空部分)
	m_apBillboard[2] = CBillboard::Create(m_pos, m_size, 9, false);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLifeBillboard::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			// ビルボードの終了処理
			m_apBillboard[nCntLife]->Uninit();
			m_apBillboard[nCntLife] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLifeBillboard::Update(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			// 位置の設定
			D3DXVECTOR3 pos = m_apBillboard[nCntLife]->GetPos();
			D3DXVECTOR3 newPos = D3DXVECTOR3(m_pos.x, m_pos.y + pos.y, m_pos.z);
			m_apBillboard[nCntLife]->SetPos(newPos);
		}
	}

	// ライフの空部分がNULLじゃなかったら
	if (m_apBillboard[2] != NULL)
	{
		// ライフバーの設定処理
		SetLifeBar();
	}

	// ライフが0を下回ったら
	if (m_fLife <= 0)
	{
		m_fLife = 0;

		// 消す
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLifeBillboard::Draw(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			m_apBillboard[nCntLife]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CLifeBillboard *CLifeBillboard::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// クラスの生成
	CLifeBillboard *pLifeBill = NULL;
	pLifeBill = new CLifeBillboard;

	if (pLifeBill != NULL)
	{
		// 初期化処理
		pLifeBill->Init(pos, D3DXVECTOR3(fSizeX, fSizeY, 0.0f));
	}
	return pLifeBill;
}

//=============================================================================
// ライフバーの設定処理
//=============================================================================
void CLifeBillboard::SetLifeBar(void)
{
	if (m_apBillboard[2] != NULL)
	{
		D3DXVECTOR3 size = m_apBillboard[2]->GetSize();

		// 画像のサイズとライフの最大値から割合を出す
		float fRatio = m_fMaxSize / m_fMaxLife;

		// <割合×減ったライフ>で目的のサイズを出す
		m_fObjectiveSize = fRatio * (m_fMaxLife - m_fLife);

		// 目的のサイズと現在のサイズが違うなら
		if (m_fObjectiveSize != m_emptySize.x)
		{
			// 目的のサイズが現在のサイズより大きい場合
			if (m_fObjectiveSize > m_emptySize.x)
			{
				// 空部分のサイズを増やす
				m_emptySize.x += 3.0f;

				if (m_emptySize.x > m_fObjectiveSize)
				{
					m_emptySize.x = m_fObjectiveSize;
				}
			}
		}

		// 空部分の頂点座標を変更
		m_apBillboard[2]->SetVertex(D3DXVECTOR3(+size.x / 2.0f - m_emptySize.x, size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f, size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f - m_emptySize.x, -size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f, -size.y / 2, 0.0f));
	}
}