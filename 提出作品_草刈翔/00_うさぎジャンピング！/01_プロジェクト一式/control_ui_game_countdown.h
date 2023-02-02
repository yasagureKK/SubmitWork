//=============================================================================
// ゲームUI：カウントダウン操作処理 [control_ui_game_countdown.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CONTROL_UI_GAME_COUNTDOWN_H_
#define _CONTROL_UI_GAME_COUNTDOWN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CControlUiGameCountdown : public CControl
{
public:
	CControlUiGameCountdown();						// コンストラクタ
	~CControlUiGameCountdown();						// デストラクタ
	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(CScene *pScene);					// 更新処理
	static CControlUiGameCountdown *Create(void);	// 生成処理
	static void SetCount(int nCount);				// カウント設定処理

private:
	static int m_nCount;							// カウント
	D3DXCOLOR m_col;								// カラー
};

#endif	//_CONTROL_UI_GAME_COUNTDOWN_H_