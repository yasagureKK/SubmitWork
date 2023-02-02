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
class CParticle;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_JUMP							(8.0f)	// ジャンプ力
#define PLAYER_DOUBLE_JUMP					(10.0f)	// 2段ジャンプでのジャンプ力
#define PLAYER_JUMP_MAGNIFICATION			(14.0f)	// ジャンプ量の倍率の大きさ
#define PLAYER_GRAVITY						(0.4f)	// 重力の大きさ
#define PLAYER_MAX_GRAVITY					(10.0f)	// 重力の最大値
#define PLAYER_MAX_DOUBLE_JUMP_GRAVITY		(7.0f)	// 2段ジャンプ中の重力の最大値
#define PLAYER_MOVE_SPEED					(0.5f)	// 移動量の基準値
#define PLAYER_ADD_SPEED					(0.05f)	// 移動量に加える値
#define PLAYER_LAND_INTERIA_SUBTRACTION		(0.86f)	// 地上での慣性の減算値
#define PLAYER_AIR_INTERIA_SUBTRACTION		(0.92f)	// 空中での慣性の減算値
#define PLAYER_MOVE_STOP_COUNT				(0.02f)	// プレイヤーの移動量を0にする時の移動量の値

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CControlPlayer : public CControl
{
	// 次に入力する操作の種類
	typedef enum
	{
		PLAYER_NEXT_INPUT_NONE = 0,	// なし
		PLAYER_NEXT_INPUT_LEFT,		// 左
		PLAYER_NEXT_INPUT_RIGHT,	// 右
		PLAYER_NEXT_MAX
	} PLAYER_NEXT_INPUT;

public:
	CControlPlayer();						// コンストラクタ
	~CControlPlayer();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pScene);			// 更新処理
	static CControlPlayer *Create(void);	// 生成処理

private:
	void Move(CPlayer *pPlayer);			// 移動処理
	void Jump(CPlayer *pPlayer);			// ジャンプ処理
	void MoveInteria(CPlayer *pPlayer);		// 移動の慣性についての処理
	void Rotate(CPlayer *pPlayer);			// 回転処理
	void CommandMove(CPlayer *pPlayer);		// 行動を指定された時の移動処理

	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_moveOld;					// 1フレーム前の移動量
	float m_fObjectiveRot;					// 目的の向き
	float m_fNumRot;						// 向きを変える量
	float m_fSpeed;							// 移動量に加える速さ
	float m_fFloat;							// 浮力
	bool m_bJump;							// ジャンプしてるかどうか
	bool m_bDoubleJump;						// 2段ジャンプしてるかどうか
	bool m_bRotate;							// 回転しているかどうか
	int m_nSpinCount;						// SE再生のカウント
	int m_nMoveCount;						// 移動時のカウント
	PLAYER_NEXT_INPUT m_nextInput;			// 次に入力する操作
	CParticle *m_pParticle;					// パーティクルのポインタ

#ifdef _DEBUG
	void DebugMove(CPlayer *pPlayer);		// デバッグ用の移動処理
#endif
};

#endif	//_CONTROL_PLAYER_H_