//=============================================================================
// ビルボードのエフェクト処理 [billboard_effect.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard_effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboardEffect::CBillboardEffect()
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_type = EFFECT_TYPE_NORMAL;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboardEffect::~CBillboardEffect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboardEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// 継承元クラスの初期化処理
	CBillboard::Init(pos, size);

	// 変数の初期化
	m_pos = pos;
	m_size = size / 2;
	m_col = col;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboardEffect::Uninit(void)
{
	// 継承元クラスの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboardEffect::Update(void)
{
	// 継承元クラスの更新処理
	CBillboard::Update();

	// 徐々に消えるタイプなら
	if (m_type == EFFECT_TYPE_VANISH)
	{
		m_col.a -= 0.05f;

		if (m_col.a <= 0)
		{
			m_col.a = 0;
		}
	}
	
	// カラーの設定処理
	SetColor(m_col);

	// 寿命を減らす
	m_nLife--;

	// 寿命がなくなったら
	if (m_nLife < 0)
	{
		// 消滅させる
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboardEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 継承元クラスの描画処理
	CBillboard::Draw();

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// αブレンディングを通常に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 生成処理
//=============================================================================
CBillboardEffect *CBillboardEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move,
	int nLife, EFFECT_TYPE type)
{
	//インスタンスの生成
	CBillboardEffect *pBillboardEffect = NULL;

	// ヌルチェック
	if (pBillboardEffect == NULL)
	{
		// クラスの生成
		pBillboardEffect = new CBillboardEffect;
		if (pBillboardEffect != NULL)
		{
			// テクスチャの割当
			pBillboardEffect->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT);
			pBillboardEffect->BindTexture(pBillboardEffect->m_pTexture);

			// 変数の設定
			pBillboardEffect->m_move = move;
			pBillboardEffect->m_nLife = nLife;
			pBillboardEffect->m_type = type;

			// 初期化処理
			pBillboardEffect->Init(pos, size, col);
		}
	}
	return pBillboardEffect;
}