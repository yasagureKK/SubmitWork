//=============================================================================
// 1人用ゲームメニュー画面 [solo_menu.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "solo_menu.h"
#include "manager.h"
#include "texture.h"
#include "scene2D.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
float CSoloMenu::m_fScroll = 0;
int CSoloMenu::m_nSelect = 1;
int CSoloMenu::m_nDifficulty = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CSoloMenu::CSoloMenu()
{
	// 変数のクリア
	m_fScroll = 0;
	m_pScene2D = NULL;
	//memset(&m_apUi, NULL, sizeof(m_apUi));
}

//=============================================================================
// デストラクタ
//=============================================================================
CSoloMenu::~CSoloMenu()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSoloMenu::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_fScroll = 0;

	// メニュー画面に戻ったら選択を最初に戻す
	if (CManager::GetBeforeMode() == CManager::MODE_MENU)
	{
		m_nSelect = 1;
	}

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_SOLOMENU);
	pSound->ControllVoice(CSound::SOUND_LABEL_SOLOMENU, 0.4f);

	// 背景
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UIの生成  |
	//+------------+
	// トップ
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_TOP);
	// CPUとたいせん
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 3, SOLO_MENU_SELECT_POS_Y + 120.0f, 0.0f),
		D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_CPU);
	// スコアアタック
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 3, SOLO_MENU_SELECT_POS_Y + 120.0f * 2, 0.0f),
		D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_SCORE);
	// もどるボタン
	m_apUi[3] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSoloMenu::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSoloMenu::Update(void)
{
	// カウンターの更新
	m_fScroll += 0.001f;

	// 背景をスクロールさせる
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

	for (int nCntUi = 1; nCntUi < 4; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			// UIのテクスチャの設定
			m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);

			// 選択されている間
			if (nCntUi == m_nSelect)
			{
				if (nCntUi != 3)
				{
					// サイズを少し大きくする
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X + 40.0f, SOLO_MENU_SELECT_SIZE_Y + 10.0f, 0.0f));
				}

				// 光らせる
				m_apUi[nCntUi]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				if (nCntUi != 3)
				{
					// サイズを戻す
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(SOLO_MENU_SELECT_SIZE_X, SOLO_MENU_SELECT_SIZE_Y, 0.0f));
				}
			}
		}
	}

	// CPU関連のUIを生成
	if (m_apUi[4] == NULL && m_nSelect == 1)
	{
		// CPUのつよさ(吹き出し)
		m_apUi[4] = CUi::Create(D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_POS_X, SOLO_MENU_DIFFICULTY_00_POS_Y, 0.0f),
			D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_SIZE_X, SOLO_MENU_DIFFICULTY_00_SIZE_Y, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_DIFFICULTY_00);
		// CPUのつよさ(文字)
		m_apUi[5] = CUi::Create(D3DXVECTOR3(SOLO_MENU_DIFFICULTY_00_POS_X + 37.0f, SOLO_MENU_DIFFICULTY_00_POS_Y + 40.0f, 0.0f),
			D3DXVECTOR3(260.0f, 90.0f, 0.0f),
			PRIORITY_UI, CUi::UI_TYPE_SOLO_MENU_DIFFICULTY_01);
	}
	else if (m_apUi[4] != NULL && m_nSelect != 1)
	{
		for (int nCntUi = 4; nCntUi < 6; nCntUi++)
		{
			// UIの破棄
			m_apUi[nCntUi]->Uninit();
			delete m_apUi[nCntUi];
			m_apUi[nCntUi] = NULL;
		}
	}

	if (m_apUi[5] != NULL)
	{
		// CPUの強さを変更
		m_apUi[5]->SetTexture(0, (float)m_nDifficulty, 1, 4);
	}

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

	// フェードしていない時のみ
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			int nUp, nDown, nLeft, nRight;

			if (nNum == 0)
			{
				nUp = DIK_W;
				nDown = DIK_S;
				nLeft = DIK_A;
				nRight = DIK_D;
			}
			else if (nNum == 1)
			{
				nUp = DIK_UP;
				nDown = DIK_DOWN;
				nLeft = DIK_LEFT;
				nRight = DIK_RIGHT;
			}

			//***********************************************************************
			// 選択 (1P:W/S、2P:↑/↓ または パッド左スティック/十字キー)
			//***********************************************************************
			//上
			if (pInputKeyboard->GetTrigger(nUp) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP, nNum) == true)
			{
				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSelect--;
			}
			//下
			else if (pInputKeyboard->GetTrigger(nDown) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nNum) == true)
			{
				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSelect++;
			}

			// 選択肢の制限
			if (m_nSelect < 1)
			{
				m_nSelect = 3;
			}
			else if (m_nSelect > 3)
			{
				m_nSelect = 1;
			}

			if (m_nSelect == 1)
			{
				//***********************************************************************
				// 選択 (1P:A/D、2P:←/→ または パッド左スティック/十字キー)
				//***********************************************************************
				//左
				if (pInputKeyboard->GetTrigger(nLeft) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nNum) == true)
				{
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_CURSOR);
					pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

					m_nDifficulty--;
				}
				//右
				else if (pInputKeyboard->GetTrigger(nRight) == true ||
					pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNum) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nNum) == true)
				{
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_CURSOR);
					pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

					m_nDifficulty++;
				}

				// 選択肢の制限
				if (m_nDifficulty < 0)
				{
					m_nDifficulty = 3;
				}
				else if (m_nDifficulty > 3)
				{
					m_nDifficulty = 0;
				}
			}

			//***********************************************************************
			// 決定 (Enter/Space または パッドAボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_SOLOMENU);

				// m_nSelectの値でフェード先を変更
				switch (m_nSelect)
				{
				case 1:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// CPU対戦ゲーム画面に遷移
					pFade->SetFade(CManager::MODE_SOLO_CPU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 2:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 1人用ゲーム画面に遷移
					pFade->SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 3:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// メニュー画面に遷移
					pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				default:
					break;
				}
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 戻る (BackSpace または パッドXボタン)
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				m_nSelect = 3;

				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_SOLOMENU);

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// メニュー画面に遷移
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSoloMenu::Draw(void)
{

}

//=============================================================================
// 難易度の取得処理
//=============================================================================
int CSoloMenu::GetDifficulty(void)
{
	return m_nDifficulty;
}