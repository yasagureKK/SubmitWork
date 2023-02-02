//=============================================================================
// カメラ処理 [camera.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "keyboard.h"
#include "gamepad.h"
#include "game.h"
#include "pause.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_HEIGHT				(170.0f)	// カメラの高さ
#define CAMERA_MAX_HEIGHT			(190.0f)	// カメラの最大高さ
#define CAMERA_MIN_HEIGHT			(150.0f)	// カメラの最小高さ
#define CAMERA_MOVE_SPEED			(5.0f)		// カメラの移動速度
#define CAMERA_INDUCTION_SPEED		(45.0f)		// カメラの誘導速度
#define CAMERA_MAX_ROTATE_SPEED		(0.03f)		// カメラの最大旋回速度
#define CAMERA_INTERIA_SUBTRACTION	(0.92f)		// 旋回の慣性の減算値
#define CAMERA_ROTATE_STOP_COUNT	(0.002f)	// プレイヤーの移動量を0にする時の移動量の値
#define CAMERA_R_DISTANCE			(650.0f)	// プレイヤーと注視点の距離
#define CAMERA_V_DISTANCE			(1000.0f)	// 視点と注視点の距離
#define CAMERA_MAX_DISTANCE			(1200.0f)	// 視点と注視点の最大距離
#define CAMERA_MIN_DISTANCE			(900.0f)	// 視点と注視点の最小距離
#define CAMERA_MIN_RENDERER			(10.0f)		// 描画範囲の最小値
#define CAMERA_MAX_RENDERER			(40000.0f)	// 描画範囲の最大値
#define CAMERA_CLEAR_HEIGHT			(60.0f)		// クリア時のカメラの高さ
#define CAMERA_CLEAR_R_DISTANCE		(650.0f)	// クリア時のプレイヤーと注視点の距離
#define CAMERA_CLEAR_V_DISTANCE		(900.0f)	// クリア時の視点と注視点の距離

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CCamera::m_bControl = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 変数のクリア
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeed = 0.0f;
	m_fRotateSpeed = 0.0f;
	m_fHeight = 0.0f;
	m_fSaveHeight = 0.0f;
	m_fDistance_PlayerPosR = 0.0f;
	m_fDistance_posVposR = 0.0f;
	m_fSaveDistance_PlayerPosR = 0;
	m_fSaveDistance_posVposR = 0;
	m_fObjectiveRot = 0.0f;
	m_fObjectiveDistance = 0.0f;
	m_fNumRot = 0.0f;
	m_viewport.X = (DWORD)0.0f;
	m_viewport.Y = (DWORD)0.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_viewport.Width = (DWORD)0.0f;
	m_viewport.Height = (DWORD)0.0f;
	m_bRotate = false;
	m_bControl = false;
	m_type = CAMERA_MOVE_TYPE_PLAYER;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight)
{
	// 変数の初期化
	m_rot = rot;
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = pos;
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fSpeed = CAMERA_MOVE_SPEED;
	m_fRotateSpeed = 0.0f;
	m_fHeight = CAMERA_HEIGHT;
	m_fSaveHeight = 0.0f;
	m_fDistance_PlayerPosR = CAMERA_R_DISTANCE;
	m_fDistance_posVposR = CAMERA_V_DISTANCE;
	m_fSaveDistance_PlayerPosR = 0;
	m_fSaveDistance_posVposR = 0;
	m_fObjectiveRot = 0.0f;
	m_fObjectiveDistance = CAMERA_V_DISTANCE;
	m_fNumRot = 0.0f;
	m_viewport.X = (DWORD)viewportX;
	m_viewport.Y = (DWORD)viewportY;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewport.Width = (DWORD)viewportWidth;
	m_viewport.Height = (DWORD)viewporHeight;
	m_bRotate = false;
	m_bControl = false;
	m_type = CAMERA_MOVE_TYPE_PLAYER;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	// ポーズしていなかったら
	if (CManager::GetPause() == false)
	{
		// プレイヤーの追尾
		if (m_type == CAMERA_MOVE_TYPE_PLAYER)
		{
			MovePlayer();
		}
		// ボス戦
		else if (m_type == CAMERA_MOVE_TYPE_BOSS)
		{
			MoveBoss();
		}
		// ゲームクリア
		else if (m_type == CAMERA_MOVE_TYPE_CLEAR)
		{
			MoveClear();
		}
	}
	
	// デバッグ用の移動
#ifdef _DEBUG
	if (m_type == CAMERA_MOVE_TYPE_DEBUG)
	{
		DebugMove();
	}
#endif
}

//=============================================================================
// 生成処理
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY,
	float viewportWidth, float viewporHeight)
{
	// クラスの生成
	CCamera *pCamera = NULL;
	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		if (pCamera != NULL)
		{
			// 初期化処理
			pCamera->Init(pos, rot, viewportX, viewportY, viewportWidth, viewporHeight);
		}
	}
	return pCamera;
}

//=============================================================================
// 設定処理
//=============================================================================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(50.0f),									// 画角
		(float)m_viewport.Width / (float)m_viewport.Height,		// 比率
		CAMERA_MIN_RENDERER,									// Z方向の描画範囲
		CAMERA_MAX_RENDERER);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// プレイヤー用の移動処理
//=============================================================================
void CCamera::MovePlayer(void)
{
	// キーボード・ゲームパッド取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	CGamePad *pGamePad = CManager::GetGamePad();

	// 操作できるなら
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// カメラ回転 (Ｚキー / Ｃキー または パッド右スティック)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_Z) == true && m_bRotate == false ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_LEFT, 0) == true)
		{
			// 注視点から時計回りに回転
			m_fRotateSpeed += 0.01f;

			if (m_fRotateSpeed >= CAMERA_MAX_ROTATE_SPEED)
			{
				m_fRotateSpeed = CAMERA_MAX_ROTATE_SPEED;
			}
		}
		else if (pKeyboard->GetPress(DIK_C) == true && m_bRotate == false ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_RIGHT, 0) == true)
		{
			// 注視点から反時計回りに回転
			m_fRotateSpeed -= 0.01f;

			if (m_fRotateSpeed <= -CAMERA_MAX_ROTATE_SPEED)
			{
				m_fRotateSpeed = -CAMERA_MAX_ROTATE_SPEED;
			}
		}
	}

	// 旋回しているなら
	if (m_fRotateSpeed != 0.0f)
	{
		// 向きに旋回速度を追加し、視点の位置を移動
		m_rot.y += m_fRotateSpeed;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}

	// 慣性の減算
	m_fRotateSpeed *= CAMERA_INTERIA_SUBTRACTION;
	m_fRotateSpeed *= CAMERA_INTERIA_SUBTRACTION;

	// 旋回速度が既定の値になったら0にする
	if (m_fRotateSpeed <= CAMERA_ROTATE_STOP_COUNT && m_fRotateSpeed >= -CAMERA_ROTATE_STOP_COUNT)
	{
		m_fRotateSpeed = 0.0f;
	}

	// 操作できるなら
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ズームイン・アウト (↑キー / ↓キー または パッド右スティック)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_UP) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_UP, 0) == true)
		{
			if (m_fDistance_posVposR <= CAMERA_MIN_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MIN_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR -= 10.0f;
				m_fHeight -= 3.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_DOWN, 0) == true)
		{
			if (m_fDistance_posVposR >= CAMERA_MAX_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MAX_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR += 10.0f;
				m_fHeight += 3.0f;
			}
		}
	}

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

			// プレイヤーの位置を取得
			pos = pPlayer->GetPos();

			// 操作できるなら
			if (m_bControl == true)
			{
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// カメラ注目 (Spaceキー または パッドLトリガー)
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (m_bRotate == false && (pKeyboard->GetTrigger(DIK_SPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER, 0) == true))
				{
					// プレイヤーの向きを取得
					D3DXVECTOR3 rot = pPlayer->GetRot();

					// 目的の向きを設定
					m_fObjectiveRot = rot.y;

					// 回転中にする
					m_bRotate = true;
				}
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	// 回転させる状態なら
	if (m_bRotate == true)
	{
		// プレイヤーの現在の向きごとにそれぞれ向きを変える量を計算
		if (m_rot.y < 0.0f && -m_rot.y + m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (-D3DX_PI - m_rot.y) + -(D3DX_PI - m_fObjectiveRot);
		}
		else if (m_rot.y >= D3DX_PI / 2.0f && m_rot.y - m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (D3DX_PI - m_rot.y) - (-D3DX_PI - m_fObjectiveRot);
		}
		else
		{
			m_fNumRot = (m_fObjectiveRot - m_rot.y);
		}

		// プレイヤーの向きに加算
		m_rot.y += m_fNumRot * 0.2f;

		// 既定の値に達したら回転をやめる
		if ((m_rot.y - m_fObjectiveRot < 0.001 && m_rot.y - m_fObjectiveRot > -0.001) ||
			(-m_rot.y - m_fObjectiveRot < 0.001 && -m_rot.y - m_fObjectiveRot > -0.001))
		{
			m_bRotate = false;
		}
	}

	//プレイヤーの位置をもとに注視点の位置を移動
	m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.y = pos.y;

	//注視点の位置をもとに視点の位置を移動
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// ボス戦用の移動処理
//=============================================================================
void CCamera::MoveBoss(void)
{
	// キーボード・ゲームパッド取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	CGamePad *pGamePad = CManager::GetGamePad();

	// 操作できるなら
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ズームイン・アウト (↑キー / ↓キー または パッド右スティック)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_UP) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_UP, 0) == true)
		{
			if (m_fDistance_posVposR <= CAMERA_MIN_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MIN_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR -= 10.0f;
				m_fHeight -= 3.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_DOWN, 0) == true)
		{
			if (m_fDistance_posVposR >= CAMERA_MAX_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MAX_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR += 10.0f;
				m_fHeight += 3.0f;
			}
		}
	}

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bBeingBoss = false;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_ENEMY)
		{
			CEnemy *pEnemy = NULL;
			pEnemy = (CEnemy*)pObject;

			// エネミーの種類がボスなら
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_BOSS)
			{
				// エネミーの位置を取得
				posEnemy = pEnemy->GetPos();
				bBeingBoss = true;

				// 回転中にする
				m_bRotate = true;
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	
	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// プレイヤーの位置を取得
			posPlayer = pPlayer->GetPos();
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	if (bBeingBoss == true)
	{
		// プレイヤーの位置とエネミーの位置から向きを算出
		m_fObjectiveRot = (float)atan2((posPlayer.x - posEnemy.x), (posPlayer.z - posEnemy.z));
		
		// 回転させる状態なら
		if (m_bRotate == true)
		{
			if (m_rot.y < 0.0f && -m_rot.y + m_fObjectiveRot > D3DX_PI)
			{
				m_fNumRot = (-D3DX_PI - m_rot.y) + -(D3DX_PI - m_fObjectiveRot);
			}
			else if (m_rot.y >= D3DX_PI / 2.0f && m_rot.y - m_fObjectiveRot > D3DX_PI)
			{
				m_fNumRot = (D3DX_PI - m_rot.y) - (-D3DX_PI - m_fObjectiveRot);
			}
			else
			{
				m_fNumRot = (m_fObjectiveRot - m_rot.y);
			}

			// カメラの向きに加算
			m_rot.y += m_fNumRot * 0.08f;

			// 既定の値に達したら回転をやめる
			if ((m_rot.y - m_fObjectiveRot < 0.001 && m_rot.y - m_fObjectiveRot > -0.001) ||
				(-m_rot.y - m_fObjectiveRot < 0.001 && -m_rot.y - m_fObjectiveRot > -0.001))
			{
				m_bRotate = false;
			}
		}

		//敵の位置をもとに注視点の位置を移動
		m_posR.x = posPlayer.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
		m_posR.z = posPlayer.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
		m_posR.y = posPlayer.y;

		//注視点の位置をもとに視点の位置を移動
		m_posV.x = m_posR.x + sinf(m_rot.y) * (m_fDistance_posVposR);
		m_posV.z = m_posR.z + cosf(m_rot.y) * (m_fDistance_posVposR);
		m_posV.y = m_posR.y + m_fHeight;
	}
	else if (bBeingBoss == false)
	{
		m_rot.y -= D3DX_PI;
	}

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// クリア時の移動処理
//=============================================================================
void CCamera::MoveClear(void)
{
	// 1秒かけてプレイヤーにズームするよう、注視点や視点・高さを移動
	if (m_fDistance_posVposR > CAMERA_CLEAR_V_DISTANCE)
	{
		m_fDistance_posVposR -= m_fSaveDistance_posVposR / 60;
	}
	if (m_fDistance_posVposR <= CAMERA_CLEAR_V_DISTANCE)
	{
		m_fDistance_posVposR = CAMERA_CLEAR_V_DISTANCE;
	}

	if (m_fDistance_PlayerPosR > CAMERA_CLEAR_R_DISTANCE)
	{
		m_fDistance_PlayerPosR -= m_fSaveDistance_PlayerPosR / 60;
	}
	if (m_fDistance_PlayerPosR <= CAMERA_CLEAR_R_DISTANCE)
	{
		m_fDistance_PlayerPosR = CAMERA_CLEAR_R_DISTANCE;
	}

	if(m_fHeight > CAMERA_CLEAR_HEIGHT)
	{
		m_fHeight -= m_fSaveHeight / 60;
	}
	if (m_fHeight <= CAMERA_CLEAR_HEIGHT)
	{
		m_fHeight = CAMERA_CLEAR_HEIGHT;
	}

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

			// プレイヤーの位置を取得
			pos = pPlayer->GetPos();
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	//プレイヤーの位置をもとに注視点の位置を移動
	m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.y = pos.y;

	//注視点の位置をもとに視点の位置を移動
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// 正面の向きを計算する処理
//=============================================================================
D3DXVECTOR3 CCamera::MathFront(void)
{
	// 視点と注視点の位置から正面の向きを計算
	D3DXVECTOR3 posR = GetPosV(), posV = GetPosR();
	float fFrontRot = (float)atan2((posR.x - posV.x), (posR.z - posV.z)) - D3DX_PI;

	return D3DXVECTOR3(0.0f, fFrontRot, 0.0f);
}

//=============================================================================
// カメラの動きの設定処理
//=============================================================================
void CCamera::SetCameraMove(CAMERA_MOVE_TYPE type)
{
	// デバッグ用の移動以外なら
	if (m_type != CAMERA_MOVE_TYPE_DEBUG)
	{
		// カメラの動きを変える
		m_type = type;
	}

	// クリアの動きになったら
	if (type == CAMERA_MOVE_TYPE_CLEAR)
	{
		// 現在のカメラ間の距離や高さを保存
		m_fSaveDistance_PlayerPosR = m_fDistance_PlayerPosR;
		m_fSaveDistance_posVposR = m_fDistance_posVposR;
		m_fSaveHeight = m_fHeight;

		m_fSaveDistance_PlayerPosR -= CAMERA_CLEAR_R_DISTANCE;
		m_fSaveDistance_posVposR -= CAMERA_CLEAR_V_DISTANCE;
		m_fSaveHeight -= CAMERA_CLEAR_HEIGHT;
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグで使う移動処理
//=============================================================================
void CCamera::DebugMove(void)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラ回転 (Ｚキー / Ｃキー または パッド右スティック)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_Z) == true && m_bRotate == false)
	{
		m_rot.y += CAMERA_MAX_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}
	else if (pKeyboard->GetPress(DIK_C) == true && m_bRotate == false)
	{
		m_rot.y -= CAMERA_MAX_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラ移動 (↑キー / ←キー / →キー / ↓キー)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_LEFT) == true)
	{
		//左奥移動
		if (pKeyboard->GetPress(DIK_UP) == true)
		{
			m_posR.x += +cosf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
		}
		//左手前移動
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{
			m_posR.x += +cosf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
		}
		//左移動
		else
		{
			m_posR.x += +cosf(m_rot.y) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_RIGHT) == true)
	{
		//右奥移動
		if (pKeyboard->GetPress(DIK_UP) == true)
		{
			m_posR.x += -cosf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
		}
		//右手前移動
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{
			m_posR.x += -cosf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
		}
		else	//右移動
		{
			m_posR.x += -cosf(m_rot.y) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y) * m_fSpeed;
		}
	}
	//奥移動
	else if (pKeyboard->GetPress(DIK_UP) == true)
	{
		m_posR.x += -sinf(m_rot.y) * m_fSpeed;
		m_posR.z += -cosf(m_rot.y) * m_fSpeed;
	}
	//手前移動
	else if (pKeyboard->GetPress(DIK_DOWN) == true)
	{
		m_posR.x += sinf(m_rot.y) * m_fSpeed;
		m_posR.z += cosf(m_rot.y) * m_fSpeed;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ズームイン / ズームアウト (Uキー / Mキー)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_U) == true)
	{
		if (m_fDistance_posVposR <= CAMERA_V_DISTANCE / 2)
		{
			m_fDistance_posVposR = CAMERA_V_DISTANCE / 2;
		}
		else
		{
			m_fDistance_posVposR -= 5.0f;

			if (m_fHeight <= CAMERA_HEIGHT - 100)
			{
				m_fHeight = CAMERA_HEIGHT - 100;
			}
			else
			{
				m_fHeight -= m_fSpeed;
			}
		}
	}
	else if (pKeyboard->GetPress(DIK_M) == true)
	{
		if (m_fDistance_posVposR >= CAMERA_V_DISTANCE * 4)
		{
			m_fDistance_posVposR = CAMERA_V_DISTANCE * 4;
		}
		else
		{
			m_fDistance_posVposR += 5.0f;

			if (m_fHeight >= CAMERA_HEIGHT * 11)
			{
				m_fHeight = CAMERA_HEIGHT * 11;
			}
			else
			{
				m_fHeight += m_fSpeed;
			}
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 上下移動 (Yキー / Nキー)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下移動
	if (pKeyboard->GetPress(DIK_Y) == true)
	{
		m_posR.y -= m_fSpeed;
	}
	//上移動
	if (pKeyboard->GetPress(DIK_N) == true)
	{
		m_posR.y += m_fSpeed;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 速度変更 (右Shiftキー)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetTrigger(DIK_RSHIFT) == true)
	{
		m_fSpeed += 5.0f;

		if (m_fSpeed > 30.0f)
		{
			m_fSpeed = CAMERA_MOVE_SPEED;
		}
	}

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// プレイヤーの位置まで移動 (Enterキー)
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (pKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				// プレイヤークラスにキャスト
				CPlayer *pPlayer = NULL;
				pPlayer = (CPlayer*)pObject;

				// プレイヤーの位置を取得
				pos = pPlayer->GetPos();

				//プレイヤーの位置をもとに注視点の位置を移動
				m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
				m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
				m_posR.y = pos.y;

				//注視点の位置をもとに視点の位置を移動
				m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
				m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
				m_posV.y = m_posR.y + m_fHeight;
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;
}
#endif