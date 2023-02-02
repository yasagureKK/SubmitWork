//=============================================================================
// ビルボード(ムーン)処理 [billboard_moon.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _BILLBOARD_MOON_H_
#define _BILLBOARD_MOON_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CBillboardMoon : public CBillboard
{
public:
	CBillboardMoon();									// コンストラクタ
	~CBillboardMoon();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nNum);										// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CBillboardMoon *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nNum);					// 生成処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	int m_nNum;											// 番号
};

#endif // _BILLBOARD_MOON_H_