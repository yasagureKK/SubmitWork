//=============================================================================
// パーティクル(サークル)処理 [particle_circle.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle_circle.h"
#include "manager.h"
#include "renderer.h"
#include "billboard_effect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticleCircle::CParticleCircle()
{
	// 変数のクリア
	memset(&pBillboardEffect, NULL, sizeof(pBillboardEffect));
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

	// 角度関連の変数の初期化
	m_fAngle = atan2f(100.0f, 100.0f);
	m_fAddAngle = 0;

	for (int nCntParticle = 0; nCntParticle < 90; nCntParticle++)
	{
		// 中心位置から円形に広がるように移動量を設定
		pBillboardEffect[nCntParticle] = CBillboardEffect::Create(m_pos, m_size, m_col,
			D3DXVECTOR3(cosf(m_fAngle - m_fAddAngle) * m_fSpeed, 0.0f, sinf(m_fAngle - m_fAddAngle) * m_fSpeed),
			m_nLife, CBillboardEffect::EFFECT_TYPE_NORMAL);

		// 角度を変更
		m_fAddAngle += (D3DX_PI / (90 / 2.0f));

		// 増やした角度が１周しそうになったら終了
		if (m_fAddAngle >= D3DX_PI * 2)
		{
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticleCircle::Uninit(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticleCircle::Update(void)
{

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
CParticleCircle* CParticleCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed)
{
	// インスタンスの生成
	CParticleCircle *pParticleCircle = NULL;

	// ヌルチェック
	if (pParticleCircle == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pParticleCircle = new CParticleCircle;
		if (pParticleCircle != NULL)
		{
			// 変数の初期化
			pParticleCircle->m_rot = rot;
			pParticleCircle->m_col = col;
			pParticleCircle->m_nLife = nLife;
			pParticleCircle->m_fSpeed = fSpeed;

			// 初期化処理
			pParticleCircle->Init(pos, size);
		}
	}

	return pParticleCircle;
}