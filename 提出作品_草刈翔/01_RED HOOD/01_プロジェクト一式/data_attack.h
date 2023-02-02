//=============================================================================
// 攻撃情報管理 [data_attack.h]
// Aouthor : 草刈 翔
//=============================================================================
#ifndef _DATA_ATTACK_H_
#define _DATA_ATTACK_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_ATTACK	(8)		// 攻撃の種類の最大数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CDataAttack
{
public:
	// 攻撃の種類
	typedef enum
	{
		ATTACK_TYPE_AXE = 0,				// 斧：攻撃1段目
		ATTACK_TYPE_AXE_2,					// 斧：攻撃2段目
		ATTACK_TYPE_AXE_3,					// 斧：攻撃3段目
		ATTACK_TYPE_BOWGUN,					// ボウガン：攻撃
		ATTACK_TYPE_WOLF00,					// 狼00：攻撃
		ATTACK_TYPE_WOLF01,					// 狼01：攻撃
		ATTACK_TYPE_BOSS_CLAW,				// ボス：攻撃(爪)
		ATTACK_TYPE_BOSS_DASH,				// ボス：攻撃(ダッシュ)
		ATTACK_TYPE_MAX
	} ATTACK_TYPE;

	// 攻撃情報の種類
	typedef enum
	{
		ATTACK_DATA_TYPE_WAIT = 0,			// 攻撃可能までのフレーム
		ATTACK_DATA_TYPE_BEGIN_COLLISION,	// コリジョンをつけるフレーム
		ATTACK_DATA_TYPE_BEGIN_EFFECT,		// エフェクトをつけるフレーム
		ATTACK_DATA_TYPE_TIME_COLLISION,	// コリジョンの持続時間
		ATTACK_DATA_TYPE_TIME_EFFECT,		// エフェクトの持続時間
		ATTACK_DATA_TYPE_DAMAGE,			// ダメージ倍率
		ATTACK_DATA_TYPE_MAX
	} ATTACK_DATA_TYPE;

	// 攻撃情報の構造体
	typedef struct
	{
		int nWait;							// 攻撃可能までのフレーム
		int nBeginCollision;				// コリジョンをつけるフレーム
		int nBeginEffect;					// エフェクトをつけるフレーム
		float fTimeCollision;				// コリジョンの持続時間
		float fTimeEffect;					// エフェクトの持続時間
		float fDamage;						// ダメージ倍率
	} ATTACK_DATA;

	CDataAttack();							// コンストラクタ
	~CDataAttack();							// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	static CDataAttack *Create();			// 生成処理
	float GetAttackData(ATTACK_TYPE atkType,
		ATTACK_DATA_TYPE dataType);			// 情報取得処理

private:
	ATTACK_DATA m_Data[MAX_ATTACK];			// 情報
};
#endif // _DATA_ATTACK_H_