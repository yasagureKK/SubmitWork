//=============================================================================
// ビルボード処理 [billboard.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "shadow.h"
#include "model_single.h"
#include "mesh_field.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboard::CBillboard(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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

	VERTEX_3D *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
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

//=============================================================================
// 終了処理
//=============================================================================
void CBillboard::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboard::Update(void)
{
	// 位置・サイズ取得
	D3DXVECTOR3 pos = GetPos();
	m_pos = pos;
	D3DXVECTOR3 size = GetSize();
	m_size = size;

	// 移動量反映
	m_pos += m_move;

	// 位置・サイズを設定
	SetPos(m_pos, m_size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	D3DXMATRIX mtxView;						//ビューマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//計算用マトリックス
	D3DXMatrixIdentity(&m_mtxWorld);		//ワールドマトリックスの初期化

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,											//開始する頂点のインデックス
		2);											//描画するプリミティブ数

	// Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CTexture::TEX_TYPE tex, int nPriolity)
{
	// インスタンスの生成
	CBillboard *pBillboard = NULL;

	// ヌルチェック
	if (pBillboard == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pBillboard = new CBillboard(nPriolity);
		if (pBillboard != NULL)
		{
			// 変数の初期化
			pBillboard->m_pTexture = CManager::GetTexture()->GetTexture(tex);

			// 初期化処理
			pBillboard->Init(pos, size);
		}
	}

	return pBillboard;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CBillboard::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 位置設定処理
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// 頂点情報
	VERTEX_3D *pVtx;	
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ設定処理
//=============================================================================
void CBillboard::SetTexture(float fU, float fV, int nCutU, int nCutV)
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

//=============================================================================
// テクスチャ割り当て処理
//=============================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// カラー設定処理
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col)
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

//=============================================================================
// 影の設定処理
//=============================================================================
void CBillboard::SetShadow(bool bOn)
{
	if (m_pShadow == NULL)
	{
		// 影の生成→影を使用している状態に→影の設定(高さにオフセットをつけて調整可能)
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.x), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pShadow->SetUseShadow(true);
	}

	// 影があるなら
	if (m_pShadow != NULL)
	{
		//オブジェクト情報を入れるポインタ
		CScene *pObject = NULL;
		//オブジェクト情報を保存するポインタ変数
		CScene *pSaveObject = NULL;

		//先頭のポインタを代入
		pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

		while (pObject != NULL)
		{
			//現在のオブジェクトのポインタを保存
			pSaveObject = pObject;

			if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
			{
				// モデルシングルクラスにキャスト
				CModelSingle *pModelSingle = NULL;
				pModelSingle = (CModelSingle*)pObject;

				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 保存用の位置
				D3DXVECTOR3 posObj = pModelSingle->GetPos();		// オブジェクトの位置を取得
				D3DXVECTOR3 posObjOld = pModelSingle->GetPosOld();	// オブジェクトの1フレーム前の位置を取得
				D3DXVECTOR3 sizeObj = pModelSingle->GetSize();		// オブジェクトのサイズを取得

				if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
					posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
					posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
					posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
					posObj.y + sizeObj.y >= m_pos.y - (m_size.y / 2) - 500.0f &&
					posObj.y + sizeObj.y <= m_pos.y - (m_size.y / 2))
				{
					//位置をモデルの上に
					pos.y = posObj.y + sizeObj.y;

					//影の追従
					m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, pos.y, m_pos.z), (m_pos.y - pos.y));

					//影を使っている状態にする
					m_pShadow->SetUseShadow(true);

					return;
				}
				else
				{
					//影を使っていない状態にする
					m_pShadow->SetUseShadow(false);
				}
			}

			pObject = pSaveObject->GetObjNext(pSaveObject);
		}

		//先頭のポインタを代入
		pObject = pObject->GetTopObj(CScene::PRIORITY_NONE);

		while (pObject != NULL)
		{
			//現在のオブジェクトのポインタを保存
			pSaveObject = pObject;

			if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
			{
				// メッシュフィールドクラスにキャスト
				CMeshField *pMeshField = NULL;
				pMeshField = (CMeshField*)pObject;

				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 保存用の位置
				D3DXVECTOR3 posObj = pMeshField->GetPos();			// オブジェクトの位置を取得
				D3DXVECTOR3 posObjOld = pMeshField->GetPosOld();	// オブジェクトの1フレーム前の位置を取得
				D3DXVECTOR3 sizeObj = pMeshField->GetSize();		// オブジェクトのサイズを取得

				if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
					posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
					posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
					posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
					posObj.y + sizeObj.y >= m_pos.y + (m_size.y / 2) - 500.0f &&
					posObj.y + sizeObj.y <= m_pos.y + (m_size.y / 2))
				{
					//位置をフィールドの上に
					pos.y = posObj.y;

					//影の追従
					m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, pos.y, m_pos.z), (m_pos.y - pos.y));

					//影を使っている状態にする
					m_pShadow->SetUseShadow(true);

					return;
				}
				else
				{
					//影を使っていない状態にする
					m_pShadow->SetUseShadow(false);
				}
			}

			pObject = pSaveObject->GetObjNext(pSaveObject);
		}
	}
}

//=============================================================================
// 影の設定処理
//=============================================================================
void CBillboard::DeleteShadow(void)
{
	//※終了処理にこの処理を書くとエラーが発生するため別の関数で処理
	// 影を使用していたら消す
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = NULL;
	}
}