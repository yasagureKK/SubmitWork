//=============================================================================
// フェード処理 [fade.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "fade.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CFade::FADE CFade::m_fade = FADE_NONE;

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade(int nPriority) : CScene2D(nPriority)
{
	m_fade = FADE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_fade = FADE_IN;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 継承元クラスの初期化処理
	CScene2D::Init(pos, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// 継承元クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		// フェードイン
		if (m_fade == FADE_IN)
		{
			//透明度を上げる
			m_color.a -= 0.02f;

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		// フェードアウト
		else if (m_fade == FADE_OUT)
		{
			//透明度を下げる
			m_color.a += 0.02f;

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_fade = FADE_IN;

				//モードの設定
				CManager::SetMode(m_modeNext);
			}
		}

		SetColor(m_color);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	// 継承元クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 設定処理
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext, D3DXCOLOR col)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_color = D3DXCOLOR(col.r, col.g, col.b, 0.0f);
}

//=============================================================================
// 取得処理
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}