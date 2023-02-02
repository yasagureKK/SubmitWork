//=============================================================================
// スコア処理 [score.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_DIGIT	(6)					// スコアの最大桁数
#define MAX_SCORE_DATA	(5)					// データの最大数
#define SAVEFILENAME "data/TEXT/score.txt"	// ファイルのパス

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore(int nPriority = PRIORITY_UI);				// コンストラクタ
	~CScore();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CScore *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// 生成処理
	void SetScore(int nScore);							// スコア設定処理
	void AddScore(int nValue);							// スコア追加処理
	int GetScore(void);									// スコア取得処理
	static void SaveScore(void);						// セーブ処理
	static void LoadScore(void);						// ロード処理
	static void SetRankScore(int nNum);					// ランキング設定処理
	void SetScorePos(D3DXVECTOR3 pos);					// スコア位置設定処理
	void SetNum(int nNum);								// スコア番号設定処理
	int GetNum(void);									// スコア番号取得処理
	void SetResultScore(int nNum);						// リザルトのスコア設定処理
	int GetResultScore(int nNum);						// リザルトのスコア取得処理
	void SetHighScore(int nHighScore);					// ハイスコア設定処理

private:
	CNumber *m_apNumber[MAX_SCORE_DIGIT];				// ナンバーのポインタ
	CUi *m_apUi[2];										// UIのポインタ
	int m_nScore;										// スコア
	static int m_nResultScore[2];						// リザルトスコア
	static int m_aScore[MAX_SCORE_DATA];				// ランキングに保存するスコア
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXCOLOR m_col;									// カラー
	int m_nNum;											// スコア番号
};

#endif	//_SCORE_H_