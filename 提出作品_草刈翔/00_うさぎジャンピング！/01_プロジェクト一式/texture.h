//=============================================================================
// テクスチャ処理 [texture.cpp]
// Authore : 草刈 翔
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{
public:
	// テクスチャの種類
	typedef enum
	{
											//*****↓ゲーム本編系↓*****//
		TEX_TYPE_FIELD00 = 0,				// フィールド00(草原)
		TEX_TYPE_FIELD01,					// フィールド01(土；スタート)
		TEX_TYPE_FIELD02,					// フィールド02(土)
		TEX_TYPE_WALL_FOREST,				// ウォール(森)
		TEX_TYPE_WALL_MOUNTAIN_00,			// ウォール(山0)[ステージの一番奥に配置する]
		TEX_TYPE_WALL_MOUNTAIN_01,			// ウォール(山1)[山0の手前に配置する]
		TEX_TYPE_CLOUD00,					// 雲00
		TEX_TYPE_CLOUD01,					// 雲01
		TEX_TYPE_MOON,						// 月
		TEX_TYPE_EFFECT,					// エフェクト/影
		TEX_TYPE_SMOKE,						// 煙
		TEX_TYPE_NUMBER,					// ナンバー
		TEX_TYPE_COMMA,						// 小数点
		TEX_TYPE_METER,						// メートル
		TEX_TYPE_FLOWER_00,					// 背景オブジェクト：花00
		TEX_TYPE_FLOWER_01,					// 背景オブジェクト：花01
		TEX_TYPE_FLOWER_02,					// 背景オブジェクト：花02
		TEX_TYPE_FLOWER_03,					// 背景オブジェクト：花03
		TEX_TYPE_FLOWER_04,					// 背景オブジェクト：花04
		TEX_TYPE_FLOWER_05,					// 背景オブジェクト：花05
		TEX_TYPE_TREE,						// 背景オブジェクト：木
		TEX_TYPE_BUSH_00,					// 背景オブジェクト：茂み
											//*********↓UI系↓*********//
		TEX_TYPE_TITLE_LOGO,				// タイトル：ロゴ
		TEX_TYPE_TITLE_ENTER,				// タイトル：エンター文字
		TEX_TYPE_TITLE_BG00,				// タイトル：背景00
		TEX_TYPE_TITLE_BG01,				// タイトル：背景01
		TEX_TYPE_MENU_TOP,					// メニュー：トップ
		TEX_TYPE_MENU_TUTORIAL,				// メニュー：チュートリアルモード
		TEX_TYPE_MENU_SINGLE,				// メニュー：ひとりようモード
		TEX_TYPE_MENU_PVP,					// メニュー：ふたりようモード
		TEX_TYPE_MENU_RANKING,				// メニュー：ランキングモード
		TEX_TYPE_MENU_SOUNDTEST,			// メニュー；サウンドテスト
		TEX_TYPE_MENU_GUIDE_TUTORIAL,		// メニュー：ガイド…チュートリアルモード
		TEX_TYPE_MENU_GUIDE_SINGLE,			// メニュー：ガイド…ひとりようモード
		TEX_TYPE_MENU_GUIDE_PVP,			// メニュー：ガイド…ふたりようモード
		TEX_TYPE_MENU_GUIDE_RANKING,		// メニュー：ガイド…ランキングモード
		TEX_TYPE_MENU_GUIDE_SOUNDTEST,		// メニュー；ガイド…サウンドテスト
		TEX_TYPE_MENU_BG,					// メニュー：背景
		TEX_TYPE_TUTORIAL_END,				// チュートリアル：終了
		TEX_TYPE_TUTORIAL_SKIP,				// チュートリアル：スキップ
		TEX_TYPE_TUTORIAL_BUTTON,			// チュートリアル：ボタン
		TEX_TYPE_SOLO_MENU_TOP,				// ソロメニュー；トップ
		TEX_TYPE_SOLO_MENU_CPU,				// ソロメニュー；CPUとたいせん
		TEX_TYPE_SOLO_MENU_SCORE,			// ソロメニュー；スコアアタック
		TEX_TYPE_SOLO_MENU_DIFFICULTY_00,	// ソロメニュー；CPUのつよさ(吹き出し)
		TEX_TYPE_SOLO_MENU_DIFFICULTY_01,	// ソロメニュー；CPUのつよさ(文字)
		TEX_TYPE_GAME_COUNTDOWN,			// ゲーム：カウントダウン
		TEX_TYPE_GAME_SCORE_SENTENCE,		// ゲーム：スコア文字
		TEX_TYPE_GAME_HIGHSCORE_SENTENCE,	// ゲーム：ハイスコア文字
		TEX_TYPE_GAME_1P,					// ゲーム：1P
		TEX_TYPE_GAME_2P,					// ゲーム：2P
		TEX_TYPE_GAME_CPU,					// ゲーム：CPU
		TEX_TYPE_GAME_JUMP_GUIDE00,			// ゲーム：ジャンプ地点まであと何mか
		TEX_TYPE_GAME_JUMP_GUIDE01,			// ゲーム：着地まであと何mか
		TEX_TYPE_RESULT_TOP,				// リザルト：トップ
		TEX_TYPE_RESULT_SENTENCE,			// リザルト：文字
		TEX_TYPE_RESULT_BAND,				// リザルト：帯
		TEX_TYPE_RESULT_BG,					// リザルト：背景
		TEX_TYPE_RESULT_RETRY,				// リザルト：リトライ
		TEX_TYPE_RESULT_WIN,				// リザルト：かち！
		TEX_TYPE_RESULT_DRAW,				// リザルト：ひきわけ…
		TEX_TYPE_RANKING_TOP,				// ランキング：トップ
		TEX_TYPE_RANKING_ONE,				// ランキング：1位の枠
		TEX_TYPE_RANKING_TWO,				// ランキング：2位の枠
		TEX_TYPE_RANKING_THREE,				// ランキング：3位の枠
		TEX_TYPE_RANKING_FOUR,				// ランキング：4位の枠
		TEX_TYPE_RANKING_FIVE,				// ランキング：5位の枠
		TEX_TYPE_NEXT,						// つぎへボタン
		TEX_TYPE_BACK,						// もどるボタン
		TEX_TYPE_PAUSE_TOP,					// ポーズ；トップ
		TEX_TYPE_PAUSE_CONTINUE,			// ポーズ；つづける
		TEX_TYPE_PAUSE_RETRY,				// ポーズ；やりなおす
		TEX_TYPE_PAUSE_QUIT,				// ポーズ；やめる
		TEX_TYPE_SOUNDTEST_TOP,				// サウンドテスト：トップ
		TEX_TYPE_SOUNDTEST_MAIN,			// サウンドテスト：メイン
		TEX_TYPE_SOUNDTEST_PLAY,			// サウンドテスト：再生
		TEX_TYPE_MAX
	} TEX_TYPE;

	// 練習モードでの説明テクスチャの種類
	typedef enum
	{
											//***↓練習モードの説明↓***//
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE00,	// 説明0 [ようこそ]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE01,	// 説明1 [説明させていただきます]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE02,	// 説明2 [助走をつけて大ジャンプ]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE03,	// 説明3 [左右でスピードアップ]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE04,	// 説明4 [どんどん繰り返そう]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE05,	// 説明5 [Aでジャンプ]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE06,	// 説明6 [もう一度ジャンプ]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE07,	// 説明7 [ホバリング]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE08,	// 説明8 [記録が決まる]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE09,	// 説明9 [どこまでとべるか]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE10,	// 説明10[操作方法]
		GUIDE_TEX_TYPE_MAX
	} GUIDE_TEX_TYPE;

	CTexture();												// コンストラクタ
	~CTexture();											// デストラクタ
	void Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	LPDIRECT3DTEXTURE9 GetTexture(TEX_TYPE Type);			// テクスチャ取得処理
	LPDIRECT3DTEXTURE9 GetGuideTex(GUIDE_TEX_TYPE Type);	// 説明テクスチャ取得処理

private:
	LPDIRECT3DTEXTURE9	m_apTexture[TEX_TYPE_MAX];			// テクスチャのポインタ
	LPDIRECT3DTEXTURE9	m_apGuideTex[GUIDE_TEX_TYPE_MAX];	// 説明テクスチャのポインタ
};

#endif