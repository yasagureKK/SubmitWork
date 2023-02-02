//=============================================================================
// ゲーム画面 [game.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
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
#include "pause.h"
#include "stage_data.h"
#include "game_signboard.h"
#include "billboard_moon.h"
#include "input_keyboard.h"
#include "sound.h"
#include "control_ui_game_countdown.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
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
	m_pPause = NULL;
	memset(&m_pStageData, NULL, sizeof(m_pStageData));
	memset(&m_pGameSignboard, NULL, sizeof(m_pGameSignboard));
	m_pBillboardMoon = NULL;
	m_phase = GAME_PHASE_NONE;
	m_nInterval = 0;
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
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_nInterval = 0;

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

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
		D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.001f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD01);

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
	m_pBillboardMoon = CBillboardMoon::Create(D3DXVECTOR3(GAME_PLAYER_START, 5000.0f, 2000.0f), D3DXVECTOR3(600.0f, 600.0f, 0.0f), 0);

	/*************************/
	/**			UI系		**/
	/*************************/
	//+--------------+
	//| スコアの生成 |
	//+--------------+
	// プレイヤーのスコア
	m_pScore[0] = CScore::Create(D3DXVECTOR3(GAME_UI_SCORE_POS_X, GAME_UI_SCORE_POS_Y, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pScore[0]->SetNum(0);
	// ハイスコア
	m_pScore[1] = CScore::Create(D3DXVECTOR3(GAME_UI_HIGHSCORE_POS_X, GAME_UI_HIGHSCORE_POS_Y, 0.0f), D3DXVECTOR3(45.0f, 60.0f, 0.0f), D3DCOLOR_RGBA(255, 90, 255, 255));

	// スコア文字
	m_apUi[0] = CUi::Create(D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_SIZE_X, GAME_UI_SCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_SCORE);
	// ハイスコア文字
	m_apUi[1] = CUi::Create(D3DXVECTOR3(GAME_UI_HIGHSCORE_SENTENCE_POS_X, GAME_UI_HIGHSCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_HIGHSCORE_SENTENCE_SIZE_X, GAME_UI_HIGHSCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_HIGHSCORE);

	//+--------------+
	//| ガイドの生成 |
	//+--------------+
	m_pJumpGuide = CJumpGuide::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 70.0f, GAME_UI_JUMP_GUIDE_POS_Y , 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));

	//+--------------+
	//| ポーズの生成 |
	//+--------------+
	m_pPause = CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	//フェード取得処理
	CFade *pFade;
	pFade = CManager::GetFade();

	// フェイズ処理(処理は関数の中)
	Phase();
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// フェイズ設定処理
//=============================================================================
void CGame::SetPhase(GAME_PHASE phase)
{
	m_phase = phase;
}

//=============================================================================
// フェイズ取得処理
//=============================================================================
CGame::GAME_PHASE CGame::GetPhase(void)
{
	return m_phase;
}

//=============================================================================
// フェーズ処理
//=============================================================================
void CGame::Phase(void)
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
	case GAME_PHASE_NONE:
		// フェード中じゃないなら
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//+----------------------+
			//| カウントダウンの生成 |
			//+----------------------+
			m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(GAME_UI_COUNTDOWN_SIZE_X, GAME_UI_COUNTDOWN_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_COUNTDOWN);

			// カウントダウンを始める
			m_phase = GAME_PHASE_COUNTDOWN;
		}
		break;

	case GAME_PHASE_COUNTDOWN:
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
			m_phase = GAME_PHASE_ACTION;
			m_nInterval = 0;
		}
		break;

	case GAME_PHASE_ACTION:
		if (m_nInterval == 0)
		{
			// BGMの再生
			pSound->Play(CSound::SOUND_LABEL_GAME);
			pSound->ControllVoice(CSound::SOUND_LABEL_GAME, 0.4f);
		}

		// プレイヤーを動かせるようにする
		m_pPlayer->SetStop(false);

		//カウントを増やす
		m_nInterval++;

		// 少し経ったら
		if (m_nInterval >= 60)
		{
			// カウントダウンのUIを消す
			if (m_apUi[2] != NULL)
			{
				m_apUi[2]->Uninit();
				delete m_apUi[2];
				m_apUi[2] = NULL;
			}
		}

		// 計測が終わったら
		if (m_pPlayer->GetLandFinish() == true)
		{
			// フェーズを切り替え
			m_phase = GAME_PHASE_ACTION_END;
			m_nInterval = 0;
		}
		break;

	case GAME_PHASE_ACTION_END:
		// プレイヤーを動かせないようにする
		m_pPlayer->SetStop(true);

		//カウントを増やす
		m_nInterval++;

		// 少し経ったら
		if (m_nInterval >= 50)
		{
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_GAME);

				// リザルト画面に遷移
				pFade->SetFade(CManager::MODE_RESULT, D3DCOLOR_RGBA(0, 0, 0, 0));

				int nScore = m_pScore[0]->GetScore();

				// スコアのロード
				m_pScore[0]->LoadScore();

				// ランキングの設定
				m_pScore[0]->SetRankScore(nScore);

				// スコアのセーブ
				m_pScore[0]->SaveScore();

				// リザルト用のスコアを設定
				m_pScore[0]->SetResultScore(0);
			}
		}
		break;

	default:
		break;
	}
}