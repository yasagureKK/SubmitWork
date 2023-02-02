//=============================================================================
// パーティクル(サークル)処理 [particle.h]
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
class CBillboardEffect;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CParticleCircle : public CParticle
{
public:
	CParticleCircle();									// コンストラクタ
	~CParticleCircle();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CParticleCircle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col, int nLife, float fSpeed);		// 生成処理

private:
	CBillboardEffect *pBillboardEffect[MAX_PARTICLE];	// エフェクトのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3	m_rot;									// 向き
	D3DXCOLOR m_col;									// カラー
	int m_nLife;										// 寿命
	float m_fSpeed;										// 速さ
	float m_fAngle;										// 角度
	float m_fAddAngle;									// 加える角度
};

#endif	//_PARTICLE_CIRCLE_H_