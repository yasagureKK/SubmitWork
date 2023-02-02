//=============================================================================
// 1人用ゲームメニュー画面 [solo_menu.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SOLO_MENU_H_
#define _SOLO_MENU_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOLO_MENU_MAX_UI		(8)			// UIの最大数
#define SOLO_MENU_SELECT_POS_Y	(190.0f)	// メニュー選択肢の位置Y
#define SOLO_MENU_SELECT_SIZE_X	(650.0f)	// メニュー選択肢のサイズX
#define SOLO_MENU_SELECT_SIZE_Y	(100.0f)	// メニュー選択肢のサイズY
#define SOLO_MENU_DIFFICULTY_00_POS_X	(1000.0f)	// メニュー選択肢の位置X
#define SOLO_MENU_DIFFICULTY_00_POS_Y	(260.0f)	// メニュー選択肢の位置Y
#define SOLO_MENU_DIFFICULTY_00_SIZE_X	(440.0f)	// メニュー選択肢のサイズX
#define SOLO_MENU_DIFFICULTY_00_SIZE_Y	(200.0f)	// メニュー選択肢のサイズY

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CUi;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSoloMenu : public CScene
{
public:
	CSoloMenu();										// コンストラクタ
	~CSoloMenu();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static int GetDifficulty(void);						// 難易度の取得処理

private:
	static float m_fScroll;								// 背景スクロール用のカウント
	static int m_nSelect;								// どのモードを選んでいるか
	static int m_nDifficulty;							// どの難易度を選んでいるか
	CScene2D *m_pScene2D;								// 2Dオブジェクトのポインタ
	CUi *m_apUi[SOLO_MENU_MAX_UI];						// UIのポインタ
};

#endif	// _SOLO_MENU_H_