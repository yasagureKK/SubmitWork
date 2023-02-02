//=============================================================================
// パーティクル(ダッシュ)処理 [particle_dash.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PARTICLE_DASH_H_
#define _PARTICLE_DASH_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffectB;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CParticleDash : public CParticle
{
public:
	CParticleDash();									// コンストラクタ
	~CParticleDash();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CParticleDash *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col, int nLife, float fSpeed,
		float fRotOff, float fCicrcleOff, int nNum);	// 生成処理

private:
	CEffectB *pEffectB_Circle[MAX_PARTICLE];			// エフェクトのポインタ
	CEffectB *pEffectB_Wind[MAX_PARTICLE];				// エフェクトのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3	m_rot;									// 向き
	D3DXCOLOR m_col;									// カラー
	int m_nLife;										// 寿命
	int m_nParticleNum;									// 1度に出すパーティクルの数
	float m_fSpeed;										// 速さ
	float m_fAngle;										// 角度
	float m_fRotOffset;									// 正面の向きからのオフセット
	float m_fCircleOffset;								// パーティクル生成位置からのオフセット
};

#endif	//_PARTICLE_DASH_H_