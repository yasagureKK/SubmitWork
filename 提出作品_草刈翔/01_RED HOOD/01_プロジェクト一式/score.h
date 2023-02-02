//=============================================================================
// スコア処理 [score.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_SCORE_DIGIT	(5)		// 最大桁数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScore : public CUI
{
public:
	CScore();																	// コンストラクタ
	~CScore();																	// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CScore *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// 生成処理
	int GetTime(void) { return m_nScore; }										// タイム取得処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理

private:
	int m_nScore;																// タイム
	D3DXVECTOR2 m_pos;															// 位置
	D3DXVECTOR2	m_size;															// サイズ
	CUI *m_apUI;																// UIのポインタ
	CUI *m_apNumber[MAX_SCORE_DIGIT];											// 数字をつくるUIのポインタ
};
#endif	//_SCORE_H_