//=============================================================================
// スコア処理 [score.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "ui.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CScore::m_aScore[MAX_SCORE_DATA] = {};
int CScore::m_nResultScore[2] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNum = 99;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;

	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		// ナンバーの生成
		m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)), m_pos.y, m_pos.z), m_size, m_col);

		// 小数点をつけるため、生成位置を途中からずらす
		if (nCntScore >= MAX_SCORE_DIGIT - 1)
		{
			// 小数点の生成
			m_apUi[0] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4) - (m_size.x / 4)), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_COMMA);
			m_apUi[0]->SetColor(m_col);

			// ナンバーの生成
			m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)) + (m_size.x / 3), m_pos.y, m_pos.z), m_size, m_col);

			// m(メートル)の生成
			m_apUi[1] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * m_size.x) + (m_size.x / 5 * 1), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_METER);
			m_apUi[1]->SetColor(m_col);
		}
	}

	// 番号が設定されていないスコア(ハイスコアのスコア)
	if (m_nNum == 99)
	{
		// ランキングの一番目のスコアを入れる
		LoadScore();
		m_nScore = m_aScore[0];
	}
	// プレイヤー毎のスコアはリセット
	else
	{
		m_nScore = 0;
	}

	// 位置・サイズの設定
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	// ナンバーの破棄
	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		// ヌルチェック
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;		
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// プレイヤーの位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// スコアの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum() && pPlayer->GetLandFinish() == false)
			{
				// リザルト画面でも反映するのを防ぐ
				if (CManager::GetMode() != CManager::MODE_RESULT)
				{
					// 正の数になる場合
					if (pos.x >= 0)
					{
						// プレイヤーのX方向の位置をスコアに設定
						//※そのままの数値だと大味すぎたため、2分の1にしている
						m_nScore = (int)pos.x / 2;
					}
					else
					{
						m_nScore = 0;
					}
				}
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_DIGIT - 1)
		{
			// 最後は10のn乗が使えなくなるため、決め打ちで処理
			m_apNumber[nCntNumber]->SetNumber(m_nScore % 10/ 1);
		}
		else
		{
			// ＜スコア % 10×n / 10×(n-1)＞ で出た値をそれぞれのナンバーに設定
			//※powf(float x, float y) … xのy乗の値
			m_apNumber[nCntNumber]->SetNumber(m_nScore % (int)powf(10, MAX_SCORE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_DIGIT - ((float)nCntNumber + 1)));
		}
	}

	// ゲーム画面なら
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// スコアを更新
		SetScore(m_nScore);
	}
	// リザルト画面なら
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		// スコアを更新
		SetScore(m_nResultScore[m_nNum]);

		// 直前のモードがCPU対戦・2人用モードだったら
		if (CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
		{
			// 1Pのスコアが高かったら(1Pのスコア番号は0)
			if (m_nResultScore[0] > m_nResultScore[1] && m_nNum == 0)
			{
				// 透明度を薄くする
				m_col.a -= 255;
				if (m_col.a < 0)
				{
					// 透明度を元に戻す
					m_col.a = 255;
				}

				m_apUi[0]->SetColor(m_col);
				m_apUi[1]->SetColor(m_col);
				for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
				{
					m_apNumber[nCntNumber]->SetColor(m_col);
				}
			}
			// 2Pのスコアが高かったら(2Pのスコア番号は1)
			else if (m_nResultScore[0] < m_nResultScore[1] && m_nNum == 1)
			{
				//カラーを薄くする
				m_col.a -= 255;
				if (m_col.a < 0)
				{
					// 透明度を元に戻す
					m_col.a = 255;
				}

				m_apUi[0]->SetColor(m_col);
				m_apUi[1]->SetColor(m_col);
				for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
				{
					m_apNumber[nCntNumber]->SetColor(m_col);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		// ヌルチェック
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// インスタンスの生成
	CScore *pScore = NULL;

	// ヌルチェック
	if (pScore == NULL)
	{
		pScore = new CScore;
		if (pScore != NULL)
		{
			// 変数の設定
			pScore->m_col = col;

			// 初期化処理
			pScore->Init(pos, size);
		}
	}
	return pScore;
}

//=============================================================================
// スコア設定処理
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// スコア追加処理
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
}

//=============================================================================
// スコア取得処理
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
// セーブ処理
//=============================================================================
void CScore::SaveScore(void)
{
	// ファイルを開く
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "w");

	// ヌルチェック
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_SCORE_DATA; nCount++)
		{
			// ランキングデータをテキストファイルにコピー
			fprintf(pFile, "%d\n", m_aScore[nCount]);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// ロード処理
//=============================================================================
void CScore::LoadScore(void)
{
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "r");

	// ヌルチェック
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_SCORE_DATA; nCount++)
		{
			// ランキングデータを読み込む
			fscanf(pFile, "%d", &m_aScore[nCount]);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// ランキング設定処理
//=============================================================================
void CScore::SetRankScore(int nNum)
{
	if (nNum != NULL)
	{
		// ランキングデータの更新
		for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
		{
			// 新しいスコアが既存のデータより高かったら
			if (nNum > m_aScore[nCnt])
			{
				// 該当したデータまで一番後ろからループ
				for (int nCnt2 = MAX_SCORE_DATA - 1; nCnt2 > nCnt; nCnt2--)
				{
					// 後ろのデータを1つ前のデータで上書き
					m_aScore[nCnt2] = m_aScore[nCnt2 - 1];
				}

				// ランキングデータに新しいスコアを入れる
				m_aScore[nCnt] = nNum;

				break;
			}
		}
	}
}

//=============================================================================
// スコア位置設定処理
//=============================================================================
void CScore::SetScorePos(D3DXVECTOR3 pos)
{
	//位置設定
	m_pos = pos;
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		//小数点をつけるため、4つ目の数字からずらす
		if (nCntNumber >= MAX_SCORE_DIGIT / 2)
		{
			m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(pos.x + 20.0f - (m_size.x * MAX_SCORE_DIGIT / 2) + (m_size.x * nCntNumber) + (m_size.x / 2.0f), pos.y, 0.0f));
		}
		else
		{
			m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(pos.x - (m_size.x * MAX_SCORE_DIGIT / 2) + (m_size.x * nCntNumber) + (m_size.x / 2.0f), pos.y, 0.0f));
		}
	}

	//位置を設定
	CScene::SetPos(m_pos);
}

//=============================================================================
// スコア番号設定処理
//=============================================================================
void CScore::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// スコア番号取得処理
//=============================================================================
int CScore::GetNum(void)
{
	return m_nNum;
}

//=============================================================================
// リザルトのスコア設定処理
//=============================================================================
void CScore::SetResultScore(int nNum)
{
	m_nResultScore[nNum] = m_nScore;
}

//=============================================================================
// リザルトのスコア取得処理
//=============================================================================
int CScore::GetResultScore(int nNum)
{
	return m_nResultScore[nNum];
}

//=============================================================================
// ハイスコア設定処理
//=============================================================================
void CScore::SetHighScore(int nHighScore)
{
	m_nScore = nHighScore;
}