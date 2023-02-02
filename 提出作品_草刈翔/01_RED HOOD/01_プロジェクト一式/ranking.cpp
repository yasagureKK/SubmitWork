//=============================================================================
// ランキング処理 [ranking.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "mouse.h"
#include "sound.h"
#include "data_clear.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_RANKING_NUMBER_POS_X		(300.0f)	// ナンバーの位置X
#define UI_RANKING_NUMBER_POS_Y		(240.0f)	// ナンバーの位置Y
#define UI_RANKING_NUMBER_SIZE_X	(50.0f)		// ナンバーのサイズX
#define UI_RANKING_NUMBER_SIZE_Y	(100.0f)	// ナンバーのサイズY
#define UI_RANKING_RANK_POS_X		(850.0f)	// ランクの位置X
#define UI_RANKING_RANK_POS_Y		(240.0f)	// ランクの位置Y
#define UI_RANKING_RANK_SIZE_X		(250.0f)	// ランクのサイズX
#define UI_RANKING_RANK_SIZE_Y		(110.0f)	// ランクのサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	memset(&m_apBgUI, NULL, sizeof(m_apBgUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apRankUI, NULL, sizeof(m_apRankUI));
	m_bPad = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos)
{
	float fBacTex = 0.0f;

	if (m_bPad == true)
	{
		fBacTex = 1.0f;
	}

	m_apBgUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 28, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apBgUI[1] = CUI::Create(D3DXVECTOR2(240.0f, 660.0f), D3DXVECTOR2(430.0f, 80.0f), 29, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apBgUI[1]->SetTexture(0, fBacTex, 1, 2);

	for (int nCntData = 0; nCntData < RANKING_DATA; nCntData++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_RANKING_DIGIT; nCntNumber++)
		{
			int nScore = 0;
			CDataClear::CLEAR_DATA data;
			data = CManager::GetDataClear()->GetRankingData(nCntData);

			m_apNumber[nCntData][nCntNumber] = CUI::Create(D3DXVECTOR2(UI_RANKING_NUMBER_POS_X + (UI_RANKING_NUMBER_SIZE_X / 1.5f) + (nCntNumber * (UI_RANKING_NUMBER_SIZE_X)), UI_RANKING_NUMBER_POS_Y + nCntData * 140.0f),
				D3DXVECTOR2(UI_RANKING_NUMBER_SIZE_X, UI_RANKING_NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[nCntData][nCntNumber]->SetTexture(float(data.nScore % (int)powf(10, MAX_RANKING_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_RANKING_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
		}
	}

	for (int nCntRank = 0; nCntRank < RANKING_DATA; nCntRank++)
	{
		float fRank = 0.0f;
		CDataClear::CLEAR_DATA data;
		data = CManager::GetDataClear()->GetRankingData(nCntRank);

		m_apRankUI[nCntRank] = CUI::Create(D3DXVECTOR2(UI_RANKING_RANK_POS_X, UI_RANKING_RANK_POS_Y + nCntRank * 140.0f),
			D3DXVECTOR2(UI_RANKING_RANK_SIZE_X, UI_RANKING_RANK_SIZE_Y), 30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (strcmp(&data.cRank[0], "S") == 0)	//Sランク
		{
			fRank = 0.0f;
		}
		else if (strcmp(&data.cRank[0], "A") == 0)	//Aランク
		{
			fRank = 1.0f;
		}
		else if (strcmp(&data.cRank[0], "B") == 0)	//Bランク
		{
			fRank = 2.0f;
		}
		else//Cランク
		{
			fRank = 3.0f;
		}
		m_apRankUI[nCntRank]->SetTexture(0, fRank, 1, 4);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCntBG = 0; nCntBG < RANKING_BG_UI; nCntBG++)
	{
		m_apBgUI[nCntBG]->Uninit();
	}

	for (int nCntData = 0; nCntData < RANKING_DATA; nCntData++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_RANKING_DIGIT; nCntNumber++)
		{
			m_apNumber[nCntData][nCntNumber]->Uninit();
		}
	}

	for (int nCntRank = 0; nCntRank < RANKING_DATA; nCntRank++)
	{
		m_apRankUI[nCntRank]->Uninit();
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, bool bPad)
{
	// クラスの生成
	CRanking *pRanking = NULL;
	pRanking = new CRanking;

	if (pRanking != NULL)
	{
		pRanking->m_bPad = bPad;
		// 初期化処理
		pRanking->Init(pos);
	}
	return pRanking;
}