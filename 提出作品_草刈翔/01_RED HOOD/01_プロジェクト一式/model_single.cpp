//=============================================================================
// 単体のモデル処理 [model_single.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "model_single.h"
#include "manager.h"
#include "model_load.h"
#include "renderer.h"
#include "model.h"
#include "mesh_field.h"
#include "player.h"
#include "shadow.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CModelSingle::CModelSingle(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	//m_pShadow = NULL;
}

//================================================
// デストラクタ
//================================================
CModelSingle::~CModelSingle()
{

}

//================================================
// 初期化処理
//================================================
HRESULT CModelSingle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType)
{
	// 変数の初期化
	m_pos = pos;
	m_posOld = m_pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モデルの生成
	m_pModel = CModel::Create(m_pos, m_rot, CManager::GetModelLoad()->GetModelName(nModelType));

	// 親モデルの設定(単体のためNULLを入れる)
	m_pModel->SetParent(NULL);

	// サイズを取得
	m_size = m_pModel->GetSize();

	SetPos(m_pos);

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CModelSingle::Uninit(void)
{
	// 開放処理
	Release();
}

//================================================
// 更新処理
//================================================
void CModelSingle::Update(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 位置の反映
	m_pos = pos;
	m_posOld = pos;

	// 移動
	m_pos += m_move;

	// 位置を設定
	SetPos(m_pos);
}

//================================================
// 描画処理
//================================================
void CModelSingle::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを設定
	m_pModel->SetRot(m_rot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	m_pModel->Draw();
}

//================================================
// 生成処理
//================================================
CModelSingle *CModelSingle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType)
{
	// クラスの生成
	CModelSingle *pModelSingle = NULL;
	pModelSingle = new CModelSingle;
	
	if (pModelSingle != NULL)
	{
		// 初期化処理
		pModelSingle->Init(pos, rot, nModelType);
	}
	return pModelSingle;
}

//================================================
// 当たり判定処理
//================================================
bool CModelSingle::Collision(CScene *pScene)
{
	//着地しているかどうか
	bool bLand = false;

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
			//モデルシングルにキャスト
			CModelSingle *pModelSingle = NULL;
			pModelSingle = (CModelSingle*)pObject;

			D3DXVECTOR3 posModel = pModelSingle->GetPos();		//モデルの位置
			D3DXVECTOR3 sizeModel = pModelSingle->GetSize();	//モデルのサイズ
			D3DXVECTOR3 pos = pScene->GetPos();					//対象の位置
			D3DXVECTOR3 size = pScene->GetSize();				//対象のサイズ
			D3DXVECTOR3 posOld = pScene->GetPosOld();			//対象の1フレーム前の位置

			if (posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				posOld.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y <= posModel.y + sizeModel.y &&
				posOld.y >= posModel.y + sizeModel.y)
			{
				//位置を設定
				pos.y = posModel.y + sizeModel.y;
				pScene->SetPos(pos);

				bLand = true;
			}
			else if (posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				posOld.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y + size.y >= posModel.y &&
				posOld.y + size.y <= posModel.y)
			{
				//位置を設定
				pos.y = posModel.y - size.y;
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.z + size.z / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x + size.x / 2.0f >= posModel.x - sizeModel.x / 2.0f &&
				posOld.x + size.x / 2.0f <= posModel.x - sizeModel.x / 2.0f)
			{
				//位置を設定
				pos.x = posModel.x - sizeModel.x / 2.0f - size.x / 2.0f;
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.z + size.z / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x - size.x / 2.0f <= posModel.x + sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f >= posModel.x + sizeModel.x / 2.0f)
			{
				//位置を設定
				pos.x = posModel.x + sizeModel.x / 2.0f + size.x / 2.0f;
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.z / 2.0f >= posModel.z - sizeModel.z / 2.0f &&
				posOld.z + size.z / 2.0f <= posModel.z - sizeModel.z / 2.0f)
			{
				//位置を設定
				pos.z = posModel.z - sizeModel.z / 2.0f - size.z / 1.95f;
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z - size.z / 2.0f <= posModel.z + sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f >= posModel.z + sizeModel.z / 2.0f)
			{
				//位置を設定
				pos.z = posModel.z + sizeModel.z / 2.0f + size.z / 1.95f;
				pScene->SetPos(pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}

//=============================================================================
// カラー設定処理
//=============================================================================
void CModelSingle::SetColor(D3DXCOLOR col)
{
	m_pModel->SetColor(col);
}