//=============================================================================
// ビルボードのエフェクト処理 [billboard_effect.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _BILLBOARD_EFFECT_H_
#define _BILLBOARD_EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CBillboardEffect : public CBillboard
{
public:
	// エフェクトの種類
	typedef enum
	{
		EFFECT_TYPE_NORMAL,		// 通常
		EFFECT_TYPE_VANISH,		// 徐々に消える
		EFFECT_TYPE_MAX
	} EFFECT_TYPE;

	CBillboardEffect();									// コンストラクタ
	~CBillboardEffect();								// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CBillboardEffect *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife, EFFECT_TYPE type);	// 生成処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	int m_nLife;										// 寿命
	D3DXCOLOR m_col;									// カラー
	EFFECT_TYPE m_type;									// エフェクトの種類
};

#endif // _BILLBOARD_EFFECT_H_