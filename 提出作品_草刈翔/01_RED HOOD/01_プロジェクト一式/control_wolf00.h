//=============================================================================
// 狼00操作処理 [control_wolf00.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CONTROL_WOLF00_H_
#define _CONTROL_WOLF00_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;
class CEnemy;
class CCollisionSphere;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CControlWolf00 : public CControl
{
public:
	CControlWolf00();						// コンストラクタ
	~CControlWolf00();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pScene);			// 更新処理
	static CControlWolf00 *Create(void);	// 生成処理

private:
	void Move(CEnemy *pEnemy);				// 基本の移動処理
	void Attack(CEnemy *pEnemy);			// 攻撃処理
	void DoingAttack(CEnemy *pEnemy);		// 攻撃中の処理
	void MoveInteria(CEnemy *pEnemy);		// 移動の慣性についての処理
	void Rotate(CEnemy *pEnemy);			// 回転処理

	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_moveOld;					// 1フレーム前の移動量
	float m_fObjectiveRot;					// 目的の向き
	float m_fNumRot;						// 向きを変える量
	float m_fSpeed;							// 移動量に加える速さ
	bool m_bMove;							// 移動しているかどうか
	bool m_bRotate;							// 回転しているかどうか
	bool m_bAttack;							// 攻撃しているかどうか
	int m_nMoveCount;						// 移動中のカウント
	int m_nAtkInterval;						// 攻撃の間隔
	int m_nAtkCount;						// 攻撃中のカウント
	int m_nAttackWaitTime;					// 攻撃までの待機時間
	float m_fAtkBeginEffect;				// 攻撃時、エフェクトを出すカウント
	float m_fAtkEffectCount;				// 攻撃中、エフェクトの持続時間
	CCollisionSphere *m_pCollisionS;		// 球体コリジョンのポインタ
};
#endif	//_CONTROL_WOLF00_H_