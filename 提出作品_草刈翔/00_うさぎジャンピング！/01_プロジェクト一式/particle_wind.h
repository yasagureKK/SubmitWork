//=============================================================================
// パーティクル(ウィンド)処理 [particle_wind.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PARTICLE_WIND_H_
#define _PARTICLE_WIND_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "particle.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffect2D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CParticleWind : public CParticle
{
public:
	CParticleWind();									// コンストラクタ
	~CParticleWind();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CParticleWind *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nNum);					// 生成処理

private:
	CEffect2D *pEffect2D;								// 2Dエフェクトのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3	m_move;									// 移動量
	D3DXCOLOR m_col;									// カラー
	int m_nNum;											// どのプレイヤーのパーティクルなのか
};

#endif	//_PARTICLE_WIND_H_