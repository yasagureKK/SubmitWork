//=============================================================================
// エネミー(狼00)操作処理 [control_wolf00.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_wolf00.h"
#include "manager.h"
#include "enemy.h"
#include "motion.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "collision_sphere.h"
#include "model.h"
#include "data_attack.h"
#include "effect_billboard.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define WOLF00_GRAVITY				(0.6f)	// 重力の大きさ
#define WOLF00_MAX_GRAVITY			(15.0f)	// 重力の最大値
#define WOLF00_MOVE_SPEED			(0.5f)	// 移動量の基準値
#define WOLF00_INTERIA_SUBTRACTION	(0.86f)	// 地上での慣性の減算値
#define WOLF00_MOVE_STOP_COUNT		(0.02f)	// プレイヤーの移動量を0にする時の移動量の値
#define MAX_WOLF00_ATTACK_INTERVAL	(75)	// 基本の攻撃間隔
#define MAX_WOLF00_MOVE_RANGE		(2000)	// プレイヤーを追尾する範囲

//=============================================================================
// コンストラクタ
//=============================================================================
CControlWolf00::CControlWolf00()
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_bMove = false;
	m_bRotate = false;
	m_bAttack = false;
	m_nMoveCount = 0;
	m_nAtkInterval = 0;
	m_nAtkCount = 0;
	m_nAttackWaitTime = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_pCollisionS = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlWolf00::~CControlWolf00()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlWolf00::Init(void)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_nMoveCount = 0;
	m_fAtkEffectCount = 0;
	m_nAtkInterval = MAX_WOLF00_ATTACK_INTERVAL;
	m_fSpeed = WOLF00_MOVE_SPEED;
	m_bMove = false;
	m_bRotate = false;
	m_bAttack = false;
	m_fAtkBeginEffect = 0;
	m_nAttackWaitTime = 0;
	m_pCollisionS = NULL;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlWolf00::Uninit(void)
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CControlWolf00::Update(CScene *pScene)
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
	m_move.y -= WOLF00_GRAVITY;

	// 重力が強くなりすぎたら重力の最大値で固定しておく
	if (m_move.y <= -WOLF00_MAX_GRAVITY)
	{
		m_move.y = -WOLF00_MAX_GRAVITY;
	}

	// 移動処理
	Move(pEnemy);

	// 攻撃処理
	Attack(pEnemy);

	// 移動の慣性
	MoveInteria(pEnemy);
	
	// 攻撃モーションじゃないかつ、モーションをつなげていないなら
	if (pEnemy->GetMotion()->GetMotion() != 2 && pEnemy->GetMotion()->GetConnect() == false)
	{
		m_bAttack = false;
	}

	//---------------------------------------------------
	// モーション遷移
	//---------------------------------------------------
	if (pMotion != NULL)
	{
		// 攻撃していないかつ、被ダメージモーションじゃないなら
		if (m_bAttack == false && pMotion->GetMotion() != 3)
		{
			// 現在止まっているかつ、今が移動モーションかつ、モーションをつなげていなかったら
			if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetMotion() == 1 && pMotion->GetConnect() == false)
			{
				// ニュートラルモーションにする
				pMotion->SetMotion(0);
			}

			// 現在動いているかつ、モーションをつなげていないかつ、今が移動モーションじゃないなら
			if (m_bMove == true && pMotion->GetConnect() == false && pMotion->GetMotion() != 1)
			{
				// 移動モーションにする
				pMotion->SetMotion(1);
			}
		}
	}

	// 攻撃中の処理
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

		if (m_nMoveCount % 32 == 0)
		{
			// 敵の足音(1)SEの再生
			pSound->Play(23);
			pSound->ControllVoice(23, 0.2f);
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
CControlWolf00 *CControlWolf00::Create(void)
{
	// クラスの生成
	CControlWolf00 *pControlEnemy = NULL;
	pControlEnemy = new CControlWolf00;

	if (pControlEnemy != NULL)
	{
		// 初期化処理
		pControlEnemy->Init();
	}
	return pControlEnemy;
}

//=============================================================================
// 移動処理
//=============================================================================
void CControlWolf00::Move(CEnemy *pEnemy)
{
	m_bMove = false;

	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// 今が攻撃モーションじゃないかつ、被ダメージモーションじゃない場合
	if (pMotion->GetMotion() != 2 && pMotion->GetMotion() != 3)
	{
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
					float fStopRadius = plySize.x * 3; // プレイヤーの前で停止する距離

					// プレイヤーを追尾する範囲内かつ、停止する距離内にいないなら
					if (fRadius <= MAX_WOLF00_MOVE_RANGE && fRadius >= fStopRadius)
					{
						// 移動している状態にする
						m_bMove = true;

						// 移動量加算
						m_move.x += -cosf(fRot) * m_fSpeed;
						m_move.z += +sinf(fRot) * m_fSpeed;

						// 目的の向きを設定
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						// 回転の慣性をオンにする
						m_bRotate = true;
					}

					// プレイヤーを追尾する範囲外なら
					if (fRadius > MAX_WOLF00_MOVE_RANGE)
					{
						// 目的の向きを設定
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						// 回転の慣性をオンにする
						m_bRotate = true;
					}

					// プレイヤーの前で停止する距離なら
					if (fRadius <= fStopRadius)
					{
						// 攻撃中じゃなかったら
						if (m_bAttack == false)
						{
							//目的の向きを設定
							m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

							//回転の慣性をオンにする
							m_bRotate = true;
						}
					}

					// プレイヤーが停止する距離内にいるなら
					if (fRadius <= fStopRadius)
					{
						m_nAttackWaitTime++;
					}
				}
			}

			// 次のオブジェクトを取得
			pObject = pSaveObject->GetObjNext(pSaveObject);
		}
	}
}

//=============================================================================
// 攻撃処理
//=============================================================================
void CControlWolf00::Attack(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// まだ攻撃していないかつ、モーションをつなげていないかつ、被ダメージモーションじゃないなら
	if (m_bAttack == false && pMotion->GetConnect() == false && pMotion->GetMotion() != 3)
	{
		if (m_nAttackWaitTime >= m_nAtkInterval)
		{
			// 攻撃している状態に
			m_bAttack = true;

			// 攻撃モーションにする
			pMotion->SetMotion(2);

			// カウンターのリセット
			m_nAttackWaitTime = 0;
			m_nAtkCount = 0;

			// 次の攻撃から、基本の攻撃間隔を基にして間隔をランダムにする
			m_nAtkInterval = (rand() % 40 - 10) + MAX_WOLF00_ATTACK_INTERVAL;
		}
	}

	// 攻撃モーション中なら
	if (pMotion->GetMotion() == 2)
	{
		if (pMotion->GetNextMotion() == 3 || pMotion->GetNextMotion() == 4)
		{
			m_nAtkCount = 0;
		}

		if (pMotion->GetNextMotion() == 2)
		{
			if (m_nAtkCount >= 20 && m_nAtkCount <= 24)
			{
				// エネミーの向きを取得し、直進させる
				D3DXVECTOR3 rot = pEnemy->GetRot();
				//移動量加算
				m_move.x += -sinf(rot.y) * 1.5f;
				m_move.z += -cosf(rot.y) * 1.5f;
			}
		}
	}
}

//=============================================================================
// 攻撃中の処理
//=============================================================================
void CControlWolf00::DoingAttack(CEnemy *pEnemy)
{
	// モーション取得処理
	CMotion *pMotion = pEnemy->GetMotion();

	// 攻撃情報取得処理
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// 攻撃モーションの時
	if (pMotion->GetMotion() == 2)
	{
		// エフェクトの開始フレームを設定
		m_fAtkBeginEffect = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// エフェクトの持続時間を設定
		m_fAtkEffectCount = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		// 特定のフレームになったらコリジョンをつける
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// 切り裂き(1)SEの再生
			CSound *pSound = CManager::GetSound();
			pSound->Play(17);
			pSound->ControllVoice(17, 0.6f);

			// エネミーの右手にあたるモデルの位置・情報を取得
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(4);
			CModel *pModel = pEnemy->GetModel(4);

			// 右手にコリジョン(敵攻撃判定)をつけ、親子関係を持たせる
			m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z), 100.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS->SetParent(pModel);

			// コリジョン発生源の位置を、自身の位置として設定
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS->SetPosSource(enemyPos);

			// 倍率をそのまま攻撃力として設定
			m_pCollisionS->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// 攻撃中かつ、次のモーションが現在のモーションと同じなら
	if (m_bAttack == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// 特定のフレームになったかつ、持続時間中なら
		if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
		{
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(4);
			CModel *pModel = pEnemy->GetModel(4);

			// 軌跡にエフェクトをつける
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z - 10.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z + 5.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 5.0f, modelPos.y, modelPos.z + 25.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
		}
		// カウントを増やす
		m_nAtkCount++;
	}

	// 被ダメージモーション、またはダウンモーションになったら
	if (pMotion->GetMotion() == 3 || pMotion->GetMotion() == 4)
	{
		if (m_pCollisionS != NULL)
		{
			// 攻撃の当たり判定を消す
			m_pCollisionS->SetTime(0);
			m_nAttackWaitTime = 0;
		}
	}
}

//=============================================================================
// 移動の慣性についての処理
//=============================================================================
void CControlWolf00::MoveInteria(CEnemy *pEnemy)
{
	// 慣性の減算
	m_move.z *= WOLF00_INTERIA_SUBTRACTION;
	m_move.x *= WOLF00_INTERIA_SUBTRACTION;

	// 移動量が既定の値になったら0にする
	if (m_move.x <= WOLF00_MOVE_STOP_COUNT && m_move.x >= -WOLF00_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= WOLF00_MOVE_STOP_COUNT && m_move.z >= -WOLF00_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// 回転処理
//=============================================================================
void CControlWolf00::Rotate(CEnemy *pEnemy)
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