//=============================================================================
// パーティクル(円形)処理 [particle_circle.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle_circle.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticleCircle::CParticleCircle()
{
	// 変数のクリア
	memset(&pEffectB, NULL, sizeof(pEffectB));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_nParticleNum = 0;
	m_fSpeed = 0.0f;
	m_fAngle = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticleCircle::~CParticleCircle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticleCircle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_fAngle = 0;

	// 角度関連の変数の初期化
	m_fAngle = 0;
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		// 中心位置から円形に広がるように移動量を設定
		pEffectB[nCntParticle] = CEffectB::Create(m_pos, m_size, m_col,
			D3DXVECTOR3(cosf(m_fAngle) * m_fSpeed, 0.0f, sinf(m_fAngle) * m_fSpeed),
			m_nLife, CEffectB::EFFECT_TYPE_VANISH, NULL);
		// 角度を変更
		m_fAngle += (D3DX_PI * 2.0f / m_nParticleNum);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticleCircle::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticleCircle::Update(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		if (pEffectB[nCntParticle] != NULL)
		{
			// パーティクルの寿命を取得
			int nLife = pEffectB[nCntParticle]->GetLife();

			// パーティクルの寿命が尽きたら
			if (nLife <= 0)
			{
				pEffectB[nCntParticle] = NULL;
			}
		}

		// 最後のパーティクルがNULLになったら
		if (pEffectB[m_nParticleNum - 1] == NULL)
		{
			// 終了処理
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticleCircle::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticleCircle *CParticleCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fSpeed, int nNum)
{
	// クラスの生成
	CParticleCircle *pParticleCircle = NULL;
	pParticleCircle = new CParticleCircle;

	if (pParticleCircle != NULL)
	{
		// 変数の初期化
		pParticleCircle->m_col = col;
		pParticleCircle->m_nLife = nLife;
		pParticleCircle->m_fSpeed = fSpeed;
		pParticleCircle->m_nParticleNum = nNum;

		// 初期化処理
		pParticleCircle->Init(pos, size);
	}
	return pParticleCircle;
}