//=============================================================================
// エネミー処理 [enemy.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CControl;
class CShadow;
class CCollisionSphere;
class CMotion;
class CLifeBillboard;
class CLife;

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_ENEMY_MODEL_WOLF00		(12)	// 狼00のモデルの数
#define MAX_ENEMY_MODEL_WOLF01		(16)	// 狼01のモデルの数
#define MAX_ENEMY_MODEL_BOSS		(15)	// ボスのモデルの数
#define MAX_ENEMY_MODEL				(16)	// モデルの最大数
#define BEGIN_BOSS_LIFE				(80)	// ボスの初期ライフ
#define MAX_ENEMY_COLLSION			(4)		// コリジョンの数

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CEnemy : public CScene3D
{
public:
	typedef enum
	{
		ENEMY_TYPE_WOLF_0 = 0,
		ENEMY_TYPE_WOLF_1,
		ENEMY_TYPE_BOSS,
		ENEMY_TYPE_MAX
	} ENEMY_TYPE;

	CEnemy(int nPriority = PRIORITY_MODEL);							// コンストラクタ
	~CEnemy();														// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fLife);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot,
		ENEMY_TYPE type, float fLife);								// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_ENEMY; }		// オブジェクトの種類の取得処理

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				// 1フレーム前の位置取得処理
	void SetSize(D3DXVECTOR3 size) { m_size = size; }				// サイズ設定処理
	D3DXVECTOR3 GetSize(void) { return m_size; }					// サイズ取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }					// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }						// 向き取得処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// 移動量設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }					// 移動量取得処理
	void SetLand(bool bLand) { m_bLand = bLand; }					// 着地設定処理
	bool GetLand(void) { return m_bLand; }							// 着地取得処理
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);				// モデル毎の位置設定処理
	D3DXVECTOR3 GetModelPos(int nCntModel);							// モデル毎の位置取得処理
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);				// モデル毎の向き設定処理
	D3DXVECTOR3 GetModelRot(int nCntModel);							// モデル毎の向き取得処理
	CMotion *GetMotion(void){ return m_pMotion; }					// モーション取得処理
	float GetLife(void) { return m_fLife; }							// ライフ取得処理
	void SetEnemyType(ENEMY_TYPE type) { m_type = type; }			// 敵の設定取得処理
	ENEMY_TYPE GetEnemyType(void) { return m_type; }				// 敵の種類取得処理
	CModel *GetModel(int nCntModel);								// エネミーのモデル取得処理
	void SetNewPos(D3DXVECTOR3 pos) { m_pos = pos; }				// 新しい位置設定処理

private:
	void Move(void);						// 移動処理
	void TrackingShadow(void);				// 影の追従処理
	void TouchCollision(void);				// それぞれのコリジョンの接触処理
	void CollisionEnemy(CEnemy *pEnemy);	// 敵同士の(押し出しのための)衝突処理
	void CreateModel(ENEMY_TYPE type);		// モデル生成時の処理
	void SendExp(void);						// 経験値受け渡し処理
	void KnockDown(void);					// 倒された時の処理
	void CanControl(CEnemy *pEnemy);		// 動作させるどうかの設定処理

	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_posOld;									// 1フレーム前の位置
	D3DXVECTOR3 m_move;										// ポリゴンの移動量
	D3DXVECTOR3	m_size;										// サイズ
	D3DXVECTOR3 m_rot;										// 向き
	D3DXMATRIX m_mtxWorld;									// ワールドマトリックス
	CModel *m_apModel[MAX_ENEMY_MODEL];						// モデルのポインタ
	CModel *m_pParent;										// 親モデルへのポインタ
	CMotion *m_pMotion;										// モーションのポインタ
	CControl *m_pControl;									// コントロールのポインタ
	CShadow *m_pShadow;										// シャドウのポインタ
	CLifeBillboard *m_pLifeBill;							// ライフ(ビルボード)のポインタ
	CLife *m_pLife;											// ライフのポインタ
	CCollisionSphere *m_pCollisionS[MAX_ENEMY_COLLSION];	// 球体コリジョンのポインタ
	int m_nModelNum;										// モデル(パーツ)の数
	int m_nDeathCount;										// 消滅までのカウント
	int m_nScore;											// 倒した時のスコア
	float m_fLife;											// ライフ
	bool m_bLand;											// 着地しているかどうか
	bool m_bKnockDown;										// 倒れたかどうか
	bool m_bTouchWall;										// 壁に触れているかどうか
	bool m_bCanControl;										// 動作させるかどうか
	ENEMY_TYPE m_type;										// 敵の種類
};
#endif // _PLAYER_H_