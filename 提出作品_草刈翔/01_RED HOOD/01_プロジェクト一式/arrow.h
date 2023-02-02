//=============================================================================
// アロー処理 [arrow.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModelSingle;
class CModel;
class CCollisionSphere;

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define SUSTAIN_TIME		(120)	// 持続時間

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CArrow : public CScene3D
{
public:
	CArrow(int nPriority = PRIORITY_MODEL);						// コンストラクタ
	~CArrow();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CArrow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		float fAttackValue);									// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }	// オブジェクトの種類の取得処理
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			// 1フレーム前の位置取得処理

	void SetModelRot(D3DXVECTOR3 rot) { m_rot = rot; }			// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }					// 向き取得処理

private:
	void TouchCollision(void);

	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR3 m_posOld;										// 1フレーム前の位置
	D3DXVECTOR3	m_size;											// サイズ
	D3DXVECTOR3 m_rot;											// 向き
	D3DXMATRIX m_mtxWorld;										// ワールドマトリックス
	CModel *m_pModel;											// モデルへのポインタ
	CCollisionSphere *m_pCollisionS;							// 球体コリジョンのポインタ
	int m_nLife;												// 寿命
	float m_fAttackValue;										// 攻撃力
};
#endif // _ARROW_H_