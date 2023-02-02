//=============================================================================
// 球体コリジョン処理 [collision_sphere.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SPHERE_COLLISION_H_
#define _SPHERE_COLLISION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CCollisionSphere : public CScene3D
{
public:
	// どの当たり判定か
	typedef enum
	{
		COLLISION_S_TYPE_NONE = 0,		// なし
		COLLISION_S_TYPE_PLAYER,		// プレイヤー
		COLLISION_S_TYPE_WEAPON,		// 武器
		COLLISION_S_TYPE_WEAPON_P,		// 武器[強]
		COLLISION_S_TYPE_ENEMY,			// 敵
		COLLISION_S_TYPE_ENEMY_ATTACK,	// 敵の攻撃
		COLLISION_S_TYPE_MAX
	} COLLISION_S_TYPE;

	CCollisionSphere(int nPriority = PRIORITY_EFFECT);						// コンストラクタ
	~CCollisionSphere();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);						// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CCollisionSphere *Create(D3DXVECTOR3 pos,
		float fSize, COLLISION_S_TYPE type, float fTime);					// 生成処理

	D3DXVECTOR3 GetSize(void) { return m_size; }							// サイズ取得処理
	void Collision(CScene *pScene);											// 衝突処理
	void SetPosCollision(D3DXVECTOR3 pos) { m_pos = pos; }					// 位置設定処理
	void SetCollisionType(COLLISION_S_TYPE type);							// コリジョンの種類設定処理
	COLLISION_S_TYPE GetCollisionType(void) { return m_collisionType; }		// コリジョンの種類の取得処理
	void SetParent(CModel *pModel) { m_pParent = pModel; }					// 親モデル設定処理
	D3DXVECTOR3 GetMtxPos(void);											// ワールドマトリックス座標取得処理
	bool OnCollisionEnter(COLLISION_S_TYPE type);							// 衝突した瞬間の処理
	void SetTime(float fTime) { m_fTime = fTime; }							// 持続時間設定処理
	void SetDamageValue(float fValue) { m_fDamage = fValue; }				// ダメージ値設定処理
	float GetDamageValue(void) { return m_fDamage; }						// ダメージ値取得処理
	float GetObjectiveRot(void) { return m_fObjectiveRot; }					// 目的の向き取得処理
	void SetPosSource(D3DXVECTOR3 pos) { m_posSource = pos; }				// コリジョン発生源の位置設定処理
	D3DXVECTOR3 GetPosSource(void) { return m_posSource; }					// コリジョン発生源の位置取得処理
#ifdef _DEBUG
	static void SetVisual(void);											// 判定の可視化設定処理
#endif

private:
#ifdef _DEBUG
	static void VisualChange(void);											// 判定の可視化処理
#endif

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;										// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;										// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;														// 位置
	D3DXVECTOR3	m_size;														// サイズ
	D3DXVECTOR3 m_rot;														// 向き
	D3DXVECTOR3 m_posSource;												// コリジョン発生源の位置
	D3DXMATRIX m_mtxWorld;													// ワールドマトリックス
	CModel *m_pParent;														// 親のモデル
	D3DXCOLOR m_col;														// カラー
	COLLISION_S_TYPE m_collisionType;										// 当たり判定の種類
	COLLISION_S_TYPE m_saveCollisionType;									// 保存するための当たり判定の種類
	int m_nVertical;														// 縦の分割数
	int m_nSide;															// 横の分割数
	float m_fTime;															// 持続時間
	float m_fDamage;														// ダメージの値
	float m_fObjectiveRot;													// 目的の向き
	static bool m_bMngVisual;												// 判定を可視化するかどうか
	bool m_bContact;														// 一度でも当たったかどうか
	bool m_bContactOld;														// 1フレーム前に当たっていたかどうか
};
#endif // _SPHERE_COLLISION_H_