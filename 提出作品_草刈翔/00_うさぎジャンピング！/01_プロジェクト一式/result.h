//=============================================================================
// リザルト画面 [result.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_MAX_UI			(8)			// UIの最大数
#define RESULT_MAX_PLAYER		(2)			// 最大人数
#define RESULT_UI_POSITION_Y	(220.0f)	// リザルト文字のY座標位置
#define RESULT_UI_SIZE_X		(520.0f)	// リザルト文字のサイズX
#define RESULT_UI_SIZE_Y		(80.0f)		// リザルト文字のサイズY
#define RESULT_UI_BAND_POS_Y	(75.0f)		// リザルト帯のY座標位置
#define RESULT_UI_BAND_SIZE_Y	(150.0f)	// リザルト帯のサイズY
#define RESULT_UI_TOP_POS_Y		(60.0f)		// リザルトトップ文字のY座標位置
#define RESULT_UI_TOP_SIZE_X	(400.0f)	// リザルトトップ文字のサイズX
#define RESULT_UI_TOP_SIZE_Y	(110.0f)	// リザルトトップ文字のサイズY
#define RESULT_UI_1P_POS_X		(360.0f)	// 1P表示の位置座標X
#define RESULT_UI_1P_POS_Y		(210.0f)	// 1P表示の位置座標Y
#define RESULT_UI_1P_SIZE_X		(190.0f)	// 1P表示のサイズX
#define RESULT_UI_1P_SIZE_Y		(90.0f)		// 1P表示のサイズY
#define RESULT_UI_WIN_SIZE_X	(250.0f)	// 「かち！」表示のサイズX
#define RESULT_UI_DRAW_SIZE_X	(400.0f)	// 「ひきわけ…」表示のサイズX

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUi;
class CBillboard;
class CPlayer;
class CScore;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();											// コンストラクタ
	~CResult();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

private:
	CUi *m_apUi[RESULT_MAX_UI];							// UIのポインタ
	CBillboard *m_pBillboard;							// 2Dオブジェクトのポインタ
	CPlayer *m_pPlayer[MAX_PLAYER_NUM];					// プレイヤーのポインタ
	CScore *m_pScore[RESULT_MAX_PLAYER];				// スコアのポインタ
};

#endif	// _RESULT_H_