//=============================================================================
// スコア処理 [score.cpp]
// Author : 草刈 翔
//=============================================================================
#include "score.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "data_clear.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUMBER_SIZE_X	(30)	// ナンバーのサイズX
#define NUMBER_SIZE_Y	(60)	// ナンバーのサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	// 変数のクリア
	m_nScore = 0;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
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
HRESULT CScore::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nScore = 0;

	// スコア文字
	m_apUI = CUI::Create(m_pos, m_size, 37, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ナンバーの生成
	for (int nCntTime = 0; nCntTime < MAX_SCORE_DIGIT; nCntTime++)
	{
		m_apNumber[nCntTime] = CUI::Create(D3DXVECTOR2(m_pos.x + (m_size.x / 1.8f) + (nCntTime * (NUMBER_SIZE_X * 0.9f)), m_pos.y),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	// UIの処理
	m_apUI->Uninit();

	// ナンバーの破棄
	for (int nCntTime = 0; nCntTime < MAX_SCORE_DIGIT; nCntTime++)
	{
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->Uninit();
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
	m_nScore = CManager::GetDataClear()->GetScore();

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber]->SetTexture(float(m_nScore % (int)powf(10, MAX_SCORE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	m_apUI->Draw();

	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CScore *CScore::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// クラスの生成
	CScore *pTime = NULL;
	pTime = new CScore;

	if (pTime != NULL)
	{
		// 初期化処理
		pTime->Init(pos, size);
	}
	return pTime;
}