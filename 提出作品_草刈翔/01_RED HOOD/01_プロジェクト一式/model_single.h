//=============================================================================
// 単体のモデル処理 [model_single.h]
// Author  : 草刈 翔
//=============================================================================
#ifndef _MODEL_SINGLE_H_
#define _MODEL_SINGLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
//class CShadow;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CModelSingle : public CScene3D
{
public:
	CModelSingle(int nPriority = PRIORITY_MODEL);					// コンストラクタ
	~CModelSingle();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CModelSingle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, int nModelType);							// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_MODEL; }		// オブジェクトの種類の取得処理
	static bool Collision(CScene *pScene);							// 当たり判定
	void SetColor(D3DXCOLOR col);									// カラー設定処理

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 1フレーム前の位置
	D3DXVECTOR3	m_size;			// サイズ
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;			// 移動量
	CModel *m_pModel;			// モデルのポインタ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
};
#endif // _MODEL_SINGLE_H_