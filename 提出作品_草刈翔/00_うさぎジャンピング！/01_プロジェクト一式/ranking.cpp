//=============================================================================
// ランキング画面 [ranking.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
float CRanking::m_fScroll = 0;
int CRanking::m_nRankingData[MAX_SCORE_DATA] = {};
CScore *CRanking::m_apScore[MAX_SCORE_DATA] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
	// 変数のクリア
	m_fScroll = 0;
	m_nFadeCount = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_fScroll = 0;
	m_nFadeCount = 0;

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_RANKING);
	pSound->ControllVoice(CSound::SOUND_LABEL_RANKING, 0.4f);

	// 背景
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UIの生成  |
	//+------------+
	// トップ
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_TOP);
	// 順位毎の枠
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_ONE);
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 1, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_TWO);
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 2, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_THREE);
	m_apUi[4] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 3, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_FOUR);
	m_apUi[5] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * 4, 0.0f),
		D3DXVECTOR3(RANKING_FRAME_SIZE_X, RANKING_FRAME_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_RANKING_FIVE);

	// 直前のモードがいずれかのゲーム画面だったら
	if (CManager::GetBeforeMode() == CManager::MODE_GAME || CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
	{
		// すすむボタン
		m_apUi[6] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
			D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_NEXT);

		// ボタンは暗くしておく
		m_apUi[6]->SetTexture(0.0f, 0.0f, 1, 2);
	}
	// 直前のモードがメニュー画面だったら
	else if(CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		// もどるボタン
		m_apUi[6] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
			D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_BACK);

		// ボタンは暗くしておく
		m_apUi[6]->SetTexture(0.0f, 0.0f, 1, 2);
	}

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
	{
		D3DXCOLOR col;

		// 1位～3位はカラーを変更
		if (nCntRanking == 0)
		{
			col = D3DCOLOR_RGBA(219, 180, 0, 255);
		}
		else if (nCntRanking == 1)
		{
			col = D3DCOLOR_RGBA(190, 193, 195, 255);
		}
		else if (nCntRanking == 2)
		{
			col = D3DCOLOR_RGBA(196, 112, 34, 255);
		}
		// 4位・5位はカラーを変更
		else
		{
			col = D3DCOLOR_RGBA(255, 200, 60, 255);
		}

		//スコアの生成
		m_apScore[nCntRanking] = CScore::Create(D3DXVECTOR3(RANKING_DATA_POS_X,
			RANKING_FRAME_POS_Y + RANKING_FRAME_SIZE_Y * nCntRanking, 0.0f),
			D3DXVECTOR3(75.0f, 100.0f, 0.0f), col);
	}

	//ロード処理
	FILE *pFile;
	// ファイルを開く
	pFile = fopen(SAVEFILENAME, "r");
	if (pFile != NULL)
	{
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
		{
			// ランキングのデータを変数に保存
			fscanf(pFile, "%d", &m_nRankingData[nCntRanking]);
		}
	}
	// ファイルを閉じる
	fclose(pFile);

	//スコアの設定
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_DATA; nCntRanking++)
	{
		m_apScore[nCntRanking]->SetScore(m_nRankingData[nCntRanking]);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	// 背景スクロールカウントの更新
	m_fScroll += 0.001f;

	// 背景をスクロールさせる
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

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
			// 直前のモードがいずれかのゲーム画面だったら
			if (CManager::GetBeforeMode() == CManager::MODE_GAME || CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
			{
				//***********************************************************************
				// 次へすすむ (Enter/Space または パッドAボタン)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
				{
					// 次へボタンを光らせる
					m_apUi[6]->SetTexture(0.0f, 1.0f, 1, 2);

					// BGMの停止
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// メニュー画面に遷移
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			// 直前のモードがメニュー画面だったら
			else if (CManager::GetBeforeMode() == CManager::MODE_MENU)
			{
				//***********************************************************************
				// 前にもどる (BackSpace または パッドXボタン)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
				{
					// もどるボタンを光らせる
					m_apUi[6]->SetTexture(0.0f, 1.0f, 1, 2);

					// BGMの停止
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// メニュー画面に遷移
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
			// 直前のモードがタイトル画面だったら
			else if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
			{
				// 遷移用カウントの更新
				m_nFadeCount++;

				//***********************************************************************
				// 次へすすむ (Enter/Space または パッドAボタン)
				//***********************************************************************
				// ※時間が経った場合には自動的に遷移
				if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
					pInputKeyboard->GetTrigger(DIK_BACK) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
					|| m_nFadeCount >= RANKING_FADE_COUNT)
				{
					// BGMの停止
					pSound->Stop(CSound::SOUND_LABEL_RANKING);

					// タイトル画面に遷移
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{

}