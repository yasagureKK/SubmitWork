//=============================================================================
// プレイヤー操作処理 [control_player.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_player.h"
#include "manager.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "player.h"
#include "motion_player.h"
#include "camera.h"
#include "tutorial.h"
#include "sound.h"
#include "particle_circle.h"
#include "particle_wind.h"
#include "billboard_smoke.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CControlPlayer::CControlPlayer()
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_fFloat = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nSpinCount = 0;
	m_nMoveCount = 0;
	m_nextInput = PLAYER_NEXT_INPUT_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlPlayer::~CControlPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlPlayer::Init(void)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = PLAYER_MOVE_SPEED;
	m_fFloat = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nSpinCount = 0;
	m_nMoveCount = 0;
	m_nextInput = PLAYER_NEXT_INPUT_NONE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlPlayer::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlPlayer::Update(CScene *pScene)
{
	// 引数のポインタをプレイヤークラスのポインタにキャスト
	CPlayer *pPlayer = NULL;
	pPlayer = (CPlayer*)pScene;

	// モーション取得処理
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラのポインタのアドレス取得
	CCamera **pCameraAddress = CManager::GetCamera();

	// カメラの最大数ぶん、ポインタ配列を進める
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// カメラの取得
		CCamera *pCamera = &**pCameraAddress;

		if (pCamera != NULL)
		{
			// プレイヤーの番号とカメラの番号が同じだったら
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// カメラの向き取得
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// 移動量を設定
	m_move = pPlayer->GetMove();
	m_moveOld = m_move;

	// 重力
	m_move.y -= (PLAYER_GRAVITY - m_fFloat);

	// 2段ジャンプ中なら
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// 重力が強くなりすぎたら重力の最大値で固定しておく
		if (m_move.y <= -PLAYER_MAX_DOUBLE_JUMP_GRAVITY)
		{
			m_move.y = -PLAYER_MAX_DOUBLE_JUMP_GRAVITY;
		}
	}
	else
	{
		if (m_move.y <= -PLAYER_MAX_GRAVITY)
		{
			m_move.y = -PLAYER_MAX_GRAVITY;
		}
	}

	// 止められていないかつ、計測が終わっていないなら
	if (pPlayer->GetStop() == false && pPlayer->GetLandFinish() == false)
	{
		// 移動処理(詳しい処理は関数の中)
		Move(pPlayer);

#ifdef _DEBUG
		// デバッグ用の移動
		//DebugMove(pPlayer);
#endif
	}

	// 移動の慣性(詳しい処理は関数の中)
	MoveInteria(pPlayer);

	// 現在止まっているかつ、今が移動モーションかつ、モーションをつなげていなかったら
	if (m_move.x == 0.0f && m_move.z == 0.0f &&
		pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_MOVE &&
		pPlayer->GetMotionPlayer()->GetConnect() == false)
	{
		// ニュートラルモーションにする
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_NEUTRAL);
	}
	// 前のフレームに止まっていて現在動いているかつ、ジャンプ関連モーションではないとき
	else if (((m_moveOld.x == 0.0f && m_move.x != 0.0f) || (m_moveOld.z == 0.0f && m_move.z != 0.0f)) &&
		(pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP) &&
		pPlayer->GetLandFinish() == false)
	{
		// 移動モーションにする
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_MOVE);
	}
	// 移動量が下がっている時かつ、ジャンプしているかつ、着地していない時かつ、2段ジャンプしていない時かつ、モーションをつなげていなかったら
	else if (m_move.y < -PLAYER_GRAVITY && pPlayer->GetJump() == true && pPlayer->GetLand() == false &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP &&
		pPlayer->GetMotionPlayer()->GetConnect() == false)
	{
		// 落下モーションにする
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL);
	}

	// 地上に着地している時
	if (pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL &&
		pPlayer->GetMotionPlayer()->GetConnect() == true)
	{
		m_fSpeed = PLAYER_MOVE_SPEED;
	}

	// 移動中なら
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_MOVE &&
		(m_move.x >= 1.0f || m_move.x <= -1.0f || m_move.z >= 1.0f || m_move.z <= -1.0f))
	{
		m_nMoveCount++;

		if (m_nMoveCount % 8 == 0)
		{
			// 風のパーティクルの生成
			m_pParticle = CParticleWind::Create(D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), D3DXVECTOR3(850.0f, 10.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 105), D3DXVECTOR3(-40.0f - m_move.x, 0.0f, 0.0f), pPlayer->GetNum());
		}

		if (m_nMoveCount % 10 == 0)
		{
			// 位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// 土煙の生成
			CBillboardSmoke::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(35.0f, 35.0f, 0.0f), 20);
		}
	}
	else
	{
		m_nMoveCount = 0;
	}

	// 2段ジャンプ中なら
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// 速度を下げる
		m_fSpeed = PLAYER_MOVE_SPEED * 0.5f;

		m_nSpinCount++;

		if (m_nSpinCount >= 8)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_SPIN);
			pSound->ControllVoice(CSound::SOUND_LABEL_SPIN, 0.2f);
			m_nSpinCount = 0;
		}
	}
	else
	{
		m_nSpinCount = 0;
	}
	
	// 回転の慣性(詳しい処理は関数の中)
	Rotate(pPlayer);

	// プレイヤーが着地しているなら
	if (pPlayer->GetLand() == true)
	{
		// Y方向の移動量を0に
		m_move.y = 0.0f;
		m_fFloat = 0.0f;
	}

	// チュートリアルで説明中じゃないかつ、CPUじゃない場合のみ
	if (CTutorial::GetExplanation() == false && pPlayer->GetCpu() == false)
	{
		// 計測が終わっていないなら
		if (pPlayer->GetLandFinish() == false)
		{
			// ジャンプ処理(詳しい処理は関数の中)
			Jump(pPlayer);
		}
	}

	// 指定された時の移動処理
	CommandMove(pPlayer); 
}

//=============================================================================
// 生成処理
//=============================================================================
CControlPlayer *CControlPlayer::Create(void)
{
	// インスタンスの生成
	CControlPlayer *pControlPlayer = NULL;

	// ヌルチェック
	if (pControlPlayer == NULL)
	{
		// クラスの生成
		pControlPlayer = new CControlPlayer;
		if (pControlPlayer != NULL)
		{
			// 初期化処理
			pControlPlayer->Init();
		}
	}

	return pControlPlayer;
}

//=============================================================================
// 移動処理
//=============================================================================
void CControlPlayer::Move(CPlayer *pPlayer)
{
	// キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetCamera();

	// カメラの最大数ぶん、ポインタ配列を進める
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// カメラの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != NULL)
		{
			// プレイヤーの番号とカメラの番号が同じだったら
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// カメラの向き取得
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// プレイヤーの番号を取得
	int nNumPlayer = pPlayer->GetNum();

	// それぞれのプレイヤーの入力キーを設定
	int nLeft, nRight;
	if (nNumPlayer == 0)
	{
		nLeft = DIK_A;
		nRight = DIK_D;
	}
	else if (nNumPlayer == 1)
	{
		nLeft = DIK_LEFT;
		nRight = DIK_RIGHT;
	}

	// 地上にいる時
	if (pPlayer->GetLand() == true)
	{
		// チュートリアルで説明中じゃないかつ、CPUじゃない場合のみ
		if (CTutorial::GetExplanation() == false && pPlayer->GetCpu() == false)
		{
			//***********************************************************************
				// 左右を交互に連打でダッシュ (1P:A/D、2P:←/→ または パッド左スティック)
				//***********************************************************************
			if (pInputKeyboard->GetTrigger(nLeft) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNumPlayer) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nNumPlayer) == true)
			{
				if (m_nextInput == PLAYER_NEXT_INPUT_NONE || m_nextInput == PLAYER_NEXT_INPUT_LEFT)
				{
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
					pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.4f);

					// 加速度に加算し、徐々に加速させる
					m_fSpeed += PLAYER_ADD_SPEED;

					// 次に入力する操作を設定
					m_nextInput = PLAYER_NEXT_INPUT_RIGHT;
				}
			}
			else if (pInputKeyboard->GetTrigger(nRight) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNumPlayer) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nNumPlayer) == true)
			{
				if (m_nextInput == PLAYER_NEXT_INPUT_NONE || m_nextInput == PLAYER_NEXT_INPUT_RIGHT)
				{
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
					pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.4f);

					// 加速度に加算し、徐々に加速させる
					m_fSpeed += PLAYER_ADD_SPEED;

					// 次に入力する操作を設定
					m_nextInput = PLAYER_NEXT_INPUT_LEFT;
				}
			}
		}

		//目的の向きを設定
		m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
		//回転の慣性をオンにする
		m_bRotate = true;
	}

	//移動量加算
	m_move.x += -cosf(rotCamera.y) * m_fSpeed;
	m_move.z += +sinf(rotCamera.y) * m_fSpeed;
}

//=============================================================================
// ジャンプ処理
//=============================================================================
void CControlPlayer::Jump(CPlayer *pPlayer)
{
	// キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	// モーション取得処理
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// プレイヤーの番号を取得
	int nNumPlayer = pPlayer->GetNum();

	// それぞれのプレイヤーの入力キーを設定
	int nJump;
	if (nNumPlayer == 0)
	{
		nJump = DIK_SPACE;
	}
	else if (nNumPlayer == 1)
	{
		nJump = DIK_RETURN;
	}

	// 2段ジャンプ中かつ、モーションが2段ジャンプなら
	if (m_bDoubleJump == true && pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// プレイヤーが止められていないかつ、Y方向の移動量がマイナスだったら
		if (pPlayer->GetStop() == false && m_move.y < 0.0f)
		{
			// キーボードでの2人の入力情報を別々で保存
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				//***********************************************************************
				// ホバリング (1P:SPACE, 2P:O または パッドAボタン)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(nJump) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
				{
					// Y方向への移動量をいったん0に
					m_move.y = 0.0f;

					// 浮かばせる
					m_fFloat += 0.1f;

					// 浮力が重力の値を超えると上昇するのを防ぐ
					if (m_fFloat >= PLAYER_GRAVITY - 0.1f)
					{
						m_fFloat = PLAYER_GRAVITY - 0.1f;
					}
				}
				else
				{
					// 浮力をリセット
					m_fFloat = 0.0f;
				}
			}
		}
	}

	// プレイヤーがジャンプしたかつ、2段ジャンプをまだしていないなら
	if (pPlayer->GetJump() == true && m_bJump == true &&
		m_bDoubleJump == false)
	{
		// 止められていない場合
		if (pPlayer->GetStop() == false)
		{
			// キーボードでの2人の入力情報を別々で保存
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				// プレイヤー毎に入力情報を分ける
				if (nNumPlayer == nCntPlayer)
				{
					//***********************************************************************
					// 2段ジャンプ (1P:SPACE, 2P:O または パッドAボタン)
					//***********************************************************************
					if (pInputKeyboard->GetTrigger(nJump) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
					{
						// 位置を取得
						D3DXVECTOR3 pos = pPlayer->GetPos();
						// パーティクルで円形のエフェクトを出す
						m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DCOLOR_RGBA(255, 255, 255, 55), 10, 2.0f);
						m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DCOLOR_RGBA(255, 255, 255, 55), 10, 3.0f);

						// Y方向の移動量を0に
						m_move.y = 0.0f;

						// 移動速度の値がマイナスにならないようにする
						if (m_fSpeed <= 0.1f)
						{
							m_fSpeed = 0.1f;
						}

						// ジャンプモーションにする
						pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP);

						// 移動量をジャンプ力ぶん加算
						m_move.y = PLAYER_DOUBLE_JUMP;

						m_bDoubleJump = true;
					}
				}
			}
		}
	}

	// プレイヤーが着地しているなら
	if (pPlayer->GetLand() == true)
	{
		// ジャンプ力の設定
		float fJump = 0.0f;
		fJump = m_fSpeed * PLAYER_JUMP_MAGNIFICATION;

		// 止められていない場合
		if (pPlayer->GetStop() == false)
		{
			// キーボードでの2人の入力情報を別々で保存
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				// プレイヤー毎に入力情報を分ける
				if (nNumPlayer == nCntPlayer)
				{
					//***********************************************************************
					// ジャンプ (1P:SPACE, 2P:O または パッドAボタン)
					//***********************************************************************
					if (pInputKeyboard->GetTrigger(nJump) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
					{
						// SEの再生
						pSound->Play(CSound::SOUND_LABEL_JUMP);
						pSound->ControllVoice(CSound::SOUND_LABEL_JUMP, 0.4f);

						// ジャンプモーションにする
						pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP);

						// ジャンプ時に値が低すぎた場合は最低限のジャンプ力を持たせる
						if (fJump <= 8.0f)
						{
							fJump = 8.0f;
						}

						// 移動量をジャンプ力ぶん加算
						m_move.y = fJump;

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
	}
}

//=============================================================================
// 移動の慣性についての処理
//=============================================================================
void CControlPlayer::MoveInteria(CPlayer *pPlayer)
{
	// 地上における慣性の減算
	if (pPlayer->GetLand() == true)
	{
		m_move.z *= PLAYER_LAND_INTERIA_SUBTRACTION;
		m_move.x *= PLAYER_LAND_INTERIA_SUBTRACTION;
	}
	// 空中における慣性の減算
	else
	{
		m_move.z *= PLAYER_AIR_INTERIA_SUBTRACTION;
		m_move.x *= PLAYER_AIR_INTERIA_SUBTRACTION;
	}

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
	pPlayer->SetRot(rotPlayer);
}

//=============================================================================
// 行動を指定された時の移動処理
//=============================================================================
void CControlPlayer::CommandMove(CPlayer *pPlayer)
{
	// サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// モーション取得処理
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// コマンドによる制御
	switch (pPlayer->GetCommand())
	{
		// ダッシュ
	case CPlayer::PLAYER_COMMAND_DASH:
		// SEの再生
		pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
		pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.3f);

		// 加速度に加算し、徐々に加速させる
		m_fSpeed += PLAYER_ADD_SPEED;
		break;

	case CPlayer::PLAYER_COMMAND_JUMP:
		// 着地しているなら
		if (pPlayer->GetLand() == true)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_JUMP);
			pSound->ControllVoice(CSound::SOUND_LABEL_JUMP, 0.4f);

			// ジャンプ力の設定
			float fJump = 0.0f;
			fJump = m_fSpeed * PLAYER_JUMP_MAGNIFICATION;

			// ジャンプモーションにする
			pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP);

			// ジャンプ時に値が低すぎた場合は最低限のジャンプ力を持たせる
			if (fJump <= 8.0f)
			{
				fJump = 8.0f;
			}

			// 移動量をジャンプ力ぶん加算
			m_move.y = fJump;

			// プレイヤーの判定をジャンプした、着地していないに設定
			pPlayer->SetJump(true);
			pPlayer->SetLand(false);
			m_bJump = true;
		}
		break;

	case CPlayer::PLAYER_COMMAND_DOUBLE_JUMP:
		// プレイヤーがジャンプしたかつ、2段ジャンプをまだしていないなら
		if (pPlayer->GetJump() == true && m_bJump == true &&
			m_bDoubleJump == false)
		{
			// 位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();
			// パーティクルで円形のエフェクトを出す
			m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 55), 10, 2.0f);
			m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 55), 10, 3.0f);

			// Y方向の移動量を0に
			m_move.y = 0.0f;

			// ジャンプモーションにする
			pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP);

			// 移動量をジャンプ力ぶん加算
			m_move.y = PLAYER_DOUBLE_JUMP;

			m_bDoubleJump = true;
		}
		break;

	case CPlayer::PLAYER_COMMAND_HOVERING:
		// 2段ジャンプ中かつ、モーションが2段ジャンプなら
		if (m_bDoubleJump == true && pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
		{
			if (m_move.y < 0.0f)
			{
				// 浮力を増やす
				m_fFloat += 0.1f;

				m_move.y = 0.0f;

				// 浮力が重力の値を超えると上昇するのを防ぐ
				if (m_fFloat >= PLAYER_GRAVITY - 0.1f)
				{
					m_fFloat = PLAYER_GRAVITY - 0.1f;
				}

				// 浮力を徐々に下げる
				m_fFloat = 0.0f;
			}
		}
		break;

	default:
		break;
	}

	// コマンドを戻す
	pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_NONE);
}

#ifdef _DEBUG
//=============================================================================
// デバッグ用の移動処理
//=============================================================================
void CControlPlayer::DebugMove(CPlayer *pPlayer)
{
	// キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetCamera();

	// カメラの最大数ぶん、ポインタ配列を進める
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// カメラの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != NULL)
		{
			// プレイヤーの番号とカメラの番号が同じだったら
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// カメラの向き取得
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// プレイヤーの番号を取得
	int nNumPlayer = pPlayer->GetNum();

	// キーボードでの2人の入力情報を別々で保存
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		int nLeft, nRight, nUp, nDown;

		// それぞれのプレイヤーの入力キーを設定
		if (nNumPlayer == 0)
		{
			nLeft = DIK_A;
			nRight = DIK_D;
			nUp = DIK_W;
			nDown = DIK_S;
		}
		else if (nNumPlayer == 1)
		{
			nLeft = DIK_LEFT;
			nRight = DIK_RIGHT;
			nUp = DIK_UP;
			nDown = DIK_DOWN;
		}

		//移動の加速度
		m_fSpeed = PLAYER_MOVE_SPEED;

		//***********************************************************************
		// 移動 (1P:W/A/S/D, 2P:↑/↓/←/→ または パッド左スティック)
		//***********************************************************************
		if (pInputKeyboard->GetPress(nLeft) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNumPlayer) == true)
		{
			//左奥移動
			if (pInputKeyboard->GetPress(nUp) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//左手前移動
			else if (pInputKeyboard->GetPress(nDown) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - (D3DX_PI / 4.0f) * 3.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//左移動
			else
			{
				//移動量加算
				m_move.x += +cosf(rotCamera.y) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
		}
		else if (pInputKeyboard->GetPress(nRight) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNumPlayer) == true)
		{
			//右奥移動
			if (pInputKeyboard->GetPress(nUp) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//右手前移動
			else if (pInputKeyboard->GetPress(nDown) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 1.5f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
			//右移動
			else
			{
				//移動量加算
				m_move.x += -cosf(rotCamera.y) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y) * m_fSpeed;

				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
				//回転の慣性をオンにする
				m_bRotate = true;
			}
		}
		//奥移動
		else if (pInputKeyboard->GetPress(nUp) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
		{
			//移動量加算
			m_move.z += -cosf(rotCamera.y) * m_fSpeed;
			m_move.x += -sinf(rotCamera.y) * m_fSpeed;

			//※奥を向くだけ
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y;
			//回転の慣性をオンにする
			m_bRotate = true;
		}
		//手前移動
		else if (pInputKeyboard->GetPress(nDown) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
		{
			//移動量加算
			m_move.z += +cosf(rotCamera.y) * m_fSpeed;
			m_move.x += +sinf(rotCamera.y) * m_fSpeed;

			//※手前を向くだけ
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y + D3DX_PI;
			//回転の慣性をオンにする
			m_bRotate = true;
		}
	}
}
#endif