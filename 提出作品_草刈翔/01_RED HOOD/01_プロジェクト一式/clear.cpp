//=============================================================================
// クリア処理 [clear.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "clear.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
#include "data_clear.h"
#include "keyboard.h"
#include "gamepad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLEAR_FANFARE_COUNT		(30)			// クリアファンファーレまでの時間
#define CLEAR_BG_COUNT			(90)			// 背景表示までの時間
#define CLEAR_SCORE_COUNT		(120)			// スコア表示までの時間
#define CLEAR_TRANS_COUNT		(200)			// 画面遷移までの時間
#define CLEAR_TOP_POS_Y			(120.0f)		// トップ文字のY座標位置
#define CLEAR_TOP_SIZE_X		(850.0f)		// トップ文字のサイズX
#define CLEAR_TOP_SIZE_Y		(170.0f)		// トップ文字のサイズY
#define CLEAR_SENTENCE_POS_X	(340.0f)		// クリア文字のX座標位置
#define CLEAR_SENTENCE_POS_Y	(240.0f)		// クリア文字のY座標位置
#define CLEAR_SENTENCE_SIZE_X	(540.0f)		// クリア文字のサイズX
#define CLEAR_SENTENCE_SIZE_Y	(80.0f)			// クリア文字のサイズY
#define CLEAR_NUMBER_POS_X		(660.0f)		// ナンバーのX座標位置
#define CLEAR_NUMBER_SIZE_X		(60.0f)			// ナンバーのサイズX
#define CLEAR_NUMBER_SIZE_Y		(120.0f)		// ナンバーのサイズY
#define CLEAR_RANK_POS_X		(1100.0f)		// ランクのX座標位置
#define CLEAR_RANK_SIZE_X		(240.0f)		// ランクのサイズX
#define CLEAR_RANK_SIZE_Y		(100.0f)		// ランクのサイズY
#define SAVEFILENAME "data/TEXT/clear_save.txt"	// ファイルのパス

//=============================================================================
// コンストラクタ
//=============================================================================
CClear::CClear()
{
	// 変数のクリア
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	m_nAlpha = 0;
	m_nClearCount = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	m_nScore = 0;
	m_bDisplay = false;
	m_phase = CLEAR_FASE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CClear::~CClear()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CClear::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_nAlpha = 0;
	m_nClearCount = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	m_nScore = 0;
	m_bDisplay = false;
	m_phase = CLEAR_FASE_NONE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CClear::Uninit(void)
{
	// UIの終了処理
	for (int nCntUI = 0; nCntUI < MAX_CLEAR_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// ナンバーの破棄
	for (int nCntDigit = 0; nCntDigit < MAX_CLEAR_DIGIT; nCntDigit++)
	{
		for (int nCntUI = 0; nCntUI < MAX_CLEAR_SCORE_UI; nCntUI++)
		{
			if (m_apNumber[nCntDigit][nCntUI] != NULL)
			{
				m_apNumber[nCntDigit][nCntUI]->Uninit();
			}
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CClear::Update(void)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();
	
	//フェード取得処理
	CFade *pFade = CManager::GetFade();

	// カウントを進める
	m_nClearCount++;

	// 画像のアルファ度を変更し、背景とトップ文字をゆっくり表示させる
	for (int nCntClear = 0; nCntClear < 2; nCntClear++)
	{
		if (m_apUI[nCntClear] != NULL)
		{
			if (nCntClear == 0)
			{
				m_nAlpha++;
			}

			int nAlpha = 8 * m_nAlpha;

			if (nAlpha >= 255)
			{
				nAlpha = 255;
			}
			m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, nAlpha));
		}
	}

	switch (m_phase)
	{
	case CLEAR_FASE_NONE:
		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_FANFARE_COUNT)
		{
			// BGMの停止
			pSound->Stop();

			// ゲームクリアSEの再生
			pSound->Play(16);
			pSound->ControllVoice(16, 0.6f);

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TOP;
		}
		break;

	case CLEAR_FASE_TOP:
		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_BG_COUNT)
		{
			// 背景・トップ文字
			m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 31, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, CLEAR_TOP_POS_Y), D3DXVECTOR2(CLEAR_TOP_SIZE_X, CLEAR_TOP_SIZE_Y), 32, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_SCORE;
		}
		break;

	case CLEAR_FASE_SCORE:
		// 入力によって表示を早める
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// スコア表示SEの再生
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// スコア
			m_apUI[2] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[2]->SetTexture(0.0f, 0.0f, 1, 4);

			// ナンバーの生成
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				// スコアを取得
				m_nScore = CManager::GetDataClear()->CDataClear::GetScore();

				m_apNumber[0][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[0][nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TIME;
		}
		break;

	case CLEAR_FASE_TIME:
		// 入力によって表示を早める
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// スコア表示SEの再生
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// タイムボーナス
			m_apUI[3] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 100.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y),33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[3]->SetTexture(0.0f, 1.0f, 1, 4);

			// タイムボーナスを取得
			m_nTimeBonus = CManager::GetDataClear()->CDataClear::GetTimeBonus();

			// ナンバーの生成
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[1][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 100.0f), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[1][nCntNumber]->SetTexture(float(m_nTimeBonus % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_LEVEL;
		}
		break;

	case CLEAR_FASE_LEVEL:
		// 入力によって表示を早める
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// スコア表示SEの再生
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// レベルボーナス
			m_apUI[4] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 200.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[4]->SetTexture(0.0f, 2.0f, 1, 4);

			// レベルボーナスを取得
			m_nLevelBonus = CManager::GetDataClear()->CDataClear::GetLevelBonus();

			// ナンバーの生成
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[2][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 200.0f),D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[2][nCntNumber]->SetTexture(float(m_nLevelBonus % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_TOTAL;
		}
		break;

	case CLEAR_FASE_TOTAL:
		// 入力によって表示を早める
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_SCORE_COUNT;
		}

		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// スコア表示SEの再生
			pSound->Play(31);
			pSound->ControllVoice(31, 0.3f);

			// トータルスコア
			m_apUI[5] = CUI::Create(D3DXVECTOR2(CLEAR_SENTENCE_POS_X, CLEAR_SENTENCE_POS_Y + 300.0f), D3DXVECTOR2(CLEAR_SENTENCE_SIZE_X, CLEAR_SENTENCE_SIZE_Y), 33, D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
			m_apUI[5]->SetTexture(0.0f, 3.0f, 1, 4);

			// スコアを取得
			m_nScore += m_nTimeBonus + m_nLevelBonus;

			// ナンバーの生成
			for (int nCntNumber = 0; nCntNumber < MAX_CLEAR_DIGIT; nCntNumber++)
			{
				m_apNumber[3][nCntNumber] = CUI::Create(D3DXVECTOR2(CLEAR_NUMBER_POS_X + (CLEAR_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (CLEAR_NUMBER_SIZE_X)), CLEAR_SENTENCE_POS_Y + 300.0f), D3DXVECTOR2(CLEAR_NUMBER_SIZE_X, CLEAR_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));
				m_apNumber[3][nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_CLEAR_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_CLEAR_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
			}

			m_nClearCount = 0;
			m_phase = CLEAR_FASE_RANK;
		}
		break;

	case CLEAR_FASE_RANK:
		// まだランクの表示がされていないなら
		if (m_nClearCount < CLEAR_SCORE_COUNT)
		{
			// 入力によって表示を早める
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				m_nClearCount = CLEAR_SCORE_COUNT;
			}
		}

		// 一定の時間が経ったら
		if (m_nClearCount == CLEAR_SCORE_COUNT)
		{
			// テクスチャの種類
			float fTexType = 0.0f;

			// スコアが18000点以上でSランク
			if (m_nScore >= CLEAR_RANK_S)
			{
				fTexType = 0.0f;

				// ランクSのSEを再生
				pSound->Play(32);
				pSound->ControllVoice(32, 0.7f);
			}
			// スコアが13000点以上、18000点未満でAランク
			else if (m_nScore >= CLEAR_RANK_A && m_nScore < CLEAR_RANK_S)
			{
				fTexType = 1.0f;

				// ランクAのSEを再生
				pSound->Play(33);
				pSound->ControllVoice(33, 0.8f);
			}
			// スコアが6000点以上、13000点未満でBランク
			else if (m_nScore >= CLEAR_RANK_B && m_nScore < CLEAR_RANK_A)
			{
				fTexType = 2.0f;

				// ランクBのSEを再生
				pSound->Play(34);
				pSound->ControllVoice(34, 0.8f);
			}
			// スコアが6000点未満でCランク
			else if (m_nScore < CLEAR_RANK_B)
			{
				fTexType = 3.0f;

				// ランクCのSEを再生
				pSound->Play(35);
				pSound->ControllVoice(35, 0.8f);
			}

			// ランク
			m_apUI[6] = CUI::Create(D3DXVECTOR2(CLEAR_RANK_POS_X, CLEAR_SENTENCE_POS_Y + 300.0f),
				D3DXVECTOR2(CLEAR_RANK_SIZE_X, CLEAR_RANK_SIZE_Y), 30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apUI[6]->SetTexture(0.0f, fTexType, 1, 4);
		}
		else if (m_nClearCount == CLEAR_SCORE_COUNT + 50.0f)
		{
			m_nClearCount = 0;
			m_phase = CLEAR_FASE_FADE;
		}
		break;

	case CLEAR_FASE_FADE:
		// 入力によって表示を早める
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			m_nClearCount = CLEAR_TRANS_COUNT;
		}

		// 一定の時間が経ち、フェードしていないなら
		if (m_nClearCount >= CLEAR_TRANS_COUNT && pFade->GetFade() == CFade::FADE_NONE)
		{
			// タイトル画面に遷移
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CClear::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CClear *CClear::Create(void)
{
	// クラスの生成
	CClear *pClear = NULL;
	pClear = new CClear;

	if (pClear != NULL)
	{
		// 初期化処理
		pClear->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pClear;
}