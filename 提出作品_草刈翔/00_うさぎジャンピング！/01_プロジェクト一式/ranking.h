//=============================================================================
// ランキング画面 [ranking.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_FADE_COUNT (1800)		// 自動遷移までのカウント
#define RANKING_MAX_DATA (5)			// ランキングデータの最大数
#define RANKING_MAX_UI	(7)				// UIの最大数
#define RANKING_TOP_POS_X (250.0f)		// ランキングトップのX座標位置
#define RANKING_TOP_POS_Y (70.0f)		// ランキングトップのY座標位置
#define RANKING_TOP_SIZE_X (750.0f)		// ランキングトップのサイズX
#define RANKING_TOP_SIZE_Y (120.0f)		// ランキングトップのサイズY
#define RANKING_FRAME_POS_Y (180.0f)	// 順位毎の枠の基準位置Y
#define RANKING_FRAME_SIZE_X (900.0f)	// 順位毎の枠のサイズX
#define RANKING_FRAME_SIZE_Y (100.0f)	// 順位毎の枠のサイズY
#define RANKING_DATA_POS_X (520.0f)		// ランキングの数字のX座標位置

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CUi;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();											// コンストラクタ
	~CRanking();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

private:
	static float m_fScroll;								// 背景スクロール用の変数
	static CScore *m_apScore[MAX_SCORE_DATA];			// スコアのポインタ
	static int m_nRankingData[RANKING_MAX_DATA];		// ランキングのスコア
	int m_nFadeCount;									// 画面遷移用のカウント
	CScene2D *m_pScene2D;								// 2Dオブジェクトのポインタ
	CUi *m_apUi[RANKING_MAX_UI];						// UIのポインタ
};

#endif	// _RANKING_H_