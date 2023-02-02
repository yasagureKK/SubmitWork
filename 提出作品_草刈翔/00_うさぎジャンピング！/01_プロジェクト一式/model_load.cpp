//=============================================================================
// モデルのロード処理 [model_load.cpp]
// Authore: 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "model_load.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CModelLoad::CModelLoad()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CModelLoad::~CModelLoad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CModelLoad::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//**********************//
	//*  モデルの読み込み  *//
	//**********************//
	//+---------------+
	//|  プレイヤー1  |
	//+---------------+
	// ウサギ(体)
	D3DXLoadMeshFromX("data/MODEL/rabbit/00_body.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_BODY], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_BODY], &m_apMesh[MODEL_TYPE_RABBIT_BODY]);
	// ウサギ(頭)
	D3DXLoadMeshFromX("data/MODEL/rabbit/01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HEAD], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HEAD], &m_apMesh[MODEL_TYPE_RABBIT_HEAD]);
	// ウサギ(右耳0)
	D3DXLoadMeshFromX("data/MODEL/rabbit/02_earR_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_RIGHT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_RIGHT_0], &m_apMesh[MODEL_TYPE_RABBIT_EAR_RIGHT_0]);
	// ウサギ(右耳1)
	D3DXLoadMeshFromX("data/MODEL/rabbit/03_earR_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_RIGHT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_RIGHT_1], &m_apMesh[MODEL_TYPE_RABBIT_EAR_RIGHT_1]);
	// ウサギ(左耳0)
	D3DXLoadMeshFromX("data/MODEL/rabbit/04_earL_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_LEFT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_LEFT_0], &m_apMesh[MODEL_TYPE_RABBIT_EAR_LEFT_0]);
	// ウサギ(左耳1)
	D3DXLoadMeshFromX("data/MODEL/rabbit/05_earL_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_LEFT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_LEFT_1], &m_apMesh[MODEL_TYPE_RABBIT_EAR_LEFT_1]);
	// ウサギ(右手)
	D3DXLoadMeshFromX("data/MODEL/rabbit/06_handR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HAND_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HAND_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_HAND_RIGHT]);
	// ウサギ(左手)
	D3DXLoadMeshFromX("data/MODEL/rabbit/07_handL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HAND_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HAND_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_HAND_LEFT]);
	// ウサギ(右腿)
	D3DXLoadMeshFromX("data/MODEL/rabbit/08_legR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_LEG_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_LEG_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_LEG_RIGHT]);
	// ウサギ(右足)
	D3DXLoadMeshFromX("data/MODEL/rabbit/09_footR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_FOOT_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_FOOT_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_FOOT_RIGHT]);
	// ウサギ(左腿)
	D3DXLoadMeshFromX("data/MODEL/rabbit/10_legL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_LEG_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_LEG_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_LEG_LEFT]);
	// ウサギ(左足)
	D3DXLoadMeshFromX("data/MODEL/rabbit/11_footL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_FOOT_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_FOOT_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_FOOT_LEFT]);
	//+---------------+
	//|  プレイヤー2  |
	//+---------------+
	// ウサギ2(体)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/00_body.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_BODY], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_BODY], &m_apMesh[MODEL_TYPE_RABBIT_2_BODY]);
	// ウサギ2(頭)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HEAD], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HEAD], &m_apMesh[MODEL_TYPE_RABBIT_2_HEAD]);
	// ウサギ2(右耳0)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/02_earR_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0]);
	// ウサギ2(右耳1)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/03_earR_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1]);
	// ウサギ2(左耳0)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/04_earL_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_0], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_LEFT_0]);
	// ウサギ2(左耳1)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/05_earL_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_1], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_LEFT_1]);
	// ウサギ2(右手)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/06_handR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HAND_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HAND_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_HAND_RIGHT]);
	// ウサギ2(左手)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/07_handL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HAND_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HAND_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_HAND_LEFT]);
	// ウサギ2(右腿)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/08_legR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_LEG_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_LEG_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_LEG_RIGHT]);
	// ウサギ2(右足)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/09_footR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_FOOT_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_FOOT_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_FOOT_RIGHT]);
	// ウサギ2(左腿)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/10_legL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_LEG_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_LEG_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_LEG_LEFT]);
	// ウサギ2(左足)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/11_footL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_FOOT_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_FOOT_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_FOOT_LEFT]);

	//+----------------+
	//|  オブジェクト  |
	//+----------------+
	// 看板
	D3DXLoadMeshFromX("data/MODEL/object/signboard.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_SIGNBOARD], NULL, &m_aNumMat[MODEL_TYPE_SIGNBOARD], &m_apMesh[MODEL_TYPE_SIGNBOARD]);
	// ニンジン
	D3DXLoadMeshFromX("data/MODEL/object/carrot.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_CARROT], NULL, &m_aNumMat[MODEL_TYPE_CARROT], &m_apMesh[MODEL_TYPE_CARROT]);
	// 岩
	D3DXLoadMeshFromX("data/MODEL/object/rock.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_ROCK], NULL, &m_aNumMat[MODEL_TYPE_ROCK], &m_apMesh[MODEL_TYPE_ROCK]);
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelLoad::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MODEL_TYPE_MAX; nCntModel++)
	{
		// メッシュの破棄
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}

		// マテリアルの破棄
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}
}

//=============================================================================
// メッシュ割当処理
//=============================================================================
LPD3DXMESH CModelLoad::GetMesh(MODEL_TYPE type)
{
	return m_apMesh[type];
}

//=============================================================================
// マテリアル割当処理
//=============================================================================
LPD3DXBUFFER CModelLoad::GetBuffMat(MODEL_TYPE type)
{
	return m_apBuffMat[type];
}

//=============================================================================
// マテリアル数割当処理
//=============================================================================
DWORD CModelLoad::GetNumMat(MODEL_TYPE type)
{
	return m_aNumMat[type];
}
