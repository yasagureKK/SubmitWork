//=============================================================================
// 死亡処理 [death.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "death.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEATH_DISPLAY_COUNT		 (50)		// 画像表示までの時間
#define DEATH_TRANS_COUNT		 (180)		// 画面遷移までの時間
#define UI_GAMEOVER_SIZE_X		 (700.0f)	// ゲームオーバー文字のサイズX
#define UI_GAMEOVER_SIZE_Y		 (180.0f)	// ゲームオーバー文字のサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CDeath::CDeath()
{
	// 変数のクリア
	memset(&m_apUI, NULL, sizeof(m_apUI));
	m_nTrans = 0;
	m_bDisplay = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CDeath::~CDeath()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDeath::Init(void)
{
	// 変数の初期化
	m_nTrans = 0;
	m_bDisplay = false;

	// 背景
	m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 22, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	// 文字
	m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(UI_GAMEOVER_SIZE_X, UI_GAMEOVER_SIZE_Y), 23, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDeath::Uninit(void)
{
	// UIの終了処理
	for (int nCntUI = 0; nCntUI < DEATH_MAX_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CDeath::Update(void)
{
	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	//フェード取得処理
	CFade *pFade = CManager::GetFade();

	// カウントを進める
	m_nTrans++;

	// 画像を表示していない時
	if (m_bDisplay == false)
	{
		// 一定の時間が経ったら
		if (m_nTrans >= DEATH_DISPLAY_COUNT)
		{
			// サウンドの停止
			pSound->Stop();

			// ゲームオーバーSEの再生
			pSound->Play(15);
			pSound->ControllVoice(15, 0.6f);

			m_nTrans = 0;
			m_bDisplay = true;
		}
	}
	else if (m_bDisplay == true)
	{
		// 画像のアルファ度を変更し、画像をゆっくり表示させる
		for (int nCntDeath = 0; nCntDeath < DEATH_MAX_UI; nCntDeath++)
		{
			int nAlpha = 2 * m_nTrans;

			if (nAlpha >= 255)
			{
				nAlpha = 255;
			}

			m_apUI[nCntDeath]->SetColor(D3DCOLOR_RGBA(255, 255, 255, nAlpha));
		}

		// 一定の時間が経ち、フェードしていないなら
		if (m_nTrans >= DEATH_TRANS_COUNT && pFade->GetFade() == CFade::FADE_NONE)
		{
			// タイトル画面に遷移
			pFade->SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDeath::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CDeath *CDeath::Create(void)
{	
	// クラスの生成
	CDeath *pDeath = NULL;
	pDeath = new CDeath;

	if (pDeath != NULL)
	{
		// 初期化処理
		pDeath->Init();
	}
	return pDeath;
}