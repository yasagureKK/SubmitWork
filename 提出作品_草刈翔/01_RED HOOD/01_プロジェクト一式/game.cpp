//=============================================================================
// ゲーム画面 [game.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "light.h"
#include "player.h"
#include "data_stage.h"
#include "enemy.h"
#include "enemy_box.h"
#include "pause.h"
#include "sound.h"
#include "fog.h"
#include "score.h"
#include "time.h"
#include "clear.h"
#include "start.h"
#include "keyboard.h"
#include "collision_sphere.h"
#include "data_clear.h"
#include "camera.h"
#include "particle_hit.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CGame::m_bDefeatBoss = false;
bool CGame::m_bBossBattle = false;
bool CGame::m_bClear = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	// 変数のクリア
	m_pPlayer = NULL;
	m_pDataStage = NULL;
	memset(&m_pEnemyBox, NULL, sizeof(m_pEnemyBox));
	m_pPause = NULL;
	m_pFog = NULL;
	m_pScore = NULL;
	m_pTime = NULL;
	memset(&m_apLight, NULL, sizeof(m_apLight));
	m_pStart = NULL;
	m_bBossBattle = false;
	m_bDefeatBoss = false;
	m_bClear = false;
	nCntGame = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_bBossBattle = false;
	m_bDefeatBoss = false;
	m_bClear = false;
	nCntGame = 0;

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// ゲームBGMの再生
	pSound->Play(1);
	pSound->ControllVoice(1, 0.3f);

	// カメラを操作不可能にする
	CCamera::SetControl(false);

	// スコアの初期化
	CManager::GetDataClear()->SetScore(0);

	// ライトの生成
	m_apLight[0] = CLight::Create(D3DXVECTOR3(0.3f, -0.8f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[1] = CLight::Create(D3DXVECTOR3(-0.7f, -0.8f, -0.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[2] = CLight::Create(D3DXVECTOR3(0.1f, -0.8f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[3] = CLight::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// フォグの生成
	m_pFog = CFog::Create(D3DCOLOR_RGBA(0, 0, 0, 255), 0.5f, 3700.0f);

	// ステージデータの生成
	m_pDataStage = CDataStage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CDataStage::STAGE_TYPE_GAME);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	// エネミーボックスの生成
	m_pEnemyBox[0] = CEnemyBox::Create(D3DXVECTOR3(-3000.0f, 0.0f, 3100.0f), D3DXVECTOR3(800.0f, 550.0f, 800.0f), 0);
	m_pEnemyBox[1] = CEnemyBox::Create(D3DXVECTOR3(5100.0f, 0.0f, 4700.0f), D3DXVECTOR3(700.0f, 500.0f, 700.0f), 1);
	m_pEnemyBox[2] = CEnemyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 6300.0f), D3DXVECTOR3(800.0f, 500.0f, 800.0f), 2);
	m_pEnemyBox[3] = CEnemyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 10000.0f), D3DXVECTOR3(1500.0f, 500.0f, 1500.0f), 3);
	
	// ポーズの生成
	m_pPause = CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ポーズを不可能にする
	CPause::SetPossiblePause(false);

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR2(70.0f, 145.0f), D3DXVECTOR2(130.0f, 35.0f));

	// タイムの生成
	m_pTime = CTime::Create(D3DXVECTOR2(1100.0f, 40.0f), D3DXVECTOR2(120.0f, 55.0f));
	m_pTime->SetTimeStop(true);

	// スタートUIの生成
	m_pStart = CUIStart::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(800.0f, 200.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// ライトの破棄
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		if (m_apLight[nCntLight] != NULL)
		{
			m_apLight[nCntLight]->Uninit();
			delete m_apLight[nCntLight];
			m_apLight[nCntLight] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	if (m_pStart != NULL &&  m_pStart->GetStartMoment() == true)
	{
		//サウンド取得処理
		CSound *pSound = CManager::GetSound();

		// スタートSEの再生
		pSound->Play(30);
		pSound->ControllVoice(30, 0.6f);

		// ポーズを可能にする
		CPause::SetPossiblePause(true);

		// プレイヤーを操作可能にする
		m_pPlayer->SetCanControl(true);

		// カメラを操作可能にする
		CCamera::SetControl(true);

		if (m_pTime != NULL)
		{
			// タイムを進ませる
			m_pTime->SetTimeStop(false);
		}
	}

	// プレイヤーのライフが0になったら
	if (m_pPlayer->GetDeath() == true)
	{
		if (m_pTime != NULL)
		{
			// タイムを止める
			m_pTime->SetTimeStop(true);
		}
	}

	// ボスとの戦いが始まったら
	if (m_bBossBattle == true)
	{
		// カメラの動きをボス戦の動きに設定
		CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_BOSS);
	}
	
	// ボスを倒したら
	if (m_bDefeatBoss == true)
	{
		if (nCntGame == 0)
		{
			// カメラの動きをプレイヤー追従に戻す
			CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_PLAYER);

			if (m_pTime != NULL)
			{
				// タイムを止める
				m_pTime->SetTimeStop(true);
			}
		}

		// カウントを進める
		nCntGame++;

		// 2秒経過したら
		if (nCntGame == 120)
		{
			if (m_pTime != NULL)
			{
				// タイムとプレイヤーのレベルを取得
				int nTime = m_pTime->GetTime();
				int nLevel = m_pPlayer->GetLevel();

				// クリア時のデータからスコアを設定
				CManager::GetDataClear()->SetNewScore(nTime, nLevel);
			}

			// クリアの状態にする
			m_bClear = true;

			// カメラの動きをクリア時の動きにする
			CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_CLEAR);

			// ポーズを不可能にする
			CPause::SetPossiblePause(false);

			// スコアのUIを消す
			if (m_pScore != NULL)
			{
				m_pScore->Uninit();
				delete m_pScore;
				m_pScore = NULL;
			}
			
			// タイムのUIを消す
			if (m_pTime != NULL)
			{
				m_pTime->Uninit();
				delete m_pTime;
				m_pTime = NULL;
			}

			// クリア画面の生成
			CClear::Create();
		}
	}

#ifdef _DEBUG
	//キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_F2) == true)
	{
		CCollisionSphere::SetVisual();
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CGame *CGame::Create(void)
{
	// クラスの生成
	CGame *pGame = NULL;
	pGame = new CGame;

	if (pGame != NULL)
	{
		// 初期化処理
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pGame;
}

//=============================================================================
// プレイヤー取得処理
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// ボス戦かどうか設定処理
//=============================================================================
void CGame::SetBossBattle(bool bDefeat)
{
	m_bBossBattle = bDefeat;

	// ボス戦フラグがtrueになったら
	if (m_bBossBattle == true)
	{
		if (m_pFog != NULL)
		{
			// 霧の範囲を調整
			m_pFog->SetFogLine(1500.0f, 5000.0f);
		}

		//サウンド取得処理
		CSound *pSound = CManager::GetSound();

		// ゲームBGMの停止
		pSound->Stop(1);

		// ボスBGMの再生
		pSound->Play(2);
		pSound->ControllVoice(2, 0.3f);
	}
}