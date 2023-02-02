//=============================================================================
// プレイヤー処理 [player.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_PLAYER_MODEL	(16)		// モデルの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CControl;
class CShadow;
class CParticle;
class CCollisionSphere;
class CLife;
class CLevel;
class CWeapon;

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CPlayer : public CScene3D
{
public:
	// プレイヤーの状態
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,	// 通常
		PLAYER_STATE_BLOWAWAY,		// 吹っ飛び
		PLAYER_STATE_DEBUG,			// 無敵(デバッグ用)
		PLAYER_STATE_MAX
	} PLAYER_STATE;

	// 武器の種類
	typedef enum
	{
		PLAYER_WEAPON_AXE = 0,	// 斧
		PLAYER_WEAPON_BOWGUN,	// ボウガン
		PLAYER_WEAPON_MAX
	} PLAYER_WEAPON;

	CPlayer(int nPriority = PRIORITY_MODEL);							// コンストラクタ
	~CPlayer();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot);			// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_PLAYER; }			// オブジェクトの種類の取得処理

	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }					// 1フレーム前の位置設定処理
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }					// 1フレーム前の位置取得処理
	
	void SetSize(D3DXVECTOR3 size) { m_size = size; }					// サイズ設定処理
	D3DXVECTOR3 GetSize(void) { return m_size; }						// サイズ取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }						// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }							// 向き取得処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }					// 移動量設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }						// 移動量取得処理
	void SetJump(bool bJump) { m_bJump = bJump; }						// ジャンプ設定処理
	bool GetJump(void) { return m_bJump; }								// ジャンプ取得処理
	void SetLand(bool bLand) { m_bLand = bLand; }						// 着地設定処理
	bool GetLand(void) { return m_bLand; }								// 着地取得処理
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);					// モデル毎の位置設定処理
	D3DXVECTOR3 GetModelPos(int nCntModel);								// モデル毎の位置取得処理
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);					// モデル毎の向き設定処理
	D3DXVECTOR3 GetModelRot(int nCntModel);								// モデル毎の向き取得処理
	D3DXVECTOR3 GetModelMtxPos(int nCntModel);							// モデル毎のワールドマトリックス座標取得処理
	CMotion *GetMotion(void) { return m_pMotion; }						// モーション取得処理
	CModel *GetModel(int nCntModel) { return m_apModel[nCntModel]; }	// モデル取得処理
	CCollisionSphere *GetCollision(void) { return m_pCollisionS; }		// コリジョン取得処理
	void SetState(PLAYER_STATE state) { m_state = state; }				// 状態設定処理
	PLAYER_STATE GetState(void) { return m_state; }						// 状態取得処理
	void SetWeapon(PLAYER_WEAPON weapon) { m_weapon = weapon; }			// 武器設定処理
	PLAYER_WEAPON GetWeapon(void) { return m_weapon; }					// 武器設定処理
	void AddExp(int nValue) { m_nExp += nValue; }						// 経験値増加処理
	int GetLevel(void);													// レベル取得処理
	float GetLife(void) { return m_fLife; }								// ライフ取得処理
	void SetWeaponChange(bool bChange) { m_bWeaponChange = bChange; }	// 武器変更取得処理
	void SetCanControl(bool bControl) { m_bCanControl = bControl; }		// 操作できるか取得処理
	bool GetCanControl(void) { return m_bCanControl; }					// 操作できるか取得処理
	bool GetDeath(void) { return m_bDeath; }							// 死亡判定取得処理

private:
	void ModelCreate(void);						// モデル生成処理
	void Move(void);							// 移動処理
	void TrackingShadow(void);					// 影の追従処理
	void TouchCollision(void);					// それぞれのコリジョンの接触処理
	void State(void);							// 状態処理
	void CollisionEnemy(void);					// 敵との(押し出しのための)衝突処理
	void ChangeWeapon(void);					// 武器変更時の処理

	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 1フレーム前の位置
	D3DXVECTOR3 m_move;							// ポリゴンの移動量
	D3DXVECTOR3	m_size;							// サイズ
	D3DXVECTOR3 m_rot;							// 向き
	CModel *m_apModel[MAX_PLAYER_MODEL];		// モデルのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	CModel *m_pParent;							// 親モデルへのポインタ
	CMotion *m_pMotion;							// モーションのポインタ
	CControl *m_pControl;						// コントロールのポインタ
	CShadow *m_pShadow;							// シャドウのポインタ
	CCollisionSphere *m_pCollisionS;			// 球体コリジョンのポインタ
	CLife *m_pLife;								// ライフのポインタ
	CLevel *m_pLevel;							// レベルのポインタ
	CWeapon *m_pWepaon;							// 武器UIのポインタ
	float m_fLife;								// ライフ
	float m_fMaxLife;							// ライフの最大値
	int m_nExp;									// 経験値
	int m_nCntInvincible;						// 無敵時間のカウント
	bool m_bLand;								// 着地しているかどうか
	bool m_bLandOld;							// 前フレームに着地しているかどうか
	bool m_bJump;								// ジャンプしているかどうか
	bool m_bWeaponChange;						// 武器を変更してるかどうか
	bool m_bInvincible;							// 無敵したかどうか
	bool m_bCanControl;							// 操作できるかどうか
	bool m_bDeath;								// 死亡したかどうか
	PLAYER_STATE m_state;						// プレイヤーの状態
	PLAYER_WEAPON m_weapon;						// 武器
};
#endif // _PLAYER_H_