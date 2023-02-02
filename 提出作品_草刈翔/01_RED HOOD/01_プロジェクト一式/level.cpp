//=============================================================================
// レベル処理 [level.cpp]
// Author : 草刈 翔
//=============================================================================
#include "level.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "level_up.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_LEVEL_POS_X		(50)		// レベル文字の位置座標X
#define MAX_LEVEL_POS_Y		(100)		// レベル文字の位置座標Y
#define MAX_LEVEL_SIZE_X	(60)		// レベル文字のサイズX
#define MAX_LEVEL_SIZE_Y	(30)		// レベル文字のサイズY
#define NUMBER_SIZE_X		(30)		// ナンバーのサイズX
#define NUMBER_SIZE_Y		(60)		// ナンバーのサイズY
#define ICON_SIZE			(50)		// アイコンのサイズ

//=============================================================================
// コンストラクタ
//=============================================================================
CLevel::CLevel()
{
	// 変数のクリア
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_emptyPos = D3DXVECTOR2(0.0f, 0.0f);
	m_emptySize = D3DXVECTOR2(0.0f, 0.0f);
	m_levelPos = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apIconUI, NULL, sizeof(m_apIconUI));
	memset(&m_apIconNumber, NULL, sizeof(m_apIconNumber));
	m_nLevel = 0;
	m_nExp = 0;
	m_nNextLevelExp = 0;
	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL; nCntLevel++)
	{
		m_nNeedExp[nCntLevel] = 0;
	}
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = 0.0f;
	m_bMoment = false;
	m_bDrawNumber = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLevel::~CLevel()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLevel::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen("data/TEXT/exp_line.txt", "r");

	int nCntExp = 0;
	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "NEED_EXP") == 0)	// NEED_EXPの文字列
			{
				// 必要経験値を取得する
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &m_nNeedExp[nCntExp]);
				nCntExp++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPTの文字列
			{
				break;
			}

			for (int nCntNeedExp = nCntExp; nCntNeedExp < MAX_LEVEL; nCntNeedExp++)
			{
				m_nNeedExp[nCntNeedExp] = m_nNeedExp[nCntNeedExp - 1];
			}
		}
		fclose(pFile);
	}

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nLevel = 1;
	m_nExp = 0;
	m_nNextLevelExp = m_nNeedExp[m_nLevel];
	m_fObjectiveSize = m_size.x;
	m_fMaxSize = m_size.x;
	m_bDrawNumber = false;

	m_emptyPos = D3DXVECTOR2(m_pos.x + (m_size.x / 2), m_pos.y);
	m_emptySize = D3DXVECTOR2(m_size.x, m_size.y);
	m_levelPos = D3DXVECTOR2(MAX_LEVEL_POS_X, MAX_LEVEL_POS_Y);

	// 経験値バー(枠)
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(m_size.x + 30.0f, m_size.y + 15.0f), 11, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 経験値バー
	m_apUI[1] = CUI::Create(m_pos, m_size, 15, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 経験値の空部分
	m_apUI[2] = CUI::Create(m_emptyPos, m_emptySize, 13, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);

	// レベル(文字)
	m_apUI[3] = CUI::Create(m_levelPos, D3DXVECTOR2(MAX_LEVEL_SIZE_X, MAX_LEVEL_SIZE_Y), 14, D3DCOLOR_RGBA(230, 0, 0, 255));

	// 数字の生成
	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
	{
		m_apNumber[nCntScore] = CUI::Create(D3DXVECTOR2(m_levelPos.x + (MAX_LEVEL_SIZE_X * 0.9f) + (nCntScore * NUMBER_SIZE_X), m_levelPos.y),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DCOLOR_RGBA(230, 90, 0, 255));
	}

	// アイコンと数字の生成
	for (int nCntIcon = 0; nCntIcon < MAX_LEVEL_ICON_UI; nCntIcon++)
	{
		float fTexType = (float)nCntIcon;

		m_apIconUI[nCntIcon] = CUI::Create(D3DXVECTOR2(m_pos.x + (nCntIcon * ICON_SIZE) + NUMBER_SIZE_X, m_levelPos.y),
			D3DXVECTOR2(ICON_SIZE, ICON_SIZE), 38, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		m_apIconUI[nCntIcon]->SetTexture(fTexType, 0, 3, 1);

		m_apIconNumber[nCntIcon] = CUI::Create(D3DXVECTOR2(m_pos.x + (nCntIcon * ICON_SIZE) + NUMBER_SIZE_X, m_levelPos.y),
			D3DXVECTOR2(NUMBER_SIZE_X * 0.8f, NUMBER_SIZE_Y * 0.8f), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// レベルアップ時の補正をリセット
	CLevelUp::SetCorrectionDef(0);
	CLevelUp::SetCorrectionAtk(0);
	CLevelUp::SetCorrectionSpd(0);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLevel::Uninit(void)
{
	// UIの破棄
	for (int nCntUI = 0; nCntUI < MAX_LEVEL_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// ナンバーの破棄
	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
		}
	}

	// アイコン関連のUIの破棄
	for (int nCntIcon = 0; nCntIcon < MAX_LEVEL_ICON_UI; nCntIcon++)
	{
		if (m_apIconUI[nCntIcon] != NULL)
		{
			m_apIconUI[nCntIcon]->Uninit();
		}
		if (m_apIconNumber[nCntIcon] != NULL)
		{
			m_apIconNumber[nCntIcon]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLevel::Update(void)
{
	m_bMoment = false;
	m_bDrawNumber = false;

	// 現在の経験値が増えるたびに空のゲージを右に動かし、経験値の増加を表現する
	SetEmptyBar();

	for (int nCntNumber = 0; nCntNumber < MAX_LEVEL_DIGIT; nCntNumber++)
	{
		// ＜レベル % 10×n / 10×(n-1)＞ で出た値をそれぞれのナンバーに設定
		//※powf(float x, float y) … xのy乗の値
		m_apNumber[nCntNumber]->SetTexture(float(m_nLevel % (int)powf(10, MAX_LEVEL_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_LEVEL_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}

	for (int nCntNumber = 0, nCntDraw = 0; nCntNumber < MAX_LEVEL_DIGIT; nCntNumber++)
	{
		// 桁毎の数字を取得
		int nNumber = m_nLevel % (int)powf(10, MAX_LEVEL_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_LEVEL_DIGIT - ((float)nCntNumber + 1));

		// 数字が0かつ、数字の描画をしないなら
		if (nNumber == 0 && m_bDrawNumber == false)
		{
			// 透明にし、見えないようにする
			m_apNumber[nCntNumber]->SetColor(D3DCOLOR_RGBA(255, 100, 0, 0));

			// 0の数だけカウントを増やす
			nCntDraw++;
		}
		// 数字が0以外だったら
		else if(nNumber != 0)
		{
			// 透明度を戻す
			m_apNumber[nCntNumber]->SetColor(D3DCOLOR_RGBA(233, 20, 0, 255));
			m_bDrawNumber = true;
		}

		// 数字の左側に0があったぶんだけ空白が開くため、消したぶんだけ左にずらす
		m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(m_levelPos.x + (MAX_LEVEL_SIZE_X * 0.9f) + (nCntNumber * NUMBER_SIZE_X) - (nCntDraw * NUMBER_SIZE_X), m_levelPos.y, 0.0f));
	}

	if (CLevelUp::GetCorrectionAtk() > 0)
	{
		m_apIconUI[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[0]->SetTexture(float(CLevelUp::GetCorrectionAtk() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (CLevelUp::GetCorrectionDef() > 0)
	{
		m_apIconUI[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[1]->SetTexture(float(CLevelUp::GetCorrectionDef() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (CLevelUp::GetCorrectionSpd() > 0)
	{
		m_apIconUI[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[2]->SetTexture(float(CLevelUp::GetCorrectionSpd() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLevel::Draw(void)
{
	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL_UI; nCntLevel++)
	{
		if (m_apUI[nCntLevel] != NULL)
		{
			m_apUI[nCntLevel]->Draw();
		}
	}

	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
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
CLevel *CLevel::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// クラスの生成
	CLevel *pLevel = NULL;
	pLevel = new CLevel;

	if (pLevel != NULL)
	{
		// 初期化処理
		pLevel->Init(pos, size);
	}
	return pLevel;
}

//=============================================================================
// 空部分の設定処理
//=============================================================================
void CLevel::SetEmptyBar()
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 画像のサイズと必要経験値の値から割合を出す
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float fRatio = m_fMaxSize / m_nNeedExp[m_nLevel];

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 現在のレベルにおける、現在の経験値と次レベルまでの合計経験値の差を求める
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int nDiffExp = m_nExp;
	if (nDiffExp >= m_nNextLevelExp)
	{
		nDiffExp = m_nNextLevelExp;
	}

	int nDifferent = nDiffExp - m_nNextLevelExp;
	if (nDifferent < 0)
	{
		nDifferent *= -1;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 必要経験値と上記の変数の差を求める
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int nExp = m_nNeedExp[m_nLevel] - nDifferent;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// <最大サイズ - 割合×現在の経験値>で目的のサイズを出す
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_fObjectiveSize = m_fMaxSize - (fRatio * nExp);

	// 目的のサイズと現在のサイズが違うなら
	if (m_fObjectiveSize != m_emptySize.x)
	{
		// サイズを少しずつ減らす
		m_emptySize.x -= 5.0f;

		// 目的のサイズが0以下かつ、現在のサイズが0以下になったら
		if (m_fObjectiveSize <= 0 && m_emptySize.x <= 0)
		{
			m_bMoment = true;

			// 目的のサイズと現在のサイズを最大サイズに設定
			m_fObjectiveSize = m_fMaxSize;
			m_emptySize.x = m_fMaxSize;
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);

			// サウンド取得処理
			CSound *pSound = CManager::GetSound();

			// レベルアップSEの再生
			pSound->Play(13);
			pSound->ControllVoice(13, 0.8f);

			// レベルアップ
			m_nLevel += 1;

			// 次のレベルまでの合計経験値の値を更新
			m_nNextLevelExp += m_nNeedExp[m_nLevel];

			CLevelUp::Create(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		}
		else
		{
			// 現在のサイズが目的のサイズより小さかったら
			if (m_emptySize.x < m_fObjectiveSize)
			{
				// 現在のサイズを目的のサイズと同じ値に設定
				m_emptySize.x = m_fObjectiveSize;
			}
		}
		
		// 最大レベルまで到達したら
		if (m_nLevel >= MAX_LEVEL)
		{
			// 空部分を消す
			m_fObjectiveSize = 0.0f;
			m_emptySize.x = 0.0f;
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
		}
		// それ以外は普通に更新
		else
		{
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
		}
	}
}