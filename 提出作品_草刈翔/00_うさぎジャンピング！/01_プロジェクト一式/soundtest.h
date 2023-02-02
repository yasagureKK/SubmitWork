//=============================================================================
// サウンドテスト画面 [soundtest.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SOUNDTEST_H_
#define _SOUNDTEST_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOUNDTEST_MAX_UI			(4)			// UIの最大数
#define SOUNDTEST_UI_MAIN_SIZE_X	(950.0f)	// メインUIのサイズX
#define SOUNDTEST_UI_MAIN_SIZE_Y	(400.0f)	// メインUIのサイズY
#define SOUNDTEST_UI_NUMBER_POS_X	(530.0f)	// サウンドナンバーの位置座標X
#define SOUNDTEST_UI_NUMBER_POS_Y	(440.0f)	// サウンドナンバーの位置座標Y
#define SOUNDTEST_UI_NUMBER_SIZE_X	(150.0f)	// サウンドナンバー文字のサイズX
#define SOUNDTEST_UI_NUMBER_SIZE_Y	(200.0f)	// サウンドナンバー文字のサイズY

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CUi;
class CNumberSound;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSoundTest : public CScene
{
public:
	CSoundTest();										// コンストラクタ
	~CSoundTest();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

private:
	static int m_nSelect;								// どれを選んでいるか
	static float m_fScroll;								// 背景スクロール用の変数
	CScene2D *m_pScene2D;								// 2Dオブジェクトのポインタ
	CUi *m_apUi[SOUNDTEST_MAX_UI];						// UIのポインタ
	CNumberSound *m_pSoundNumber;						// サウンドナンバーのポインタ
};

#endif	// _SOUNDTEST_H_