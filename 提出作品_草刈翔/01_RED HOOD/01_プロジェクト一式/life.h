//=============================================================================
// ライフ処理 [life.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE_POLYGON	(3)	// ポリゴンの数
#define MAX_LIFE_DIGIT		(3)	// 最大桁数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLife : public CUI
{
public:
	// ライフの種類
	typedef enum
	{
		LIFE_TYPE_PLAYER = 0,	// プレイヤー
		LIFE_TYPE_BOSS,			// ボス
		LIFE_TYPE_MAX
	} LIFE_TYPE;

	CLife();																	// コンストラクタ
	~CLife();																	// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type);			// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CLife *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type);	// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetLife(float fLife) { m_fLife = fLife; }								// ライフ設定処理
	void SetMaxLife(float fLife) { m_fMaxLife = fLife; }						// 最大ライフ設定処理

private:
	void SetLifeBar(void);			// ライフバーの設定処理

	float m_fLife;					// ライフ
	float m_fMaxLife;				// 最大ライフ
	float m_fObjectiveSize;			// 目的のサイズ
	float m_fMaxSize;				// 最大のサイズ
	D3DXVECTOR2 m_pos;				// 位置
	D3DXVECTOR2 m_size;				// サイズ
	D3DXVECTOR2 m_emptyPos;			// 空部分の位置
	D3DXVECTOR2 m_emptySize;		// 空部分のサイズ
	CUI *m_apUI[MAX_LIFE_POLYGON];	// UIのポインタ
};
#endif	//_LIFE_H_