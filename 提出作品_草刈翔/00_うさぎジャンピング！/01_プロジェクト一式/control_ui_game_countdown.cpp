//=============================================================================
// ゲームUI：カウントダウン操作処理 [control_ui_game_countdown.h]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "control_ui_game_countdown.h"
#include "manager.h"
#include "ui.h"
#include "scene.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CControlUiGameCountdown::m_nCount = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CControlUiGameCountdown::CControlUiGameCountdown()
{
	// 変数のクリア
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlUiGameCountdown::~CControlUiGameCountdown()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlUiGameCountdown::Init(void)
{
	// 変数の初期化
	m_nCount = 3;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlUiGameCountdown::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlUiGameCountdown::Update(CScene *pScene)
{
	// 引数のポインタをUIクラスのポインタにキャスト
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;
	
	switch (m_nCount)
	{
	case 3:
		pUi->SetTexture(0.0f, 0.0f, 1, 4);
		break;

	case 2:
		pUi->SetTexture(0.0f, 1.0f, 1, 4);
		break;

	case 1:
		pUi->SetTexture(0.0f, 2.0f, 1, 4);
		break;

	case 0:
		pUi->SetTexture(0.0f, 3.0f, 1, 4);
		break;

	default:
		break;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CControlUiGameCountdown *CControlUiGameCountdown::Create(void)
{
	// インスタンスの生成
	CControlUiGameCountdown *pControlUiGameCountdown = NULL;

	// ヌルチェック
	if (pControlUiGameCountdown == NULL)
	{
		// クラスの生成
		pControlUiGameCountdown = new CControlUiGameCountdown;
		if (pControlUiGameCountdown != NULL)
		{
			// 初期化処理
			pControlUiGameCountdown->Init();
		}
	}

	return pControlUiGameCountdown;
}

//=============================================================================
// カウント設定処理
//=============================================================================
void CControlUiGameCountdown::SetCount(int nCount)
{
	m_nCount = nCount;
}