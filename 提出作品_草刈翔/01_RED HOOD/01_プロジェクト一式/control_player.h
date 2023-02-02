//=============================================================================
// プレイヤー操作処理 [control_player.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CONTROL_PLAYER_H_
#define _CONTROL_PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;
class CPlayer;
class CCollisionSphere;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CControlPlayer : public CControl
{
public:
	CControlPlayer();						// コンストラクタ
	~CControlPlayer();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pScene);			// 更新処理
	static CControlPlayer *Create(void);	// 生成処理

private:
	void Move(CPlayer *pPlayer);			// 移動処理
	void Dash(CPlayer *pPlayer);			// ダッシュ処理
	void Attack(CPlayer *pPlayer);			// 攻撃処理
	void DoingAttack(CPlayer *pPlayer);		// 攻撃中の処理
	void WeaponChange(CPlayer *pPlayer);	// 武器切り替え処理
	void TakeDamage(CPlayer *pPlayer);		// 被ダメージ処理
	void Clear(CPlayer *pPlayer);			// クリア時の処理
	void MoveInteria(CPlayer *pPlayer);		// 移動の慣性についての処理
	void Rotate(CPlayer *pPlayer);			// 回転処理
#ifdef _DEBUG
	void Jump(CPlayer *pPlayer);			// ジャンプ処理
#endif

	D3DXVECTOR3 m_rot;						// 向き
	float m_fObjectiveRot;					// 目的の向き
	float m_fNumRot;						// 向きを変える量
	float m_fSpeed;							// 移動量に加える速さ
	float m_fBasicSpeed;					// 基本の速さ
	float m_fJump;							// ジャンプ力
	float m_fAttackValue;					// 攻撃力
	float m_fAtkBeginEffect;				// 攻撃時、エフェクトを出すカウント
	float m_fAtkEffectCount;				// 攻撃中、エフェクトの持続時間
	bool m_bJump;							// ジャンプしているかどうか
	bool m_bDoubleJump;						// 2段ジャンプしているかどうか
	bool m_bDash;							// ダッシュしているかどうか
	bool m_bRotate;							// 回転しているかどうか
	bool m_bAttack;							// 攻撃しているかどうか
	bool m_bInputAttack;					// 攻撃の入力をしているかどうか
	bool m_bDamage;							// ダメージを受けているかどうか
	int m_nMoveCount;						// 移動中のカウント
	int m_nDashCount;						// ダッシュ中のカウント
	int m_nDashCoolTime;					// ダッシュ後のクールタイム
	int m_nAtkCount;						// 攻撃中のカウント
	CCollisionSphere *m_pCollisionS;		// 球体コリジョンのポインタ
};
#endif	//_CONTROL_PLAYER_H_