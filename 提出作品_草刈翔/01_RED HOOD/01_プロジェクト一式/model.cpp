//=============================================================================
// モデル処理 [model.cpp]
// Aouthor : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 変数のクリア
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;
	for (int nCntTex = 0; nCntTex < MAX_MODEL_TEXTURE; nCntTex++)
	{
		m_pTexture[nCntTex] = NULL;
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
	m_col = D3DCOLOR_RGBA(0, 0, 0, 255);
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aModelName)
{
	// 頂点の最大値・最小値は後で再計算するため極端な数字にしておく
	m_vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
	m_vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

	// 変数の初期化
	m_rot = rot;
	m_pos = pos;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Xファイルの情報読み込み
	D3DXLoadMeshFromX(aModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// マテリアル情報に対するポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	int nCntTex = 0;
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntTex]);
			nCntTex++;
		}
	}

	// 頂点情報の取得
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 全ての頂点を比較して最大値と最小値を出す
		// 最大値
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}
		// 最小値
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// モデルのサイズ
		m_size.x = m_vtxMax.x - m_vtxMin.x;
		m_size.y = m_vtxMax.y - m_vtxMin.y;
		m_size.z = m_vtxMax.z - m_vtxMin.z;

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点の最大値と最小値から8頂点の位置を求める
	m_vtxPos[0].x = m_vtxMin.x;
	m_vtxPos[0].z = m_vtxMax.z;
	m_vtxPos[0].y = m_vtxMax.y;
	m_vtxPos[1].x = m_vtxMax.x;
	m_vtxPos[1].z = m_vtxMax.z;
	m_vtxPos[1].y = m_vtxMax.y;
	m_vtxPos[2].x = m_vtxMin.x;
	m_vtxPos[2].z = m_vtxMax.z;
	m_vtxPos[2].y = m_vtxMin.y;
	m_vtxPos[3].x = m_vtxMax.x;
	m_vtxPos[3].z = m_vtxMax.z;
	m_vtxPos[3].y = m_vtxMin.y;
	m_vtxPos[4].x = m_vtxMin.x;
	m_vtxPos[4].z = m_vtxMin.z;
	m_vtxPos[4].y = m_vtxMax.y;
	m_vtxPos[5].x = m_vtxMax.x;
	m_vtxPos[5].z = m_vtxMin.z;
	m_vtxPos[5].y = m_vtxMax.y;
	m_vtxPos[6].x = m_vtxMin.x;
	m_vtxPos[6].z = m_vtxMin.z;
	m_vtxPos[6].y = m_vtxMin.y;
	m_vtxPos[7].x = m_vtxMax.x;
	m_vtxPos[7].z = m_vtxMin.z;
	m_vtxPos[7].y = m_vtxMin.y;

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_MODEL_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;			// 親のマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 親のモデルのマトリックスの設定
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtx();
	}
	// 親のモデルが無い場合
	else
	{
		// ワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出したパーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	int nCntTex = 0;
	// それぞれのパーツにマテリアル・テクスチャを設定して描画
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse.a = m_col.a;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntTex]);
			nCntTex++;
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 生成処理
//=============================================================================
CModel* CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aModelName)
{
	// クラスの生成
	CModel *pModel = NULL;
	pModel = new CModel;

	if (pModel != NULL)
	{
		// 初期化処理
		pModel->Init(pos, rot, aModelName);
	}
	return pModel;
}

//=============================================================================
// 親モデル設定処理
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
// ワールドマトリックス座標取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetMtxPos(void)
{
	return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}