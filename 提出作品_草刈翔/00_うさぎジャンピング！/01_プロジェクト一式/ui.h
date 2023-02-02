//=============================================================================
// UI処理 [ui.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_TOP_POS_X (250.0f)	// トップの位置X
#define UI_TOP_POS_Y (60.0f)	// トップの位置Y
#define UI_TOP_SIZE_X (750.0f)	// トップのサイズX
#define UI_TOP_SIZE_Y (120.0f)	// トップのサイズY
#define UI_BACK_POS_X (100.0f)	// もどるボタンの位置X
#define UI_BACK_POS_Y (680.0f)	// もどるボタンの位置Y
#define UI_BACK_SIZE_X (300.0f)	// もどるボタンのサイズX
#define UI_BACK_SIZE_Y (100.0f)	// もどるボタンのサイズY

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CControl;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CUi : public CScene2D
{
public:
	// UIの種類
	typedef enum
	{
		UI_TYPE_NEXT = 0,				// つぎへボタン
		UI_TYPE_BACK,					// もどるボタン
		UI_TYPE_COMMA,					// 小数点
		UI_TYPE_METER,					// メートル
		UI_TYPE_TITLE_LOGO,				// タイトル：ロゴ
		UI_TYPE_TITLE_ENTER,			// タイトル：エンター
		UI_TYPE_MENU_TOP,				// メニュー：トップ
		UI_TYPE_MENU_TUTORIAL,			// メニュー：チュートリアルモード
		UI_TYPE_MENU_SINGLE,			// メニュー：ひとりようモード
		UI_TYPE_MENU_PVP,				// メニュー：ふたりようモード
		UI_TYPE_MENU_RANKING,			// メニュー：ランキングモード
		UI_TYPE_MENU_SOUNDTEST,			// メニュー；サウンドテスト
		UI_TYPE_MENU_GUIDE_TUTORIAL,	// メニュー：ガイド…チュートリアルモード
		UI_TYPE_MENU_GUIDE_SINGLE,		// メニュー：ガイド…ひとりようモード
		UI_TYPE_MENU_GUIDE_PVP,			// メニュー：ガイド…ふたりようモード
		UI_TYPE_MENU_GUIDE_RANKING,		// メニュー：ガイド…ランキングモード
		UI_TYPE_MENU_GUIDE_SOUNDTEST,	// メニュー；ガイド…サウンドテスト
		UI_TYPE_TUTORIAL,				// チュートリアル：
		UI_TYPE_TUTORIAL_END,			// チュートリアル：終了
		UI_TYPE_TUTORIAL_SKIP,			// チュートリアル：スキップ
		UI_TYPE_TUTORIAL_BUTTON,		// チュートリアル：ボタン
		UI_TYPE_SOLO_MENU_TOP,			// ソロメニュー：トップ
		UI_TYPE_SOLO_MENU_CPU,			// ソロメニュー：CPUとたいせん
		UI_TYPE_SOLO_MENU_SCORE,		// ソロメニュー：スコアアタック
		UI_TYPE_SOLO_MENU_DIFFICULTY_00,// ソロメニュー；CPUのつよさ(吹き出し)
		UI_TYPE_SOLO_MENU_DIFFICULTY_01,// ソロメニュー；CPUのつよさ(文字)
		UI_TYPE_GAME_COUNTDOWN,			// ゲーム：カウントダウン
		UI_TYPE_GAME_SCORE,				// ゲーム：スコア文字
		UI_TYPE_GAME_HIGHSCORE,			// ゲーム：ハイスコア文字
		UI_TYPE_GAME_1P,				// ゲーム：1P
		UI_TYPE_GAME_2P,				// ゲーム：2P
		UI_TYPE_GAME_CPU,				// ゲーム：CPU
		UI_TYPE_GAME_JUMP_GUIDE,		// ゲーム：ジャンプ地点まであと何mか
		UI_TYPE_RESULT_TOP,				// リザルト：トップ
		UI_TYPE_RESULT_SENTENCE,		// リザルト：文字
		UI_TYPE_RESULT_BAND,			// リザルト：帯
		UI_TYPE_RESULT_RETRY,			// リザルト：リトライ
		UI_TYPE_RESULT_WIN,				// リザルト：かち！
		UI_TYPE_RESULT_DRAW,			// リザルト：ひきわけ…
		UI_TYPE_RANKING_TOP,			// ランキング：トップ
		UI_TYPE_RANKING_ONE,			// ランキング：1位の枠
		UI_TYPE_RANKING_TWO,			// ランキング：2位の枠
		UI_TYPE_RANKING_THREE,			// ランキング：3位の枠
		UI_TYPE_RANKING_FOUR,			// ランキング：4位の枠
		UI_TYPE_RANKING_FIVE,			// ランキング：5位の枠
		UI_TYPE_PAUSE_TOP,				// ポーズ；トップ
		UI_TYPE_PAUSE_CONTINUE,			// ポーズ；つづける
		UI_TYPE_PAUSE_RETRY,			// ポーズ；やりなおす
		UI_TYPE_PAUSE_QUIT,				// ポーズ；やめる
		UI_TYPE_SOUNDTEST_TOP,			// サウンドテスト：トップ
		UI_TYPE_SOUNDTEST_MAIN,			// サウンドテスト：メイン
		UI_TYPE_SOUNDTEST_PLAY,			// サウンドテスト：再生
		UI_TYPE_MAX
	}UI_TYPE;

	CUi(int nPriority = PRIORITY_UI);						// コンストラクタ
	~CUi();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nPriolty, UI_TYPE type);						// 生成処理
	UI_TYPE GetUIType(void);								// 種類取得処理

private:
	UI_TYPE m_type;											// UIの種類
	LPDIRECT3DTEXTURE9 m_pTexture;							// テクスチャへのポインタ
	D3DXVECTOR3 m_size;										// サイズ
	CControl *m_pControl;									// コントロールクラスのポインタ
};

#endif // _UI_H_