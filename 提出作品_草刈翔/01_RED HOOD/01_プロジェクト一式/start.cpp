//=============================================================================
// スタートUI処理 [start.cpp]
// Author : 草刈 翔
//=============================================================================
#include "start.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CUIStart::CUIStart()
{
	// 変数のクリア
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_expansionSize = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	m_bStartMoment = false;
	m_nWaitTime = 0;
	m_fAlpha = 0.0f;
	m_fExpansion = 0.0f;
	m_phase = START_PHASE_READY;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUIStart::~CUIStart()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUIStart::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_bStartMoment = false;
	m_nWaitTime = 0;
	m_fAlpha = 0.0f;
	m_fExpansion = 1.5f;
	m_phase = START_PHASE_READY;
	m_expansionSize = m_size * m_fExpansion;

	// Ready?～Go!のUI
	m_apUI = CUI::Create(m_pos, m_expansionSize, 39, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI->SetTexture(0.0f, 0.0f, 1, 2);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUIStart::Uninit(void)
{
	// UIの終了処理
	if (m_apUI != NULL)
	{
		m_apUI->Uninit();
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUIStart::Update(void)
{
	m_bStartMoment = false;

	// フェード取得処理
	CFade *pFade = CManager::GetFade();

	// フェードしていない時のみ
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		switch (m_phase)
		{
		case START_PHASE_READY:
			// サイズ・カラーの設定
			m_expansionSize = m_size * m_fExpansion;
			m_apUI->SetSize(m_expansionSize);
			m_apUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

			// 拡大率の変更
			m_fExpansion -= 0.5f / 30;

			if (m_fExpansion <= 1.0f)
			{
				m_fExpansion = 1.0f;
			}

			// アルファ度の変更
			m_fAlpha += 1.0f / 30;

			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;
				m_nWaitTime++;
			}

			// 1秒経ったら
			if (m_nWaitTime >= 60)
			{
				// 画像のテクスチャ座標を切り替える
				m_apUI->SetTexture(0.0f, 1.0f, 1, 2);

				// スタートした瞬間にする
				m_bStartMoment = true;

				// フェーズを切り替えるs
				m_phase = START_PHASE_GO;
			}
			break;

		case START_PHASE_GO:
			// サイズ・カラーの設定
			m_expansionSize = m_size * m_fExpansion;
			m_apUI->SetSize(m_expansionSize);
			m_apUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

			// サイズの変更
			m_fExpansion += 0.5f / 60;

			if (m_fExpansion >= 1.5f)
			{
				m_fExpansion = 1.5f;
			}

			// アルファ度の変更
			m_fAlpha -= 1.0f / 60;

			// アルファ度が0以下になったら
			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;

				// 終了処理
				Uninit();
			}
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CUIStart::Draw(void)
{
	
}

//=============================================================================
// 生成処理
//=============================================================================
CUIStart *CUIStart::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// クラスの生成
	CUIStart *pUIStart = NULL;
	pUIStart = new CUIStart;

	if (pUIStart != NULL)
	{
		// 初期化処理
		pUIStart->Init(pos, size);
	}
	return pUIStart;
}