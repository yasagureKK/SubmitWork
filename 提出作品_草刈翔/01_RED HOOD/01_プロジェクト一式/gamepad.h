//=============================================================================
// ゲームパッド入力処理 [gamepad.h]
// Author : Kusakari Kakeru
//=============================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CGamePad : CInput
{
public:
	// ボタン以外の入力情報
	typedef enum
	{
		PAD_INPUT_LSTICK_UP = 0,	// 左スティック：上
		PAD_INPUT_LSTICK_DOWN,		// 左スティック：下
		PAD_INPUT_LSTICK_LEFT,		// 左スティック：左
		PAD_INPUT_LSTICK_RIGHT,		// 左スティック：右
		PAD_INPUT_RSTICK_UP,		// 右スティック：上
		PAD_INPUT_RSTICK_DOWN,		// 右スティック：下
		PAD_INPUT_RSTICK_LEFT,		// 右スティック：左
		PAD_INPUT_RSTICK_RIGHT,		// 右スティック：右
		PAD_INPUT_LEFT_TRIGGER,		// 左トリガー
		PAD_INPUT_RIGHT_TRIGGER,	// 右トリガー
		PAD_INPUT_MAX
	} PAD_INPUT;

	CGamePad();												// コンストラクタ
	~CGamePad();											// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	bool GetPress(int nInput, int nNum);					// 入力状態の取得処理(プレス)
	bool GetTrigger(int nInput, int nNum);					// 入力状態の取得処理(トリガー)
	bool GetRelease(int nInput, int nNum);					// 入力状態の取得処理(リリース)
	bool GetButtonPress(int nButton, int nNum);				// ボタン入力状態の取得処理(プレス)
	bool GetButtonTrigger(int nButton, int nNum);			// ボタン入力状態の取得処理(トリガー)
	bool GetButtonRelease(int nButton, int nNum);			// ボタン入力状態の取得処理(リリース)
	bool GetAnyButton(int nNum);							// 全ての入力状態の取得処理(トリガー)
	bool GetConnect(int nNum) { return m_bConnect[nNum]; }	// コントローラ―が接続されているかどうかの取得処理

private:
	int ReturnDPAD(int nButton);							// ボタンの値を返す処理
	bool ReturnDenialButton(XINPUT_STATE aState[],
		int nNum, int nInput);								// ボタン以外の入力情報を返す処理

	// 最大人数は4(XUSER_MAX_COUNT)
	XINPUT_STATE m_aState[XUSER_MAX_COUNT];					// 入力の状態
	XINPUT_STATE m_aStateTrigger[XUSER_MAX_COUNT];			// 入力の状態(トリガー)
	XINPUT_STATE m_aStateRelease[XUSER_MAX_COUNT];			// 入力の状態(リリース)
	bool m_bConnect[XUSER_MAX_COUNT];						// 接続しているかどうか
};
#endif