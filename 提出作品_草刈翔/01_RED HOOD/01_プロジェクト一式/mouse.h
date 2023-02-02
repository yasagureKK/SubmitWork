//=============================================================================
// マウス入力処理 [mouse.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMouse : public CInput
{
public:
	// 入力情報
	typedef enum
	{
		MOUSE_INPUT_LEFT = 0,	// 左ボタン
		MOUSE_INPUT_RIGHT,		// 右ボタン
		MOUSE_INPUT_WHEEL,		// ホイール(中央ボタン)
		MOUSE_INPUT_MAX
	} MOUSE_INPUT;

	CMouse();										// コンストラクタ
	~CMouse();										// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	bool GetPress(MOUSE_INPUT nType);				// 入力状態の取得処理(プレス)
	bool GetTrigger(MOUSE_INPUT nType);				// 入力状態の取得処理(トリガー)

private:
	DIMOUSESTATE  m_aMouseState;					// マウスの情報
	DIMOUSESTATE  m_aMouseStateTrigger;				// マウスの情報(トリガー)
	bool m_bConnect;								// 接続しているかどうか
	HWND m_hWnd;									// 保存用のハンドル
};
#endif	// _MOUSE_H_