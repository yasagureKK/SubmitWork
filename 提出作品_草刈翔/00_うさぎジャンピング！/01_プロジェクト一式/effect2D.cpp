//=============================================================================
// 2Dのエフェクト処理 [effect_2D.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect2D::CEffect2D(int nPriority) :CScene2D(nPriority)
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// 継承元クラスの初期化処理
	CScene2D::Init(pos, size);

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_col = col;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect2D::Uninit(void)
{
	// 継承元クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect2D::Update(void)
{
	// 位置・サイズ取得
	D3DXVECTOR3 pos = GetPos();
	m_pos = pos;
	D3DXVECTOR3 size = GetSize();
	m_size = size;

	// 移動量を反映
	m_pos += m_move;

	// 位置を設定
	CScene2D::SetPos(m_pos, m_size);

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
void CEffect2D::Draw(void)
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
	CScene2D::Draw();

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
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nLife)
{
	//インスタンスの生成
	CEffect2D *pScene2D = NULL;

	// ヌルチェック
	if (pScene2D == NULL)
	{
		// クラスの生成
		pScene2D = new CEffect2D(PRIORITY_EFFECT);
		if (pScene2D != NULL)
		{
			// テクスチャの割当
			pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT));

			// 変数の設定
			pScene2D->m_move = move;
			pScene2D->m_nLife = nLife;

			// 初期化処理
			pScene2D->Init(pos, size, col);
		}
	}
	return pScene2D;
}