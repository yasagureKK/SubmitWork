//=============================================================================
// エネミー(ボス)操作処理 [control_enemy_boss.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_boss.h"
#include "manager.h"
#include "enemy.h"
#include "motion.h"
#include "sound.h"
#include "player.h"
#include "collision_sphere.h"
#include "data_attack.h"
#include "effect_billboard.h"
#include "particle_dash.h"
#include "particle_circle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_GRAVITY				(0.6f)	// 重力の大きさ
#define BOSS_MAX_GRAVITY			(15.0f)	// 重力の最大値
#define BOSS_MOVE_SPEED				(0.5f)	// 移動量の基準値
#define BOSS_INTERIA_SUBTRACTION	(0.86f)	// 地上での慣性の減算値
#define BOSS_MOVE_STOP_COUNT		(0.02f)	// プレイヤーの移動量を0にする時の移動量の値
#define MAX_BOSS_JUMP_INTERVAL		(100)	// ジャンプの間隔
#define BOSS_JUMP_NORMAL			(23.0f)	// 通常のジャンプ力
#define BOSS_JUMP_BIG				(47.0f)	// 大ジャンプでのジャンプ力
#define BOSS_JUMP_BIG_GRAVITY		(45.0f)	// 大ジャンプ時の重力の最大値
#define MAX_BOSS_ATTACK_INTERVAL	(80)	// 基本の攻撃間隔
#define BOSS_DASH_TIME				(70)	// ダッシュ時間
#define BOSS_CLAW_NUM				(3)		// 爪攻撃の回数
#define BOSS_JUMP_NUM				(3)		// ジャンプ回数
#define BOSS_AREA	(D3DXVECTOR3(0.0f, 0.0f, 10000.0f))	// ボス戦エリアの中心位置

//=============================================================================
// コンストラクタ
//=============================================================================
CControlBoss::CControlBoss()
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDash = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_bRotate = false;
	m_bClaw = false;
	m_nAtkInterval = 0;
	m_nAtkCount = 0;
	m_nClawNum = 0;
	m_nJumpInterval = 0;
	m_nJump = 0;
	m_nJumpNum = 0;
	m_nMoveCount = 0;
	m_nMoveJump = 0;
	m_nMoveStop = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_nAttackWaitTime = 0;
	m_nDashCount = 0;
	m_nDashNum = 0;
	memset(&m_pCollisionS, NULL, sizeof(m_pCollisionS));
	m_think = BOSS_THINK_STOP;
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlBoss::~CControlBoss()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlBoss::Init(void)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDash = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = BOSS_MOVE_SPEED;
	m_bRotate = false;
	m_bClaw = false;
	m_nAtkInterval = MAX_BOSS_ATTACK_INTERVAL;
	m_nAtkCount = 0;
	m_nClawNum = 0;
	m_nJumpInterval = MAX_BOSS_JUMP_INTERVAL;
	m_nJump = rand() % 3 + 1;
	m_nJumpNum = 0;
	m_nMoveCount = 0;
	m_nMoveJump = 0;
	m_nMoveStop = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_nAttackWaitTime = 0;
	m_nDashCount = 0;
	m_nDashNum = 0;
	for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
	{
		m_pCollisionS[nCntColl] = NULL;
	}
	m_think = BOSS_THINK_MOVE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlBoss::Uninit(void)
{
	for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
	{
		if (m_pCollisionS[nCntColl]  != NULL)
		{
			m_pCollisionS[nCntColl] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CControlBoss::Update(CScene *pScene)
{
	// 引数のポインタをプレイヤークラスのポインタにキャスト
	CEnemy *pEnemy = NULL;
	pEnemy = (CEnemy*)pScene;

	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// 移動量を設定
	m_move = pEnemy->GetMove();
	m_moveOld = m_move;

	// 重力
	m_move.y -= BOSS_GRAVITY;


	if (m_think == BOSS_THINK_JUMP_BIG)
	{
		// 重力が強くなりすぎたら重力の最大値で固定しておく
		if (m_move.y <= -BOSS_JUMP_BIG_GRAVITY)
		{
			m_move.y = -BOSS_JUMP_BIG_GRAVITY;
		}
	}
	else
	{
		// 重力が強くなりすぎたら重力の最大値で固定しておく
		if (m_move.y <= -BOSS_MAX_GRAVITY)
		{
			m_move.y = -BOSS_MAX_GRAVITY;
		}
	}

	if (m_think == BOSS_THINK_STOP)
	{
		m_nMoveStop--;

		if (m_nMoveStop <= 0)
		{
			pMotion->SetMotion(1);
			m_think = BOSS_THINK_MOVE;
		}
	}

	// 移動処理
	Move(pEnemy);

	if (m_bClaw == false)
	{
		// ジャンプ処理
		Jump(pEnemy);
	}

	// 攻撃(爪)処理
	ClawAttack(pEnemy);

	// 攻撃(ダッシュ)処理
	DashAttack(pEnemy);

	// 移動の慣性
	MoveInteria(pEnemy);

	// 攻撃モーションじゃないかつ、モーションをつなげていないなら
	if (pMotion->GetMotion() != 2 && pMotion->GetConnect() == false)
	{
		m_bClaw = false;
	}

	//---------------------------------------------------
	// モーション遷移
	//---------------------------------------------------
	// 現在止まっているかつ、今が移動モーションかつ、モーションをつなげていなかったら
	if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetMotion() == 1 && pMotion->GetConnect() == false)
	{
		// ニュートラルモーションにする
		pMotion->SetMotion(0);
	}

	// 現在動いていてモーションをつなげていないかつ、今がニュートラルモーションで移動モーションじゃないなら
	if ((m_move.x != 0.0f || m_move.z != 0.0f) && pMotion->GetConnect() == false &&
		pMotion->GetMotion() == 0 && pMotion->GetMotion() != 1)
	{
		// 移動モーションにする
		pMotion->SetMotion(1);
	}

	// 攻撃時のコリジョン
	DoingAttack(pEnemy);

	// 回転の慣性(詳しい処理は関数の中)
	Rotate(pEnemy);

	//---------------------------------------------------
	// 足音
	//---------------------------------------------------
	// 現在動いているなら
	if (m_bMove == true)
	{
		// サウンド取得処理
		CSound *pSound = CManager::GetSound();

		if (m_nMoveCount % 50 == 0)
		{
			// 敵の着地SEの再生
			pSound->Play(20);
			pSound->ControllVoice(20, 0.15f);
		}

		m_nMoveCount++;
	}
	else
	{
		m_nMoveCount = 0;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CControlBoss *CControlBoss::Create(void)
{
	// クラスの生成
	CControlBoss *pControlBoss = NULL;
	pControlBoss = new CControlBoss;

	if (pControlBoss != NULL)
	{
		// 初期化処理
		pControlBoss->Init();
	}
	return pControlBoss;
}

//=============================================================================
// 移動処理
//=============================================================================
void CControlBoss::Move(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// プレイヤーが死亡していなければ
			if (pPlayer->GetDeath() == false)
			{
				D3DXVECTOR3 plyPos = pPlayer->GetPos();			// プレイヤーの位置
				D3DXVECTOR3 plyPosOld = pPlayer->GetPosOld();	// プレイヤーの1フレーム前の位置
				D3DXVECTOR3 plySize = pPlayer->GetSize();		// プレイヤーのサイズ
				D3DXVECTOR3 enePos = pEnemy->GetPos();			// 敵の位置
				D3DXVECTOR3 enePosOld = pEnemy->GetPosOld();	// 敵の1フレーム前の位置

				float fRadius = sqrtf((plyPos.x - enePos.x) * (plyPos.x - enePos.x) + (plyPos.z - enePos.z) * (plyPos.z - enePos.z));
				float fRot = (float)atan2((plyPos.x - enePos.x), (plyPos.z - enePos.z)) + D3DX_PI / 2.0f;
				float fStopRadius = plySize.x * 6; // プレイヤーの前で停止する距離

				// 思考が<移動>の場合
				if (m_think == BOSS_THINK_MOVE)
				{
					// 停止する距離内にいないなら
					if (fRadius >= fStopRadius)
					{
						// 移動している状態にする
						m_bMove = true;

						// ジャンプ回数が3回未満だったら
						if (m_nJumpNum < BOSS_JUMP_NUM)
						{
							if (m_nMoveJump >= m_nJumpInterval)
							{
								// 思考を<通常ジャンプ>にする
								m_think = BOSS_THINK_JUMP_NORMAL;
								m_nMoveJump = 0;

								// 次のジャンプから、基本の間隔を基にして間隔をランダムにする
								m_nJumpInterval = (rand() % 60) + MAX_BOSS_JUMP_INTERVAL;
							}
						}
						else
						{
							if (m_nMoveJump >= 150)
							{
								// 思考を<大ジャンプ>にする
								m_think = BOSS_THINK_JUMP_BIG;
								m_nMoveJump = 0;
							}
						}

						//移動量加算
						m_move.x += -cosf(fRot) * m_fSpeed;
						m_move.z += +sinf(fRot) * m_fSpeed;

						//目的の向きを設定
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

						//回転の慣性をオンにする
						m_bRotate = true;

						// カウントを増やす
						m_nMoveJump++;
					}

					// プレイヤーの前で停止する距離なら
					if (fRadius <= fStopRadius)
					{
						// 移動していない状態にする
						m_bMove = false;

						// 攻撃中じゃなかったら
						if (m_bClaw == false)
						{
							//目的の向きを設定
							m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

							//回転の慣性をオンにする
							m_bRotate = true;
						}
					}

					// プレイヤーに近い場合かつ、攻撃モーションじゃないなら
					if (fRadius <= fStopRadius && pMotion->GetMotion() != 2)
					{
						m_nAttackWaitTime++;
					}
				}
				// 思考が<通常ジャンプ>の場合
				else if (m_think == BOSS_THINK_JUMP_NORMAL)
				{
					// ジャンプ(溜め)モーションなら
					if (pMotion->GetMotion() == 4)
					{
						//目的の向きを設定
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						//回転の慣性をオンにする
						m_bRotate = true;
					}
					// ジャンプモーションなら
					else if (pMotion->GetMotion() == 5)
					{
						// エネミーの向きを取得し、直進させる
						D3DXVECTOR3 rot = pEnemy->GetRot();
						m_move.x += -cosf(rot.y - D3DX_PI / 2.0f) * m_fSpeed * 3;
						m_move.z += +sinf(rot.y - D3DX_PI / 2.0f) * m_fSpeed * 3;
					}
				}
				// 思考が<大ジャンプ>の場合
				else if (m_think == BOSS_THINK_JUMP_NORMAL)
				{
					// ジャンプモーションなら
					if (pMotion->GetMotion() == 5 && pMotion->GetStop() == true)
					{
						//目的の向きを設定
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						//回転の慣性をオンにする
						m_bRotate = true;
					}
				}
				// 思考が<チャージ>または<ダッシュターン>の場合
				else if (m_think == BOSS_THINK_CHARGE || m_think == BOSS_THINK_DASH_TURN)
				{
					//目的の向きを設定
					m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
					//回転の慣性をオンにする
					m_bRotate = true;
				}

				if (m_think != BOSS_THINK_MOVE)
				{
					m_bMove = false;
				}
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 攻撃(爪)処理
//=============================================================================
void CControlBoss::ClawAttack(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// 爪攻撃を3回行ったら
	if (m_nClawNum >= BOSS_CLAW_NUM)
	{
		m_nClawNum = 0;

		// ジャンプ回数が3回未満だったら
		if (m_nJumpNum < BOSS_JUMP_NUM)
		{
			// 思考を<通常ジャンプ>にする
			m_think = BOSS_THINK_JUMP_NORMAL;
			m_nMoveJump = 0;

			// 次のジャンプから、基本の間隔を基にして間隔をランダムにする
			m_nJumpInterval = (rand() % 60) + MAX_BOSS_JUMP_INTERVAL;
		}
		else
		{
			// 思考を<大ジャンプ>にする
			m_think = BOSS_THINK_JUMP_BIG;
			m_nMoveJump = 0;
		}
	}

	// まだ攻撃していないかつ、モーションをつなげていないかつ
	if (m_bClaw == false && pMotion->GetConnect() == false)
	{
		if (m_nAttackWaitTime >= m_nAtkInterval)
		{
			m_nClawNum++;

			// 攻撃(爪)モーションにする
			pMotion->SetMotion(2);

			m_bClaw = true;

			// カウンターのリセット
			m_nAttackWaitTime = 0;
			m_nAtkCount = 0;

			// 次の攻撃から、基本の攻撃間隔を基にして間隔をランダムにする
			m_nAtkInterval = (rand() % 60 - 30) + MAX_BOSS_ATTACK_INTERVAL;
		}
	}

	// 攻撃モーション中なら
	if (pMotion->GetMotion() == 2)
	{
		//回転の慣性をオフにする
		m_bRotate = false;

		if (m_nAtkCount >= 24 && m_nAtkCount <= 28)
		{
			// エネミーの向きを取得し、直進させる
			D3DXVECTOR3 rot = pEnemy->GetRot();
			//移動量加算
			m_move.x += -sinf(rot.y) * 1.5f;
			m_move.z += -cosf(rot.y) * 1.5f;
		}
	}
}

//=============================================================================
// 攻撃(ダッシュ)処理
//=============================================================================
void CControlBoss::DashAttack(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// 思考が<チャージ開始>の場合
	if (m_think == BOSS_THINK_CHARGE_BEGIN)
	{
		// チャージ(ボス)SEの再生
		pSound->Play(21);
		pSound->ControllVoice(21, 0.3f);

		// ライフの割合から攻撃回数を設定
		float fLifeRatio = pEnemy->GetLife() / BEGIN_BOSS_LIFE;

		// 体力が7割より上なら
		if (fLifeRatio > 0.7f)
		{
			m_nDashNum = 1;
		}
		// 体力が3割より上、7割以下なら
		else if (fLifeRatio >= 0.3f && fLifeRatio <= 0.7f)
		{
			m_nDashNum = 2;
		}
		// 体力が3割以下なら
		else if (fLifeRatio <= 0.3f)
		{
			m_nDashNum = 3;
		}

		m_think = BOSS_THINK_CHARGE;
	}
	// 思考が<チャージ>の場合
	if (m_think == BOSS_THINK_CHARGE)
	{
		// カウントを減らす
		m_nDashCount--;

		// カウントが0になったら
		if (m_nDashCount == 0)
		{
			// ダッシュモーションにする
			pMotion->SetMotion(8);

			// 思考を<ダッシュ>にする
			m_think = BOSS_THINK_DASH;
			m_nDashCount = 0;

			// カウンターのリセット
			m_nAtkCount = 0;
		}
	}
	// 思考が<ダッシュ>の場合
	if (m_think == BOSS_THINK_DASH)
	{
		// チャージ(ボス)SEの停止
		pSound->Stop(21);

		if (m_nDashCount % 18 == 0)
		{
			// ダッシュ(ボス)SEの再生
			pSound->Play(22);
			pSound->ControllVoice(22, 0.3f);
		}

		// カウントを増やす
		m_nDashCount++;

		// エネミーの向きを取得し、直進させる
		D3DXVECTOR3 rot = pEnemy->GetRot();
		m_move.x += -sinf(rot.y) * 5.0f;
		m_move.z += -cosf(rot.y) * 5.0f;

		// ダッシュのパーティクルを発生させる
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 size = pEnemy->GetSize();
		pos.y += size.y / 3.0f;
		CParticleDash::Create(pos, D3DXVECTOR3(120.0f, 120.0f, 0.0f), rot, D3DCOLOR_RGBA(255, 255, 128, 153), 6, 20.0f, 350.0f, 85.0f, 3);

		// ダッシュ時間が終わったら
		if (m_nDashCount >= BOSS_DASH_TIME)
		{
			// 思考を<ダッシュターン>にする
			m_think = BOSS_THINK_DASH_TURN;
			m_nDashCount = 0;
		}
	}
	// 思考が<ダッシュターン>の場合
	if (m_think == BOSS_THINK_DASH_TURN)
	{
		if (pMotion->GetConnect() == false && pMotion->GetMotion() != 7)
		{
			// 攻撃回数を減らす
			m_nDashNum--;

			// チャージモーションにする(滑りの表現)
			pMotion->SetMotion(7);
		}

		// 攻撃回数が残っていたら
		if (m_nDashNum > 0)
		{
			// チャージ(ボス)SEの再生
			pSound->Play(21);
			pSound->ControllVoice(21, 0.3f);

			// 思考を<チャージ>にし、止める時間を設定
			m_think = BOSS_THINK_CHARGE;
			m_nDashCount = 100;
		}
		// 攻撃回数が残っていなかったら
		else if (m_nDashNum <= 0)
		{
			// 思考を<ダッシュ終了>にする
			m_think = BOSS_THINK_DASH_END;
		}
	}
	// 思考が<ダッシュ終了>の場合
	if (m_think == BOSS_THINK_DASH_END)
	{
		m_nDashCount++;

		if (m_nDashCount >= BOSS_DASH_TIME)
		{
			// 思考を<停止>にし、止める時間を設定
			m_think = BOSS_THINK_STOP;
			m_nMoveStop = 50;
			m_nDashCount = 0;
		}
	}
}

//=============================================================================
// 攻撃時などのコリジョン生成処理
//=============================================================================
void CControlBoss::DoingAttack(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// 攻撃情報取得処理
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// 攻撃(爪)モーションの時
	if (pMotion->GetMotion() == 2)
	{
		// エフェクトの開始フレームを設定
		m_fAtkBeginEffect = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// エフェクトの持続時間を設定
		m_fAtkEffectCount = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		if (m_nAtkCount == 0)
		{
			// 切り裂き(2)SEの再生
			CSound *pSound = CManager::GetSound();
			pSound->Play(18);
			pSound->ControllVoice(18, 0.6f);
		}

		// 特定のフレームになったらコリジョンをつける
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// エネミーの右手にコリジョンをつける
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(5);
			CModel *pModel = pEnemy->GetModel(5);
			m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[0]->SetParent(pModel);

			// エネミーの左手にコリジョンをつける
			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);
			m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[1]->SetParent(pModel);

			// エネミーの右腕にコリジョンをつける
			modelPos = pEnemy->GetModelPos(4);
			pModel = pEnemy->GetModel(4);
			m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[2]->SetParent(pModel);

			// エネミーの左腕にコリジョンをつける
			modelPos = pEnemy->GetModelPos(7);
			pModel = pEnemy->GetModel(7);
			m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[3]->SetParent(pModel);

			// コリジョン発生源の位置を、自身の位置として設定
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS[0]->SetPosSource(enemyPos);
			m_pCollisionS[1]->SetPosSource(enemyPos);
			m_pCollisionS[2]->SetPosSource(enemyPos);
			m_pCollisionS[3]->SetPosSource(enemyPos);

			// 倍率をそのまま攻撃力として設定
			m_pCollisionS[0]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[1]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[2]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[3]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// 攻撃中かつ、次のモーションが現在のモーションと同じなら
	if (m_bClaw == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// 特定のフレームになったかつ、持続時間中なら
		if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
		{
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(5);
			CModel *pModel = pEnemy->GetModel(5);

			// 爪の軌跡にエフェクトをつける
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z - 15.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z + 20.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 5.0f, modelPos.y, modelPos.z + 50.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);

			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);

			// 爪の軌跡にエフェクトをつける
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 25.0f, modelPos.y, modelPos.z - 15.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 25.0f, modelPos.y, modelPos.z + 20.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 5.0f, modelPos.y, modelPos.z + 50.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
		}
		// カウントを増やす
		m_nAtkCount++;
	}

	// 攻撃(ダッシュ)モーションの時
	if (pMotion->GetMotion() == 8)
	{
		// 特定のフレームになったらコリジョンをつける
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// エネミーの体にコリジョンをつける
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(1);
			CModel *pModel = pEnemy->GetModel(1);
			m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y + 100.0f, modelPos.z - 50.0f), 300.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[0]->SetParent(pModel);

			// エネミーの頭にコリジョンをつける
			modelPos = pEnemy->GetModelPos(2);
			pModel = pEnemy->GetModel(2);
			m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - 100.0f, modelPos.z + 40.0f), 200.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[1]->SetParent(pModel);

			// エネミーの右手にコリジョンをつける
			modelPos = pEnemy->GetModelPos(5);
			pModel = pEnemy->GetModel(5);
			m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 70.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[2]->SetParent(pModel);

			// エネミーの左手にコリジョンをつける
			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);
			m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 70.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[3]->SetParent(pModel);

			// コリジョン発生源の位置を、自身の位置として設定
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS[0]->SetPosSource(enemyPos);
			m_pCollisionS[1]->SetPosSource(enemyPos);
			m_pCollisionS[2]->SetPosSource(enemyPos);
			m_pCollisionS[3]->SetPosSource(enemyPos);

			// 倍率をそのまま攻撃力として設定
			m_pCollisionS[0]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[1]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[2]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[3]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}

		m_nAtkCount++;
	}

	// 攻撃以外のモーションになったら
	if (pMotion->GetMotion() != 2 && pMotion->GetMotion() != 8)
	{
		for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
		{
			if (m_pCollisionS[nCntColl] != NULL)
			{
				// 当たり判定を消す
				m_pCollisionS[nCntColl] = NULL;
			}
		}
	}
}

//=============================================================================
// ジャンプ処理
//=============================================================================
void CControlBoss::Jump(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// エネミーが着地しているなら
	if (pEnemy->GetLand() == true)
	{
		// 思考が<通常ジャンプ>の時
		if (m_think == BOSS_THINK_JUMP_NORMAL)
		{
			// モーションをつなげていないかつ、ジャンプ(溜め)モーションじゃないかつ、モーションが止まっていないなら
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 4 && pMotion->GetStop() == false)
			{
				// ジャンプ回数をカウント
				m_nJumpNum++;

				// ジャンプ(溜め)モーションにする
				pMotion->SetMotion(4);
			}

			// モーションをつなげていないかつ、ジャンプモーションじゃないかつ、モーションが止まっているなら
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 5 && pMotion->GetStop() == true)
			{
				// ジャンプモーションにする
				pMotion->SetMotion(5);

				// 移動量をジャンプ力ぶん加算
				m_move.y += BOSS_JUMP_NORMAL;

				// エネミーの判定を着地していないに設定
				pEnemy->SetLand(false);
			}
		}
		// 思考が<大ジャンプ>の時
		else if (m_think == BOSS_THINK_JUMP_BIG)
		{
			// モーションをつなげていないかつ、ジャンプ(溜め)モーションじゃないかつ、モーションが止まっていないなら
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 4 && pMotion->GetStop() == false)
			{
				// ジャンプ回数をカウント
				m_nJumpNum++;

				// ジャンプ(溜め)モーションにする
				pMotion->SetMotion(4);
			}

			// モーションをつなげていないかつ、ジャンプモーションじゃないかつ、モーションが止まっているなら
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 5 && pMotion->GetStop() == true)
			{
				// ジャンプモーションにする
				pMotion->SetMotion(5);

				// 移動量をジャンプ力ぶん加算
				m_move.y += BOSS_JUMP_BIG;

				// エネミーの判定を着地していないに設定
				pEnemy->SetLand(false);
			}
		}
		
		// 着地する瞬間
		if (pMotion->GetConnect() == false && pMotion->GetMotion() == 5)
		{
			// 円形のパーティクルの生成
			CParticleCircle::Create(pEnemy->GetPos(), D3DXVECTOR3(100.0f, 170.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 180, 153), 20,
				20.0f, 50);

			// 着地(ボス)SEの再生
			pSound->Play(20);
			pSound->ControllVoice(20, 0.3f);

			// 思考が<通常ジャンプ>の時
			if (m_think == BOSS_THINK_JUMP_NORMAL)
			{
				// 着地モーションにする
				pMotion->SetMotion(6);

				// 思考を<停止>にし、止める時間を設定
				m_think = BOSS_THINK_STOP;
				m_nMoveStop = 40;

			}
			// 思考が<大ジャンプ>の時
			else if (m_think == BOSS_THINK_JUMP_BIG)
			{
				m_nJumpNum = 0;

				// チャージモーションにする
				pMotion->SetMotion(7);

				// 思考を<チャージ開始>にし、止める時間を設定
				m_think = BOSS_THINK_CHARGE_BEGIN;
				m_nDashCount = 100;
			}
		}
	}
	// エネミーが着地していないなら
	else if (pEnemy->GetLand() == false)
	{
		// ジャンプモーションかつ、思考が<大ジャンプ>の時
		if (pMotion->GetMotion() == 5 && m_think == BOSS_THINK_JUMP_BIG)
		{
			if (m_moveOld.y >= 0 && m_move.y < 0)
			{
				// 5箇所の位置にランダムに移動
				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 posRand[5] = { BOSS_AREA,
					D3DXVECTOR3(BOSS_AREA.x - 1000.0f, 0.0f, BOSS_AREA.z + 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x + 1000.0f, 0.0f, BOSS_AREA.z + 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x - 1000.0f, 0.0f, BOSS_AREA.z - 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x + 1000.0f, 0.0f, BOSS_AREA.z - 1000.0f) };

				int nRandPos = rand() % 5;
				pos = D3DXVECTOR3(posRand[nRandPos].x, pos.y, posRand[nRandPos].z);

				pEnemy->SetNewPos(pos);
			}
		}
	}
}

//=============================================================================
// 移動の慣性についての処理
//=============================================================================
void CControlBoss::MoveInteria(CEnemy *pEnemy)
{
	if (m_think == BOSS_THINK_CHARGE || m_think == BOSS_THINK_DASH_END)
	{
		m_move.z *= 0.94f;
		m_move.x *= 0.94f;
	}
	else
	{
		// 慣性の減算
		m_move.z *= BOSS_INTERIA_SUBTRACTION;
		m_move.x *= BOSS_INTERIA_SUBTRACTION;
	}

	// 移動量が既定の値になったら0にする
	if (m_move.x <= BOSS_MOVE_STOP_COUNT && m_move.x >= -BOSS_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= BOSS_MOVE_STOP_COUNT && m_move.z >= -BOSS_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// 回転処理
//=============================================================================
void CControlBoss::Rotate(CEnemy *pEnemy)
{
	// プレイヤーの向きを取得
	D3DXVECTOR3 rotPlayer = pEnemy->GetRot();

	// 回転させる状態なら
	if (m_bRotate == true)
	{
		// 目的の向きを計算
		if (m_fObjectiveRot > D3DX_PI)
		{
			m_fObjectiveRot = -D3DX_PI - (D3DX_PI - m_fObjectiveRot);
		}
		else if (m_fObjectiveRot < -D3DX_PI)
		{
			m_fObjectiveRot = D3DX_PI - (-D3DX_PI - m_fObjectiveRot);
		}

		// プレイヤーの現在の向きごとにそれぞれ向きを変える量を計算
		if (rotPlayer.y < 0.0f && -rotPlayer.y + m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (-D3DX_PI - rotPlayer.y) + -(D3DX_PI - m_fObjectiveRot);
		}
		else if (rotPlayer.y >= D3DX_PI / 2.0f && rotPlayer.y - m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (D3DX_PI - rotPlayer.y) - (-D3DX_PI - m_fObjectiveRot);
		}
		else
		{
			m_fNumRot = (m_fObjectiveRot - rotPlayer.y);
		}

		// プレイヤーの向きに加算
		rotPlayer.y += m_fNumRot * 0.2f;

		// 既定の値に達したら回転をやめる
		if (rotPlayer.y - m_fObjectiveRot < 0.001 && rotPlayer.y - m_fObjectiveRot > -0.001)
		{
			m_bRotate = false;
		}
	}

	// 向きを3.14から-3.14の値の範囲内に収める
	if (rotPlayer.y > D3DX_PI)
	{
		rotPlayer.y -= D3DX_PI * 2.0f;
	}
	else if (rotPlayer.y < -D3DX_PI)
	{
		rotPlayer.y += D3DX_PI * 2.0f;
	}

	// 向きを反映
	pEnemy->SetRot(rotPlayer);
}