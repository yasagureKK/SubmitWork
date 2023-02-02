//=============================================================================
// 3Dオブジェクト処理 [scene3D.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//================================================
// 初期化処理
//================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;

	// 位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

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

//================================================
// 終了処理
//================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//================================================
// 更新処理
//================================================
void CScene3D::Update(void)
{

}

//================================================
// 描画処理
//================================================
void CScene3D::Draw(void)
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
}

//================================================
// 生成処理
//================================================
CScene3D* CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nPriolity)
{
	// インスタンスの生成
	CScene3D *pScene3D = NULL;

	// ヌルチェック
	if (pScene3D == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pScene3D = new CScene3D(nPriolity);
		if (pScene3D != NULL)
		{
			// 変数の初期化
			pScene3D->m_rot = rot;

			// 初期化処理
			pScene3D->Init(pos, size);
		}
	}

	return pScene3D;
}

//================================================
// 位置設定処理
//================================================
void CScene3D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 位置・サイズに代入
	m_pos = pos;
	m_size = size;

	// 位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, m_size.y / 2.0f, m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x / 2.0f, m_size.y / 2.0f, m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
// 位置取得処理
//================================================
D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_pos;
}

//================================================
// テクスチャ設定処理
//================================================
void CScene3D::SetTexture(float fU, float fV, int nCutU, int nCutV)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	//テクスチャ設定(U座標の位置、V座標の位置、横の分割数、縦の分割数を入れる)
	pVtx[0].tex = D3DXVECTOR2(fU * (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[1].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[2].tex = D3DXVECTOR2(fU * (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));
	pVtx[3].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));

	m_pVtxBuff->Unlock();
}

//================================================
// テクスチャ割当処理
//================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//================================================
// カラー設定処理
//================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 色を更新
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	m_pVtxBuff->Unlock();
}