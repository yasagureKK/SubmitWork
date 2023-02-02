//=============================================================================
// パーティクル(ヒット)処理 [particle_hit.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle_hit.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticleHit::CParticleHit()
{
	// 変数のクリア
	memset(&pEffectB, NULL, sizeof(pEffectB));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_fSpeed = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticleHit::~CParticleHit()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticleHit::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;

	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		// -3.14～3.14の値をランダムで出す
		float fAngleX = (-314 + rand() % 618) / 100.0f;
		float fAngleZ = (-314 + rand() % 618) / 100.0f;

		// 
		pEffectB[nCntParticle] = CEffectB::Create(m_pos,
			m_size, m_col, D3DXVECTOR3((float)(sinf(fAngleX) * -cos(fAngleZ) * m_fSpeed),
												cosf(fAngleX) * m_fSpeed,
										(float)(sinf(fAngleX) * -cos(fAngleZ + D3DX_PI / 2.0f) * m_fSpeed)),
			m_nLife, CEffectB::EFFECT_TYPE_SMALL, NULL);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticleHit::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticleHit::Update(void)
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
void CParticleHit::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticleHit *CParticleHit::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed, int nNum)
{
	// クラスの生成
	CParticleHit *pParticleHit = NULL;
	pParticleHit = new CParticleHit;

	if (pParticleHit != NULL)
	{
		// 変数の初期化
		pParticleHit->m_rot = rot;
		pParticleHit->m_col = col;
		pParticleHit->m_nLife = nLife;
		pParticleHit->m_fSpeed = fSpeed;
		pParticleHit->m_nParticleNum = nNum;

		// 初期化処理
		pParticleHit->Init(pos, size);
	}
	return pParticleHit;
}