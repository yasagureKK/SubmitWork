//=============================================================================
// チュートリアル画面 [tutorial.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "tutorial.h"
#include "game.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "model_single.h"
#include "player.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_cylinder.h"
#include "score.h"
#include "jump_guide.h"
#include "tutorial_guide.h"
#include "stage_data.h"
#include "game_signboard.h"
#include "billboard_moon.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"
#include "control_ui_game_countdown.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CTutorial::m_bStop = false;
bool CTutorial::m_bExplanation = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
	// 変数のクリア
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pPlayer = NULL;
	memset(&m_pMeshField, NULL, sizeof(m_pMeshField));
	m_pMeshSphere = NULL;
	memset(&m_pMeshCylinder, NULL, sizeof(m_pMeshCylinder));
	memset(&m_pMeshWall, NULL, sizeof(m_pMeshWall));
	memset(&m_pScore, NULL, sizeof(m_pScore));
	m_pJumpGuide = NULL;
	m_pTutorialGuide = NULL;
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	m_pTutorialPause = NULL;
	memset(&m_pStageData, NULL, sizeof(m_pStageData));
	memset(&m_pGameSignboard, NULL, sizeof(m_pGameSignboard));
	m_pBillboardMoon = NULL;
	m_phase = TUTORIAL_PHASE_NONE;
	m_nInterval = 0;
	m_nTime = 0;
	m_nPlayerCommandCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_nInterval = 0;
	m_nTime = 0;

	// 前のモードがメニュー画面だったら
	if (CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		m_bExplanation = true;
	}
	else
	{
		m_bExplanation = false;
	}

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_TUTORIAL);
	pSound->ControllVoice(CSound::SOUND_LABEL_TUTORIAL, 0.4f);

	/*************************/
	/**	  オブジェクト系	**/
	/*************************/
	//+------------------+
	//| プレイヤーの生成 |
	//+------------------+
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);

	//+------------------------------------------------+
	//| メッシュフィールドの生成・テクスチャの割り当て |
	//+------------------------------------------------+
	// 草原
	m_pMeshField[0] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[1] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START + 2000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[2] = CMeshField::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	m_pMeshField[3] = CMeshField::Create(D3DXVECTOR3(3000.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
	// 土；スタート
	m_pMeshField[4] = CMeshField::Create(D3DXVECTOR3(-119.0f, 0.0f, 0.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshField::MESHFIELD_TYPE_NONE, CTexture::TEX_TYPE_FIELD01);
	m_pMeshField[4]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_FIELD01));
	// 土
	m_pMeshField[5] = CMeshField::Create(D3DXVECTOR3(980.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);
	m_pMeshField[6] = CMeshField::Create(D3DXVECTOR3(2980.0f, 0.0f, 0.0f), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);

	//+------------------------------------------------+
	//| メッシュシリンダーの生成・テクスチャの割り当て |
	//+------------------------------------------------+
	//※PNG画像の仕様上、外側から描画させる
	// 空の色
	m_pMeshCylinder[0] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(6500.0f, 10000.0f, 6500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 8,
		D3DCOLOR_RGBA(180, 240, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_BACK_COLOR, CTexture::TEX_TYPE_CLOUD00);
	// 雲
	m_pMeshCylinder[1] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(6000.0f, 4000.0f, 6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
		D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD00);
	m_pMeshCylinder[2] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5000.0f, 3500.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
		D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.001f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD00);

	//+----------------------------------------------+
	//| メッシュウォールの生成・テクスチャの割り当て |
	//+----------------------------------------------+
	// 山
	m_pMeshWall[0] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -450.0f, 600.0f), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
	m_pMeshWall[1] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 20000.0f, -450.0f, 600.0f), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
	// 山
	m_pMeshWall[2] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -350.0f, 500.0f), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
	m_pMeshWall[3] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 15000.0f, -350.0f, 500.0f), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
	// 森
	m_pMeshWall[4] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -10.0f, 200.0f), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);
	m_pMeshWall[5] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 6400.0f, -10.0f, 200.0f), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);

	//+------------+
	//| 看板の生成 |
	//+------------+
	for (int nCntGameObj = 0; nCntGameObj < MAX_PLAYER_NUM; nCntGameObj++)
	{
		if (m_pGameSignboard[nCntGameObj] == NULL)
		{
			m_pGameSignboard[nCntGameObj] = CGameSignboard::Create(D3DXVECTOR3(nCntGameObj * 2000.0f, 0.0f, 80.0f), D3DXVECTOR3(2000.0f, 0.0f, 0.0f), 0);
		}
	}

	//+------------------------+
	//| 背景オブジェクトの生成 |
	//+------------------------+
	for (int nCntStageData = 0; nCntStageData < MAX_PLAYER_NUM; nCntStageData++)
	{
		if (m_pStageData[nCntStageData] == NULL)
		{
			m_pStageData[nCntStageData] = CStageData::Create(D3DXVECTOR3(GAME_PLAYER_START + nCntStageData * 1200.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 0);
		}
	}

	//+----------------------+
	//| ビルボード(月)の生成 |
	//+----------------------+
	m_pBillboardMoon = CBillboardMoon::Create(D3DXVECTOR3(GAME_PLAYER_START, 5000.0f, 2000.0f), D3DXVECTOR3(1200.0f, 1200.0f, 0.0f), 0);

	/*************************/
	/**			UI系		**/
	/*************************/
	//+--------------+
	//| スコアの生成 |
	//+--------------+
	// プレイヤーのスコア
	m_pScore[0] = CScore::Create(D3DXVECTOR3(GAME_UI_SCORE_POS_X, GAME_UI_SCORE_POS_Y, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pScore[0]->SetNum(0);

	// スコア文字
	m_apUi[0] = CUi::Create(D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_SIZE_X, GAME_UI_SCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_SCORE);

	// 直前のモードがメニューまたはチュートリアルの場合のみ
	if (CManager::GetBeforeMode() == CManager::MODE_MENU || CManager::GetBeforeMode() == CManager::MODE_TUTORIAL)
	{
		// 終了の文字
		m_apUi[1] = CUi::Create(D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_POS_X, TUTORIAL_UI_SKIP_SENTENCE_POS_Y, 0.0f),
			D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_SIZE_X, TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_TUTORIAL_END);

		// 説明中の場合のみ
		if (m_bExplanation == true && CManager::GetBeforeMode())
		{
			// スキップの文字
			m_apUi[2] = CUi::Create(D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_POS_X, TUTORIAL_UI_SKIP_SENTENCE_POS_Y + 60.0f, 0.0f),
				D3DXVECTOR3(TUTORIAL_UI_SKIP_SENTENCE_SIZE_X, TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y, 0.0f),
				PRIORITY_UI, CUi::UI_TYPE_TUTORIAL_SKIP);
		}
	}

	//+--------------+
	//| ガイドの生成 |
	//+--------------+
	m_pJumpGuide = CJumpGuide::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 70.0f, GAME_UI_JUMP_GUIDE_POS_Y, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));

	//+----------------------------+
	//| チュートリアルガイドの生成 |
	//+----------------------------+
	m_pTutorialGuide = CTutorialGuide::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// 前のモードがチュートリアルだったら
	if (CManager::GetBeforeMode() == CManager::MODE_TUTORIAL)
	{
		// ページ9を表示させておく
		m_pTutorialGuide->SetPage(10);
	}
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// 終了時に説明をオフにしておく
	m_bExplanation = false;

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド取得処理
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	//フェード取得処理
	CFade *pFade;
	pFade = CManager::GetFade();

	// 説明から始めるなら
	if (m_bExplanation == true)
	{
		// フェードしていないなら
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			// タイムライン(処理は関数の中)
			TimeLine();
		}
	}
	else if (m_bExplanation == false)
	{
		// フェイズ処理(処理は関数の中)
		Phase();
	}

	// フェードしていない時のみ操作できる
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			int nEnter, nBack;

			if (nNum == 0)
			{
				nEnter = DIK_RETURN;
				nBack = DIK_BACK;
			}
			else if (nNum == 1)
			{
				// エラー回避のたｍ、変数を使わない場合には0を割り当てておく
				nEnter = 0;
				nBack = 0;
			}

			// 直前のモードがタイトルの時
			if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
			{
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
				{
					// BGMの停止
					pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

					// タイトル画面に遷移
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			else
			{
				//***********************************************************************
				// スキップ (Enter または パッドSTARTボタン)
				//***********************************************************************
				//※説明中の場合のみ入力できる
				if (m_bExplanation == true)
				{
					if (pInputKeyboard->GetTrigger(nEnter) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
					{
						// BGMの停止
						pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

						// 直前のモードをチュートリアルに設定
						CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

						// チュートリアル画面に遷移
						pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
					}
				}

				//***********************************************************************
				// 終了 (BackSpace または パッドBACKボタン)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(nBack) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_BACK, nNum) == true)
				{
					// BGMの停止
					pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

					// 直前のモードをメニューに設定
					CManager::SetBeforeMode(CManager::MODE_MENU);

					// メニュー画面に遷移
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// フェイズ設定処理
//=============================================================================
void CTutorial::SetPhase(TUTORIAL_PHASE phase)
{
	m_phase = phase;
}

//=============================================================================
// フェイズ取得処理
//=============================================================================
CTutorial::TUTORIAL_PHASE CTutorial::GetPhase(void)
{
	return m_phase;
}

//=============================================================================
// 説明中かどうか取得処理
//=============================================================================
bool CTutorial::GetExplanation(void)
{
	return m_bExplanation;
}

//=============================================================================
// タイムライン管理処理
//=============================================================================
void CTutorial::TimeLine(void)
{
	//フェード取得処理
	CFade *pFade;
	pFade = CManager::GetFade();

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// カウントを増やす
	m_nTime++;

	// 現在説明しているページ数を取得
	int nPage = CTutorialGuide::GetPage();

	//++++++++++++++++++++++++//
	//   説明のページを更新   //
	//++++++++++++++++++++++++//
	if (m_nTime % 240 == 0)
	{
		if (nPage < CTexture::GUIDE_TEX_TYPE_MAX - 1)
		{
			// ページを進める
			m_pTutorialGuide->AdvancePage();
		}
		else if (nPage == CTexture::GUIDE_TEX_TYPE_MAX - 1)
		{
			// ページを進め、メッセージウィンドウを消す
			m_pTutorialGuide->AddPageAndDestroyWindow();
		}
	}

	if (m_nTime >= 240 * 10)
	{
		// 直前のモードがメニュー画面の時
		if (CManager::GetBeforeMode() == CManager::MODE_MENU)
		{
			// 直前のモードをチュートリアルに設定
			CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

			// チュートリアル画面に遷移
			pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
		// 直前のモードがタイトル画面の時
		else if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
		{
			// BGMの停止
			pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

			// タイトル画面に遷移
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}

	//++++++++++++++++++++++++++++//
	//   プレイヤーに動きを指示   //
	//++++++++++++++++++++++++++++//
	// ページ番号が3になったら
	if (nPage == 3)
	{
		// プレイヤーを動かせるようにする
		m_pPlayer->SetStop(false);
	}

	// ページ番号が3～4の間
	if (nPage >= 3 && nPage <= 4)
	{
		m_nPlayerCommandCount++;

		if (m_nPlayerCommandCount >= 15)
		{
			// ダッシュさせる
			m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_DASH);

			m_nPlayerCommandCount = 0;
		}
	}

	if (nPage == 5 && m_nTime % 118 == 0)
	{
		// ジャンプさせる
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_JUMP);
	}

	// ページ番号が6になったら
	if (nPage == 6 && m_nTime % 10 == 0)
	{
		// 2段ジャンプさせる
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_DOUBLE_JUMP);
	}
	
	if ((nPage == 6 && m_nTime % 7 == 0) || (nPage == 7 && m_nTime % 7 == 0))
	{
		// ホバリングさせる
		m_pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_HOVERING);
	}

	// 計測し終わったらプレイヤーを止める
	if (m_pPlayer->GetLandFinish() == true)
	{
		// プレイヤーを動かせないようにする
		m_pPlayer->SetStop(true);
	}
}

//=============================================================================
// フェーズ処理
//=============================================================================
void CTutorial::Phase(void)
{
	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	//フェード取得処理
	CFade *pFade;
	pFade = CManager::GetFade();

	//フェーズによる処理分け
	switch (m_phase)
	{
	case TUTORIAL_PHASE_NONE:
		// 練習中かつフェード中じゃないなら
		if (m_bExplanation == false && pFade->GetFade() == CFade::FADE_NONE)
		{
			//+----------------------+
			//| カウントダウンの生成 |
			//+----------------------+
			m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(GAME_UI_COUNTDOWN_SIZE_X, GAME_UI_COUNTDOWN_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_COUNTDOWN);

			// カウントダウンを始める
			m_phase = TUTORIAL_PHASE_COUNTDOWN;
		}
		break;

	case TUTORIAL_PHASE_COUNTDOWN:
		if (m_nInterval == 0)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);
		}

		//カウントを増やす
		m_nInterval++;

		if (m_nInterval == 30)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);

			CControlUiGameCountdown::SetCount(2);
		}
		else if (m_nInterval == 60)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);

			CControlUiGameCountdown::SetCount(1);
		}
		else if (m_nInterval == 90)
		{
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_START);
			pSound->ControllVoice(CSound::SOUND_LABEL_START, 0.4f);

			CControlUiGameCountdown::SetCount(0);
			// フェーズを切り替え
			m_phase = TUTORIAL_PHASE_ACTION;
			m_nInterval = 0;
		}
		break;

	case TUTORIAL_PHASE_ACTION:
		// プレイヤーを動かせるようにする
		m_pPlayer->SetStop(false);

		//カウントを増やす
		m_nInterval++;

		// 少し経ったら
		if (m_nInterval >= 60)
		{
			// カウントダウンのUIを消す
			if (m_apUi[3] != NULL)
			{
				m_apUi[3]->Uninit();
				delete m_apUi[3];
				m_apUi[3] = NULL;
			}
		}
		if (m_pPlayer->GetLandFinish() == true)
		{
			m_phase = TUTORIAL_PHASE_ACTION_END;
			m_nInterval = 0;
		}
		break;

	case TUTORIAL_PHASE_ACTION_END:
		// プレイヤーを動かせないようにする
		m_pPlayer->SetStop(true);

		//カウントを増やす
		m_nInterval++;

		if (m_nInterval >= 50)
		{
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_TUTORIAL);

				// 直前のモードをチュートリアルに設定
				CManager::SetBeforeMode(CManager::MODE_TUTORIAL);

				// チュートリアル画面に遷移してリトライ
				pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
		break;

	default:
		break;
	}
}