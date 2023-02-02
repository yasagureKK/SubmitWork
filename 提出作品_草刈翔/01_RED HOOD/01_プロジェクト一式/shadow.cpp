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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = pos;
	m_size = size;
	m_Defaultsize = size;
	m_bUse = false;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);

	// 法線
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)  //ヌルチェック
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

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
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScale;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// カリングを行う
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

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
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// クラスの生成
	CShadow *pShadow = NULL;
	pShadow = new CShadow;

	if (pShadow != NULL)
	{
		// エフェクトのテクスチャを取得
		pShadow->m_pTexture = CManager::GetTexture()->GetTexture3D(0);
		pShadow->Init(pos, size);
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

	//プレイヤーと影の位置の距離によって影を大きくする
	m_size.x = m_Defaultsize.x + (fRange / 4.0f);
	m_size.z = m_Defaultsize.z + (fRange / 4.0f);

	int nAlpha = (int)(fRange / 3.0f);

	if (nAlpha >= 190)
	{
		nAlpha = 190;
	}

	D3DXCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 205 - nAlpha);

	//プレイヤーと影の位置の距離によって影を薄くする
	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 使用状態の設定処理
//=============================================================================
void CShadow::SetUseShadow(bool bUse)
{
	m_bUse = bUse;
}