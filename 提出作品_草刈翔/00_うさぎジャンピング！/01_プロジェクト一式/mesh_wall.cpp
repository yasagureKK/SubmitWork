//=============================================================================
// メッシュ(ウォール―ル)処理 [mesh_wall.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mesh_wall.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshWall::CMeshWall(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = 0;
	m_nRow = 0;
	m_nNum = 0;
	m_type = MESHWALL_TYPE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
	//※(縦の分割数＋１)×(横の分割数＋１)の値の頂点を生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nVertical + 1) * (m_nRow + 1),
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
	for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3((-m_size.x / 2.0f) + (nCntRow * (m_size.x / m_nRow)),
				(m_size.y / 2.0f) + (m_size.y / 2.0f) - (nCntVertical * (m_size.y / m_nVertical)),
				m_pos.z);

			// 法線
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// カラーの設定
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + (nCntRow * 1.0f), 0.0f + (nCntVertical * 1.0f));
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	//※(頂点の合計)+(重複して置かれた頂点)の値を生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nRow + 1) * (m_nVertical + 1)) + ((m_nRow + 3) * (m_nVertical - 1))),
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
	for (int nCntVertical = 0; nCntVertical < m_nVertical; nCntVertical++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++)
		{
			// 横の列は2つずつ設定する
			// pIdx[(基準の位置)×(現在の奥の分割数)＋(ずらす数)] = (ずらす数)＋(基準の位置)＋(横の分割数+1×現在の縦の分割数)
			pIdx[(m_nRow * 2 + 4) * nCntVertical + 0 + (nCntRow * 2)] = nCntRow + (m_nRow + 1) + (m_nRow + 1) * nCntVertical;
			pIdx[(m_nRow * 2 + 4) * nCntVertical + 1 + (nCntRow * 2)] = nCntRow + 0 + (m_nRow + 1) * nCntVertical;
		}
	}
	// ポリゴンを描画させない部分の番号データの設定
	for (int nCntVertical = 0; nCntVertical < m_nVertical - 1; nCntVertical++)
	{
		// pIdx[(基準の位置)＋(ずらす数)] = (横の分割数)/(横の分割数+2＋ずらす数)＋(横の分割数+1×現在の縦の分割数)
		pIdx[(m_nRow * 2 + 2) + 0 + nCntVertical * (m_nRow * 2 + 4)] = m_nRow + (m_nRow + 1) * nCntVertical;
		pIdx[(m_nRow * 2 + 2) + 1 + nCntVertical * (m_nRow * 2 + 4)] = (m_nRow * 2 + 2) + (m_nRow + 1) * nCntVertical;
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{
	if (m_type == MESHWALL_TYPE_RIGHT_LOOP)
	{
		// プレイヤーを追尾
		TrackingPlayer();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshWall::Draw(void)
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
		((m_nRow + 1) * (m_nVertical + 1)),						// 頂点の数
		0,														// 開始する頂点のインデックス
		(m_nRow * m_nVertical * 2) + (m_nVertical * 4) - 4);	// 描画するプリミティブ数
}

//=============================================================================
// 生成処理
//=============================================================================
CMeshWall* CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nVertical, int nRow, MESHWALL_TYPE type, CTexture::TEX_TYPE tex)
{
	// インスタンスの生成
	CMeshWall *pMeshWall = NULL;

	// ヌルチェック
	if (pMeshWall == NULL)
	{
		// クラスの生成
		pMeshWall = new CMeshWall;
		if (pMeshWall != NULL)
		{
			// 変数の初期化
			pMeshWall->m_rot = rot;
			pMeshWall->m_nVertical = nVertical;
			pMeshWall->m_nRow = nRow;
			pMeshWall->m_type = type;
			pMeshWall->m_pTexture = CManager::GetTexture()->GetTexture(tex);

			// 初期化処理
			pMeshWall->Init(pos, size);
		}
	}

	return pMeshWall;
}

//=============================================================================
// テクスチャ割当処理
//=============================================================================
void CMeshWall::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 番号設定処理
//=============================================================================
void CMeshWall::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// プレイヤー追尾処理
//=============================================================================
void CMeshWall::TrackingPlayer(void)
{
	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// 番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				// プレイヤーの位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				if (pos.x > m_pos.x + m_size.x)
				{
					m_pos.x += m_size.x * 2;
				}

				SetPos(m_pos);
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}