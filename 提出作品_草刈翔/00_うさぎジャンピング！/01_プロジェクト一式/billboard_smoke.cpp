//=============================================================================
// ビルボード(スモーク)処理 [billboard_smoke.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard_smoke.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboardSmoke::CBillboardSmoke()
{
	// 変数のクリア
	m_nAlpha = 0;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboardSmoke::~CBillboardSmoke()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboardSmoke::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 継承元クラスの初期化処理
	CBillboard::Init(pos, size);

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nAlpha = 0;

	// 上昇量を1.0～2.5の値からランダムに設定
	m_move.y = float(rand() % 15 / 10) + 1.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboardSmoke::Uninit(void)
{
	// 継承元クラスの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboardSmoke::Update(void)
{
	// 継承元クラスの更新処理
	CBillboard::Update();

	// 透明度を上げていく
	m_nAlpha+= 7;
	if (m_nAlpha >= 255)
	{
		m_nAlpha = 255;
	}
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - m_nAlpha));

	// 寿命が尽きたら消滅させる
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboardSmoke::Draw(void)
{
	// 継承元クラスの描画処理
	CBillboard::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CBillboardSmoke* CBillboardSmoke::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nLife)
{
	// インスタンスの生成
	CBillboardSmoke *pBillboardSmoke = NULL;

	// ヌルチェック
	if (pBillboardSmoke == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pBillboardSmoke = new CBillboardSmoke;
		if (pBillboardSmoke != NULL)
		{
			// テクスチャの割当
			pBillboardSmoke->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SMOKE);
			pBillboardSmoke->BindTexture(pBillboardSmoke->m_pTexture);

			// 変数の初期化
			pBillboardSmoke->m_nLife = nLife;

			// 初期化処理
			pBillboardSmoke->Init(pos, size);
		}
	}

	return pBillboardSmoke;
}