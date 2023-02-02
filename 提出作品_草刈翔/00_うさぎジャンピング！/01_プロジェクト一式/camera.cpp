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
#include "input_keyboard.h"
#include "input_pad.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 変数のクリア
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの向き
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的のカメラの向き
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 法線
	m_fHeight = 0.0f;								// 視点から注視点の高さ
	m_fRotDiffer = 0.0f;							// 目的と向きの差
	m_fDistance = 0.0f;								// 視点と注視点の距離
	m_viewport.X = (DWORD)0.0f;						// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)0.0f;						// 描画する画面の左上Y座標
	m_viewport.MinZ = 0.0f;							// 描画するz座標の範囲の最小値
	m_viewport.MaxZ = 0.0f;							// 描画するz座標の範囲の最大値
	m_viewport.Width = (DWORD)0.0f;					// 描画する画面の幅
	m_viewport.Height = (DWORD)0.0f;				// 描画する画面の高さ
	m_bTurn = false;								// カメラが回転しているかどうか
	m_nNum = 0;										// カメラの番号
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
	m_rot = rot;									// カメラの向き
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的のカメラの向き
	m_posV = pos;									// 現在の視点
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 法線
													// 視点から注視点の高さ
	if (CManager::GetMode() == CManager::MODE_PVP || CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		m_fHeight = CAMERA_HEIGHT - 20;
	}
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		m_fHeight = CAMERA_HEIGHT - 30;
	}
	else
	{
		m_fHeight = CAMERA_HEIGHT;
	}

	// 目的と向きの差
	m_fRotDiffer = 0.0f;
	
	// 視点と注視点の距離
	if (CManager::GetMode() == CManager::MODE_PVP || CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		m_fDistance = CAMERA_DISTANCE - 125;
	}
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		m_fDistance = CAMERA_DISTANCE - 100;
	}
	else
	{
		m_fDistance = CAMERA_DISTANCE;
	}
	m_viewport.X = (DWORD)viewportX;				// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)viewportY;				// 描画する画面の左上Y座標
	m_viewport.MinZ = 0.0f;							// 描画するz座標の範囲の最小値
	m_viewport.MaxZ = 1.0f;							// 描画するz座標の範囲の最大値
	m_viewport.Width = (DWORD)viewportWidth;		// 描画する画面の幅
	m_viewport.Height = (DWORD)viewporHeight;		// 描画する画面の高さ
	m_bTurn = false;								// カメラが回転しているかどうか

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
	// カメラ移動
	Move();

	// デバッグ用のカメラ移動
	//DebugMove();
}

//=============================================================================
// 生成処理
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY,
	float viewportWidth, float viewporHeight)
{
	// インスタンスの生成
	CCamera *pCamera = NULL;

	// ヌルチェック
	if (pCamera == NULL)
	{
		// クラスの生成
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
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

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
// 視点取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_posV;
}

//=============================================================================
// 注視点取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_posR;
}

//=============================================================================
// 視点設定処理
//=============================================================================
void CCamera::SetPosV(D3DXVECTOR3 pos)
{
	m_posV = pos;
}

//=============================================================================
// 注視点設定処理
//=============================================================================
void CCamera::SetPosR(D3DXVECTOR3 pos)
{
	m_posR = pos;
}


//=============================================================================
// 視点の向き取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRotV(void)
{
	return m_rot;
}

//=============================================================================
// 視点の向き設定処理
//=============================================================================
void CCamera::SetRotV(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// カメラの番号設定処理
//=============================================================================
void CCamera::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// カメラの番号取得処理
//=============================================================================
int CCamera::GetNum(void)
{
	return m_nNum;
}

//=============================================================================
// カメラの移動
//=============================================================================
void CCamera::Move(void)
{
	// キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//パッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

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

			// カメラの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				// プレイヤーの位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				if (CManager::GetMode() == CManager::MODE_RESULT)
				{
					// カメラの注視点をプレイヤーの位置に追従させる
					m_posR = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
				}
				else
				{
					// カメラの注視点をプレイヤーの位置に追従させる
					m_posR = D3DXVECTOR3(pos.x + 100.0f, pos.y + 40.0f, pos.z);
				}

				// プレイヤーの位置＋カメラの高さより少し高めの位置をを視点のY座標に設定
				m_posV.y = pos.y + m_fHeight;
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	//注視点を元に視点の場所を移動
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance;

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ用のカメラの移動
//=============================================================================
void CCamera::DebugMove(void)
{
	// キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//パッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

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

			// カメラの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				// プレイヤーの位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// カメラの注視点をプレイヤーの位置に追従させる
				m_posR = pos;

				// プレイヤーの位置＋カメラの高さを視点のY座標に設定
				D3DXVECTOR3 move = pPlayer->GetMove();
				m_posV.y = pos.y + m_fHeight - 30.0f;
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラ上下移動(視点の位置の移動)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//上移動
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{
		m_fHeight += 3.0f;
	}
	//下移動
	if (pInputKeyboard->GetPress(DIK_N) == true)
	{
		m_fHeight += -3.0f;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラ回転
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pInputKeyboard->GetPress(DIK_Z) == true && m_bTurn == false)
	{
		m_rot.y += CAMERA_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_DISTANCE;
	}
	else if (pInputKeyboard->GetPress(DIK_C) == true && m_bTurn == false)
	{
		m_rot.y -= CAMERA_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_DISTANCE;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 視点と注視点の距離
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		m_fDistance -= 4.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{
		m_fDistance += 4.0f;
	}

	//注視点を元に視点の場所を移動
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance;

	// 回転中なら
	if (m_bTurn == true)
	{
		// 目的の向きを求める
		if (m_rotAsk.y > D3DX_PI)
		{
			m_rotAsk.y = -D3DX_PI - (D3DX_PI - m_rotAsk.y);
		}
		else if (m_rotAsk.y < -D3DX_PI)
		{
			m_rotAsk.y = D3DX_PI - (-D3DX_PI - m_rotAsk.y);
		}

		// 目的の向きとの差を求める
		if (m_rot.y < 0.0f && -m_rot.y + m_rotAsk.y > D3DX_PI)
		{
			m_fRotDiffer = (-D3DX_PI - m_rot.y) + -(D3DX_PI - m_rotAsk.y);
		}
		else if (m_rot.y >= D3DX_PI / 2.0f && m_rot.y - m_rotAsk.y > D3DX_PI)
		{
			m_fRotDiffer = (D3DX_PI - m_rot.y) - (-D3DX_PI - m_rotAsk.y);
		}
		else
		{
			m_fRotDiffer = (m_rotAsk.y - m_rot.y);
		}

		// 目的の向きとの差だけ回転させる
		m_rot.y += m_fRotDiffer * 0.1f;
	}

	// 向きを3.14から-3.14の値の範囲内に収める
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}
#endif