//=============================================================================
// テクスチャ処理 [texture.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CTexture::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//**************************
	//*  テクスチャの読み込み  *
	//**************************
	// フィールド00(草原)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field00.jpg",
		&m_apTexture[TEX_TYPE_FIELD00]);
	// フィールド02(土；スタート)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field01.png",
		&m_apTexture[TEX_TYPE_FIELD01]);
	// フィールド03(土)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field02.png",
		&m_apTexture[TEX_TYPE_FIELD02]);

	// ウォール00(森)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/woods00.png",
		&m_apTexture[TEX_TYPE_WALL_FOREST]);
	// ウォール(山0)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mountains00.png",
		&m_apTexture[TEX_TYPE_WALL_MOUNTAIN_00]);
	// ウォール(山1)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mountains01.png",
		&m_apTexture[TEX_TYPE_WALL_MOUNTAIN_01]);
	// 雲00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud000.png",
		&m_apTexture[TEX_TYPE_CLOUD00]);
	// 雲01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud001.png",
		&m_apTexture[TEX_TYPE_CLOUD01]);
	// 月
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/moon.png",
		&m_apTexture[TEX_TYPE_MOON]);
	// エフェクト/影
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&m_apTexture[TEX_TYPE_EFFECT]);
	// 煙
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/smoke.png",
		&m_apTexture[TEX_TYPE_SMOKE]);
	// ナンバー
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number00.png",
		&m_apTexture[TEX_TYPE_NUMBER]);
	// 小数点
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/comma.png",
		&m_apTexture[TEX_TYPE_COMMA]);
	// メートル
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/meter.png",
		&m_apTexture[TEX_TYPE_METER]);

	// 背景オブジェクト：花00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower00.png",
		&m_apTexture[TEX_TYPE_FLOWER_00]);
	// 背景オブジェクト：花01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower01.png",
		&m_apTexture[TEX_TYPE_FLOWER_01]);
	// 背景オブジェクト：花02
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower02.png",
		&m_apTexture[TEX_TYPE_FLOWER_02]);
	// 背景オブジェクト：花03
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower03.png",
		&m_apTexture[TEX_TYPE_FLOWER_03]);
	// 背景オブジェクト：花04
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower04.png",
		&m_apTexture[TEX_TYPE_FLOWER_04]);
	// 背景オブジェクト：花05
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower05.png",
		&m_apTexture[TEX_TYPE_FLOWER_05]);
	// 背景オブジェクト：木
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tree.png",
		&m_apTexture[TEX_TYPE_TREE]);
	// 背景オブジェクト：茂み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bush00.png",
		&m_apTexture[TEX_TYPE_BUSH_00]);

	//+----------------+
	//|  シーン毎のUI  |
	//+----------------+
	// タイトル：ロゴ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_title_logo.png",
		&m_apTexture[TEX_TYPE_TITLE_LOGO]);
	// タイトル：エンター文字
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_title_enter.png",
		&m_apTexture[TEX_TYPE_TITLE_ENTER]);
	// タイトル：背景00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_bg00.jpg",
		&m_apTexture[TEX_TYPE_TITLE_BG00]);
	// タイトル：背景01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_bg01.png",
		&m_apTexture[TEX_TYPE_TITLE_BG01]);

	// メニュー：トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_top.png",
		&m_apTexture[TEX_TYPE_MENU_TOP]);
	// メニュー：チュートリアルモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_tutorial.png",
		&m_apTexture[TEX_TYPE_MENU_TUTORIAL]);
	// メニュー：ひとりようモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_singlemode.png",
		&m_apTexture[TEX_TYPE_MENU_SINGLE]);
	// メニュー：ふたりようモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_pvpmode.png",
		&m_apTexture[TEX_TYPE_MENU_PVP]);
	// メニュー：ランキングモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_ranking.png",
		&m_apTexture[TEX_TYPE_MENU_RANKING]);
	// メニュー：サウンドテスト
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_soundtest.png",
		&m_apTexture[TEX_TYPE_MENU_SOUNDTEST]);
	// メニュー：ガイド…チュートリアルモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_tutorial.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_TUTORIAL]);
	// メニュー：ガイド…ひとりようモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_game.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_SINGLE]);
	// メニュー：ガイド…ふたりようモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_pvp.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_PVP]);
	// メニュー：ガイド…ランキングモード
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_ranking.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_RANKING]);
	// メニュー：ガイド…サウンドテスト
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_soundtest.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_SOUNDTEST]);
	// メニュー：背景
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/menu_bg.jpg",
		&m_apTexture[TEX_TYPE_MENU_BG]);

	// チュートリアル：終了
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_end.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_END]);
	// チュートリアル：スキップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_skip.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_SKIP]);
	// チュートリアル：ボタン
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_button.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_BUTTON]);

	// ソロメニュー：トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_top.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_TOP]);
	// ソロメニュー：CPUとたいせん
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_cpu.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_CPU]);
	// ソロメニュー：スコアアタック
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_score.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_SCORE]);
	// ソロメニュー；CPUのつよさ(吹き出し)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_difficulty00.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_DIFFICULTY_00]);
	// ソロメニュー；CPUのつよさ(文字)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_difficulty01.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_DIFFICULTY_01]);

	// ゲーム：カウントダウン
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_game_countdown.png",
		&m_apTexture[TEX_TYPE_GAME_COUNTDOWN]);
	// ゲーム：スコア文字
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_score_sentence.png",
		&m_apTexture[TEX_TYPE_GAME_SCORE_SENTENCE]);
	// ゲーム：ハイスコア文字
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_highscore_sentence.png",
		&m_apTexture[TEX_TYPE_GAME_HIGHSCORE_SENTENCE]);
	// ゲーム：1P
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_1p.png",
		&m_apTexture[TEX_TYPE_GAME_1P]);
	// ゲーム：2P
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_2p.png",
		&m_apTexture[TEX_TYPE_GAME_2P]);
	// ゲーム：CPU
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_cpu.png",
		&m_apTexture[TEX_TYPE_GAME_CPU]);
	// ゲーム：ジャンプ地点まであと何mか
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_jump_guide00.png",
		&m_apTexture[TEX_TYPE_GAME_JUMP_GUIDE00]);
	// ゲーム：着地まであと何mか
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_jump_guide01.png",
		&m_apTexture[TEX_TYPE_GAME_JUMP_GUIDE01]);

	// リザルト：トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_top.png",
		&m_apTexture[TEX_TYPE_RESULT_TOP]);
	// リザルト：文字
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_sentence.png",
		&m_apTexture[TEX_TYPE_RESULT_SENTENCE]);
	// リザルト：帯
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_band.png",
		&m_apTexture[TEX_TYPE_RESULT_BAND]);
	// リザルト：背景
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result_bg.jpg",
		&m_apTexture[TEX_TYPE_RESULT_BG]);
	// リザルト：リトライ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_retry.png",
		&m_apTexture[TEX_TYPE_RESULT_RETRY]);
	// リザルト：かち！
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_win.png",
		&m_apTexture[TEX_TYPE_RESULT_WIN]);
	// リザルト：ひきわけ…
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_draw.png",
		&m_apTexture[TEX_TYPE_RESULT_DRAW]);

	// ランキング：トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_top.png",
		&m_apTexture[TEX_TYPE_RANKING_TOP]);
	// ランキング：1位の枠
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_one.png",
		&m_apTexture[TEX_TYPE_RANKING_ONE]);
	// ランキング：2位の枠
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_two.png",
		&m_apTexture[TEX_TYPE_RANKING_TWO]);
	// ランキング：3位の枠
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_three.png",
		&m_apTexture[TEX_TYPE_RANKING_THREE]);
	// ランキング：4位の枠
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_four.png",
		&m_apTexture[TEX_TYPE_RANKING_FOUR]);
	// ランキング：5位の枠
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_five.png",
		&m_apTexture[TEX_TYPE_RANKING_FIVE]);

	// つぎへボタン
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_next.png",
		&m_apTexture[TEX_TYPE_NEXT]);
	// もどるボタン
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_back.png",
		&m_apTexture[TEX_TYPE_BACK]);

	// ポーズ；トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_top.png",
		&m_apTexture[TEX_TYPE_PAUSE_TOP]);
	// ポーズ；つづける
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_continue.png",
		&m_apTexture[TEX_TYPE_PAUSE_CONTINUE]);
	// ポーズ；やりなおす
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_retry.png",
		&m_apTexture[TEX_TYPE_PAUSE_RETRY]);
	// ポーズ；やめる
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_quit.png",
		&m_apTexture[TEX_TYPE_PAUSE_QUIT]);

	// サウンドテスト：トップ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_top.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_TOP]);
	// サウンドテスト：メイン
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_main.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_MAIN]);
	// サウンドテスト：再生
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_play.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_PLAY]);

	//+------------------------+
	//|  チュートリアルの説明  |
	//+------------------------+
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_00.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE00]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_01.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_02.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE02]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_03.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE03]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_04.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE04]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_05.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE05]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_06.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE06]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_07.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE07]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_08.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE08]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_09.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE09]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_10.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE10]);

	// テクスチャのパスや名前が間違っていると、終了処理でエラーが発生するので注意!!
}

//=============================================================================
// 終了処理
//=============================================================================
void CTexture::Uninit(void)
{
	// テクスチャの総数ぶん繰り返す
	for (int nCntTexture = 0; nCntTexture < TEX_TYPE_MAX; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < GUIDE_TEX_TYPE_MAX; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_apGuideTex[nCntTexture] != NULL)
		{
			m_apGuideTex[nCntTexture]->Release();
			m_apGuideTex[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// テクスチャの取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEX_TYPE type)
{
	return m_apTexture[type];
}

//=============================================================================
// テクスチャの取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetGuideTex(GUIDE_TEX_TYPE type)
{
	return m_apGuideTex[type];
}