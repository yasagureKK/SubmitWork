//=============================================================================
// 3Dオブジェクトでのナンバー管理処理 [number3D.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "number3D.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "texture.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber3D::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber3D::CNumber3D()
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber3D::~CNumber3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_color = col;

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
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);

	// 法線
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// カラー
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 10.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber3D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

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

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CNumber3D *CNumber3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	// インスタンスの生成
	CNumber3D *pNumber3D = NULL;

	// ヌルチェック
	if (pNumber3D == NULL)
	{
		pNumber3D = new CNumber3D;
		if (pNumber3D != NULL)
		{
			// テクスチャの割当
			m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_NUMBER);

			// 変数の設定
			pNumber3D->m_rot = rot;

			// 初期化処理
			pNumber3D->Init(pos, size, col);
		}
	}

	return pNumber3D;
}

//=============================================================================
// ナンバー設定処理
//=============================================================================
void CNumber3D::SetNumber(int nNumber)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//テクスチャ座標設定
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置設定処理
//=============================================================================
void CNumber3D::SetPos(D3DXVECTOR3 pos)
{
	// 位置を更新
	m_pos = pos;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, m_size.y / 2.0f, m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x / 2.0f, m_size.y / 2.0f, m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//カラー設定処理
//=============================================================================
void CNumber3D::SetColor(D3DXCOLOR col)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーを更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}