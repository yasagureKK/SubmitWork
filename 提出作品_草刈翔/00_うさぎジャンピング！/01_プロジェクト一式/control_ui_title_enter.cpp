//=============================================================================
// タイトルUI：エンター文字操作処理 [control_ui_title_enter.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_ui_title_enter.h"
#include "manager.h"
#include "ui.h"
#include "scene.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CControlUiTitleEnter::CControlUiTitleEnter()
{
	// 変数のクリア
	m_nCount = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bAlphaUp = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlUiTitleEnter::~CControlUiTitleEnter()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlUiTitleEnter::Init(void)
{
	// 変数の初期化
	m_nCount = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bAlphaUp = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlUiTitleEnter::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlUiTitleEnter::Update(CScene *pScene)
{
	// 引数のポインタをUIクラスのポインタにキャスト
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;

	m_nCount++;

	// アルファ値を下げる(透明にする)
	if (m_bAlphaUp == false)
	{
		if (m_nCount % 2 == 0)
		{
			m_col.a += -0.05f;
		}

		if (m_col.a < 0)
		{
			m_nCount = 0;
			m_col.a = 0;
			m_bAlphaUp = true;
		}
	}
	// アルファ値を上げる(不透明にする)
	else if (m_bAlphaUp == true)
	{
		if (m_nCount % 2 == 0)
		{
			m_col.a += 0.05f;
		}

		if (m_col.a > 1.0f && m_nCount >= 70)
		{
			m_nCount = 0;
			m_col.a = 1.0f;
			m_bAlphaUp = false;
		}
	}

	// m_fAlphaの値を透明度に入れる
	pUi->SetColor(m_col);
}

//=============================================================================
// 生成処理
//=============================================================================
CControlUiTitleEnter *CControlUiTitleEnter::Create(void)
{
	// インスタンスの生成
	CControlUiTitleEnter *pControlUiTitleEnter = NULL;

	// ヌルチェック
	if (pControlUiTitleEnter == NULL)
	{
		// クラスの生成
		pControlUiTitleEnter = new CControlUiTitleEnter;
		if (pControlUiTitleEnter != NULL)
		{
			// 初期化処理
			pControlUiTitleEnter->Init();
		}
	}

	return pControlUiTitleEnter;
}