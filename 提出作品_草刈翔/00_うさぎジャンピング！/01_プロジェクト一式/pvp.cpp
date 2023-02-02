//=============================================================================
// 2人用ゲーム画面 [pvp.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "pvp.h"
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
#include "scene2D.h"
#include "stage_data.h"
#include "game_signboard.h"
#include "billboard_moon.h"
#include "input_keyboard.h"
#include "sound.h"
#include "control_ui_game_countdown.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPvP::CPvP()
{
	// 変数のクリア
	memset(&m_apUi, NULL, sizeof(m_apUi));
	memset(&m_apPlayer, NULL, sizeof(m_apPlayer));
	memset(&m_pMeshField, NULL, sizeof(m_pMeshField));
	m_pMeshSphere = NULL;
	memset(&m_pMeshCylinder, NULL, sizeof(m_pMeshCylinder));
	memset(&m_pMeshWall, NULL, sizeof(m_pMeshWall));
	memset(&m_pScore, NULL, sizeof(m_pScore));
	memset(&m_pJumpGuide, NULL, sizeof(m_pJumpGuide));
	m_pPause = NULL;
	m_pScene2D = NULL;
	memset(&m_pStageData, NULL, sizeof(m_pStageData));
	memset(&m_apGameSignboard, NULL, sizeof(m_apGameSignboard));
	memset(&m_apBillboardMoon, NULL, sizeof(m_apBillboardMoon));
	m_phase = PVP_PHASE_NONE;
	m_nInterval = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPvP::~CPvP()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPvP::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
	m_apPlayer[0] = CPlayer::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);
	
	if (CManager::GetMode() == CManager::MODE_PVP)
	{
		m_apPlayer[1] = CPlayer::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, PVP_2P_START), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1, false);
	}
	else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		// CPUを生成
		m_apPlayer[1] = CPlayer::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, PVP_2P_START), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1, true);
	}
	
	//+------------------------------------------------+
	//| メッシュフィールドの生成・テクスチャの割り当て |
	//+------------------------------------------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		// 草原
		m_pMeshField[nCntPlayer][0] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
		m_pMeshField[nCntPlayer][0]->SetNum(nCntPlayer);
		m_pMeshField[nCntPlayer][1] = CMeshField::Create(D3DXVECTOR3(GAME_PLAYER_START + 2000.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
		m_pMeshField[nCntPlayer][1]->SetNum(nCntPlayer);
		m_pMeshField[nCntPlayer][2] = CMeshField::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
		m_pMeshField[nCntPlayer][2]->SetNum(nCntPlayer);
		m_pMeshField[nCntPlayer][3] = CMeshField::Create(D3DXVECTOR3(3000.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20, 5, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD00);
		m_pMeshField[nCntPlayer][3]->SetNum(nCntPlayer);
		// 土；スタート
		m_pMeshField[nCntPlayer][4] = CMeshField::Create(D3DXVECTOR3(-119.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshField::MESHFIELD_TYPE_NONE, CTexture::TEX_TYPE_FIELD01);
		// 土
		m_pMeshField[nCntPlayer][5] = CMeshField::Create(D3DXVECTOR3(980.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);
		m_pMeshField[nCntPlayer][5]->SetNum(nCntPlayer);
		m_pMeshField[nCntPlayer][6] = CMeshField::Create(D3DXVECTOR3(2980.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 1, CMeshField::MESHFIELD_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_FIELD02);
		m_pMeshField[nCntPlayer][6]->SetNum(nCntPlayer);
	}

	//+------------------------------------------------+
	//| メッシュシリンダーの生成・テクスチャの割り当て |
	//+------------------------------------------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		//※PNG画像の仕様上、外側から描画させる
		// 空の色
		m_pMeshCylinder[nCntPlayer][0] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(6500.0f, 10000.0f, 6500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 8,
			D3DCOLOR_RGBA(180, 240, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_BACK_COLOR, CTexture::TEX_TYPE_CLOUD00);
		m_pMeshCylinder[nCntPlayer][0]->SetNum(nCntPlayer);
		// 雲
		m_pMeshCylinder[nCntPlayer][1] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(6000.0f, 4000.0f, 6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
			D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.0005f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD00);
		m_pMeshCylinder[nCntPlayer][1]->SetNum(nCntPlayer);
		m_pMeshCylinder[nCntPlayer][2] = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(5000.0f, 3500.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, 8,
			D3DCOLOR_RGBA(255, 255, 255, 255), false, 0.001f, CMeshCylinder::CYLINDER_TYPE_NONE, CTexture::TEX_TYPE_CLOUD01);
		m_pMeshCylinder[nCntPlayer][2]->SetNum(nCntPlayer);
	}

	//+----------------------------------------------+
	//| メッシュウォールの生成・テクスチャの割り当て |
	//+----------------------------------------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		//※一番内側になるPNG画像のため最後に描画
		// 山
		m_pMeshWall[nCntPlayer][0] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -450.0f, 500.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
		m_pMeshWall[nCntPlayer][0]->SetNum(nCntPlayer);
		m_pMeshWall[nCntPlayer][1] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 20000.0f, -450.0f, 500.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(20000.0f, 2000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_00);
		m_pMeshWall[nCntPlayer][1]->SetNum(nCntPlayer);
		// 山
		m_pMeshWall[nCntPlayer][2] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -350.0f, 400.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
		m_pMeshWall[nCntPlayer][2]->SetNum(nCntPlayer);
		m_pMeshWall[nCntPlayer][3] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 15000.0f, -350.0f, 400.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(15000.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_MOUNTAIN_01);
		m_pMeshWall[nCntPlayer][3]->SetNum(nCntPlayer);
		// 森
		m_pMeshWall[nCntPlayer][4] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START, -10.0f, 200.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);
		m_pMeshWall[nCntPlayer][4]->SetNum(nCntPlayer);
		m_pMeshWall[nCntPlayer][5] = CMeshWall::Create(D3DXVECTOR3(GAME_PLAYER_START + 6400.0f, -10.0f, 200.0f + nCntPlayer * (PVP_2P_START / 2)), D3DXVECTOR3(6400.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 2, CMeshWall::MESHWALL_TYPE_RIGHT_LOOP, CTexture::TEX_TYPE_WALL_FOREST);
		m_pMeshWall[nCntPlayer][5]->SetNum(nCntPlayer);
	}

	//+------------+
	//| 看板の生成 |
	//+------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		for (int nCntGameObj = 0; nCntGameObj < MAX_PLAYER_NUM; nCntGameObj++)
		{
			if (m_apGameSignboard[nCntPlayer][nCntGameObj] == NULL)
			{
				m_apGameSignboard[nCntPlayer][nCntGameObj] = CGameSignboard::Create(D3DXVECTOR3(nCntGameObj * 2000.0f, 0.0f, 80.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(2000.0f, 0.0f, 0.0f), nCntPlayer);
			}
		}
	}

	//+----------------------+
	//| ビルボード(月)の生成 |
	//+----------------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		m_apBillboardMoon[nCntPlayer] = CBillboardMoon::Create(D3DXVECTOR3(GAME_PLAYER_START, 5000.0f, 2000.0f + nCntPlayer * PVP_2P_START), D3DXVECTOR3(1200.0f, 1200.0f, 0.0f), nCntPlayer);
	}

	//+------------------------+
	//| 背景オブジェクトの生成 |
	//+------------------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		for (int nCntStageData = 0; nCntStageData < MAX_PLAYER_NUM; nCntStageData++)
		{
			if (m_pStageData[nCntPlayer][nCntStageData] == NULL)
			{
				m_pStageData[nCntPlayer][nCntStageData] = CStageData::Create(D3DXVECTOR3(GAME_PLAYER_START + nCntStageData * 1200.0f, 0.0f, nCntPlayer * PVP_2P_START), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), nCntPlayer);
			}
		}
	}

	/*************************/
	/**			UI系		**/
	/*************************/
	//+--------------+
	//| 画面分割の堺 |
	//+--------------+
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, 10.0f, 0.0f), PRIORITY_UI);
	m_pScene2D->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));

	//+--------------+
	//| スコアの生成 |
	//+--------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		// プレイヤーのスコア
		m_pScore[nCntPlayer] = CScore::Create(D3DXVECTOR3(GAME_UI_SCORE_POS_X, GAME_UI_SCORE_POS_Y + nCntPlayer * (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pScore[nCntPlayer]->SetNum(nCntPlayer);
	}
	// ハイスコア
	m_pScore[2] = CScore::Create(D3DXVECTOR3(GAME_UI_HIGHSCORE_POS_X, GAME_UI_HIGHSCORE_POS_Y, 0.0f), D3DXVECTOR3(45.0f, 60.0f, 0.0f), D3DCOLOR_RGBA(255, 90, 255, 255));

	// スコア文字
	m_apUi[0] = CUi::Create(D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_POS_X + 180.0f, GAME_UI_SCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_SIZE_X, GAME_UI_SCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_SCORE);
	m_apUi[1] = CUi::Create(D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_POS_X + 180.0f, GAME_UI_SCORE_SENTENCE_POS_Y + SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(GAME_UI_SCORE_SENTENCE_SIZE_X, GAME_UI_SCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_SCORE);
	// 1P・2P(CPU)の表示
	m_apUi[2] = CUi::Create(D3DXVECTOR3(PVP_UI_1P_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(PVP_UI_1P_SIZE_X, PVP_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
	
	if (CManager::GetMode() == CManager::MODE_PVP)
	{
		m_apUi[3] = CUi::Create(D3DXVECTOR3(PVP_UI_1P_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y + SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(PVP_UI_1P_SIZE_X, PVP_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
	}
	else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		m_apUi[3] = CUi::Create(D3DXVECTOR3(PVP_UI_1P_POS_X, GAME_UI_SCORE_SENTENCE_POS_Y + SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(PVP_UI_1P_SIZE_X, PVP_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
	}
	
	// ハイスコア文字
	m_apUi[4] = CUi::Create(D3DXVECTOR3(GAME_UI_HIGHSCORE_SENTENCE_POS_X, GAME_UI_HIGHSCORE_SENTENCE_POS_Y, 0.0f), D3DXVECTOR3(GAME_UI_HIGHSCORE_SENTENCE_SIZE_X, GAME_UI_HIGHSCORE_SENTENCE_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_HIGHSCORE);

	//+--------------+
	//| ガイドの生成 |
	//+--------------+
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		m_pJumpGuide[nCntPlayer] = CJumpGuide::Create(D3DXVECTOR3(GAME_UI_JUMP_GUIDE_POS_X, GAME_UI_JUMP_GUIDE_POS_Y + nCntPlayer * (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(60.0f, 80.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pJumpGuide[nCntPlayer]->SetNum(nCntPlayer);
	}

	//+--------------+
	//| ポーズの生成 |
	//+--------------+
	m_pPause = CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPvP::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPvP::Update(void)
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

//================================================
// 描画処理
//================================================
void CPvP::Draw(void)
{

}

//=============================================================================
// フェイズ設定処理
//=============================================================================
void CPvP::SetPhase(PVP_PHASE phase)
{
	m_phase = phase;
}

//=============================================================================
// フェイズ取得処理
//=============================================================================
CPvP::PVP_PHASE CPvP::GetPhase(void)
{
	return m_phase;
}

//=============================================================================
// フェーズ処理
//=============================================================================
void CPvP::Phase(void)
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
	case PVP_PHASE_NONE:
		// フェード中じゃないなら
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//+----------------------+
			//| カウントダウンの生成 |
			//+----------------------+
			m_apUi[5] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(GAME_UI_COUNTDOWN_SIZE_X, GAME_UI_COUNTDOWN_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_COUNTDOWN);
			
			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_COUNTDOWN);
			pSound->ControllVoice(CSound::SOUND_LABEL_COUNTDOWN, 0.4f);

			// カウントダウンを始める
			m_phase = PVP_PHASE_COUNTDOWN;
		}
		break;

	case PVP_PHASE_COUNTDOWN:
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
			// フェーズ切り替え
			m_phase = PVP_PHASE_ACTION;
			m_nInterval = 0;
		}
		break;

	case PVP_PHASE_ACTION:
		if (m_nInterval == 0)
		{
			// モードによって違う曲を流す
			if (CManager::GetMode() == CManager::MODE_PVP)
			{
				// BGMの再生
				pSound->Play(CSound::SOUND_LABEL_PVP);
				pSound->ControllVoice(CSound::SOUND_LABEL_PVP, 0.4f);
			}
			else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
			{
				// BGMの再生
				pSound->Play(CSound::SOUND_LABEL_CPU);
				pSound->ControllVoice(CSound::SOUND_LABEL_CPU, 0.4f);
			}
		}

		//カウントを増やす
		m_nInterval++;

		// 少し経ったら
		if (m_nInterval >= 60)
		{
			// カウントダウンのUIを消す
			if (m_apUi[5] != NULL)
			{
				m_apUi[5]->Uninit();
				delete m_apUi[5];
				m_apUi[5] = NULL;
			}
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
		{
			// プレイヤーを動かせるようにする
			m_apPlayer[nCntPlayer]->SetStop(false);
		}

		// 両方のプレイヤーが計測し終わったら
		if (m_apPlayer[0]->GetLandFinish() == true &&
			m_apPlayer[1]->GetLandFinish() == true)
		{
			// フェーズ切り替え
			m_phase = PVP_PHASE_ACTION_END;
			m_nInterval = 0;
		}
		break;

	case PVP_PHASE_ACTION_END:
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
		{
			// プレイヤーを動かせないようにする
			m_apPlayer[nCntPlayer]->SetStop(true);
		}

		//カウントを増やす
		m_nInterval++;

		// 少し経ったら
		if (m_nInterval >= 50)
		{
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				// BGMの停止
				// モードによって違う曲を流す
				if (CManager::GetMode() == CManager::MODE_PVP)
				{
					pSound->Stop(CSound::SOUND_LABEL_PVP);
				}
				else if (CManager::GetMode() == CManager::MODE_SOLO_CPU)
				{
					pSound->Stop(CSound::SOUND_LABEL_CPU);
				}

				// リザルト画面に遷移
				pFade->SetFade(CManager::MODE_RESULT, D3DCOLOR_RGBA(0, 0, 0, 0));

				for (int nCntScore = 0; nCntScore < MAX_PLAYER_NUM; nCntScore++)
				{
					// プレイヤー毎のスコアを取得
					int nScore = m_pScore[nCntScore]->GetScore();

					// CPUじゃなかったらスコアを記録
					if (m_apPlayer[nCntScore]->GetCpu() == false)
					{
						// スコアのロード
						m_pScore[nCntScore]->LoadScore();

						// ランキングの設定
						m_pScore[nCntScore]->SetRankScore(nScore);

						// スコアのセーブ
						m_pScore[nCntScore]->SaveScore();
					}

					// リザルト用のスコアを設定
					m_pScore[nCntScore]->SetResultScore(nCntScore);
				}
			}
		}
		break;

	default:
		break;
	}
}