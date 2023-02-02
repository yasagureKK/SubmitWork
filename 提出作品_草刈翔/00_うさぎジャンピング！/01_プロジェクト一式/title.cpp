//=============================================================================
// タイトル画面 [title.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"
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
float CTitle::m_fScroll = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	// 変数のクリア
	m_fScroll = 0;
	m_nFadeCount = 0;
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	memset(&m_apUi, NULL, sizeof(m_apUi));
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
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_fScroll = 0;
	m_nFadeCount = 0;

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_TITLE);
	pSound->ControllVoice(CSound::SOUND_LABEL_TITLE, 0.4f);

	// 背景
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D[0]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_BG00));
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D[1]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_BG01));

	//+------------+
	//|  UIの生成  |
	//+------------+
	// タイトルロゴ(上昇する動きをするため、画面下に生成)
	m_apUi[0] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + TITLE_LOGO_POS_Y, 0.0f),
		D3DXVECTOR3(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_TITLE_LOGO);
	// エンター文字
	m_apUi[1] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLE_ENTER_POS_Y, 0.0f),
		D3DXVECTOR3(TITLE_ENTER_SIZE_X, TITLE_ENTER_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_TITLE_ENTER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 背景スクロールカウントの更新
	m_fScroll += 0.001f;

	// 背景をスクロールさせる
	m_pScene2D[0]->SetTexture(m_fScroll / 4, 1.0f, 1, 1);

	// 遷移用カウントの更新
	m_nFadeCount++;

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
			//***********************************************************************
			// 次へすすむ (Enter/Space または パッドAボタン/Xボタン/STARTボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_START, nNum) == true)
			{
				// BGMの停止
				pSound->Stop(CSound::SOUND_LABEL_TITLE);
				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_TITLE_ENTER);
				pSound->ControllVoice(CSound::SOUND_LABEL_TITLE_ENTER, 0.4f);
				// メニュー画面に遷移
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
				// 直前のモードを設定
				CManager::SetBeforeMode(CManager::MODE_TITLE);
			}
		}

		// 自動的に遷移
		if (m_nFadeCount >= TITLE_FADE_COUNT)
		{
			// BGMの停止
			pSound->Stop(CSound::SOUND_LABEL_TITLE);

			// ランキング画面に遷移
			pFade->SetFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}