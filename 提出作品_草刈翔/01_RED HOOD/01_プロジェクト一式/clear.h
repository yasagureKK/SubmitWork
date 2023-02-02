//=============================================================================
// クリア処理 [clear.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CLEAR_UI		(7)	// UIの最大数
#define MAX_CLEAR_SCORE_UI	(4)	// スコアUIの種類数
#define MAX_CLEAR_DIGIT		(5)	// 最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CClear : public CUI
{
public:
	// UI表示の段階
	typedef enum
	{
		CLEAR_FASE_NONE = 0,	// なし
		CLEAR_FASE_TOP,			// トップ文字
		CLEAR_FASE_SCORE,		// スコア
		CLEAR_FASE_TIME,		// タイムボーナス
		CLEAR_FASE_LEVEL,		// レベルボーナス
		CLEAR_FASE_TOTAL,		// 最終スコア
		CLEAR_FASE_RANK,		// ランク
		CLEAR_FASE_FADE,		// 画面遷移
		CLEAR_FASE_MAX
	} CLEAR_FASE;

	CClear();											// コンストラクタ
	~CClear();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CClear *Create(void);						// 生成処理

private:
	CUI *m_apUI[MAX_CLEAR_UI];								// UIのポインタ
	CUI *m_apNumber[MAX_CLEAR_DIGIT][MAX_CLEAR_SCORE_UI];	// 数字をつくるUIのポインタ
	int m_nAlpha;											// アルファ度
	int m_nClearCount;										// UI表示のカウント
	int m_nTimeBonus;										// タイムボーナス
	int m_nLevelBonus;										// レベルボーナス
	int m_nScore;											// スコア
	bool m_bDisplay;										// 画像を表示するかどうか
	CLEAR_FASE m_phase;										// UI表示の段階
};
#endif	//_CLEAR_H_