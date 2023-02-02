//=============================================================================
// タイトルロゴコントロール処理 [control_title_logo.cpp]
// Author : KUSAKARI KAKERU
//=============================================================================
#include "control_ui_title_logo.h"
#include "manager.h"
#include "ui.h"
#include "title.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CControlUiTitleLogo::CControlUiTitleLogo()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CControlUiTitleLogo::~CControlUiTitleLogo()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlUiTitleLogo::Init(void)
{
	m_fMaxMove = 1.5f;
	m_bOnce = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlUiTitleLogo::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlUiTitleLogo::	Update(CScene *pScene)
{
	// 引数のポインタをUIクラスのポインタにキャスト
	CUi *pUi = NULL;
	pUi = (CUi*)pScene;

	//オブジェクトの位置・サイズを取得
	D3DXVECTOR3 pos;
	pos = pUi->GetPos();
	D3DXVECTOR3 size;
	size = pUi->GetSize();

	if (m_bOnce == false && pos.y >= SCREEN_HEIGHT)
	{
		m_move.y -= TITLE_LOGO_RISE;
		m_bOnce = true;
	}
	else if (m_move.y >= 0.0f && pos.y >= TITLE_LOGO_POS_Y)
	{
		m_move.y = 0.0f;
		m_move.y -= TITLE_LOGO_RISE_SECOND;
	}
	else
	{
		//常に下に下げる
		m_move.y += TITLE_LOGO_GRAVITY;
	}

	// 位置やサイズの設定
	pUi->SetPos(pos, size);
}

//=============================================================================
// 生成処理
//=============================================================================
CControlUiTitleLogo *CControlUiTitleLogo::Create()
{
	//インスタンスの生成
	CControlUiTitleLogo *pControlTitleLogo = NULL;

	if (pControlTitleLogo == NULL)
	{
		pControlTitleLogo = new CControlUiTitleLogo;

		if (pControlTitleLogo != NULL)
		{
			pControlTitleLogo->Init();
		}
	}

	return pControlTitleLogo;
}