//=============================================================================
// ビルボードのライフ処理 [life_billboard.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _LIFE_BILLBOARD_H_
#define _LIFE_BILLBOARD_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE_BILLBOARD	(3)	// ビルボードの数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLifeBillboard : public CBillboard
{
public:
	CLifeBillboard();															// コンストラクタ
	~CLifeBillboard();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);							// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CLifeBillboard *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	// 生成処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }								// 位置設定処理
	void SetSize(D3DXVECTOR3 size) { m_size = size; }							// サイズ設定処理
	void SetLife(float fValue) { m_fLife = fValue; }							// ライフ設定処理
	void DecLife(int nValue) {m_fLife -= nValue; }								// ライフ減少処理
	void SetMaxLife(float nValue) { m_fMaxLife = nValue; }						// 最大ライフ設定処理

private:
	void SetLifeBar(void);														// ライフバーの設定処理

	float m_fLife;																// ライフ
	float m_fMaxLife;															// 最大ライフ
	float m_fObjectiveSize;														// 目的のサイズ
	float m_fMaxSize;															// 最大のサイズ
	D3DXVECTOR3 m_pos;															// 位置
	D3DXVECTOR3	m_size;															// サイズ
	D3DXVECTOR3 m_emptySize;													// 空部分のサイズ
	CBillboard *m_apBillboard[MAX_LIFE_BILLBOARD];								// ビルボードのポインタ
};
#endif	//_LIFE_BILLBOARD_H_