//=============================================================================
// ゲームパッド入力処理 [input_pad.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _INPUT_PAD_H_
#define _INPUT_PAD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CInputPad : public CInput
{
public:
	typedef enum {
		PAD_INPUTTYPE_LSTICK_UP = 0,
		PAD_INPUTTYPE_LSTICK_DOWN,
		PAD_INPUTTYPE_LSTICK_LEFT,
		PAD_INPUTTYPE_LSTICK_RIGHT,
		PAD_INPUTTYPE_RSTICK_UP,
		PAD_INPUTTYPE_RSTICK_DOWN,
		PAD_INPUTTYPE_RSTICK_LEFT,
		PAD_INPUTTYPE_RSTICK_RIGHT,
		PAD_INPUTTYPE_LEFT,
		PAD_INPUTTYPE_RIGHT,
		PAD_INPUTTYPE_MAX
	} PAD_INPUTTYPE;

	CInputPad();									// コンストラクタ
	~CInputPad();									// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	bool GetPress(int nInput, int nNum);			// 入力状態の取得処理(プレス)
	bool GetTrigger(int nInput, int nNum);			// 入力状態の取得処理(トリガー)
	bool GetButtonPress(int nButton, int nNum);		// ボタン入力状態の取得処理(プレス)
	bool GetButtonTrigger(int nButton, int nNum);	// ボタン入力状態の取得処理(トリガー)

private:
	// 最大人数は4(XUSER_MAX_COUNT)だが、今作は2人ぶん用意
	XINPUT_STATE m_aState[MAX_PLAYER_NUM];			// 入力の状態
	XINPUT_STATE m_aStateTrigger[MAX_PLAYER_NUM];	// 入力の状態(トリガー)
	XINPUT_VIBRATION m_aVibration[MAX_PLAYER_NUM];	// バイブレーション
	int m_CntVibration[MAX_PLAYER_NUM];
	bool m_bConnect[MAX_PLAYER_NUM];				// 接続しているかどうか
	bool m_bVibration[MAX_PLAYER_NUM];
};

#endif  // _INPUT_PAD_H_