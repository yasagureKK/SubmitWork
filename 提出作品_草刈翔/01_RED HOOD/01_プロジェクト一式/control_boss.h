//=============================================================================
// ボス操作処理 [control_boss.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CONTROL_BOSS_H_
#define _CONTROL_BOSS_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_BOSS_COLLSION	(4)		// コリジョンの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;
class CEnemy;
class CCollisionSphere;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CControlBoss : public CControl
{
public:
	typedef enum
	{
		BOSS_THINK_STOP = 0,
		BOSS_THINK_MOVE,
		BOSS_THINK_CLAW,
		BOSS_THINK_JUMP_NORMAL,
		BOSS_THINK_JUMP_BIG,
		BOSS_THINK_CHARGE,
		BOSS_THINK_CHARGE_BEGIN,
		BOSS_THINK_DASH,
		BOSS_THINK_DASH_TURN,
		BOSS_THINK_DASH_END,
	} BOSS_THINK;

	CControlBoss();							// コンストラクタ
	~CControlBoss();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pScene);			// 更新処理
	static CControlBoss *Create(void);		// 生成処理

private:
	void Move(CEnemy *pEnemy);							// 基本の移動処理
	void ClawAttack(CEnemy *pEnemy);					// 攻撃(爪)処理
	void DashAttack(CEnemy *pEnemy);					// 攻撃(ダッシュ)処理
	void DoingAttack(CEnemy *pEnemy);					// 攻撃中の処理
	void Jump(CEnemy *pEnemy);							// 基本の移動処理
	void MoveInteria(CEnemy *pEnemy);					// 移動の慣性についての処理
	void Rotate(CEnemy *pEnemy);						// 回転処理

	D3DXVECTOR3 m_rot;									// 向き
	D3DXVECTOR3 m_moveOld;								// 1フレーム前の移動量
	D3DXVECTOR3 m_posDash;								// ダッシュ前の着地位置
	D3DXVECTOR3 m_beginPos;								// 初期位置
	float m_fObjectiveRot;								// 目的の向き
	float m_fNumRot;									// 向きを変える量
	float m_fSpeed;										// 移動量に加える速さ
	bool m_bMove;										// 移動しているかどうか
	bool m_bRotate;										// 回転しているかどうか
	bool m_bClaw;										// 爪の攻撃をしているかどうか
	int m_nAtkInterval;									// 攻撃の間隔
	int m_nAtkCount;									// 攻撃中のカウント
	int m_nAttackWaitTime;								// 攻撃までの待機時間
	int m_nClawNum;										// 爪攻撃の数
	int m_nJumpInterval;								// ジャンプの間隔
	int m_nJump;										// ジャンプするまでのカウント
	int m_nJumpNum;										// ジャンプの回数
	int m_nMoveCount;									// 移動中のカウント
	int m_nMoveJump;									// 移動中、ジャンプするまでのカウント
	int m_nMoveStop;									// 動きを止めるカウント
	int m_nDashCount;									// ダッシュ中のカウント
	int m_nDashNum;										// ダッシュ攻撃の数
	float m_fAtkBeginEffect;							// 攻撃時、エフェクトを出すカウント
	float m_fAtkEffectCount;							// 攻撃中、エフェクトの持続時間
	CCollisionSphere *m_pCollisionS[MAX_BOSS_COLLSION];	// 球体コリジョンのポインタ
	BOSS_THINK m_think;									// 思考
};
#endif	// _CONTROL_BOSS_H_