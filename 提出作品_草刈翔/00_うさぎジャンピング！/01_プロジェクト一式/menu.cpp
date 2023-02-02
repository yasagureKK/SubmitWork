//=============================================================================
// メニュー画面 [menu.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "menu.h"
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
int CMenu::m_nSelect = 1;
float CMenu::m_fScroll = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CMenu::CMenu()
{
	// 変数のクリア
	m_fScroll = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_apGuideUi = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMenu::~CMenu()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMenu::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_fScroll = 0;

	// タイトル画面に戻ったら選択を最初に戻す
	if (CManager::GetBeforeMode() == CManager::MODE_TITLE)
	{
		m_nSelect = 1;
	}

	// 直前のモードを設定
	CManager::SetBeforeMode(CManager::MODE_MENU);

	// サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_MENU);
	pSound->ControllVoice(CSound::SOUND_LABEL_MENU, 0.4f);

	// 背景
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	//+------------+
	//|  UIの生成  |
	//+------------+
	// トップ
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_TOP);
	// チュートリアルモード
	m_apUi[1] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 2, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_TUTORIAL);
	// 1人用モード
	m_apUi[2] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 3, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_SINGLE);
	// 2人用モード
	m_apUi[3] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 4, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_PVP);
	// ランキングモード
	m_apUi[4] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 5, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_RANKING);
	// サウンドテストモード
	m_apUi[5] = CUi::Create(D3DXVECTOR3(MENU_SELECT_POS_X, MENU_SELECT_POS_Y * 6, 0.0f),
		D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_MENU_SOUNDTEST);
	// もどるボタン
	m_apUi[6] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMenu::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMenu::Update(void)
{
	// カウンターの更新
	m_fScroll += 0.001f;

	// 背景をスクロールさせる
	m_pScene2D->SetTexture(m_fScroll / 2, -m_fScroll / 4, 1, 1);

	for (int nCntUi = 1; nCntUi < MENU_MAX_UI; nCntUi++)
	{
		if (m_apUi[nCntUi] != NULL)
		{
			// UIのテクスチャの設定
			m_apUi[nCntUi]->SetTexture(0.0f, 0.0f, 1, 2);

			// 選択されている間
			if (nCntUi == m_nSelect)
			{
				if (nCntUi != 6)
				{
					// サイズを少し大きくする
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(MENU_SELECT_SIZE_X + 40.0f, MENU_SELECT_SIZE_Y + 10.0f, 0.0f));
				}

				// 光らせる
				m_apUi[nCntUi]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				if (nCntUi != 6)
				{
					// サイズを戻す
					D3DXVECTOR3 pos = m_apUi[nCntUi]->GetPos();
					m_apUi[nCntUi]->SetPos(pos, D3DXVECTOR3(MENU_SELECT_SIZE_X, MENU_SELECT_SIZE_Y, 0.0f));
				}
			}
		}
	}

	// 説明UIの画像切り替え処理
	GuideChange();

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
			int nUp, nDown;

			if (nNum == 0)
			{
				nUp = DIK_W;
				nDown = DIK_S;
			}
			else if (nNum == 1)
			{
				nUp = DIK_UP;
				nDown = DIK_DOWN;
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

				// 説明UIがヌルじゃなかったら
				if (m_apGuideUi != NULL)
				{
					// 説明UIの破棄
					m_apGuideUi->Uninit();
					delete m_apGuideUi;
					m_apGuideUi = NULL;
				}
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

				// 説明UIがヌルじゃなかったら
				if (m_apGuideUi != NULL)
				{
					// 説明UIの破棄
					m_apGuideUi->Uninit();
					delete m_apGuideUi;
					m_apGuideUi = NULL;
				}
			}

			// 選択肢の制限
			if (m_nSelect < 1)
			{
				m_nSelect = 6;
			}
			else if (m_nSelect > 6)
			{
				m_nSelect = 1;
			}

			//***********************************************************************
			// 決定 (Enter/Space または パッドAボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_MENU);

				// m_nSelectの値でフェード先を変更
				switch (m_nSelect)
				{
				case 1:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 直前のモードをメニューに設定(説明の表示の有無を区別するため)
					CManager::SetBeforeMode(CManager::MODE_MENU);

					// チュートリアル画面に遷移
					pFade->SetFade(CManager::MODE_TUTORIAL, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 2:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 1人用メニュ－画面に遷移
					pFade->SetFade(CManager::MODE_SOLO_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 3:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// 2人用ゲーム画面に遷移
					pFade->SetFade(CManager::MODE_PVP, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 4:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// ランキング画面に遷移
					pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 5:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_DECIDE);
					pSound->ControllVoice(CSound::SOUND_LABEL_DECIDE, 0.4f);

					// サウンドテスト画面に遷移
					pFade->SetFade(CManager::MODE_SOUNDTEST, D3DCOLOR_RGBA(0, 0, 0, 0));
					break;

				case 6:
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_CANCEL);
					pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

					// タイトル画面に遷移
					pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
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
				m_nSelect = 6;

				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_MENU);

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// タイトル画面に遷移
				pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMenu::Draw(void)
{

}

//=============================================================================
// 説明UIの画像切り替え処理
//=============================================================================
void CMenu::GuideChange(void)
{
	// ヌルの時だけ処理
	if (m_apGuideUi == NULL)
	{
		// 選択してる番号で処理を変える
		switch (m_nSelect)
		{
			// れんしゅうモード
		case 1:
			// 説明UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f),
				PRIORITY_UI, CUi::UI_TYPE_MENU_GUIDE_TUTORIAL);
			break;
			// ひとりようモード
		case 2:
			// 説明UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI, 
				CUi::UI_TYPE_MENU_GUIDE_SINGLE);
			break;
			// ふたりようモード
		case 3:
			// 説明UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_PVP);
			break;
			// ランキングモード
		case 4:
			// 説明UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_RANKING);
			break;
			// サウンドテストモード
		case 5:
			// 説明UI
			m_apGuideUi = CUi::Create(D3DXVECTOR3(MENU_GUIDE_POS_X, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(MENU_GUIDE_SIZE_X, MENU_GUIDE_SIZE_Y, 0.0f), PRIORITY_UI,
				CUi::UI_TYPE_MENU_GUIDE_SOUNDTEST);
			break;
		default:
			break;
		}
	}
}