//=============================================================================
// タイム処理 [time.cpp]
// Author : 草刈 翔
//=============================================================================
#include "time.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUMBER_SIZE_X	(40)	// ナンバーのサイズX
#define NUMBER_SIZE_Y	(80)	// ナンバーのサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CTime::CTime()
{
	// 変数のクリア
	m_nTime = 0;
	m_nNumber = 0;
	m_bTimeStop = false;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
}

//=============================================================================
// デストラクタ
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTime::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nTime = 0;
	m_nNumber = 0;
	
	// タイム文字
	m_apUI = CUI::Create(m_pos, m_size, 27, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ナンバーの生成
	for (int nCntTime = 0; nCntTime < MAX_TIME_DIGIT; nCntTime++)
	{
		m_apNumber[nCntTime] = CUI::Create(D3DXVECTOR2(m_pos.x + (m_size.x / 1.5f) + (nCntTime * (NUMBER_SIZE_X * 0.9f)), m_pos.y - (m_size.y * 0.1f)),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTime::Uninit(void)
{
	// UIの処理
	m_apUI->Uninit();

	// ナンバーの破棄
	for (int nCntTime = 0; nCntTime < MAX_TIME_DIGIT; nCntTime++)
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
void CTime::Update(void)
{
	if (m_bTimeStop == false)
	{
		//制限時間減らす
		m_nNumber++;
	}

	//60フレーム経ったとき(＝1秒毎)
	if (m_nNumber != 0 && m_nNumber % 60 == 0)
	{
		if (m_nTime < MAX_TIME)
		{
			m_nTime++;
		}
	}

	for (int nCntNumber = 0; nCntNumber < MAX_TIME_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber]->SetTexture(float(m_nTime % (int)powf(10, MAX_TIME_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_TIME_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTime::Draw(void)
{
	m_apUI->Draw();

	for (int nCntScore = 0; nCntScore < MAX_TIME_DIGIT; nCntScore++)
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
CTime *CTime::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// クラスの生成
	CTime *pTime = NULL;
	pTime = new CTime;

	if (pTime != NULL)
	{
		// 初期化処理
		pTime->Init(pos, size);
	}
	return pTime;
}