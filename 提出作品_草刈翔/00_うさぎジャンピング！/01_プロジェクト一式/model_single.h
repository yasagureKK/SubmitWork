//=============================================================================
// 単体のモデル処理 [model_single.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _MODEL_SINGLE_H_
#define _MODEL_SINGLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "model_load.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CScoreBoard;
class CShadow;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CModelSingle : public CScene
{
public:
	// モデルの動きの種類
	typedef enum
	{
		MODEL_SINGLE_TYPE_NONE = 0,
		MODEL_SINGLE_TYPE_SIGNBOARD,	// 看板
		MODEL_SINGLE_TYPE_MAX
	} MODEL_SINGLE_TYPE;

	CModelSingle(int nPriority = PRIORITY_MODEL);		// コンストラクタ
	~CModelSingle();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CModelSingle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type,
		MODEL_SINGLE_TYPE modelType);					// 生成処理
	static bool Collision(CScene *pScene);				// 当たり判定
	void SetShadow(bool bOnShadow, float fHeight);		// 影の設定処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_posOld;								// 1フレーム前の位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3 m_rot;									// 向き
	D3DXVECTOR3 m_move;									// 移動量
	CModel *m_pModel;									// モデルのポインタ
	D3DXMATRIX m_mtxWorld;								// ワールドマトリックス
	CModelLoad::MODEL_TYPE m_type;						// モデルの種類
	MODEL_SINGLE_TYPE m_modelType;						// モデルの動きの種類
	CScoreBoard *m_pScoreBoard;							// スコアボードのポインタ
	CShadow *m_pShadow;									// シャドウのポインタ
};

#endif // _MODEL_SINGLE_H_