//=============================================================================
// UI処理 [ui.h]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "scene.h"
#include "control.h"
#include "control_ui_title_logo.h"
#include "control_ui_title_enter.h"
#include "control_ui_game_countdown.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CUi::CUi(int nPriority) :CScene2D(nPriority)
{
	// 変数のクリア
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pControl = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 継承元クラスの初期化処理
	CScene2D::Init(pos, size);

	// 変数の初期化
	m_size = size;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUi::Uninit(void)
{
	// 継承元クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUi::Update(void)
{
	// コントロールクラスがヌルじゃなかったら
	if (m_pControl != NULL)
	{
		// 自身のポインタを入れ、更新処理
		m_pControl->Update(this);

		// Scene2Dから位置を取得
		D3DXVECTOR3 pos;
		pos = GetPos();

		// 取得した位置に移動量を加算
		pos += m_pControl->GetMove();

		// 位置、サイズを設定
		SetPos(pos, GetSize());
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CUi::Draw(void)
{
	// 継承元クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CUi* CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriolity, UI_TYPE type)
{
	// インスタンスの生成
	CUi *pUi = NULL;

	// ヌルチェック
	if (pUi == NULL)
	{
		// クラスの生成
		pUi = new CUi(nPriolity);
		if (pUi != NULL)
		{
			// 指定したUIの種類でテクスチャを変え、割り当てる
			switch (type)
			{
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 汎用系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// つぎへボタン
			case UI_TYPE_NEXT:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_NEXT);
				break;
				// もどるボタン
			case UI_TYPE_BACK:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_BACK);
				break;
			case UI_TYPE_COMMA:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_COMMA);
				break;
				// メートル
			case UI_TYPE_METER:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_METER);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// タイトル系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// タイトル：ロゴ
			case UI_TYPE_TITLE_LOGO:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_LOGO);
				pUi->m_pControl = CControlUiTitleLogo::Create();
				break;
				// タイトル：エンター文字
			case UI_TYPE_TITLE_ENTER:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_ENTER);
				pUi->m_pControl = CControlUiTitleEnter::Create();
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// メニュー系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// メニュー：トップ
			case UI_TYPE_MENU_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_TOP);
				break;
				// メニュー：チュートリアルモード
			case UI_TYPE_MENU_TUTORIAL:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_TUTORIAL);
				break;
				// メニュー：ひとりようモード
			case UI_TYPE_MENU_SINGLE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_SINGLE);
				break;
				// メニュー：ふたりようモード
			case UI_TYPE_MENU_PVP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_PVP);
				break;
				// メニュー：ランキングモード
			case UI_TYPE_MENU_RANKING:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_RANKING);
				break;
				// メニュー：サウンドテスト
			case UI_TYPE_MENU_SOUNDTEST:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_SOUNDTEST);
				break;
				// メニュー：ガイド…チュートリアルモード
			case UI_TYPE_MENU_GUIDE_TUTORIAL:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_TUTORIAL);
				break;
				// メニュー：ガイド…ひとりようモード
			case UI_TYPE_MENU_GUIDE_SINGLE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_SINGLE);
				break;
				// メニュー：ガイド…ふたりようモード
			case UI_TYPE_MENU_GUIDE_PVP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_PVP);
				break;
				// メニュー：ガイド…ランキングモード
			case UI_TYPE_MENU_GUIDE_RANKING:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_RANKING);
				break;
				// メニュー：ガイド…サウンドテスト
			case UI_TYPE_MENU_GUIDE_SOUNDTEST:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_SOUNDTEST);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// チュートリアル系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// チュートリアル：終了
			case UI_TYPE_TUTORIAL_END:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_END);
				break;
				// チュートリアル：スキップ
			case UI_TYPE_TUTORIAL_SKIP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_SKIP);
				break;
				// チュートリアル：ボタン
			case UI_TYPE_TUTORIAL_BUTTON:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_BUTTON);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 1人用ゲームメニュー系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ソロメニュー：トップ
			case UI_TYPE_SOLO_MENU_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_TOP);
				break;
				// ソロメニュー：CPUとたいせん
			case UI_TYPE_SOLO_MENU_CPU:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_CPU);
				break;
				// ソロメニュー：スコアアタック
			case UI_TYPE_SOLO_MENU_SCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_SCORE);
				break;
				// ソロメニュー：トップ
			case UI_TYPE_SOLO_MENU_DIFFICULTY_00:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_DIFFICULTY_00);
				break;
				// ソロメニュー：CPUとたいせん
			case UI_TYPE_SOLO_MENU_DIFFICULTY_01:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_DIFFICULTY_01);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ゲーム系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			case UI_TYPE_GAME_COUNTDOWN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_COUNTDOWN);
				pUi->m_pControl = CControlUiGameCountdown::Create();
				break;
			case UI_TYPE_GAME_SCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_SCORE_SENTENCE);
				break;
			case UI_TYPE_GAME_HIGHSCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_HIGHSCORE_SENTENCE);
				break;
			case UI_TYPE_GAME_1P:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_1P);
				break;
			case UI_TYPE_GAME_2P:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_2P);
				break;
			case UI_TYPE_GAME_CPU:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_CPU);
				break;
			case UI_TYPE_GAME_JUMP_GUIDE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_JUMP_GUIDE00);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// リザルト系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// リザルト：トップ
			case UI_TYPE_RESULT_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_TOP);
				break;
				// リザルト：文字
			case UI_TYPE_RESULT_SENTENCE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_SENTENCE);
				break;
				// リザルト：帯
			case UI_TYPE_RESULT_BAND:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_BAND);
				break;
				// リザルト：リトライ
			case UI_TYPE_RESULT_RETRY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_RETRY);
				break;
				// リザルト：かち！
			case UI_TYPE_RESULT_WIN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_WIN);
				break;
				// リザルト：ひきわけ…
			case UI_TYPE_RESULT_DRAW:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_DRAW);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ランキング系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ランキング：トップ
			case UI_TYPE_RANKING_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_TOP);
				break;
				// ランキング：1位の枠
			case UI_TYPE_RANKING_ONE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_ONE);
				break;
				// ランキング：2位の枠
			case UI_TYPE_RANKING_TWO:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_TWO);
				break;
				// ランキング：3位の枠
			case UI_TYPE_RANKING_THREE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_THREE);
				break;
				// ランキング：4位の枠
			case UI_TYPE_RANKING_FOUR:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_FOUR);
				break;
				// ランキング：5位の枠
			case UI_TYPE_RANKING_FIVE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_FIVE);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ポーズ系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ポーズ；トップ
			case UI_TYPE_PAUSE_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_TOP);
				break;
				// ポーズ；つづける
			case UI_TYPE_PAUSE_CONTINUE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_CONTINUE);
				break;
				// ポーズ；やりなおす
			case UI_TYPE_PAUSE_RETRY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_RETRY);
				break;
				// ポーズ；やめる
			case UI_TYPE_PAUSE_QUIT:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_QUIT);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ポーズ系
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// サウンドテスト：トップ
			case UI_TYPE_SOUNDTEST_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_TOP);
				break;
				// サウンドテスト：メイン
			case UI_TYPE_SOUNDTEST_MAIN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_MAIN);
				break;
				// サウンドテスト：再生
			case UI_TYPE_SOUNDTEST_PLAY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_PLAY);
				break;

			default:
				break;
			}
			pUi->BindTexture(pUi->m_pTexture);

			// 変数の初期化
			pUi->m_type = type;

			// 初期化処理
			pUi->Init(pos, size);
		}
	}

	return pUi;
}

//=============================================================================
// 種類の取得処理
//=============================================================================
CUi::UI_TYPE CUi::GetUIType(void)
{
	return m_type;
}