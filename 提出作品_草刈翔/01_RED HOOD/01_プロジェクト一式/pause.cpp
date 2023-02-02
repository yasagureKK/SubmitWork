//=============================================================================
// ポーズ処理 [pause.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_TOP_POS_X			(340.0f)	// ポーズトップの位置X
#define PAUSE_TOP_POS_Y			(90.0f)		// ポーズトップの位置Y
#define PAUSE_TOP_SIZE_X		(640.0f)	// ポーズトップのサイズX
#define PAUSE_TOP_SIZE_Y		(120.0f)	// ポーズトップのサイズY
#define PAUSE_SELECT_POS_X		(400.0f)	// 選択肢の位置X
#define PAUSE_SELECT_POS_Y		(250.0f)	// 選択肢の位置Y
#define PAUSE_SELECT_SIZE_X		(640.0f)	// 選択肢のサイズX
#define PAUSE_SELECT_SIZE_Y		(100.0f)	// 選択肢のサイズY
#define PAUSE_SENTENCE_POS_X	(360.0f)	// 項目説明文の位置X
#define PAUSE_SENTENCE_POS_Y	(660.0f)	// 項目説明文の位置Y
#define PAUSE_SENTENCE_SIZE_X	(600.0f)	// 項目説明文のサイズX
#define PAUSE_SENTENCE_SIZE_Y	(90.0f)		// 項目説明文のサイズY
#define PAUSE_BACK_POS_X		(240.0f)	// 戻るのX座標位置
#define PAUSE_BACK_POS_Y		(660.0f)	// 戻るのY座標位置
#define PAUSE_BACK_SIZE_X		(430.0f)	// 戻るのサイズX
#define PAUSE_BACK_SIZE_Y		(80.0f)		// 戻るのサイズY

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CPause::m_nSelect = 0;
bool CPause::m_bCanPause = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	memset(&m_apUi, NULL, sizeof(m_apUi));
	memset(&m_apSelectUi, NULL, sizeof(m_apSelectUi));
	m_bCanPause = false;
	m_bPause = false;
	m_nSelect = 0;
	m_Phase = PAUSE_PHASE_NORMAL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_bCanPause = true;
	m_bPause = false;
	m_nSelect = 0;
	m_Phase = PAUSE_PHASE_NORMAL;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	// ポーズ状態を解除
	CManager::SetPause(false);

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();
	// マウス取得処理
	CMouse *pMouse = CManager::GetMouse();

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	//フェード取得処理
	CFade *pFade = CManager::GetFade();

	switch (m_Phase)
	{
	case PAUSE_PHASE_NORMAL:
		for (int nCntUi = 0; nCntUi < PAUSE_MAX_SELECT_UI; nCntUi++)
		{
			if (m_apSelectUi[nCntUi] != NULL)
			{
				// 選択されているUIは
				if (nCntUi == m_nSelect)
				{
					// 透明度を0に
					m_apSelectUi[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

					// サイズを少し大きくする
					m_apSelectUi[nCntUi]->SetSize(D3DXVECTOR2(PAUSE_SELECT_SIZE_X + 40.0f, PAUSE_SELECT_SIZE_Y + 10.0f));
				}
				// 選択されていないUIは
				else
				{
					// 透明にする
					m_apSelectUi[nCntUi]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));

					// サイズを元に戻す
					m_apSelectUi[nCntUi]->SetSize(D3DXVECTOR2(PAUSE_SELECT_SIZE_X, PAUSE_SELECT_SIZE_Y));
				}

				// 現在の選択肢に合わせて文を変える
				m_apUi[2]->SetTexture(0, (float)m_nSelect, 1, 3);
			}
		}


		// ポーズできる場合かつ、フェードしていないなら
		if (m_bCanPause == true && pFade->GetFade() == CFade::FADE_NONE)
		{
			//***********************************************************************
			// ポーズ (1キー または パッドSTARTボタン)
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_1) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
			{
				// BGMの音量を下げる
				pSound->ControllVoice(1, 0.2f);
				pSound->ControllVoice(2, 0.2f);

				// マネージャー側、このクラスの両方でポーズしていない場合に処理
				if (CManager::GetPause() == false && m_bPause == false)
				{
					// UIの生成
					UICreate(m_Phase, 0.0f);

					// ポーズした状態にする
					CManager::SetPause(true);
					m_bPause = true;

				}
				// ポーズしている場合に処理
				else if (CManager::GetPause() == true && m_bPause == true)
				{
					// ポーズ解除処理
					CancelPause();
				}
			}

			// このクラス内でポーズしてるなら操作可能
			if (m_bPause == true)
			{
				//***********************************************************************
				// 選択 (キーボードＷ/Ｓ または パッド左スティック)
				//***********************************************************************
				//上
				if (pKeyboard->GetTrigger(DIK_W) == true ||
					pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
				{
					// 選択SEの再生
					pSound->Play(27);
					pSound->ControllVoice(27, 0.4f);

					m_nSelect--;
				}
				//下
				else if (pKeyboard->GetTrigger(DIK_S) == true ||
					pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
				{
					// 選択SEの再生
					pSound->Play(27);
					pSound->ControllVoice(27, 0.4f);

					m_nSelect++;
				}

				// 選択肢の制限
				if (m_nSelect < 0)
				{
					m_nSelect = PAUSE_MAX_SELECT_UI - 1;
				}
				else if (m_nSelect >= PAUSE_MAX_SELECT_UI)
				{
					m_nSelect = 0;
				}

				//***********************************************************************
				// 決定 (Enter/Space または Aボタン)
				//***********************************************************************
				if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
				{
					float fBackTex = 0.0f;
					// m_nSelectの値で処理を変える
					switch (m_nSelect)
					{
						// つづける
					case 0:
						// 決定SEの再生
						pSound->Play(28);
						pSound->ControllVoice(28, 0.4f);

						// ポーズ解除処理
						CancelPause();
						break;

						// 操作説明
					case 1:
						// 決定SEの再生
						pSound->Play(28);
						pSound->ControllVoice(28, 0.4f);

						// フェーズを操作ガイドにする
						m_Phase = PAUSE_PHASE_GUIDE;

						if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
							pKeyboard->GetTrigger(DIK_RETURN) == true)
						{
							fBackTex = 0.0f;
						}
						else if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true ||
							pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START, 0) == true)
						{
							fBackTex = 1.0f;
						}

						// UIの生成
						UICreate(m_Phase, fBackTex);

						break;

						// 終了
					case 2:
						// BGMの停止
						pSound->Stop();

						// キャンセルSEの再生
						pSound->Play(29);
						pSound->ControllVoice(29, 0.6f);

						// タイトル画面に遷移
						pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
						break;

					default:
						break;
					}
				}
				//***********************************************************************
				// キャンセル (Backspace または Xボタン)
				//***********************************************************************
				else if (pKeyboard->GetTrigger(DIK_BACKSPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
				{
					// キャンセルSEの再生
					pSound->Play(29);
					pSound->ControllVoice(29, 0.6f);

					// ポーズ解除処理
					CancelPause();
				}
			}
		}

		break;

	case PAUSE_PHASE_GUIDE:
		//***********************************************************************
		// 戻る (キーボードBackspace または パッドXボタン または マウス右クリック)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_BACK) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true ||
			pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true)
		{
			if (m_apUi[3] != NULL && m_apUi[4] != NULL)
			{
				// キャンセルSEの再生
				pSound->Play(29);
				pSound->ControllVoice(29, 0.6f);

				// 操作ガイドを消す
				m_apUi[3]->Uninit();
				delete m_apUi[3];
				m_apUi[3] = NULL;
				m_apUi[4]->Uninit();
				delete m_apUi[4];
				m_apUi[4] = NULL;

				// フェーズを通常にする
				m_Phase = PAUSE_PHASE_NORMAL;
			}
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPause *CPause::Create(D3DXVECTOR3 pos)
{
	// クラスの生成
	CPause *pPause = NULL;
	pPause = new CPause;

	if (pPause != NULL)
	{
		// 初期化処理
		pPause->Init(pos);
	}
	return pPause;
}

//=============================================================================
// UI生成処理
//=============================================================================
void CPause::UICreate(PAUSE_PHASE phase, float fTex)
{
	switch (phase)
	{
	case PAUSE_PHASE_NORMAL:
		// ポーズ：背景
		m_apUi[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポーズ：トップ文字
		m_apUi[1] = CUI::Create(D3DXVECTOR2(PAUSE_TOP_POS_X, PAUSE_TOP_POS_Y), D3DXVECTOR2(PAUSE_TOP_SIZE_X, PAUSE_TOP_SIZE_Y), 17, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポーズ：項目説明文
		m_apUi[2] = CUI::Create(D3DXVECTOR2(PAUSE_SENTENCE_POS_X, PAUSE_SENTENCE_POS_Y), D3DXVECTOR2(PAUSE_SENTENCE_SIZE_X, PAUSE_SENTENCE_SIZE_Y), 21, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUi[2]->SetTexture(0, 0, 1, 3);

		// ポーズ：選択肢
		for (int nCntUI = 0; nCntUI < PAUSE_MAX_SELECT_UI; nCntUI++)
		{
			m_apSelectUi[nCntUI] = CUI::Create(D3DXVECTOR2(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + (PAUSE_SELECT_SIZE_Y * nCntUI)),
				D3DXVECTOR2(PAUSE_SELECT_SIZE_X, PAUSE_SELECT_SIZE_Y), 18 + nCntUI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apSelectUi[nCntUI]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 153));
		}
		break;

	case PAUSE_PHASE_GUIDE:
		// 操作ガイドの生成
		m_apUi[3] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		m_apUi[4] = CUI::Create(D3DXVECTOR2(PAUSE_BACK_POS_X, PAUSE_BACK_POS_Y),
			D3DXVECTOR2(PAUSE_BACK_SIZE_X, PAUSE_BACK_SIZE_Y), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apUi[4]->SetTexture(0.0f, fTex, 1, 2);
		break;

	default:
		break;
	}
}

//=============================================================================
// ポーズ解除時の処理
//=============================================================================
void CPause::CancelPause(void)
{
	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// ポーズを解除
	CManager::SetPause(false);
	m_bPause = false;

	// BGMの音量を上げる
	pSound->ControllVoice(1, 0.3f);
	pSound->ControllVoice(2, 0.3f);

	// UIの破棄
	for (int nCntUi = 0; nCntUi < PAUSE_MAX_UI; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			m_apUi[nCntUi]->Uninit();
			delete m_apUi[nCntUi];
			m_apUi[nCntUi] = NULL;
		}
	}

	for (int nCntUi = 0; nCntUi < PAUSE_MAX_SELECT_UI; nCntUi++)
	{
		if (m_apSelectUi[nCntUi] != NULL)
		{
			m_apSelectUi[nCntUi]->Uninit();
			delete m_apSelectUi[nCntUi];
			m_apSelectUi[nCntUi] = NULL;
		}
	}

	// 選択している番号を初期化
	m_nSelect = 0;
}

//=============================================================================
// ポーズできるか設定処理
//=============================================================================
void CPause::SetPossiblePause(bool bPause)
{
	m_bCanPause = bPause;
}