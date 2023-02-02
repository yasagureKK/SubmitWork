//=============================================================================
// リザルト画面 [result.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "billboard.h"
#include "player.h"
#include "motion_player.h"
#include "score.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
	// 変数のクリア
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pBillboard = NULL;
	memset(&m_pPlayer, NULL, sizeof(m_pPlayer));
	memset(&m_pScore, NULL, sizeof(m_pScore));
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_RESULT);
	pSound->ControllVoice(CSound::SOUND_LABEL_RESULT, 0.4f);

	// 背景
	m_pBillboard = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 230.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CTexture::TEX_TYPE_RESULT_BG, PRIORITY_NONE);
	
	//+------------+
	//|  UIの生成  |
	//+------------+
	// つぎへボタン
	m_apUi[0] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_NEXT);

	// リトライボタン
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_RETRY);

	// 帯
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_BAND_POS_Y, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, RESULT_UI_BAND_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_BAND);

	// トップ文字
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(RESULT_UI_TOP_SIZE_X, RESULT_UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RESULT_TOP);

	// 直前のモードが1人用ゲームだったら
	if (CManager::GetBeforeMode() == CManager::MODE_GAME)
	{
		// 文字
		m_apUi[4] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_POSITION_Y, 0.0f),
			D3DXVECTOR3(RESULT_UI_SIZE_X, RESULT_UI_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_RESULT_SENTENCE);

		// スコアの生成
		m_pScore[0] = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 300.0f, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pScore[0]->SetNum(0);

		// プレイヤー
		m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);
		m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_WIN);
	}
	// 直前のモードが2人用ゲームまたはCPU対戦ゲームだったら
	if (CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
	{
		// スコアを保存するための変数
		int n1PScore = 0, n2PScore = 0;

		for (int nCntPlayer = 0; nCntPlayer < RESULT_MAX_PLAYER; nCntPlayer++)
		{
			// スコアの生成
			m_pScore[nCntPlayer] = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 3 * (nCntPlayer + 1) - 250.0f + (nCntPlayer * 140.0f), 300.0f, 0.0f), D3DXVECTOR3(75.0f, 100.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
			m_pScore[nCntPlayer]->SetNum(nCntPlayer);
		}

		// それぞれのスコアを保存
		n1PScore = m_pScore[0]->GetResultScore(0);
		n2PScore = m_pScore[1]->GetResultScore(1);

		// プレイヤー
		m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0, false);
		m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1, true);

		// 1P勝利の時
		if (n1PScore > n2PScore)
		{
			// 1P・2P(/CPU)の表示
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			// 「かち！」の文字を表示
			m_apUi[7] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X + RESULT_UI_1P_SIZE_X / 1.5f, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_WIN_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_WIN);

			// それぞれにモーションを設定
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_WIN);
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_LOSE);
		}
		// 2P勝利の時
		else if (n2PScore > n1PScore)
		{
			// 1P・2Pの表示
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X - 100.0f, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			
			// 「かち！」の文字を表示
			m_apUi[7] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X + RESULT_UI_1P_SIZE_X / 1.5f, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_WIN_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_WIN);

			// それぞれにモーションを設定
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_WIN);
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_LOSE);
		}
		// 引き分けの時
		else if (n1PScore == n2PScore)
		{
			// 1P・2Pの表示
			m_apUi[5] = CUi::Create(D3DXVECTOR3(RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_1P);
			if (CManager::GetBeforeMode() == CManager::MODE_PVP)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_2P);
			}
			else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - RESULT_UI_1P_POS_X, RESULT_UI_1P_POS_Y, 0.0f), D3DXVECTOR3(RESULT_UI_1P_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_CPU);
			}
			
			// 「ひきわけ…」の文字を表示
			m_apUi[7] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RESULT_UI_1P_POS_Y, 0.0f),
				D3DXVECTOR3(RESULT_UI_DRAW_SIZE_X, RESULT_UI_1P_SIZE_Y, 0.0f), PRIORITY_UI, CUi::UI_TYPE_RESULT_DRAW);

			// 両方に負けモーションを設定
			m_pPlayer[0]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_1P_LOSE);
			m_pPlayer[1]->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_2P_LOSE);
		}
	}

	// ボタンは暗くしておく
	m_apUi[0]->SetTexture(0.0f, 0.0f, 1, 2);
	m_apUi[1]->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
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

	// フェードしていない時のみ操作できる
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			//***********************************************************************
			// 次へすすむ (Enter/Space または パッドAボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// ボタンを光らせる
				m_apUi[0]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_RESULT);

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_DECIDE);
				pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

				// ランキング画面に遷移
				pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
			}

			//***********************************************************************
			// リトライ (BackSpace または パッドXボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				// ボタンを光らせる
				m_apUi[1]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_RESULT);

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_DECIDE);
				pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

				//再びゲーム画面に遷移
				// 直前のモードが1人用ゲームだったら
				if (CManager::GetBeforeMode() == CManager::MODE_GAME)
				{
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
				// 直前のモードが2人用ゲームだったら
				else if (CManager::GetBeforeMode() == CManager::MODE_PVP)
				{
					pFade->SetFade(CManager::MODE_PVP, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
				// 直前のモードがCPU対戦ゲームだったら
				else if (CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
				{
					pFade->SetFade(CManager::MODE_SOLO_CPU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}