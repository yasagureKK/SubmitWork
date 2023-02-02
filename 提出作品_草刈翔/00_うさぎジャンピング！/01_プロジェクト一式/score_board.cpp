//=============================================================================
// 看板に表示させるスコア処理 [score_board.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "score_board.h"
#include "number3D.h"
#include "scene.h"
#include "player.h"
#include "ui.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScoreBoard::CScoreBoard(int nPriority) : CScene3D(nPriority)
{
	memset(&m_apNumber3D, NULL, sizeof(m_apNumber3D));
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScoreBoard::~CScoreBoard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScoreBoard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DCOLOR col)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;

	for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	{
		// ナンバーの生成
		m_apNumber3D[nCntScore] = CNumber3D::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x - 10.0f)) - (1.5f * (m_size.x - 10.0f)), m_pos.y, m_pos.z), m_size, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col);
	}

	m_nScore = (int)m_pos.x / 20;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScoreBoard::Uninit(void)
{
	//ナンバーの破棄
	//for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	//{
	//	// ヌルチェック
	//	if (m_apNumber3D[nCntScore] != NULL)
	//	{
	//		m_apNumber3D[nCntScore]->Uninit();
	//		delete m_apNumber3D[nCntScore];
	//		m_apNumber3D[nCntScore] = NULL;
	//	}
	//}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScoreBoard::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_BOARD_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_BOARD_DIGIT - 1)
		{
			// 最後は10のn乗が使えなくなるため、決め打ちで処理
			m_apNumber3D[nCntNumber]->SetNumber(m_nScore % 10 / 1);
		}
		else
		{
			// ＜スコア % 10×n / 10×(n-1)＞ で出た値をそれぞれのナンバーに設定
			//※powf(float x, float y) … xのy乗の値
			m_apNumber3D[nCntNumber]->SetNumber(m_nScore % (int)powf(10, MAX_SCORE_BOARD_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_BOARD_DIGIT - ((float)nCntNumber + 1)));
		}

		// 位置の設定・スコアの更新
		m_apNumber3D[nCntNumber]->SetPos(D3DXVECTOR3(m_pos.x + (nCntNumber * (m_size.x - 10.0f)) - (1.5f * (m_size.x - 10.0f)), m_pos.y, m_pos.z));
		m_nScore = (int)m_pos.x / 20;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScoreBoard::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	{
		// ヌルチェック
		if (m_apNumber3D[nCntScore] != NULL)
		{
			m_apNumber3D[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CScoreBoard *CScoreBoard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DCOLOR col)
{
	// インスタンスの生成
	CScoreBoard *pScoreBoard = NULL;

	// ヌルチェック
	if (pScoreBoard == NULL)
	{
		pScoreBoard = new CScoreBoard;
		if (pScoreBoard != NULL)
		{
			// 初期化処理
			pScoreBoard->Init(pos, size, col);
		}
	}
	return pScoreBoard;
}

//=============================================================================
// 位置設定処理
//=============================================================================
void CScoreBoard::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}