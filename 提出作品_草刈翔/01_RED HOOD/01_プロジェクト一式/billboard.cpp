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
#include "texture.h"
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
	m_bShadow = false;
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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// 変数の初期化
	m_pos = pos;
	m_size = size;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

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
	// 位置取得
	D3DXVECTOR3 pos = GetPos();
	m_pos = pos;

	// 移動量反映
	m_pos += m_move;

	// 影をつけるなら
	if (m_bShadow == true)
	{
		// 影の追従処理
		TrackingShadow();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 計算用マトリックス
	D3DXMATRIX mtxView, mtxRot, mtxTrans, mtxScale;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// アルファテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成処理
//=============================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType, bool bShadow)
{
	// クラスの生成
	CBillboard *pBillboard = NULL;
	pBillboard = new CBillboard();

	if (pBillboard != NULL)
	{
		// 変数の初期化
		pBillboard->m_pTexture = CManager::GetTexture()->GetTexture3D(nTexType);
		pBillboard->m_bShadow = bShadow;

		// 初期化処理
		pBillboard->Init(pos, size);
	}
	return pBillboard;
}

//=============================================================================
// 位置設定処理
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	// 位置を更新
	m_pos = pos;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点座標を更新
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズ設定処理
//=============================================================================
void CBillboard::SetSize(D3DXVECTOR3 size)
{
	// サイズを更新
	m_size = size;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点座標を更新
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
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
// 影の追従処理
//=============================================================================
void CBillboard::TrackingShadow(void)
{
	// 影がなかったら
	if (m_pShadow == NULL)
	{
		// 影の生成→影を使用している状態に設定
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.x));
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
// 影の消去処理
//=============================================================================
void CBillboard::DeleteShadow(void)
{
	// 影を使用していたら消す
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = NULL;
	}
}

//=============================================================================
// 頂点座標位置設定処理
//=============================================================================
void CBillboard::SetVertex(D3DXVECTOR3 Vertex0, D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 位置を更新
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(Vertex0.x, Vertex0.y, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(Vertex1.x, Vertex1.y, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(Vertex2.x, Vertex2.y, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(Vertex3.x, Vertex3.y, -m_size.z / 2.0f);
	m_pVtxBuff->Unlock();
}