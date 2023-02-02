//=============================================================================
// プレイヤー操作処理 [control_player.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_player.h"
#include "manager.h"
#include "game.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "player.h"
#include "motion.h"
#include "level_up.h"
#include "camera.h"
#include "sound.h"
#include "collision_sphere.h"
#include "model.h"
#include "arrow.h"
#include "effect_billboard.h"
#include "data_attack.h"
#include "particle_dash.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_JUMP					(15.5f)	// ジャンプ力
#define PLAYER_GRAVITY				(0.6f)	// 重力の大きさ
#define PLAYER_MAX_GRAVITY			(15.0f)	// 重力の最大値
#define PLAYER_MOVE_SPEED			(1.1f)	// 移動量の基準値
#define PLAYER_INTERIA_SUBTRACTION	(0.86f)	// 地上での慣性の減算値
#define PLAYER_MOVE_STOP_COUNT		(0.02f)	// プレイヤーの移動量を0にする時の移動量の値
#define PLAYER_DASH					(4.0f)	// ダッシュの基準値
#define PLAYER_DASH_TIME			(15)	// ダッシュ時間
#define PLAYER_DASH_COOLTIME		(12)	// ダッシュ後のクールタイム

//=============================================================================
// コンストラクタ
//=============================================================================
CControlPlayer::CControlPlayer()
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_fJump = 0.0f;
	m_fAttackValue = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_bDash = false;
	m_bAttack = false;
	m_bInputAttack = false;
	m_bDamage = false;
	m_nMoveCount = 0;
	m_nDashCount = 0;
	m_nDashCoolTime = 0;
	m_nAtkCount = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_pCollisionS = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlPlayer::~CControlPlayer()
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlPlayer::Init(void)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = D3DX_PI;
	m_fNumRot = 0.0f;
	m_fSpeed = PLAYER_MOVE_SPEED;
	m_fBasicSpeed = m_fSpeed;
	m_fJump = PLAYER_JUMP;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_bDash = false;
	m_bAttack = false;
	m_bInputAttack = false;
	m_bDamage = false;
	m_nMoveCount = 0;
	m_nDashCount = 0;
	m_nDashCoolTime = PLAYER_DASH_COOLTIME;
	m_nAtkCount = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;

#ifdef _DEBUG
	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen("data/TEXT/motion_player.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "CHARACTERSET") == 0)	//CHARACTERSETの文字列
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "MOVE") == 0) //移動量
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_fSpeed);
						m_fBasicSpeed = m_fSpeed;
					}
					if (strcmp(&cString[0], "JUMP") == 0) //ジャンプ
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_fJump);
					}
					if (strcmp(&cString[0], "END_CHARACTERSET") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPTの文字列
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlPlayer::Uninit(void)
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CControlPlayer::Update(CScene *pScene)
{
	// 引数のポインタをプレイヤークラスのポインタにキャスト
	CPlayer *pPlayer = NULL;
	pPlayer = (CPlayer*)pScene;

	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	if (CManager::GetGame()->GetClear() == true)
	{
		// クリア時の処理
		Clear(pPlayer);
	}

	// 移動量を設定
	m_move = pPlayer->GetMove();

	//---------------------------------------------------
	// 重力
	//---------------------------------------------------
	m_move.y -= PLAYER_GRAVITY;

	// 重力が強くなりすぎたら重力の最大値で固定しておく
	if (m_move.y <= -PLAYER_MAX_GRAVITY)
	{
		m_move.y = -PLAYER_MAX_GRAVITY;
	}

	// 被ダメージ処理
	TakeDamage(pPlayer);

	// 死亡していないなら
	if (pPlayer->GetDeath() == false)
	{
		//---------------------------------------------------
		// 基本アクション
		//---------------------------------------------------
		if (m_bDamage == false)
		{
			// ダッシュしていなかったら
			if (m_bDash == false)
			{
				// 移動処理(詳しい処理は関数の中)
				Move(pPlayer);

				// アタック処理
				Attack(pPlayer);
			}

			// 攻撃していなかったら
			if (m_bAttack == false)
			{
				// ダッシュ処理
				Dash(pPlayer);

#ifdef _DEBUG
				// ジャンプ処理
				//Jump(pPlayer);
#endif
			}

			// 武器切り替え処理
			WeaponChange(pPlayer);
		}

		// 攻撃モーションじゃない場合、攻撃していない状態に設定
		if (pMotion->GetMotion() != 5 &&
			pMotion->GetMotion() != 6 &&
			pMotion->GetMotion() != 7 &&
			pMotion->GetMotion() != 8 &&
			pMotion->GetConnect() == false)
		{
			m_bAttack = false;
		}

		//---------------------------------------------------
		// モーション遷移
		//---------------------------------------------------
		// 現在動いておらず、モーションをつなげていないかつ、今が移動モーションかつ、ダッシュモーションじゃないなら
		if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetConnect() == false &&
			pMotion->GetMotion() == 1)
		{
			// ニュートラルモーションにする
			pMotion->SetMotion(0);
		}

		// 現在動いているかつ、モーションをつなげていないかつ、ニュートラルモーションだったら
		if ((m_move.x != 0.0f || m_move.z != 0.0f) && pMotion->GetConnect() == false &&
			pMotion->GetMotion() == 0)
		{
			// 慣性の移動量で移動モーションに遷移するため、移動の入力も条件に加える
			if ((pKeyboard->GetPress(DIK_W) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true) ||
				(pKeyboard->GetPress(DIK_A) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true) ||
				(pKeyboard->GetPress(DIK_S) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true) ||
				(pKeyboard->GetPress(DIK_D) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true))
			{
				// 移動モーションにする
				pMotion->SetMotion(1);
			}
		}

		// ダッシュモーションかつ、モーションをつなげていないかつ、ダッシュしていないなら
		if (pMotion->GetMotion() == 4 && pMotion->GetConnect() == false && m_bDash == false)
		{
			if (pPlayer->GetLand() == false)
			{
				// ジャンプモーションにする
				pMotion->SetMotion(2);
			}
			// 移動の入力中なら
			else if ((pKeyboard->GetPress(DIK_W) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true) ||
				(pKeyboard->GetPress(DIK_A) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true) ||
				(pKeyboard->GetPress(DIK_S) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true) ||
				(pKeyboard->GetPress(DIK_D) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true))
			{
				// 移動モーションにする
				pMotion->SetMotion(1);
			}
			else
			{
				// ニュートラルモーションにする
				pMotion->SetMotion(0);
			}
		}
	}

	//---------------------------------------------------
	// 慣性
	//---------------------------------------------------
	// 移動の慣性(詳しい処理は関数の中)
	MoveInteria(pPlayer);

	// 回転の慣性(詳しい処理は関数の中)
	Rotate(pPlayer);

	// 攻撃時のコリジョン
	DoingAttack(pPlayer);

	//---------------------------------------------------
	// 足音
	//---------------------------------------------------
	// 移動モーションかつ、着地しているなら
	if (pMotion->GetMotion() == 1 && pPlayer->GetLand() == true)
	{
		if (m_nMoveCount % 40 == 0)
		{
			// 足音(2)SEの再生
			pSound->Play(5);
			pSound->ControllVoice(5, 0.3f);
		}
		else
		{
			if (m_nMoveCount % 20 == 0)
			{
				// 足音(1)SEの再生
				pSound->Play(6);
				pSound->ControllVoice(6, 0.3f);
			}
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
CControlPlayer *CControlPlayer::Create(void)
{
	// クラスの生成
	CControlPlayer *pControlPlayer = NULL;
	pControlPlayer = new CControlPlayer;

	// ヌルチェック
	if (pControlPlayer != NULL)
	{
		// 初期化処理
		pControlPlayer->Init();
	}
	return pControlPlayer;
}

//=============================================================================
// 移動処理
//=============================================================================
void CControlPlayer::Move(CPlayer *pPlayer)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// カメラの向き取得
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rotCamera = pCamera->GetRotV();

	//---------------------------------------------------
	// 移動速度の設定
	//---------------------------------------------------
	// 攻撃していなかったら
	if (m_bAttack == false)
	{
		// 移動は基本の速度に
		m_fSpeed = m_fBasicSpeed;
	}
	// ボウガン以外の攻撃中なら
	else if (m_bAttack == true && pMotion->GetMotion() != 8)
	{
		// 移動スピードを遅くする
		m_fSpeed = m_fBasicSpeed / 3.0f;
	}
	// ボウガンで攻撃中なら
	else if (m_bAttack == true && pMotion->GetMotion() == 8)
	{
		// 移動できなくする
		m_fSpeed = 0.0f;
	}

	// 速度補正値によって速度を変える
	float fSpeed = m_fSpeed * (1.0f + CLevelUp::GetCorrectionSpd() * 0.3f);

	// 操作可能なら
	if (pPlayer->GetCanControl() == true)
	{
		//***********************************************************************
		// 移動 (キーボードＷ/Ａ/Ｓ/Ｄ または パッド左スティック)
		//***********************************************************************
		//左移動
		if (pKeyboard->GetPress(DIK_A) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true)
		{
			//左奥移動
			if (pKeyboard->GetPress(DIK_W) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				m_move.z += -sinf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//左手前移動
			else if (pKeyboard->GetPress(DIK_S) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				m_move.z += -sinf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - (D3DX_PI / 4.0f) * 3.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			else
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y) * fSpeed;
				m_move.z += -sinf(rotCamera.y) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
		}
		//右移動
		else if (pKeyboard->GetPress(DIK_D) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true)
		{
			//右奥移動
			if (pKeyboard->GetPress(DIK_W) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				m_move.z += +sinf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//右手前移動
			else if (pKeyboard->GetPress(DIK_S) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				m_move.z += +sinf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 1.5f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			else
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y) * fSpeed;
				m_move.z += +sinf(rotCamera.y) * fSpeed;
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
		}
		//奥移動
		else if (pKeyboard->GetPress(DIK_W) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
		{
			//移動量加算
			m_move.x += -sinf(rotCamera.y) * fSpeed;
			m_move.z += -cosf(rotCamera.y) * fSpeed;
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y;
			//回転の慣性をオンにする
			m_bRotate = true;
		}
		//手前移動
		else if (pKeyboard->GetPress(DIK_S) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
		{
			//移動量加算
			m_move.x += +sinf(rotCamera.y) * fSpeed;
			m_move.z += +cosf(rotCamera.y) * fSpeed;
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y + D3DX_PI;
			//回転の慣性をオンにする
			m_bRotate = true;
		}
	}
}

//=============================================================================
// ダッシュ処理
//=============================================================================
void CControlPlayer::Dash(CPlayer *pPlayer)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// コリジョン取得処理
	CCollisionSphere *pSphere = pPlayer->GetCollision();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// ダッシュ中じゃないなら
	if (m_bDash == false)
	{
		// クールタイムのカウントを増やす
		m_nDashCoolTime++;

		// クールタイムを過ぎているなら
		if (m_nDashCoolTime >= PLAYER_DASH_COOLTIME)
		{
			// 操作可能なら
			if (pPlayer->GetCanControl() == true)
			{
				//***********************************************************************
				// ダッシュ (キーボードShift または パッドAボタン)
				//***********************************************************************
				if (pKeyboard->GetTrigger(DIK_LSHIFT) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
				{
					// ダッシュしている状態に設定
					m_bDash = true;

					// プレイヤーの向きを目的の向きに設定
					pPlayer->SetRot(D3DXVECTOR3(0.0f, m_fObjectiveRot, 0.0f));

					// ダッシュSEの再生
					pSound->Play(4);
					pSound->ControllVoice(4, 0.5f);

					// ダッシュモーションにする
					pMotion->SetMotion(4);

					// ダッシュのパーティクルを発生させる
					D3DXVECTOR3 pos = pPlayer->GetPos();
					D3DXVECTOR3 size = pPlayer->GetSize();
					D3DXVECTOR3 rot = pPlayer->GetRot();
					pos.y += size.y / 2.0f;
					CParticleDash::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f), rot, D3DCOLOR_RGBA(150, 220, 255, 153),
						5, 10.0f, 70.0f, 15.0f, 15);
				}
			}
		}
	}

	// ダッシュ中
	if (m_bDash == true)
	{
		// クールタイムを進ませないようにする
		m_nDashCoolTime = 0;

		// コリジョンの判定を消す
		pSphere->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

		// カウントを増やす
		m_nDashCount++;

		if (m_nDashCount <= PLAYER_DASH_TIME)
		{
			// プレイヤーの向きを取得し、直進させる
			D3DXVECTOR3 rot = pPlayer->GetRot();
			m_move.x += -sinf(rot.y) * PLAYER_DASH;
			m_move.z += -cosf(rot.y) * PLAYER_DASH;
		}
		else if (m_nDashCount > PLAYER_DASH_TIME)
		{
			pSphere->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_PLAYER);
			m_bDash = false;
			m_nDashCount = 0;
		}
	}
}

//=============================================================================
// 攻撃処理
//=============================================================================
void CControlPlayer::Attack(CPlayer *pPlayer)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();
	// マウス取得処理
	CMouse *pMouse = CManager::GetMouse();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// 攻撃情報取得処理
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// 操作可能なら
	if (pPlayer->GetCanControl() == true)
	{
		//---------------------------------------------------
		// 斧を武器にしている時
		//---------------------------------------------------
		if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_AXE)
		{
			//***********************************************************************
			// 攻撃 (マウス左クリック または パッドXボタン)
			//***********************************************************************
			if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_LEFT) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
			{
				//***********************************************************************
				// 攻撃3段目
				//***********************************************************************
				// 攻撃中かつ、斧攻撃(2段目)のモーション中なら
				if (m_bAttack == true && pMotion->GetMotion() == 6)
				{
					// 攻撃の入力している状態にする
					m_bInputAttack = true;
				}

				//***********************************************************************
				// 攻撃2段目
				//***********************************************************************
				// 攻撃中かつ、斧攻撃(1段目)のモーション中なら
				if (m_bAttack == true && pMotion->GetMotion() == 5)
				{
					// 攻撃の入力している状態にする
					m_bInputAttack = true;
				}

				//***********************************************************************
				// 攻撃1段目
				//***********************************************************************
				// まだ攻撃していないかつ、モーションをつなげていなかったら
				if (m_bAttack == false && pMotion->GetConnect() == false)
				{
					m_bAttack = true;

					// 斧攻撃(1段目)のモーションにする
					pMotion->SetMotion(5);

					// 斧のスイングSEの再生
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// カウンターのリセット
					m_nAtkCount = 0;
				}
			}
			
			// 攻撃中かつ、斧攻撃(1段目)のモーション中なら
			if (m_bAttack == true && pMotion->GetMotion() == 5)
			{
				if (m_bInputAttack == true && m_nAtkCount >= pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_AXE_2, CDataAttack::ATTACK_DATA_TYPE_WAIT))
				{
					// 斧攻撃(2段目)のモーションにする
					pMotion->SetMotion(6);

					// 斧のスイングSEの再生
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// カウンターのリセット
					m_nAtkCount = 0;

					// 攻撃の入力の状態を戻す
					m_bInputAttack = false;
				}
			}
			
			// 攻撃中かつ、斧攻撃(2段目)のモーション中なら
			if (m_bAttack == true && pMotion->GetMotion() == 6)
			{
				if (m_bInputAttack == true && m_nAtkCount >= pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_AXE_3, CDataAttack::ATTACK_DATA_TYPE_WAIT))
				{
					// 斧攻撃(3段目)のモーションにする
					pMotion->SetMotion(7);

					// 斧のスイングSEの再生
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// カウンターのリセット
					m_nAtkCount = 0;

					// 攻撃の入力の状態を戻す
					m_bInputAttack = false;
				}
			}
		}
		//---------------------------------------------------
		// 武器をボウガンにしている時
		//---------------------------------------------------
		else if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_BOWGUN)
		{
			//***********************************************************************
			// 攻撃 (マウス左クリック または パッドXボタン)
			//***********************************************************************
			if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_LEFT) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
			{
				// まだ攻撃していないかつ、モーションをつなげていなかったら
				if (m_bAttack == false && pMotion->GetConnect() == false)
				{
					m_bAttack = true;

					// ボウガン攻撃のモーションにする
					pMotion->SetMotion(8);

					// カウンターのリセット
					m_nAtkCount = 0;
				}
			}
		}
	}
}

//=============================================================================
// 攻撃中の処理
//=============================================================================
void CControlPlayer::DoingAttack(CPlayer *pPlayer)
{
	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// 攻撃情報取得処理
	CDataAttack *pAtkData = CManager::GetDataAtk();
	CDataAttack::ATTACK_TYPE atkType = CDataAttack::ATTACK_TYPE_AXE;
	CCollisionSphere::COLLISION_S_TYPE collisionType = CCollisionSphere::COLLISION_S_TYPE_WEAPON;

	// 攻撃補正値によって基本の攻撃力を変える
	m_fAttackValue = (1.0f + CLevelUp::GetCorrectionAtk() * 0.5f);

	// 斧攻撃(1段目)の時
	if (pMotion->GetMotion() == 5)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE;
	}
	// 斧攻撃(2段目)の時
	else if (pMotion->GetMotion() == 6)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE_2;
	}
	// 斧攻撃(3段目)の時
	else if (pMotion->GetMotion() == 7)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE_3;
		collisionType = CCollisionSphere::COLLISION_S_TYPE_WEAPON_P;
	}

	// 何らかの斧攻撃
	if ((pMotion->GetMotion() == 5 && pMotion->GetNextMotion() == 5) ||
		(pMotion->GetMotion() == 6 && pMotion->GetNextMotion() == 6) ||
		pMotion->GetMotion() == 7 && pMotion->GetNextMotion() == 7)
	{
		// エフェクトの開始フレームを設定
		m_fAtkBeginEffect = pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// エフェクトの持続時間を設定
		m_fAtkEffectCount = pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		// 特定のフレームになったらコリジョンをつける
		if (m_nAtkCount == pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// プレイヤーの武器にあたるモデルの位置・情報を取得
			D3DXVECTOR3 modelPos = pPlayer->GetModelPos(15);
			CModel *pModel = pPlayer->GetModel(15);

			// 右手の武器にコリジョン(武器判定)をつけ、親子関係を持たせる
			m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z - 50.0f), 70.0f, collisionType, pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS->SetParent(pModel);

			// ダメージ量×倍率で攻撃力を設定
			m_pCollisionS->SetDamageValue(m_fAttackValue * pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// ボウガン攻撃の時
	if (pMotion->GetMotion() == 8)
	{
		// 特定のフレームになったらコリジョンをつける
		if (m_nAtkCount == pAtkData->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// プレイヤーの武器にあたるモデルの位置、プレイヤーの向きを取得
			CModel *pModel = pPlayer->GetModel(15);
			D3DXVECTOR3 pos = pModel->GetMtxPos();
			D3DXVECTOR3 rot = pPlayer->GetRot();

			// 矢を生成
			CArrow::Create(pos, rot, m_fAttackValue);
		}
	}

	// 攻撃中かつ、次のモーションが現在のモーションと同じなら
	if (m_bAttack == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// ボウガンのモーション中じゃなかったら
		if (pMotion->GetMotion() != 8)
		{
			// 特定のフレームになったかつ、持続時間中なら
			if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
			{
				D3DXVECTOR3 modelPos = pPlayer->GetModelPos(15);
				CModel *pModel = pPlayer->GetModel(15);

				// 軌跡にエフェクトをつける
				CEffectB::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z - 40.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
					D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			}
		}

		// カウントを増やす
		m_nAtkCount++;
	}
}

//=============================================================================
// 武器切り替え処理
//=============================================================================
void CControlPlayer::WeaponChange(CPlayer *pPlayer)
{
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();
	// マウス取得処理
	CMouse *pMouse = CManager::GetMouse();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// 操作可能なら
	if (pPlayer->GetCanControl() == true)
	{
		//***********************************************************************
		// 武器チェンジ (マウス右クリック または パッドRトリガー)
		//***********************************************************************
		if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true)
		{
			// 攻撃中じゃないかつ、モーションをつなげていなかったら
			if (m_bAttack == false && pMotion->GetConnect() == false)
			{
				// 切り替え中の状態に
				pPlayer->SetWeaponChange(true);

				// 武器切り替えSEの再生
				pSound->Play(11);
				pSound->ControllVoice(11, 0.8f);

				if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_AXE)
				{
					pPlayer->SetWeapon(CPlayer::PLAYER_WEAPON_BOWGUN);
				}
				else if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_BOWGUN)
				{
					pPlayer->SetWeapon(CPlayer::PLAYER_WEAPON_AXE);
				}
			}
		}
	}
}

//=============================================================================
// 被ダメージ処理
//=============================================================================
void CControlPlayer::TakeDamage(CPlayer *pPlayer)
{
	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// プレイヤーの状態が<吹っ飛び>になったかつ、ダメージを受けていなかったら
	if (pPlayer->GetState() == CPlayer::PLAYER_STATE_BLOWAWAY && m_bDamage == false)
	{
		// Y方向への移動量をリセットし、ジャンプさせる
		m_move.y = 0.0f;
		m_move.y += 6.0f;

		// ダッシュと攻撃状態の解除
		m_bDash = false;
		m_nDashCount = 0;
		m_bAttack = false;
		m_nAtkCount = 0;
		m_bInputAttack = false;

		// 攻撃時に発生していたコリジョンは消す
		if (m_pCollisionS != NULL)
		{
			// 攻撃時の判定を消す
			m_pCollisionS = NULL;
		}

		// ダメージを受けた状態にし、着地していない状態にする
		m_bDamage = true;
		pPlayer->SetLand(false);

		// 目的の向きを設定
		D3DXVECTOR3 rot = pPlayer->GetRot();
		m_fObjectiveRot = rot.y;

		// 被ダメージモーションにする
		pMotion->SetMotion(9);
	}

	// ダメージ中
	if (m_bDamage == true)
	{
		// 着地していないなら
		if (pPlayer->GetLand() == false)
		{
			// プレイヤーの正面から逆方向へ後退させる
			D3DXVECTOR3 rot = pPlayer->GetRot();
			m_move.x = sinf(rot.y) * 10.0f;
			m_move.z = cosf(rot.y) * 10.0f;
		}
		// 着地しているなら
		else if (pPlayer->GetLand() == true)
		{
			// モーションをつなげていないかつ、ライフが0になったら
			if (pMotion->GetMotion() != 10 && pMotion->GetConnect() == false && pPlayer->GetLife() <= 0)
			{
				// ダウンモーションにする
				pMotion->SetMotion(10);
			}
			// ライフがまだ残っているなら
			else if(pPlayer->GetLife() > 0)
			{
				// ニュートラルモーションにする
				pMotion->SetMotion(0);

				// ダメージ状態を戻す
				m_bDamage = false;

				// プレイヤーの状態を<通常>に戻す
				pPlayer->SetState(CPlayer::PLAYER_STATE_NORMAL);
			}
		}
	}
}

//=============================================================================
// 移動の慣性についての処理
//=============================================================================
void CControlPlayer::MoveInteria(CPlayer *pPlayer)
{
	// 慣性の減算
	m_move.z *= PLAYER_INTERIA_SUBTRACTION;
	m_move.x *= PLAYER_INTERIA_SUBTRACTION;

	// 移動量が既定の値になったら0にする
	if (m_move.x <= PLAYER_MOVE_STOP_COUNT && m_move.x >= -PLAYER_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= PLAYER_MOVE_STOP_COUNT && m_move.z >= -PLAYER_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// 回転処理
//=============================================================================
void CControlPlayer::Rotate(CPlayer *pPlayer)
{
	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// プレイヤーの向きを取得
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

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

		// 攻撃していない場合か、ボウガンで攻撃している場合
		if (m_bAttack == false ||
			m_bAttack == true && pMotion->GetMotion() == 8)
		{
			// プレイヤーの向きに加算
			rotPlayer.y += m_fNumRot * 0.2f;
		}
		// 斧で派生攻撃中の場合
		else if (pMotion->GetMotion() == 6 || pMotion->GetMotion() == 7)
		{
			// プレイヤーの向きに加算し、回転を止める
			rotPlayer.y += m_fNumRot * 0.025f;
			m_bRotate = false;
		}

		// 既定の値に達したら回転をやめる
		if (rotPlayer.y - m_fObjectiveRot < 0.001f && rotPlayer.y - m_fObjectiveRot > -0.001f)
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
	pPlayer->SetRot(rotPlayer);
}

//=============================================================================
// クリア時の処理
//=============================================================================
void CControlPlayer::Clear(CPlayer *pPlayer)
{
	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	if (pPlayer->GetLand() == true)
	{
		// モーションをつなげていないかつ、クリア関連のモーションじゃなかったら
		if (pMotion->GetConnect() == false && (pMotion->GetMotion() != 11 && pMotion->GetMotion() != 12))
		{
			// クリアモーションにする
			pMotion->SetMotion(11);

			// カウンターや状態のリセット
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_nMoveCount = 0;
			m_nDashCount = 0;
			m_nAtkCount = 0;
			m_bDash = false;
			m_bAttack = false;
			m_bDamage = false;
		}

		// カメラの取得
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 rotCamera = pCamera->GetRotV();

		//目的の向きを設定
		rotCamera.y += D3DX_PI;
		m_fObjectiveRot = rotCamera.y;
		pPlayer->SetRot(rotCamera);
	}
}

#ifdef _DEBUG
//=============================================================================
// ジャンプ処理 (デバッグ用として使用)
//=============================================================================
void CControlPlayer::Jump(CPlayer *pPlayer)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	// モーション取得処理
	CMotion *pMotion = pPlayer->GetMotion();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// 操作可能なら
	if (pPlayer->GetCanControl() == true)
	{
		// プレイヤーが着地していないかつ、2段ジャンプをまだしていないなら
		if (pPlayer->GetLand() == false && m_bDoubleJump == false)
		{
			//***********************************************************************
			// 2段ジャンプ
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				// ジャンプSEの再生
				pSound->Play(3);
				pSound->ControllVoice(3, 0.8f);

				// 位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// Y方向の移動量を0に
				m_move.y = 0.0f;

				// ジャンプモーションにする
				pMotion->SetMotion(2);

				// 移動量をジャンプ力ぶん加算
				m_move.y += m_fJump;

				m_bDoubleJump = true;
			}
		}

		// プレイヤーが着地しているなら
		if (pPlayer->GetLand() == true)
		{
			//***********************************************************************
			// ジャンプ
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				// ジャンプSEの再生
				pSound->Play(3);
				pSound->ControllVoice(3, 0.8f);

				// ジャンプモーションにする
				pMotion->SetMotion(2);

				// 移動量をジャンプ力ぶん加算
				m_move.y += m_fJump;

				// プレイヤーの判定をジャンプした、着地していないに設定
				pPlayer->SetJump(true);
				pPlayer->SetLand(false);

				m_bJump = true;
			}
			else
			{
				// 着地したらまたジャンプを可能にする
				m_bJump = false;
				m_bDoubleJump = false;
			}
		}
	}
}
#endif