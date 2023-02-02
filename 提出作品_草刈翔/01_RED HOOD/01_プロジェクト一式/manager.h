//=============================================================================
// マネージャー [manager.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CTitle;
class CGame;
class CKeyboard;
class CGamePad;
class CMouse;
class CTexture;
class CModelLoad;
class CCamera;
class CFade;
class CSound;
class CDataAttack;
class CDataClear;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CManager
{
public:
	// モードの種類
	typedef enum
	{
		MODE_TITLE = 0,	// タイトル
		MODE_GAME,		// ゲーム
		MODE_MAX,
	} MODE;

	CManager();														// コンストラクタ
	~CManager();													// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static void SetMode(MODE mode);									// モード設定処理
	static MODE GetMode(void) { return m_mode; }					// モード取得処理
	static CTitle *GetTitle(void) { return m_pTitle; }				// タイトル取得処理
	static CGame *GetGame(void) { return m_pGame; }					// ゲーム取得処理
	static CRenderer *GetRenderer(void) { return m_pRenderer; }		// レンダラー取得処理
	static CCamera *GetCamera(void) { return m_pCamera; }			// カメラ取得処理
	static CKeyboard *GetKeyboard(void) { return m_pKeyboard; }		// キーボード取得処理
	static CGamePad *GetGamePad(void) { return m_pGamePad; }		// ゲームパッド取得処理
	static CMouse *GetMouse(void) { return m_pMouse; }				// マウス取得処理
	static CTexture *GetTexture(void) { return m_pTexture; }		// テクスチャ取得処理
	static CModelLoad *GetModelLoad(void) { return m_pModelLoad; }	// モデルロード取得処理
	static CFade *GetFade(void) { return m_pFade; }					// フェード取得処理
	static CSound *GetSound(void) { return m_pSound; }				// サウンド取得処理
	static CDataAttack *GetDataAtk(void) { return m_pDataAtk; }		// 攻撃情報の取得処理
	static CDataClear *GetDataClear(void) { return m_pDataClear; }	// クリア情報の取得処理

	static void SetPause(bool bPause) { m_bPause = bPause; }		// ポーズ設定処理
	static bool GetPause() { return m_bPause; };					// ポーズ取得処理

private:
	static void GameSetUp();										// ゲーム画面のセットアップ処理

	static MODE m_mode;												// 現在のモード
	static CTitle *m_pTitle;										// タイトルのポインタ
	static CGame *m_pGame;											// ゲームのポインタ
	static CRenderer *m_pRenderer;									// レンダラークラスのポインタ
	static CCamera *m_pCamera;										// カメラのポインタ
	static CKeyboard *m_pKeyboard;									// キーボードクラスのポインタ
	static CGamePad *m_pGamePad;									// ゲームパッドクラスのポインタ
	static CMouse *m_pMouse;										// マウスのポインタ
	static CTexture *m_pTexture;									// テクスチャのポインタ
	static CModelLoad *m_pModelLoad;								// モデルロードのポインタ
	static CFade *m_pFade;											// フェードのポインタ
	static CSound *m_pSound;										// サウンドのポインタ
	static CDataAttack *m_pDataAtk;									// 攻撃情報のポインタ
	static CDataClear *m_pDataClear;								// クリア情報のポインタ
	static bool m_bPause;											// ポーズしているかどうか
};
#endif // _MANAGER_H_