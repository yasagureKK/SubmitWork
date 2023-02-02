//=============================================================================
// モデルのロード処理 [model_load.h]
// Authore: 草刈 翔
//=============================================================================
#ifndef _MODEL_LOAD_H_
#define _MODEL_LOAD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModelLoad
{
public:
	// モデルの種類
	typedef enum
	{
										//*********↓プレイヤー1↓********//
		MODEL_TYPE_RABBIT_BODY = 0,		// ウサギ(体)
		MODEL_TYPE_RABBIT_HEAD,			// ウサギ(頭)
		MODEL_TYPE_RABBIT_EAR_RIGHT_0,	// ウサギ(右耳0)
		MODEL_TYPE_RABBIT_EAR_RIGHT_1,	// ウサギ(右耳1)
		MODEL_TYPE_RABBIT_EAR_LEFT_0,	// ウサギ(左耳0)
		MODEL_TYPE_RABBIT_EAR_LEFT_1,	// ウサギ(左耳0)
		MODEL_TYPE_RABBIT_HAND_RIGHT,	// ウサギ(右手)
		MODEL_TYPE_RABBIT_HAND_LEFT,	// ウサギ(左手)
		MODEL_TYPE_RABBIT_LEG_RIGHT,	// ウサギ(右腿)
		MODEL_TYPE_RABBIT_FOOT_RIGHT,	// ウサギ(右足)
		MODEL_TYPE_RABBIT_LEG_LEFT,		// ウサギ(左腿)
		MODEL_TYPE_RABBIT_FOOT_LEFT,	// ウサギ(左足)
										//*********↓プレイヤー2↓********//
		MODEL_TYPE_RABBIT_2_BODY,		// ウサギ2(体)
		MODEL_TYPE_RABBIT_2_HEAD,		// ウサギ2(頭)
		MODEL_TYPE_RABBIT_2_EAR_RIGHT_0,// ウサギ2(右耳0)
		MODEL_TYPE_RABBIT_2_EAR_RIGHT_1,// ウサギ2(右耳1)
		MODEL_TYPE_RABBIT_2_EAR_LEFT_0,	// ウサギ2(左耳0)
		MODEL_TYPE_RABBIT_2_EAR_LEFT_1,	// ウサギ2(左耳0)
		MODEL_TYPE_RABBIT_2_HAND_RIGHT,	// ウサギ2(右手)
		MODEL_TYPE_RABBIT_2_HAND_LEFT,	// ウサギ2(左手)
		MODEL_TYPE_RABBIT_2_LEG_RIGHT,	// ウサギ2(右腿)
		MODEL_TYPE_RABBIT_2_FOOT_RIGHT,	// ウサギ2(右足)
		MODEL_TYPE_RABBIT_2_LEG_LEFT,	// ウサギ2(左腿)
		MODEL_TYPE_RABBIT_2_FOOT_LEFT,	// ウサギ2(左足)
										//*********↓オブジェクト↓*********//
		MODEL_TYPE_SIGNBOARD,			// 看板
		MODEL_TYPE_CARROT,				// ニンジン
		MODEL_TYPE_ROCK,				// 岩
		MODEL_TYPE_MAX
	} MODEL_TYPE;

	CModelLoad();								// コンストラクタ
	~CModelLoad();								// デストラクタ
	void Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	LPD3DXMESH GetMesh(MODEL_TYPE type);		// メッシュ割当処理
	LPD3DXBUFFER GetBuffMat(MODEL_TYPE type);	// マテリアル割当処理
	DWORD GetNumMat(MODEL_TYPE type);			// マテリアル数の割当処理

private:
	LPD3DXMESH m_apMesh[MODEL_TYPE_MAX];		// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_apBuffMat[MODEL_TYPE_MAX];	// マテリアル(材質情報)へのポインタ
	DWORD m_aNumMat[MODEL_TYPE_MAX];			// マテリアル数
};

#endif // _MODEL_LOAD_H_