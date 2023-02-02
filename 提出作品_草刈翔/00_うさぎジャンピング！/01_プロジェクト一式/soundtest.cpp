//=============================================================================
// サウンドテスト画面 [soundtest.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "soundtest.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"
#include "number_sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CSoundTest::m_nSelect = 0;
float CSoundTest::m_fScroll = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CSoundTest::CSoundTest()
{
	// 変数のクリア
	m_fScroll = 0;
	m_pScene2D = NULL;
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_pSoundNumber = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSoundTest::~CSoundTest()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSoundTest::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_nSelect = 1;
	m_fScroll = 0;

	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの再生
	pSound->Play(CSound::SOUND_LABEL_SOUNDTEST);
	pSound->ControllVoice(CSound::SOUND_LABEL_SOUNDTEST, 0.4f);

	// 背景
	m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), PRIORITY_NONE);
	m_pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_BG));

	// トップ
	m_apUi[0] = CUi::Create(D3DXVECTOR3(UI_TOP_POS_X, UI_TOP_POS_Y, 0.0f),
		D3DXVECTOR3(UI_TOP_SIZE_X, UI_TOP_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_TOP);
	// もどるボタン
	m_apUi[1] = CUi::Create(D3DXVECTOR3(UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_BACK);
	// 再生ボタン
	m_apUi[2] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - UI_BACK_POS_X, UI_BACK_POS_Y, 0.0f),
		D3DXVECTOR3(UI_BACK_SIZE_X, UI_BACK_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_PLAY);
	// メイン
	m_apUi[3] = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SOUNDTEST_UI_MAIN_SIZE_X, SOUNDTEST_UI_MAIN_SIZE_Y, 0.0f),
		PRIORITY_UI, CUi::UI_TYPE_SOUNDTEST_MAIN);

	// サウンドナンバー
	m_pSoundNumber = CNumberSound::Create(D3DXVECTOR3(SOUNDTEST_UI_NUMBER_POS_X, SOUNDTEST_UI_NUMBER_POS_Y, 0.0f), D3DXVECTOR3(SOUNDTEST_UI_NUMBER_SIZE_X, SOUNDTEST_UI_NUMBER_SIZE_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255));

	// ボタンは暗くしておく
	m_apUi[1]->SetTexture(0.0f, 0.0f, 1, 2);
	m_apUi[2]->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSoundTest::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSoundTest::Update(void)
{
	// カウンターの更新
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
			//***********************************************************************
			// 戻る (BackSpace または パッドXボタン)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_BACK) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_X, nNum) == true)
			{
				// 光らせる
				m_apUi[1]->SetTexture(0.0f, 1.0f, 1, 2);

				// BGMの停止
				pSound->Stop();

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_CANCEL);
				pSound->ControllVoice(CSound::SOUND_LABEL_CANCEL, 0.4f);

				// メニュー画面に遷移
				pFade->SetFade(CManager::MODE_MENU, D3DCOLOR_RGBA(0, 0, 0, 0));
			}

			//***********************************************************************
			// 決定 (Enter/Space または パッドAボタン)
			//***********************************************************************
			if (pInputKeyboard->GetPress(DIK_RETURN) == true ||
				pInputKeyboard->GetPress(DIK_SPACE) == true ||
				pInputPad->GetButtonPress(XINPUT_GAMEPAD_A, 0) == true ||
				pInputPad->GetButtonPress(XINPUT_GAMEPAD_A, 1) == true)
			{
				// 光らせる
				m_apUi[2]->SetTexture(0.0f, 1.0f, 1, 2);
			}
			else
			{
				// 暗くする
				m_apUi[2]->SetTexture(0.0f, 0.0f, 1, 2);
			}
		}
	}
}

//================================================
// 描画処理
//================================================
void CSoundTest::Draw(void)
{

}