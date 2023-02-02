//=============================================================================
// 球体コリジョン処理 [collision_sphere.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "collision_sphere.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "model.h"
#include "keyboard.h"
#include "fade.h"
#include "sound.h"
#include "particle_hit.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CCollisionSphere::m_bMngVisual = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CCollisionSphere::CCollisionSphere(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posSource = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
	m_col = D3DCOLOR_RGBA(0, 0, 0, 0);
	m_collisionType = COLLISION_S_TYPE_NONE;
	m_saveCollisionType = COLLISION_S_TYPE_NONE;
	m_nVertical = 0;
	m_nSide = 0;
	m_fTime = 0.0f;
	m_fDamage = 0.0f;
	m_fObjectiveRot = 0.0f;
	m_bContact = false;
	m_bContactOld = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCollisionSphere::~CCollisionSphere()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCollisionSphere::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_posSource = pos;
	m_size = size;
	m_nVertical = 16;
	m_nSide = 16;
	m_fObjectiveRot = 0.0f;
	m_bContact = false;
	m_bContactOld = false;

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
	for (int nCntSide = m_nSide + 1; nCntSide > 0; nCntSide--)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3(cosf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)),
											cosf((D3DX_PI * 2.0f / m_nVertical) * nCntSide) * (m_size.x / 2.0f),
											sinf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)));

			// 法線
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// カラー
			pVtx[nCntVtx].col = m_col;

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
void CCollisionSphere::Uninit(void)
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
void CCollisionSphere::Update(void)
{
	// 前のフレームに当たったかどうかの情報を代入
	m_bContactOld = m_bContact;

	m_bContact = false;

	// 球同士の当たり判定
	Collision(this);

#ifdef _DEBUG
	// 判定の可視化処理
	//VisualChange();
#endif

	// 寿命を減らす
	if (m_fTime > -1.0f)
	{
		m_fTime -= 1.0f;

		// 時間経過で消す
		if (m_fTime <= 0.0f)
		{
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCollisionSphere::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のモデルのマトリックスの設定
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtx();

		// ワールドマトリックスと親のワールドマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}
	// 親のモデルが無い場合
	else
	{
		// ワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

#ifdef _DEBUG
	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	if (m_bMngVisual == true)
	{
		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,
			0,
			((m_nVertical + 1) * (m_nSide + 1)),				//頂点の数
			0,													//開始する頂点のインデックス
			(m_nSide * m_nVertical * 2) + (m_nSide * 4) - 4);	//描画するプリミティブ数
	}
#endif

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CCollisionSphere* CCollisionSphere::Create(D3DXVECTOR3 pos, float fSize, COLLISION_S_TYPE type, float fTime)
{
	// クラスの生成
	CCollisionSphere *pCollisionS = NULL;
	pCollisionS = new CCollisionSphere;

	if (pCollisionS != NULL)
	{
		// 変数の初期化
		pCollisionS->m_fTime = fTime;
		pCollisionS->m_collisionType = type;

		// ワールドマトリックス設定前に衝突判定を行うため、先に引数の位置をワールドマトリックスに設定しておく
		pCollisionS->m_mtxWorld._41 = pos.x;
		pCollisionS->m_mtxWorld._42 = pos.y;
		pCollisionS->m_mtxWorld._43 = pos.z;

		switch (type)
		{
			// なし
		case COLLISION_S_TYPE_NONE:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 255, 255, 0);
			break;
			// プレイヤー：白
		case COLLISION_S_TYPE_PLAYER:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 255, 255, 153);
			break;
			// 武器：緑
		case COLLISION_S_TYPE_WEAPON:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
			break;
			// 武器[強]：緑
		case COLLISION_S_TYPE_WEAPON_P:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
			break;
			// 敵：青
		case COLLISION_S_TYPE_ENEMY:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 0, 255, 153);
			break;
			// 敵の攻撃：赤
		case COLLISION_S_TYPE_ENEMY_ATTACK:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 0, 0, 153);
			break;
		default:
			break;
		}

		// 初期化処理
		pCollisionS->Init(pos, D3DXVECTOR3(fSize, fSize, fSize));
	}
	return pCollisionS;
}

//=============================================================================
// 衝突処理
//=============================================================================
void CCollisionSphere::Collision(CScene *pScene)
{
	// 衝突しているかどうか
	m_saveCollisionType = COLLISION_S_TYPE_NONE;

	//オブジェクト情報を入れるポインタ
	CScene *pObject = NULL;
	//オブジェクト情報を保存するポインタ変数
	CScene *pSaveObject = NULL;

	//先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_EFFECT);

	while (pObject != NULL)
	{
		//現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_NONE)
		{
			CCollisionSphere *pCollisionS = NULL;
			pCollisionS = (CCollisionSphere*)pObject;

			// 自分以外の球体コリジョンがあったら
			if (pObject != this)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);	//自身の位置
				D3DXVECTOR3 size = pScene->GetSize();											//自身サイズ
				D3DXVECTOR3 posCollisionS = pCollisionS->GetMtxPos();							//対象のコリジョンの位置
				D3DXVECTOR3 sizeCollisionS = pCollisionS->GetSize();							//対象のコリジョンのサイズ
				COLLISION_S_TYPE collisionType = pCollisionS->GetCollisionType();				//対象のコリジョンの種類

				// コリジョン同士の距離と半径を求める
				float fDistance = sqrtf((posCollisionS.x - pos.x) * (posCollisionS.x - pos.x) + (posCollisionS.y - pos.y) * (posCollisionS.y - pos.y) + (posCollisionS.z - pos.z) * (posCollisionS.z - pos.z));
				float fRadius = sqrtf((sizeCollisionS.x / 2 + size.x / 2) * (sizeCollisionS.x / 2 + size.x / 2));

				// 距離が半径より小さくなったら衝突
				if (fDistance < fRadius)
				{
					// プレイヤー・敵の攻撃
					if ((m_collisionType == COLLISION_S_TYPE_PLAYER && collisionType == COLLISION_S_TYPE_ENEMY_ATTACK) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY_ATTACK && collisionType == COLLISION_S_TYPE_PLAYER))
					{
						// 対象のコリジョンの種類を保存
						m_saveCollisionType = collisionType;

						// 触れている状態にする
						m_bContact = true;

						// 対象のコリジョン発生位置から、対象のコリジョンの向きを設定
						D3DXVECTOR3 posSource = pCollisionS->GetPosSource();
						m_fObjectiveRot = (float)atan2((posSource.x - pos.x), (posSource.z - pos.z)) - D3DX_PI;

						if (m_collisionType == COLLISION_S_TYPE_ENEMY_ATTACK)
						{
							// ダメージを設定
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
					// 武器・敵
					else if ((m_collisionType == COLLISION_S_TYPE_WEAPON && collisionType == COLLISION_S_TYPE_ENEMY) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY && collisionType == COLLISION_S_TYPE_WEAPON))
					{
						// 対象のコリジョンの種類を保存
						m_saveCollisionType = collisionType;

						// 触れている状態にする
						m_bContact = true;

						if (m_collisionType == COLLISION_S_TYPE_WEAPON)
						{
							// ダメージを設定
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
					// 武器[強]・敵
					else if ((m_collisionType == COLLISION_S_TYPE_WEAPON_P && collisionType == COLLISION_S_TYPE_ENEMY) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY && collisionType == COLLISION_S_TYPE_WEAPON_P))
					{
						// 対象のコリジョンの種類を保存
						m_saveCollisionType = collisionType;

						// 触れている状態にする
						m_bContact = true;

						if (m_collisionType == COLLISION_S_TYPE_WEAPON_P)
						{
							// ダメージを設定
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
				}
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// ワールドマトリックス座標取得処理
//=============================================================================
D3DXVECTOR3 CCollisionSphere::GetMtxPos(void)
{
	return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

//=============================================================================
// 衝突した瞬間の処理
//=============================================================================
bool CCollisionSphere::OnCollisionEnter(COLLISION_S_TYPE type)
{
	if (m_saveCollisionType == type && (m_bContactOld == false && m_bContact == true))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// コリジョンの種類設定処理
//=============================================================================
void CCollisionSphere::SetCollisionType(COLLISION_S_TYPE type)
{
	m_collisionType = type;

	switch (type)
	{
		// なし
	case COLLISION_S_TYPE_NONE:
		m_col = D3DCOLOR_RGBA(255, 255, 255, 0);
		break;
		// プレイヤー：白
	case COLLISION_S_TYPE_PLAYER:
		m_col = D3DCOLOR_RGBA(255, 255, 255, 153);
		break;
		// 武器：緑
	case COLLISION_S_TYPE_WEAPON:
		m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
		break; 
		// 武器[強]：緑
	case COLLISION_S_TYPE_WEAPON_P:
		m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
		break;
		// 敵：青
	case COLLISION_S_TYPE_ENEMY:
		m_col = D3DCOLOR_RGBA(0, 0, 255, 153);
		break;
		// 敵の攻撃：赤
	case COLLISION_S_TYPE_ENEMY_ATTACK:
		m_col = D3DCOLOR_RGBA(255, 0, 0, 153);
		break;
	default:
		break;
	}

	if (m_pVtxBuff != NULL)
	{
		VERTEX_3D *pVtx;
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVtx = 0;
		for (int nCntSide = m_nSide + 1; nCntSide > 0; nCntSide--)
		{
			for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
			{
				// カラー
				pVtx[nCntVtx].col = m_col;
			}
		}
		m_pVtxBuff->Unlock();
	}
}

#ifdef _DEBUG
//=============================================================================
// 判定の可視化設定処理
//=============================================================================
void CCollisionSphere::SetVisual(void)
{
	m_bMngVisual = !m_bMngVisual;
}

//=============================================================================
// 判定の可視化処理
//=============================================================================
void CCollisionSphere::VisualChange(void)
{
	//キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_F2) == true)
	{
		m_bMngVisual = !m_bMngVisual;
	}
}
#endif