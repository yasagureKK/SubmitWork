//=============================================================================
// クリア処理 [data_clear.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _DATA_CLEAR_H_
#define _DATA_CLEAR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CLEAR_DATA	(3)		// データの最大数
#define CLEAR_RANK_S	(18000)	// Sランクの基準値
#define CLEAR_RANK_A	(13000)	// Aランクの基準値
#define CLEAR_RANK_B	(6000)	// Bランクの基準値

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CDataClear
{
public:
	// クリア情報の構造体
	typedef struct
	{
		int nScore;		// クリアスコア
		char cRank[64];	// クリアランク
		int nTime;		// クリアタイム
		int nLevel;		// クリアレベル
	} CLEAR_DATA;

	CDataClear();										// コンストラクタ
	~CDataClear();										// デストラクタ
	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	static CDataClear *Create(void);					// 生成処理
	void AddScore(int nValue) { m_nScore += nValue; }	// スコア加算処理
	void SetScore(int nScore) { m_nScore = nScore; }	// スコア設定処理
	int GetScore(void) { return m_nScore; }				// スコア取得処理
	int GetTimeBonus(void) { return m_nTimeBonus; }		// タイムボーナス取得処理
	int GetLevelBonus(void) { return m_nLevelBonus; }	// レベルボーナス取得処理
	void SetNewScore(int nTime, int nLevel);			// 新しいスコアの設定
	void SetRanking(CLEAR_DATA data);					// ランキングの設定
	CLEAR_DATA GetRankingData(int nNum);				// ランキングの取得

private:
	void Save(void);						// セーブ処理
	void Load(void);						// ロード処理

	int m_nScore;							// スコア
	int m_nTimeBonus;						// タイムボーナス
	int m_nLevelBonus;						// レベルボーナス
	CLEAR_DATA m_data[MAX_CLEAR_DATA];		// クリアデータ
	CLEAR_DATA m_newData;					// クリアデータ
};
#endif	// _DATA_CLEAR_H_