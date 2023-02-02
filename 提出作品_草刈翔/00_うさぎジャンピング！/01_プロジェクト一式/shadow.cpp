//=============================================================================
// シャドウ処理 [shadow.h]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority) :CScene3D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Defaultsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//変数の初期化
	m_pos = pos;
	m_size = size;
	m_Defaultsize = size;
	m_bUse = false;

	// 継承元クラスの初期化処理
	CScene3D::Init(pos, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 継承元クラスの終了処理
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	//使用している時のみ描画させる
	if (m_bUse == true)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice;
		pDevice = CManager::GetRenderer()->GetDevice();

		//ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//アルファテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		// 継承元クラスの描画処理
		CScene3D::Draw();

		//アルファテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

		//減算合成から通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	//インスタンスの生成
	CShadow *pShadow = NULL;
	if (pShadow == NULL)
	{
		pShadow = new CShadow;
		if (pShadow != NULL)
		{
			pShadow->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT);
			pShadow->m_rot = rot;
			pShadow->Init(pos, size);
		}
	}
	return pShadow;
}

//=============================================================================
// 位置の設定処理
//=============================================================================
void CShadow::SetShadow(D3DXVECTOR3 pos, float fRange)
{
	// 位置を更新
	m_pos = pos;

	int nAlpha = (int)(fRange / 2.0f);

	if (nAlpha >= 205)
	{
		nAlpha = 205;
	}

	//プレイヤーと影の位置の距離によって影を薄くする
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 205 - nAlpha));

	//位置・サイズ設定処理
	CScene3D::SetPos(m_pos, m_size);
}

//=============================================================================
// 使用状態の設定処理
//=============================================================================
void CShadow::SetUseShadow(bool bUse)
{
	m_bUse = bUse;
}