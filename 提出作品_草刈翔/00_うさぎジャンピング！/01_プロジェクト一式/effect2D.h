//=============================================================================
// 2Dのエフェクト処理 [effect_2D.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEffect2D : public CScene2D
{
public:
	CEffect2D(int nPriority = PRIORITY_UI);				// コンストラクタ
	~CEffect2D();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CEffect2D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife);					// 生成処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXVECTOR3 m_move;									// 移動量
	int m_nLife;										// 寿命
	D3DXCOLOR m_col;									// カラー
};

#endif // _EFFECT_2D_H_