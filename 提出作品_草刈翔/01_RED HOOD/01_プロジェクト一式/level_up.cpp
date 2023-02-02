//=============================================================================
// レベルアップ処理 [level_up.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "level_up.h"
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
#define LEVEL_UP_TOP_POS_Y			(120.0f)	// トップ文字のY座標位置
#define LEVEL_UP_TOP_SIZE_X			(650.0f)	// トップ文字のサイズX
#define LEVEL_UP_TOP_SIZE_Y			(170.0f)	// トップ文字のサイズY
#define LEVEL_UP_SELECT_POS_X		(320.0f)	// 選択肢のX座標位置
#define LEVEL_UP_SELECT_POS_Y		(360.0f)	// 選択肢のY座標位置
#define LEVEL_UP_SELECT_SIZE_X		(360.0f)	// 選択肢のサイズX
#define LEVEL_UP_SELECT_SIZE_Y		(250.0f)	// 選択肢のサイズY
#define LEVEL_UP_SENTENCE_POS_Y		(630.0f)	// 項目説明文の位置Y
#define LEVEL_UP_SENTENCE_SIZE_X	(700.0f)	// 項目説明文のサイズX
#define LEVEL_UP_SENTENCE_SIZE_Y	(100.0f)	// 項目説明文のサイズY

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CLevelUp::m_nCorrectionDef = 0;
int CLevelUp::m_nCorrectionAtk = 0;
int CLevelUp::m_nCorrectionSpd = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CLevelUp::CLevelUp(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apSelectUI, NULL, sizeof(m_apSelectUI));
	m_nSelect = 0;
	m_nFlash = 0;
	m_Phase = LEVELUP_PHASE_BG;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLevelUp::~CLevelUp()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLevelUp::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_nSelect = 0;
	m_nFlash = 0;
	m_Phase = LEVELUP_PHASE_BG;

	// ポーズ状態にする
	CManager::SetPause(true);

	// 背景・トップ文字
	m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, LEVEL_UP_TOP_POS_Y), D3DXVECTOR2(LEVEL_UP_TOP_SIZE_X, LEVEL_UP_TOP_SIZE_Y), 34, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 説明文
	m_apUI[2] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, LEVEL_UP_SENTENCE_POS_Y), D3DXVECTOR2(LEVEL_UP_SENTENCE_SIZE_X, LEVEL_UP_SENTENCE_SIZE_Y), 36, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI[2]->SetTexture(0, 0, 1, 3);

	// 選択肢
	for (int nCntIcon = 0; nCntIcon < LEVELUP_MAX_SELECT; nCntIcon++)
	{
		float fTexType = (float)nCntIcon;
		m_apSelectUI[nCntIcon] = CUI::Create(D3DXVECTOR2(LEVEL_UP_SELECT_POS_X + (nCntIcon * LEVEL_UP_SELECT_POS_X), LEVEL_UP_SELECT_POS_Y), D3DXVECTOR2(LEVEL_UP_SELECT_SIZE_X, LEVEL_UP_SELECT_SIZE_Y), 35, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f));
		m_apSelectUI[nCntIcon]->SetTexture(fTexType, 0, 3, 1);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLevelUp::Uninit(void)
{
	// ポーズ状態を解除
	CManager::SetPause(false);

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLevelUp::Update(void)
{
	// キーボード取得処理
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// ゲームパッド取得処理
	CGamePad *pGamePad = CManager::GetGamePad();

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	switch (m_Phase)
	{
	case LEVELUP_PHASE_BG:
		// アルファ度を上げる
		m_nAlpha += 8;

		if (m_nAlpha >= 255)
		{
			m_nAlpha = 255;
		}

		// 画像のアルファ度を変更し、ゆっくり表示させる
		for (int nCntClear = 0; nCntClear < LEVELUP_MAX_UI; nCntClear++)
		{
			if (m_apUI[nCntClear] != NULL)
			{
				m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
			}
		}

		for (int nCntSelect = 0; nCntSelect < LEVELUP_MAX_SELECT; nCntSelect++)
		{
			if (m_apSelectUI[nCntSelect] != NULL)
			{
				m_apSelectUI[nCntSelect]->SetColor(D3DCOLOR_RGBA(127, 127, 127, m_nAlpha));
			}
		}

		// 完全に不透明になったら
		if (m_nAlpha == 255)
		{
			m_Phase = LEVELUP_PHASE_SELECT;
		}
		break;

	case LEVELUP_PHASE_SELECT:
		//***********************************************************************
		// 選択 (Ａキ ー/ Ｄキー または パッド左スティック)
		//***********************************************************************
		//左
		if (pKeyboard->GetTrigger(DIK_A) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true)
		{
			// 選択SEの再生
			pSound->Play(27);
			pSound->ControllVoice(27, 0.4f);

			m_nSelect--;
		}
		//右
		else if (pKeyboard->GetTrigger(DIK_D) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true)
		{
			// 選択SEの再生
			pSound->Play(27);
			pSound->ControllVoice(27, 0.4f);

			m_nSelect++;
		}

		// 選択肢の制限
		if (m_nSelect < 0)
		{
			m_nSelect = LEVELUP_MAX_SELECT - 1;
		}
		else if (m_nSelect >= LEVELUP_MAX_SELECT)
		{
			m_nSelect = 0;
		}

		for (int nCntUi = 0; nCntUi < LEVELUP_MAX_SELECT; nCntUi++)
		{
			if (m_apSelectUI[nCntUi] != NULL)
			{
				// 選択されているUIは
				if (nCntUi == m_nSelect)
				{
					// 色を明るくする
					m_apSelectUI[nCntUi]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					// 色を暗くする
					m_apSelectUI[nCntUi]->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}
			}
		}

		if (m_apUI[2] != NULL)
		{
			// 説明文のテクスチャを更新
			m_apUI[2]->SetTexture(0, (float)m_nSelect, 1, 3);
		}

		//***********************************************************************
		// 選択 (Spaceキー または パッドAボタン)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			// 選択されていた項目で上げるステータスを変える
			if (m_nSelect == 0)
			{
				// 攻撃補正値の上昇
				m_nCorrectionAtk++;
			}
			else if (m_nSelect == 1)
			{
				// 防御補正値の上昇
				m_nCorrectionDef++;
			}
			else if (m_nSelect == 2)
			{
				// 速度補正値の上昇
				m_nCorrectionSpd++;
			}

			// レベルアップ時の決定SEの再生
			pSound->Play(14);
			pSound->ControllVoice(14, 0.4f);

			m_Phase = LEVELUP_PHASE_FINISH;
		}
		break;

	case LEVELUP_PHASE_FINISH:
		// 点滅用のカウントを増やす
		m_nFlash++;

		// 点滅用のカウントが一定数経ったら
		if (m_nFlash >= 30)
		{
			// アルファ度を減らす
			m_nAlpha -= 12;
		}

		if (m_nAlpha <= 0)
		{
			m_nAlpha = 0;
		}

		// 画像のアルファ度を変更し、消していく
		for (int nCntClear = 0; nCntClear < LEVELUP_MAX_UI; nCntClear++)
		{
			if (m_apUI[nCntClear] != NULL)
			{
				m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
			}
		}

		for (int nCntSelect = 0; nCntSelect < LEVELUP_MAX_SELECT; nCntSelect++)
		{
			if (m_apSelectUI[nCntSelect] != NULL)
			{
				// 選択されているUIは
				if (nCntSelect == m_nSelect)
				{
					if (m_nFlash % 6 == 0 && m_nFlash != 0)
					{
						// 点滅
						m_apSelectUI[m_nSelect]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
					}
					else
					{
						// 点滅
						m_apSelectUI[m_nSelect]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
					}
				}
				else
				{
					m_apSelectUI[nCntSelect]->SetColor(D3DCOLOR_RGBA(127, 127, 127, m_nAlpha));
				}
			}
		}

		// 完全に透明になったら
		if (m_nAlpha == 0)
		{
			// UIの破棄
			for (int nCntUi = 0; nCntUi < LEVELUP_MAX_UI; nCntUi++)
			{
				if (m_apUI[nCntUi] != NULL)
				{
					m_apUI[nCntUi]->Uninit();
				}
			}

			for (int nCntUi = 0; nCntUi < LEVELUP_MAX_SELECT; nCntUi++)
			{
				if (m_apSelectUI[nCntUi] != NULL)
				{
					m_apSelectUI[nCntUi]->Uninit();
				}
			}

			// 終了処理
			Uninit();
		}
		break;

	default:
		break;
	}

	if (m_nCorrectionAtk > 9)
	{
		m_nCorrectionAtk = 9;
	}
	if (m_nCorrectionDef > 9)
	{
		m_nCorrectionDef = 9;
	}
	if (m_nCorrectionSpd > 9)
	{
		m_nCorrectionSpd = 9;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLevelUp::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CLevelUp *CLevelUp::Create(D3DXVECTOR3 pos)
{
	// クラスの生成
	CLevelUp *pPause = NULL;
	pPause = new CLevelUp;

	if (pPause != NULL)
	{
		// 初期化処理
		pPause->Init(pos);
	}
	return pPause;
}