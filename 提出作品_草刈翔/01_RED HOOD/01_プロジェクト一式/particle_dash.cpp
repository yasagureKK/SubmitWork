//=============================================================================
// パーティクル(ダッシュ)処理 [particle_dash.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle_dash.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticleDash::CParticleDash()
{
	// 変数のクリア
	memset(&pEffectB_Circle, NULL, sizeof(pEffectB_Circle));
	memset(&pEffectB_Wind, NULL, sizeof(pEffectB_Wind));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_fRotOffset = 0.0f;
	m_fCircleOffset = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticleDash::~CParticleDash()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticleDash::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_fAngle = 0;

	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.x += -sinf(m_rot.y) * m_fRotOffset;
		pos.z += -cosf(m_rot.y) * m_fRotOffset;

		// -3.14～3.14の値をランダムで出す
		float fAngle = (-314 + rand() % 618) / 100.0f;

		// プレイヤーの向きに合わせてエフェクトを生成
		pEffectB_Wind[nCntParticle] = CEffectB::Create(D3DXVECTOR3(pos.x + cosf(fAngle) * -cosf(m_rot.y) * m_fCircleOffset,
																   pos.y + sinf(fAngle) * m_fCircleOffset,
																   pos.z + cosf(fAngle) * sinf(m_rot.y) * m_fCircleOffset),
			m_size, m_col, D3DXVECTOR3((sinf(m_rot.y) * 7.0f) + (-cosf(fAngle) * cosf(m_rot.y) * m_fSpeed),
																  sinf(fAngle) * m_fSpeed,
										(cosf(m_rot.y) * 7.0f) + (cosf(fAngle) * -cosf(m_rot.y + D3DX_PI / 2) * m_fSpeed)),
			m_nLife, CEffectB::EFFECT_TYPE_VANISH, NULL);

		// 角度を変更
		m_fAngle += (D3DX_PI * 2.0f / 15);

		// 増やした角度が１周しそうになったら終了
		if (m_fAngle >= D3DX_PI * 2)
		{
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticleDash::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticleDash::Update(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		if (pEffectB_Wind[nCntParticle] != NULL)
		{
			// パーティクルの寿命と位置を取得
			int nLife = pEffectB_Wind[nCntParticle]->GetLife();
			D3DXVECTOR3 pos = pEffectB_Wind[nCntParticle]->GetPos();

			// 持続時間中にエフェクトを出す
			CEffectB::Create(pos, m_size, m_col, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, CEffectB::EFFECT_TYPE_SMALL, NULL);

			// パーティクルの寿命が尽きたら
			if (nLife <= 0)
			{
				pEffectB_Wind[nCntParticle] = NULL;
			}
		}

		// 最後のパーティクルがNULLになったら
		if (pEffectB_Wind[m_nParticleNum - 1] == NULL)
		{
			// 終了処理
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticleDash::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticleDash *CParticleDash::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed, float fRotOff, float fCicrcleOff, int nNum)
{
	// クラスの生成
	CParticleDash *pParticleDash = NULL;
	pParticleDash = new CParticleDash;

	if (pParticleDash != NULL)
	{
		// 変数の初期化
		pParticleDash->m_rot = rot;
		pParticleDash->m_col = col;
		pParticleDash->m_nLife = nLife;
		pParticleDash->m_fSpeed = fSpeed;
		pParticleDash->m_fRotOffset = fRotOff;
		pParticleDash->m_fCircleOffset = fCicrcleOff;
		pParticleDash->m_nParticleNum = nNum;

		// 初期化処理
		pParticleDash->Init(pos, size);
	}
	return pParticleDash;
}