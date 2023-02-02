//=============================================================================
// クリア処理 [clear.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "data_clear.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAVEFILENAME "data/TEXT/clear_save.txt"	// ファイルのパス

//=============================================================================
// コンストラクタ
//=============================================================================
CDataClear::CDataClear()
{
	// 変数のクリア
	m_nScore = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	memset(&m_data, NULL, sizeof(m_data));
	memset(&m_newData, NULL, sizeof(m_newData));
}

//=============================================================================
// デストラクタ
//=============================================================================
CDataClear::~CDataClear()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDataClear::Init(void)
{
	// 変数の初期化
	m_nScore = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	memset(&m_data, NULL, sizeof(m_data));
	memset(&m_newData, NULL, sizeof(m_newData));

	Load();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDataClear::Uninit(void)
{
	
}

//=============================================================================
// 生成処理
//=============================================================================
CDataClear *CDataClear::Create(void)
{
	// クラスの生成
	CDataClear *pDataClear = NULL;
	pDataClear = new CDataClear;
	
	if (pDataClear != NULL)
	{
		// 初期化処理
		pDataClear->Init();
	}
	return pDataClear;
}

//=============================================================================
// セーブ処理
//=============================================================================
void CDataClear::Save(void)
{
	// ファイルを開く
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "w");

	// ヌルチェック
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_CLEAR_DATA; nCount++)
		{
			// ランクのパラメータがNULLだったら
			if (m_data[nCount].cRank[0] == NULL)
			{
				// "C"の文字列を入れておく
				strcpy(&m_data[nCount].cRank[0], "C");
			}

			// ランキングデータをテキストファイルにコピー
			fprintf(pFile, "%s\n", "RANK");
			fprintf(pFile, "%s%d\n", "	SCORE = ", m_data[nCount].nScore);
			fprintf(pFile, "%s%s\n", "	RANK = ", m_data[nCount].cRank);
			fprintf(pFile, "%s\n\n", "RANK_END");
		}
		fprintf(pFile, "%s", "END_SCRIPT		# この行は絶対消さないこと！");

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// ロード処理
//=============================================================================
void CDataClear::Load(void)
{
	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen(SAVEFILENAME, "r");

	if (pFile != NULL)
	{
		int nCntData = 0;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "RANK") == 0)	//RANKの文字列
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "SCORE") == 0) //スコア
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_data[nCntData].nScore);
					}
					if (strcmp(&cString[0], "RANK") == 0) //ランク
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%s", &m_data[nCntData].cRank[0]);
					}
					if (strcmp(&cString[0], "RANK_END") == 0)
					{
						break;
					}
				}

				nCntData++;
			}

			// データが最大数になったら読み込みを終了
			if (nCntData >= MAX_CLEAR_DATA)
			{
				break;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// データ設定処理
//=============================================================================
void CDataClear::SetNewScore(int nTime, int nLevel)
{
	m_newData.nTime = nTime;
	m_newData.nLevel = nLevel;

	// タイムボーナス、レベルボーナスを設定
	m_nTimeBonus = (999 - nTime) * 10;
	m_nLevelBonus = nLevel * 100;

	// 既存のスコアと合計して、新しいスコアを設定
	m_newData.nScore = m_nScore + m_nTimeBonus + m_nLevelBonus;

	// スコアが18000点以上でSランク
	if (m_newData.nScore >= CLEAR_RANK_S)
	{
		strcpy(&m_newData.cRank[0], "S");
	}
	// スコアが13000点以上、18000点未満でAランク
	else if (m_newData.nScore >= CLEAR_RANK_A && m_newData.nScore < CLEAR_RANK_S)
	{
		strcpy(&m_newData.cRank[0], "A");
	}
	// スコアが6000点以上、13000点未満でBランク
	else if (m_newData.nScore >= CLEAR_RANK_B && m_newData.nScore < CLEAR_RANK_A)
	{
		strcpy(&m_newData.cRank[0], "B");
	}
	// スコアが6000点未満でCランク
	else if (m_newData.nScore < CLEAR_RANK_B)
	{
		strcpy(&m_newData.cRank[0], "C");
	}

	SetRanking(m_newData);
}

//=============================================================================
// ランキング設定処理
//=============================================================================
void CDataClear::SetRanking(CLEAR_DATA data)
{
	// ランキングデータの更新
	for (int nCnt = 0; nCnt < MAX_CLEAR_DATA; nCnt++)
	{
		// 新しいスコアが既存のデータより高かったら
		if (data.nScore > m_data[nCnt].nScore)
		{
			// 該当したデータまで一番後ろからループ
			for (int nCnt2 = MAX_CLEAR_DATA - 1; nCnt2 > nCnt; nCnt2--)
			{
				// 後ろのデータを1つ前のデータで上書き
				m_data[nCnt2] = m_data[nCnt2 - 1];
				strcpy(&m_data[nCnt2].cRank[0], &m_data[nCnt2 - 1].cRank[0]);
			}

			// ランキングデータに新しいデータを入れる
			m_data[nCnt] = data;
			strcpy(&m_data[nCnt].cRank[0], &data.cRank[0]);

			break;
		}
	}

	// データのセーブ
	Save();
}

//=============================================================================
// ランキング取得処理
//=============================================================================
CDataClear::CLEAR_DATA CDataClear::GetRankingData(int nNum)
{
	return m_data[nNum];
}