//=============================================================================
// メッシュ(円柱)処理 [mesh_cylinder.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mesh_cylinder.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshCylinder::CMeshCylinder(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nSide = 0;
	m_nVertical = 0;
	m_fRadius = 0;
	m_fAngle = 0;
	m_fAddAngle = 0;
	m_fSpinSpeed = 0;
	m_fAddSpinSpeed = 0;
	m_bOutSide = false;
	m_nNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nNum = 0;
	m_fAddSpinSpeed = 0;

	// 位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	m_fRadius = sqrtf(m_size.x * m_size.x + m_size.z * m_size.z);
	m_fAngle = atan2f(m_size.x, m_size.z);
	m_fAddAngle = 0;

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

	for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{
		for (int nCntSide = 0; nCntSide < m_nSide + 1; nCntSide++, nCntVtx++)
		{
			// 頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3((-m_size.x / 2) * cosf(m_fAngle - m_fAddAngle),
				m_size.y - (nCntVertical * (m_size.y / m_nVertical)),
				(-m_size.z / 2) * sinf(m_fAngle - m_fAddAngle));

			// 頂点が重複する場合、角度は加えない
			if (nCntSide != m_nSide)
			{
				if (m_bOutSide == false)
				{
					// シリンダーの内側に描画
					m_fAddAngle += (D3DX_PI / (m_nSide / 2.0f));
				}
				else
				{
					// シリンダーの外側に描画
					m_fAddAngle -= (D3DX_PI / (m_nSide / 2.0f));
				}
			}

			// 法線
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// カラー
			pVtx[nCntVtx].col = m_col;

			// テクスチャ
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + (nCntSide * 1.0f), 0.0f + (nCntVertical * 1.0f));
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nSide + 1) * (m_nVertical + 1)) + ((m_nSide + 3) * (m_nVertical - 1))),
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
	for (int nCntVertical = 0; nCntVertical < m_nVertical; nCntVertical++)
	{
		for (int nCntSide = 0; nCntSide < m_nSide + 1; nCntSide++)
		{
			// 横の列は2つずつ設定する
			pIdx[(m_nSide * 2 + 4) * nCntVertical + 0 + (nCntSide * 2)] = nCntSide + (m_nSide + 1) + (m_nSide + 1) * nCntVertical;
			pIdx[(m_nSide * 2 + 4) * nCntVertical + 1 + (nCntSide * 2)] = nCntSide + 0			   + (m_nSide + 1) * nCntVertical;
		}
	}
	// ポリゴンを描画させない部分の番号データの設定
	for (int nCntVertical = 0; nCntVertical < m_nVertical - 1; nCntVertical++)
	{
		// pIdx[(基準の位置)＋(ずらす数)] = (横の分割数)/(横の分割数+2＋ずらす数)＋(横の分割数+1×現在の奥の分割数)
		pIdx[(m_nSide * 2 + 2) + 0 + nCntVertical * (m_nSide * 2 + 4)] = m_nSide		   + (m_nSide + 1) * nCntVertical;
		pIdx[(m_nSide * 2 + 2) + 1 + nCntVertical * (m_nSide * 2 + 4)] = (m_nSide * 2 + 2) + (m_nSide + 1) * nCntVertical;
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshCylinder::Uninit(void)
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
void CMeshCylinder::Update(void)
{
	// 回転処理
	Spin();

	// プレイヤーを追尾
	TrackingPlayer();

	// 種類が背景色だったら
	if (m_type == CYLINDER_TYPE_BACK_COLOR)
	{
		// カラー変更処理
		ChangeColor();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshCylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		0,
		((m_nSide + 1) * (m_nVertical + 1)),					// 頂点の数
		0,														// 開始する頂点のインデックス
		(m_nSide * m_nVertical * 2) + (m_nVertical * 4) - 4);	// 描画するプリミティブ数

														//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CMeshCylinder* CMeshCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nVertical, int nSide,
	D3DXCOLOR col, bool bOutSide, float fSpinSpeed, CYLINDER_TYPE type, CTexture::TEX_TYPE tex)
{
	// インスタンスの生成
	CMeshCylinder *pMeshCylinder = NULL;

	// ヌルチェック
	if (pMeshCylinder == NULL)
	{
		// クラスの生成
		pMeshCylinder = new CMeshCylinder;
		if (pMeshCylinder != NULL)
		{
			// 変数の初期化
			pMeshCylinder->m_rot = rot;
			pMeshCylinder->m_nVertical = nVertical;
			pMeshCylinder->m_nSide = nSide;
			pMeshCylinder->m_col = col;
			pMeshCylinder->m_bOutSide = bOutSide;
			pMeshCylinder->m_fSpinSpeed = fSpinSpeed;
			pMeshCylinder->m_type = type;

			if (pMeshCylinder->m_type == CYLINDER_TYPE_BACK_COLOR)
			{
				pMeshCylinder->m_pTexture = NULL;
			}
			else
			{
				pMeshCylinder->m_pTexture = CManager::GetTexture()->GetTexture(tex);
			}

			// 初期化処理
			pMeshCylinder->Init(pos, size);
		}
	}

	return pMeshCylinder;
}

//=============================================================================
// テクスチャ割当処理
//=============================================================================
void CMeshCylinder::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 番号設定処理
//=============================================================================
void CMeshCylinder::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// プレイヤー追尾処理
//=============================================================================
void CMeshCylinder::TrackingPlayer(void)
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

			// プレイヤーの位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// 番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				m_pos.x = pos.x;
				m_pos.z = pos.z;

				// 種類が背景色だったら
				if (m_type == CYLINDER_TYPE_BACK_COLOR)
				{
					m_pos.y = pos.y - (m_size.y / 2);
				}

				SetPos(m_pos);
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 回転処理
//=============================================================================
void CMeshCylinder::Spin(void)
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

			// スコアの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				// プレイヤーの移動量を取得
				D3DXVECTOR3 move = pPlayer->GetMove();

				m_fAddSpinSpeed = -move.x / 2000;

				// 求めた値を加算して回転させる
				m_rot.y += m_fSpinSpeed + m_fAddSpinSpeed;
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// カラー変更処理
//=============================================================================
void CMeshCylinder::ChangeColor(void)
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

			// プレイヤーの位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// スコアの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				int nAlpha = (int)pos.y / 25;

				if (nAlpha > 255)
				{
					nAlpha = 255;
				}
				else if(nAlpha < 0)
				{
					nAlpha = 0;
				}

				// プレイヤーの高さによって透明度を変える
				//※透明度を上げ、空の色を明るい→暗いに徐々に変えている
				m_col = D3DCOLOR_RGBA(180 , 240, 255, 255 - nAlpha);

				// 頂点バッファへのポインタ
				VERTEX_3D *pVtx;
				// 頂点バッファをロックし、頂点データのポインタを取得
				m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				int nCntVtx = 0;
				for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
				{
					for (int nCntSide = 0; nCntSide < m_nSide + 1; nCntSide++, nCntVtx++)
					{
						// カラー
						pVtx[nCntVtx].col = m_col;
					}
				}
				// 頂点バッファをアンロックする
				m_pVtxBuff->Unlock();
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}