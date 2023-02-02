//=============================================================================
// パーティクル(円形)処理 [particle_circle.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PARTICLE_CIRCLE_H_
#define _PARTICLE_CIRCLE_H_

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
class CParticleCircle : public CParticle
{
public:
	CParticleCircle();										// コンストラクタ
	~CParticleCircle();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	static CParticleCircle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size,  D3DXCOLOR col, int nLife,
		float fSpeed, int nNum);							// 生成処理

private:
	CEffectB *pEffectB[MAX_PARTICLE];	// エフェクトのポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3	m_size;					// サイズ
	D3DXCOLOR m_col;					// カラー
	int m_nLife;						// 寿命
	int m_nParticleNum;					// 1度に出すパーティクルの数
	float m_fSpeed;						// 速さ
	float m_fAngle;						// 角度
};

#endif	//_PARTICLE_CIRCLE_H_