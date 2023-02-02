//=============================================================================
// 単体のモデル処理 [model_single.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "model_single.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "model_load.h"
#include "score_board.h"
#include "shadow.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CModelSingle::CModelSingle(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	m_modelType = MODEL_SINGLE_TYPE_NONE;
	m_pScoreBoard = NULL;
	m_pShadow = NULL;
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
HRESULT CModelSingle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_pos = pos;
	m_posOld = m_pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モデルの生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot, m_type);

	// 親モデルの設定(単体のためNULLを入れる)
	m_pModel->SetParent(NULL);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJECT_TYPE_MODEL);

	// サイズを取得
	m_size = m_pModel->GetSize();
	SetSize(m_size);
	SetPos(m_pos);

	// 看板ならスコアも同時に生成
	if (m_modelType == MODEL_SINGLE_TYPE_SIGNBOARD)
	{
		m_pScoreBoard = CScoreBoard::Create(D3DXVECTOR3(pos.x, pos.y + 85.0f, pos.z - 10.0f),
			D3DXVECTOR3(40.0f, 40.0f, 0.0f), D3DCOLOR_RGBA(255, 200, 0, 255));
	}

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CModelSingle::Uninit(void)
{
	// 影があったら消しておく
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

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
	SetPosOld(m_posOld);

	// 看板ならスコアも同時に生成
	if (m_modelType == MODEL_SINGLE_TYPE_SIGNBOARD)
	{
		m_pScoreBoard->SetPos(D3DXVECTOR3(pos.x, pos.y + 85.0f, pos.z - 10.0f));
	}
}

//================================================
// 描画処理
//================================================
void CModelSingle::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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
CModelSingle *CModelSingle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type, MODEL_SINGLE_TYPE modelType)
{
	// インスタンスの生成
	CModelSingle *pModelSingle = NULL;

	// ヌルチェック
	if (pModelSingle == NULL)
	{
		// クラスの生成
		pModelSingle = new CModelSingle;
		if (pModelSingle != NULL)
		{
			// 動きをつけたい場合
			switch (modelType)
			{
			case MODEL_SINGLE_TYPE_SIGNBOARD:
				break;

			default:
				break;
			}

			// 変数の初期化
			pModelSingle->m_rot = rot;
			pModelSingle->m_type = type;
			pModelSingle->m_modelType = modelType;

			// 初期化処理
			pModelSingle->Init(pos, pModelSingle->m_size);
		}
	}

	return pModelSingle;
}

//================================================
// 当たり判定処理
//================================================
bool CModelSingle::Collision(CScene *pScene)
{
	bool bLand = false;	//着地しているかどうか

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


			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象の位置
			D3DXVECTOR3 posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの位置
			D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象のサイズ
			D3DXVECTOR3 sizeModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルのサイズ
			D3DXVECTOR3 posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象の1フレーム前の位置

																		//モデルの位置・サイズ取得
			posModel = pModelSingle->GetPos();
			sizeModel = pModelSingle->GetSize();

			//対象の位置・サイズ取得
			pos = pScene->GetPos();
			posOld = pScene->GetPosOld();
			size = pScene->GetSize();

			if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y <= posModel.y + sizeModel.y &&
				posOld.y >= posModel.y + sizeModel.y)
			{
				pos.y = posModel.y + sizeModel.y;
				//位置を設定
				pScene->SetPos(pos);

				bLand = true;
			}
			else if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y + size.y >= posModel.y &&
				posOld.y + size.y <= posModel.y)
			{
				pos.y = posModel.y - size.y;
				//位置を設定
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x + size.x / 2.0f >= posModel.x - sizeModel.x / 2.0f &&
				posOld.x + size.x / 2.0f <= posModel.x - sizeModel.x / 2.0f)
			{
				pos.x = posModel.x - sizeModel.x / 2.0f - size.x / 2.0f;
				//位置を設定
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x - size.x / 2.0f <= posModel.x + sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f >= posModel.x + sizeModel.x / 2.0f)
			{
				pos.x = posModel.x + sizeModel.x / 2.0f + size.x / 2.0f;
				//位置を設定
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f >= posModel.z - sizeModel.z / 2.0f &&
				posOld.z + size.x / 2.0f <= posModel.z - sizeModel.z / 2.0f)
			{
				pos.z = posModel.z - sizeModel.z / 2.0f - size.x / 2.0f;
				//位置を設定
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z - size.x / 2.0f <= posModel.z + sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f >= posModel.z + sizeModel.z / 2.0f)
			{
				pos.z = posModel.z + sizeModel.z / 2.0f + size.x / 2.0f;
				//位置を設定
				pScene->SetPos(pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}

//=============================================================================
// 影の設定処理
//=============================================================================
void CModelSingle::SetShadow(bool bOnShadow, float fHeight)
{
	if (bOnShadow == true && m_pShadow == NULL)
	{
		// 影の生成→影を使用している状態に→影の追従(高さにオフセットをつけて調整可能)
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x + m_size.x / 3, 0.0f, m_size.x + m_size.x / 4), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pShadow->SetUseShadow(true);
		m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, m_pos.y + fHeight, m_pos.z), 0.0f);
	}
}