//=============================================================================
// 練習モードのガイド処理 [tutorial_guide.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "tutorial_guide.h"
#include "manager.h"
#include "texture.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CTutorialGuide::m_nNowPage = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorialGuide::CTutorialGuide(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	m_bDisplay = false;
	m_bDeleteDisplay = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorialGuide::~CTutorialGuide()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialGuide::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_nNowPage = 0;
	m_bDisplay = false;
	m_bDeleteDisplay = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorialGuide::Uninit(void)
{
	// 破棄する前に表示状態を解除
	m_bDisplay = false;

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorialGuide::Update(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//パッド取得処理
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
		// ガイドを表示していないかつ、まだ消さない場合に処理
		if (m_bDisplay == false && m_bDeleteDisplay == false)
		{
			// すぐにtrueにして1回だけ処理させる
			m_bDisplay = true;

			// メッセージウィンドウの生成
			CreateMessageWindow();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorialGuide::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTutorialGuide *CTutorialGuide::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスの生成
	CTutorialGuide *pPause = NULL;

	// ヌルチェック
	if (pPause == NULL)
	{
		// クラスの生成
		pPause = new CTutorialGuide;
		if (pPause != NULL)
		{
			// 初期化処理
			pPause->Init(pos, size);
		}
	}

	return pPause;
}

//=============================================================================
// ページ数設定処理
//=============================================================================
void CTutorialGuide::SetPage(int nNum)
{
	m_nNowPage = nNum;
}

//=============================================================================
// ページ数取得処理
//=============================================================================
int CTutorialGuide::GetPage(void)
{
	return m_nNowPage;
}

//=============================================================================
// メッセージウィンドウを作る処理
//=============================================================================
void CTutorialGuide::CreateMessageWindow(void)
{
	// メッセージウィンドウがまだ無い場合
	if (m_pScene2D[0] == NULL)
	{
		// メッセージウィンドウを作る
		m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, TUTORIAL_GUIDE_UI_FRAME_POS_Y, 0.0f), D3DXVECTOR3(TUTORIAL_GUIDE_UI_FRAME_SIZE_X, TUTORIAL_GUIDE_UI_FRAME_SIZE_Y, 0.0f), PRIORITY_UI);
		m_pScene2D[0]->SetColor(D3DCOLOR_RGBA(0, 0, 0, 155));
	}

	// 現在のページのint型を列挙型にキャストする
	CTexture::GUIDE_TEX_TYPE num = (CTexture::GUIDE_TEX_TYPE)m_nNowPage;

	// 説明
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, TUTORIAL_GUIDE_UI_FRAME_POS_Y, 0.0f), D3DXVECTOR3(TUTORIAL_GUIDE_UI_FRAME_SIZE_X - 30.0f, TUTORIAL_GUIDE_UI_FRAME_SIZE_Y - 35.0f, 0.0f), PRIORITY_UI);
	m_pScene2D[1]->BindTexture(CManager::GetTexture()->GetGuideTex(num));
}

//=============================================================================
// ページを進める処理
//=============================================================================
void CTutorialGuide::AdvancePage(void)
{
	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// すぐにfalseにして1回だけ処理させる
	m_bDisplay = false;

	// 列挙型であるページの最大値をint型にキャストする
	int nNum = (int)CTexture::GUIDE_TEX_TYPE_MAX;

	// 現在のページがページの最大値-2より下だったら
	if (m_nNowPage < nNum - 2)
	{
		// ページを進める
		m_nNowPage++;
	}

	// 2Dオブジェクトの文字のみ破棄
	m_pScene2D[1]->Uninit();
	delete m_pScene2D[1];
	m_pScene2D[1] = NULL;
}

//=============================================================================
// ページを進め、メッセージウィンドウを破棄する処理
//=============================================================================
void CTutorialGuide::AddPageAndDestroyWindow(void)
{
	//サウンド取得処理
	CSound *pSound;
	pSound = CManager::GetSound();

	// すぐにfalseにして1回だけ処理させる
	m_bDisplay = false;

	// 列挙型であるページの最大値をint型にキャストする
	int nNum = (int)CTexture::GUIDE_TEX_TYPE_MAX;

	// 現在のページがページの最大値-2より下だったら
	if (m_nNowPage < nNum - 2)
	{
		// ページを進める
		m_nNowPage++;
	}

	// 2Dオブジェクトの破棄
	for (int nCntScene2D = 0; nCntScene2D < 2; nCntScene2D++)
	{
		m_pScene2D[nCntScene2D]->Uninit();
		delete m_pScene2D[nCntScene2D];
		m_pScene2D[nCntScene2D] = NULL;
	}

	// ガイドの表示を消す
	m_bDeleteDisplay = true;
}