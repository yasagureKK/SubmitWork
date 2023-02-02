//=============================================================================
// メッシュ(空)処理 [mesh_sky.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mesh_sky.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshSky::CMeshSky(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = 0;
	m_nSide = 0;
	m_fSpin = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshSky::~CMeshSky()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshSky::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = pos;
	m_size = size;
	m_nVertical = 128;
	m_nSide = 32;

	// 頂点バッファの生成
	//※(縦の分割数＋１)×(横の分割数＋１)の値の頂点を生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nVertical + 1) * (m_nSide + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファへのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// 頂点座標の設定
	for (int nCntSide = 0; nCntSide < m_nSide + 1; nCntSide++)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3(cosf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)),
				m_pos.y + sinf((D3DX_PI * 2.0f / m_nVertical) * nCntSide) * (m_size.x / 2.0f),
				sinf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)));

			// 法線
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// カラー
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + -(1.0f / m_nVertical * nCntVertical), 0.0f + -(1.0f / m_nSide * nCntSide));
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nSide - 1) * (m_nVertical + 3) + (m_nVertical + 1) * (m_nSide + 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 番号データの設定
	for (int nCntSide = 0; nCntSide < m_nSide; nCntSide++)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
		{
			pIdx[(nCntVertical * 2) + 0 + (m_nVertical + 2) * 2 * nCntSide] = (m_nVertical + 1) + nCntVertical + (m_nVertical + 1) * nCntSide;
			pIdx[(nCntVertical * 2) + 1 + (m_nVertical + 2) * 2 * nCntSide] = 0 + nCntVertical + (m_nVertical + 1) * nCntSide;
		}
	}

	for (int nCntSide = 0; nCntSide < m_nSide - 1; nCntSide++)
	{
		pIdx[((m_nVertical + 1) * 2 + 0) * (nCntSide + 1) + (2 * nCntSide)] = m_nVertical + (m_nVertical + 1) * nCntSide;
		pIdx[((m_nVertical + 1) * 2 + 1) * (nCntSide + 1) + (1 * nCntSide)] = m_nVertical * 2 + 2 + (m_nVertical + 1) * nCntSide;
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshSky::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshSky::Update(void)
{
	// 回転させる
	m_rot.y += m_fSpin;

	// 向きが3.14から超えないようにする
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshSky::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		0,
		((m_nVertical + 1) * (m_nSide + 1)),				//頂点の数
		0,													//開始する頂点のインデックス
		(m_nSide * m_nVertical * 2) + (m_nSide * 4) - 4);	//描画するプリミティブ数

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CMeshSky* CMeshSky::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fSpin, int nTexType)
{
	// クラスの生成
	CMeshSky *pMeshSphere = NULL;
	pMeshSphere = new CMeshSky;
	
	if (pMeshSphere != NULL)
	{
		// 変数の初期化
		pMeshSphere->m_fSpin = fSpin;
		pMeshSphere->m_pTexture = CManager::GetTexture()->GetTexture3D(nTexType);

		// 初期化処理
		pMeshSphere->Init(pos, size);
	}
	return pMeshSphere;
}

//=============================================================================
// テクスチャ割り当て処理
//=============================================================================
void CMeshSky::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}