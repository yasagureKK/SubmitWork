//=============================================================================
// タイトル画面 [title.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "ui.h"
#include "ranking.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CTitle::m_nSelect = 0;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_LOGO_POS_Y		(210.0f)	// タイトルロゴのY座標位置
#define TITLE_LOGO_SIZE_X		(640.0f)	// タイトルロゴのサイズX
#define TITLE_LOGO_SIZE_Y		(330.0f)	// タイトルロゴのサイズY
#define TITLE_PRESS_POS_Y		(520.0f)	// プレス文字のY座標位置
#define TITLE_PRESS_SIZE_X		(860.0f)	// プレス文字のサイズX
#define TITLE_PRESS_SIZE_Y		(120.0f)	// プレス文字のサイズY
#define TITLE_START_POS_Y		(430.0f)	// スタート文字のY座標位置
#define TITLE_START_SIZE_X		(570.0f)	// スタート文字のサイズX
#define TITLE_START_SIZE_Y		(90.0f)		// スタート文字のサイズY
#define TITLE_SELECT_SPACE		(80.0f)		// 選択肢の間隔
#define PAUSE_BACK_POS_X		(240.0f)	// 戻るのX座標位置
#define PAUSE_BACK_POS_Y		(660.0f)	// 戻るのY座標位置
#define PAUSE_BACK_SIZE_X		(430.0f)	// 戻るのサイズX
#define PAUSE_BACK_SIZE_Y		(80.0f)		// 戻るのサイズY
#define PAUSE_PAGINATION_POS_X	(1020.0f)	// ページ送りのX座標位置
#define PAUSE_PAGINATION_POS_Y	(660.0f)	// ページ送りのY座標位置
#define PAUSE_PAGINATION_SIZE_X (490.0f)	// ページ送りのサイズX
#define PAUSE_PAGINATION_SIZE_Y (80.0f)		// ページ送りのサイズY
#define PAUSE_HOWTO_TEXTURE		(41)		// 遊び方のテクスチャ番号

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	// 変数のクリア
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apSelectUI, NULL, sizeof(m_apSelectUI));
	m_pRanking = NULL;
	m_Phase = TITLE_PHASE_PRESS_START;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_nSelect = 0;
	m_nHowtoTex = PAUSE_HOWTO_TEXTURE;
	m_Phase = TITLE_PHASE_PRESS_START;

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// タイトルBGMの再生
	pSound->Play(0);
	pSound->ControllVoice(0, 0.4f);

	// 背景の生成
	CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// タイトルロゴの生成
	CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_LOGO_POS_Y), D3DXVECTOR2(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y), 1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// UIの生成
	UiCreate(m_Phase, false);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// UIの破棄
	for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// 選択肢のUIの破棄
	for (int nCntIcon = 0; nCntIcon < TITLE_MAX_SELECT; nCntIcon++)
	{
		if (m_apSelectUI[nCntIcon] != NULL)
		{
			m_apSelectUI[nCntIcon]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();
	// ゲームパッド取得処理
	CMouse *pMouse = CManager::GetMouse();

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// フェード取得処理
	CFade *pFade = CManager::GetFade();

	// 現在のフェーズで処理を変える
	if (m_Phase == TITLE_PHASE_PRESS_START)
	{
		// フェードしていない時のみ
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// PressStart (キーボードの何らかのキー または パッドの何らかの操作)
			//***********************************************************************
			if (pKeyboard->GetAnyKey() == true ||
				pGamePad->GetAnyButton(0) == true)
			{
				// 決定SEの再生
				pSound->Play(28);
				pSound->ControllVoice(28, 0.4f);

				// フェーズを項目選択にする
				m_Phase = TITLE_PHASE_MODE_SELECT;

				// UIの生成
				UiCreate(m_Phase, false);
			}
		}
	}
	else if (m_Phase == TITLE_PHASE_MODE_SELECT)
	{
		for (int nCntUi = 0; nCntUi < TITLE_MAX_SELECT; nCntUi++)
		{
			if (m_apSelectUI[nCntUi] != NULL)
			{
				// 選択されているUIは
				if (nCntUi == m_nSelect)
				{
					// 透明度を0に
					m_apSelectUI[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

					// サイズを少し大きくする
					D3DXVECTOR3 pos = m_apSelectUI[nCntUi]->GetPos();
					m_apSelectUI[nCntUi]->SetSize(D3DXVECTOR2(TITLE_START_SIZE_X + 40.0f, TITLE_START_SIZE_Y + 10.0f));

				}
				// 選択されていないUIは
				else
				{
					// 透明にする
					m_apSelectUI[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));

					// サイズを戻す
					D3DXVECTOR3 pos = m_apSelectUI[nCntUi]->GetPos();
					m_apSelectUI[nCntUi]->SetSize(D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y));
				}
			}
		}
		// フェードしていない時のみ
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// 選択 (キーボードＷ/Ｓ または パッド左スティック/十字キー)
			//***********************************************************************
			//上
			if (pKeyboard->GetTrigger(DIK_W) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true)
			{
				// 選択SEの再生
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);

				m_nSelect--;

			}
			//下
			else if (pKeyboard->GetTrigger(DIK_S) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN, 0) == true)
			{
				// 選択SEの再生
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);

				m_nSelect++;
			}

			// 選択肢の制限
			if (m_nSelect < 0)
			{
				m_nSelect = TITLE_MAX_SELECT - 1;
			}
			else if (m_nSelect >= TITLE_MAX_SELECT)
			{
				m_nSelect = 0;
			}

			//***********************************************************************
			// 決定 (キーボードEnter/Space または パッドA/STARTボタン)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pKeyboard->GetTrigger(DIK_RETURN) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
			{
				// m_nSelectの値でフェード先を変更
				switch (m_nSelect)
				{
				case 0:
					// サウンドの停止
					pSound->Stop();

					// 決定SEの再生
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// ゲーム画面に遷移
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 1:
					// 決定SEの再生
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// フェーズを操作ガイドにする
					m_Phase = TITLE_PHASE_HOWTO;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						// UIの生成
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						// UIの生成
						UiCreate(m_Phase, true);
					}
					break;

				case 2:
					// 決定SEの再生
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// フェーズを操作ガイドにする
					m_Phase = TITLE_PHASE_GUIDE;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						// UIの生成
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						// UIの生成
						UiCreate(m_Phase, true);
					}
					break;

				case 3:
					// 決定SEの再生
					pSound->Play(28);
					pSound->ControllVoice(28, 0.4f);

					// フェーズをランキングにする
					m_Phase = TITLE_PHASE_RANKING;

					if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
						pKeyboard->GetTrigger(DIK_RETURN) == true)
					{
						UiCreate(m_Phase, false);
					}
					else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
						pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
					{
						UiCreate(m_Phase, true);
					}

					break;

				default:
					break;
				}
			}

			//***********************************************************************
			// 戻る (キーボードBackspace または パッドXボタン または マウス右クリック)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_BACK) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
				pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
			{
				// キャンセルSEの再生
				pSound->Play(29);
				pSound->ControllVoice(29, 0.6f);

				// フェーズをスタート待ちにする
				m_Phase = TITLE_PHASE_PRESS_START;

				// UIの生成
				UiCreate(m_Phase, false);
			}
		}
	}
	else if (m_Phase == TITLE_PHASE_HOWTO)
	{
		//***********************************************************************
		// ページ送り (キーボードＡ/Ｄ または パッド左スティック/十字キー)
		//***********************************************************************
		//左
		if (pKeyboard->GetTrigger(DIK_A) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, 0) == true)
		{
			// テクスチャ番号を減らす
			m_nHowtoTex--;

			if (m_nHowtoTex < PAUSE_HOWTO_TEXTURE)
			{
				m_nHowtoTex = PAUSE_HOWTO_TEXTURE;
			}
			else
			{
				// 選択SEの再生
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);
			}

			// テクスチャを設定
			m_apUI[0]->BindTexture(m_nHowtoTex);
		}
		//右
		else if (pKeyboard->GetTrigger(DIK_D) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, 0) == true)
		{
			// テクスチャ番号を増やす
			m_nHowtoTex++;

			if (m_nHowtoTex >= PAUSE_HOWTO_TEXTURE + 3)
			{
				m_nHowtoTex = PAUSE_HOWTO_TEXTURE + 2;
			}
			else
			{
				// 選択SEの再生
				pSound->Play(27);
				pSound->ControllVoice(27, 0.4f);
			}

			// テクスチャを設定
			m_apUI[0]->BindTexture(m_nHowtoTex);
		}
	}
	
	if (m_Phase == TITLE_PHASE_HOWTO || m_Phase == TITLE_PHASE_GUIDE || m_Phase == TITLE_PHASE_RANKING )
	{
		//***********************************************************************
		// 戻る (キーボードBackspace または パッドXボタン または マウス右クリック)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_BACK) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
			pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
		{
			// キャンセルSEの再生
			pSound->Play(29);
			pSound->ControllVoice(29, 0.6f);

			for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
			{
				if (m_apUI[nCntUI] != NULL)
				{
					m_apUI[nCntUI]->Uninit();
					delete m_apUI[nCntUI];
					m_apUI[nCntUI] = NULL;
				}
			}

			if (m_pRanking != NULL)
			{
				m_pRanking->Uninit();
				delete m_pRanking;
				m_pRanking = NULL;
			}

			// 遊び方のテクスチャ番号をリセット
			m_nHowtoTex = PAUSE_HOWTO_TEXTURE;

			// フェーズを項目選択にする
			m_Phase = TITLE_PHASE_MODE_SELECT;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTitle *CTitle::Create(void)
{
	// クラスの生成
	CTitle *pTitle = NULL;
	pTitle = new CTitle;
	
	if (pTitle != NULL)
	{
		// 初期化処理
		pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pTitle;
}

//=============================================================================
// UI生成処理
//=============================================================================
void CTitle::UiCreate(TITLE_PHASE phase, bool bPad)
{
	// テクスチャの種類
	int nTexType = 6;
	float fBackTex = 0.0f;

	// 一旦全てのUIを消しておく
	for (int nCntUI = 0; nCntUI < TITLE_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
			delete m_apUI[nCntUI];
			m_apUI[nCntUI] = NULL;
		}
	}

	switch (phase)
	{
	case TITLE_PHASE_PRESS_START:
		// 選択肢のUIを消す
		for (int nCntSelectUI = 0; nCntSelectUI < TITLE_MAX_SELECT; nCntSelectUI++)
		{
			if (m_apSelectUI[nCntSelectUI] != NULL)
			{
				m_apSelectUI[nCntSelectUI]->Uninit();
				delete m_apSelectUI[nCntSelectUI];
				m_apSelectUI[nCntSelectUI] = NULL;
			}
		}

		// タイトル：プレス
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_PRESS_POS_Y),
			D3DXVECTOR2(TITLE_PRESS_SIZE_X, TITLE_PRESS_SIZE_Y), 2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_MODE_SELECT:
		// 選択肢：スタート文字
		m_apSelectUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// 選択肢：遊び方
		m_apSelectUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 40, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// 選択肢：操作ガイド
		m_apSelectUI[2] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE * 2),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// 選択肢：ランキング
		m_apSelectUI[3] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, TITLE_START_POS_Y + TITLE_SELECT_SPACE * 3),
			D3DXVECTOR2(TITLE_START_SIZE_X, TITLE_START_SIZE_Y), 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_HOWTO:
		if (bPad == false)
		{
			fBackTex = 0.0f;
		}
		else
		{
			fBackTex = 1.0f;
		}
		// 遊び方の生成
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 41, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y), D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1]->SetTexture(0.0f, fBackTex, 1, 2);
		m_apUI[2] = CUI::Create(D3DXVECTOR2(PAUSE_PAGINATION_POS_X, PAUSE_PAGINATION_POS_Y), D3DXVECTOR2(PAUSE_PAGINATION_SIZE_X, PAUSE_PAGINATION_SIZE_Y), 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case TITLE_PHASE_GUIDE:
		if (bPad == false)
		{
			nTexType = 6;
			fBackTex = 0.0f;
		}
		else
		{
			nTexType = 7;
			fBackTex = 1.0f;
		}

		// 操作ガイドの生成
		m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), nTexType, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y), D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUI[1]->SetTexture(0.0f, fBackTex, 1, 2);
		break;

	case TITLE_PHASE_RANKING:
		m_pRanking = CRanking::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), bPad);
		break;

	default:
		break;
	}
}