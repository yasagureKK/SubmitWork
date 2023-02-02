//=============================================================================
// メッシュフィールド処理 [mesh_field.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mesh_field.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRow = 0;
	m_nLine = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;

	// 頂点バッファの生成
	//※(縦の分割数＋１)×(横の分割数＋１)の値の頂点を生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nRow + 1) * (m_nLine + 1),
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
	for (int nCntLine = 0; nCntLine < m_nLine + 1; nCntLine++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3((-m_size.x / 2.0f) + (nCntRow * (m_size.x / m_nRow)),
				m_size.y,
				(m_size.z / 2.0f) - (nCntLine * (m_size.z / m_nLine)));

			// 法線
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// カラーの設定
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ
			pVtx[nCntVtx].tex = D3DXVECTOR2(nCntRow * 1.0f, nCntLine * 1.0f);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	//※(頂点の合計)+(重複して置かれた頂点)の値を生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nRow + 1) * (m_nLine + 1)) + ((m_nRow + 3) * (m_nLine - 1))),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);

	// 番号データの設定
	for (int nCntLine = 0; nCntLine < m_nLine; nCntLine++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++)
		{
			// 横の列は2つずつ設定する
			// pIdx[(基準の位置)×(現在の奥の分割数)＋(ずらす数)] = (ずらす数)＋(基準の位置)＋(横の分割数+1×現在の奥の分割数)
			pIdx[(m_nRow * 2 + 4) * nCntLine + 0 + (nCntRow * 2)] = nCntRow + (m_nRow + 1) + (m_nRow + 1) * nCntLine;
			pIdx[(m_nRow * 2 + 4) * nCntLine + 1 + (nCntRow * 2)] = nCntRow + 0 + (m_nRow + 1) * nCntLine;
		}
	}
	// ポリゴンを描画させない部分の番号データの設定
	for (int nCntLine = 0; nCntLine < m_nLine - 1; nCntLine++)
	{
		// pIdx[(基準の位置)＋(ずらす数)] = (横の分割数)/(横の分割数+2＋ずらす数)＋(横の分割数+1×現在の奥の分割数)
		pIdx[(m_nRow * 2 + 2) + 0 + nCntLine * (m_nRow * 2 + 4)] = m_nRow + (m_nRow + 1) * nCntLine;
		pIdx[(m_nRow * 2 + 2) + 1 + nCntLine * (m_nRow * 2 + 4)] = (m_nRow * 2 + 2) + (m_nRow + 1) * nCntLine;
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CMeshField::Uninit(void)
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
void CMeshField::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		((m_nRow + 1) * (m_nLine + 1)),					// 頂点の数
		0,												// 開始する頂点のインデックス
		(m_nRow * m_nLine * 2) + (m_nLine * 4) - 4);	// 描画するプリミティブ数
}

//=============================================================================
// 生成処理
//=============================================================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nRow, int nLine, int nTexType)
{
	// クラスの生成
	CMeshField *pMeshField = NULL;
	pMeshField = new CMeshField;

	if (pMeshField != NULL)
	{
		// 変数の初期化
		pMeshField->m_rot = rot;
		pMeshField->m_nRow = nRow;
		pMeshField->m_nLine = nLine;
		pMeshField->m_pTexture = CManager::GetTexture()->GetTexture3D(nTexType);

		// 初期化処理
		pMeshField->Init(pos, size);
	}
	return pMeshField;
}

//=============================================================================
// カラー設定処理
//=============================================================================
void CMeshField::SetColor(D3DXCOLOR col)
{
	// 頂点バッファへのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int nCntVtx = 0;

	// 頂点座標の設定
	for (int nCntLine = 0; nCntLine < m_nLine + 1; nCntLine++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++, nCntVtx++)
		{
			// カラーの設定
			pVtx[nCntVtx].col = col;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
// 当たり判定処理
//================================================
bool CMeshField::Collision(CScene *pScene)
{
	//着地しているかどうか
	bool bLand = false;

	//オブジェクト情報を入れるポインタ
	CScene *pObject = NULL;
	//オブジェクト情報を保存するポインタ変数
	CScene *pSaveObject = NULL;

	//先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_NONE);

	while (pObject != NULL)
	{
		//現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
		{
			CMeshField *pMeshField = NULL;
			pMeshField = (CMeshField*)pObject;

			D3DXVECTOR3 posField = pMeshField->GetPos();		//地面の位置
			D3DXVECTOR3 sizeField = pMeshField->GetSize();		//地面のサイズ
			D3DXVECTOR3 pos = pScene->GetPos();					//対象の位置
			D3DXVECTOR3 size = pScene->GetSize();				//対象のサイズ
			D3DXVECTOR3 posOld = pScene->GetPosOld();			//対象の1フレーム前の位置

			if (pos.x + size.x / 2.0f > posField.x - sizeField.x / 2.0f &&
				pos.x - size.x / 2.0f < posField.x + sizeField.x / 2.0f &&
				pos.z + size.x / 2.0f > posField.z - sizeField.z / 2.0f &&
				pos.z - size.x / 2.0f < posField.z + sizeField.z / 2.0f &&
				pos.y <= posField.y &&
				posOld.y >= posField.y)
			{
				//位置を設定
				pos.y = posField.y;
				pScene->SetPos(pos);

				bLand = true;
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}