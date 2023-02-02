//=============================================================================
// ポーズ処理 [pause.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MAX_UI	(4)				// UIの最大数
#define UI_PAUSE_TOP_POS_Y (180.0f)		// ポーズトップの位置Y
#define UI_PAUSE_TOP_SIZE_X (290.0f)	// ポーズトップのサイズX
#define UI_PAUSE_TOP_SIZE_Y (120.0f)	// ポーズトップのサイズY
#define UI_PAUSE_SELECT_POS_Y (310.0f)	// ポーズ選択肢の位置Y
#define UI_PAUSE_SELECT_SIZE_X (580.0f)	// ポーズ選択肢のサイズX
#define UI_PAUSE_SELECT_SIZE_Y (110.0f)	// ポーズ選択肢のサイズY

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUi;
class CScene2D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPause : public CScene
{
public:
	CPause(int nPriority = PRIORITY_PAUSE);						// コンストラクタ
	~CPause();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成処理
	static void SetPause(bool bPause);							// ポーズ設定処理
	static bool GetPause(void);									// ポーズ取得処理

private:
	void CancelPause(void);										// ポーズ解除時の処理
	CUi *m_apUi[PAUSE_MAX_UI];									// UIのポインタ
	CScene2D *m_pScene2D;										// 2Dオブジェクトのポインタ
	static bool m_bPause;										// ポーズしてるかどうか
	static int m_nSelect;										// どの選択肢を選んでいるか
};

#endif	// _PAUSE_H_