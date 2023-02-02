//=============================================================================
// ビルボード(スモーク)処理 [billboard_smoke.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _BILLBOARD_SMOKE_H_
#define _BILLBOARD_SMOKE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBillboardEffect;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CBillboardSmoke : public CBillboard
{
public:
	CBillboardSmoke();									// コンストラクタ
	~CBillboardSmoke();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CBillboardSmoke *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nLife);					// 生成処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	int m_nLife;										// 寿命
	int m_nAlpha;										// 透明度
};

#endif	//_PARTICLE_SMOKE_H_