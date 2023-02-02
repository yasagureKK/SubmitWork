//=============================================================================
// パーティクル(ウィンド)処理 [particle_wind.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle_wind.h"
#include "manager.h"
#include "renderer.h"
#include "effect2D.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticleWind::CParticleWind()
{
	// 変数のクリア
	memset(&pEffect2D, NULL, sizeof(pEffect2D));
	m_nNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticleWind::~CParticleWind()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticleWind::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;

	if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// 位置を10～710の値からランダムに設定
		m_pos.y = float(rand() % 700) + 10;
	}
	else if (CManager::GetMode() == CManager::MODE_PVP || CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		if (m_nNum == 0)
		{
			// 位置を10～350の値からランダムに設定
			m_pos.y = float(rand() % 340) + 10;
		}
		else if (m_nNum == 1)
		{
			// 位置を370～710の値からランダムに設定
			m_pos.y = float(rand() % 700) + 370;
		}
	}

	// 中心位置から円形に広がるように移動量を設定
	pEffect2D = CEffect2D::Create(m_pos, m_size, m_col, m_move, 10000);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticleWind::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CParticleWind::Update(void)
{
	if (pEffect2D != NULL)
	{
		// 位置・サイズ取得
		D3DXVECTOR3 pos = pEffect2D->GetPos();

		// 画面左端にいったら
		if (pos.x < 0)
		{
			// 消滅させる
			pEffect2D->Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticleWind::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticleWind* CParticleWind::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nNum)
{
	// インスタンスの生成
	CParticleWind *pParticleWind = NULL;

	// ヌルチェック
	if (pParticleWind == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pParticleWind = new CParticleWind;
		if (pParticleWind != NULL)
		{
			// 変数の初期化
			pParticleWind->m_move = move;
			pParticleWind->m_col = col;
			pParticleWind->m_nNum = nNum;

			// 初期化処理
			pParticleWind->Init(pos, size);
		}
	}

	return pParticleWind;
}