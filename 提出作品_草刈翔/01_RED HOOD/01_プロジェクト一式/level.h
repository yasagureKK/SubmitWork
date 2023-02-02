//=============================================================================
// レベル処理 [level.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _LEVEL_H_
#define _LEVEL_H_

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
#define MAX_LEVEL_UI		(4)		// UIの最大数
#define MAX_LEVEL			(10)	// 最大レベル
#define MAX_LEVEL_DIGIT		(2)		// レベルの最大桁数
#define MAX_LEVEL_ICON_UI	(3)		// UIの最大数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLevel : public CUI
{
public:
	CLevel();																	// コンストラクタ
	~CLevel();																	// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CLevel *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// 生成処理
	void SetExp(int nValue) { m_nExp = nValue; }								// 経験値設定処理
	void AddExp(int nValue) { m_nExp += nValue; }								// 経験値増加処理
	int GetLevel(void) { return m_nLevel; }										// レベル取得処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理
	bool GetMoment(void) { return m_bMoment; }

private:
	void SetEmptyBar();															// 空部分の設定処理

	int m_nLevel;																// レベル
	int m_nExp;																	// 経験値
	int m_nNextLevelExp;														// 次のレベルまでの合計経験値
	int m_nNeedExp[MAX_LEVEL];													// レベルごとの必要経験値
	float m_fObjectiveSize;														// 目的のサイズ
	float m_fMaxSize;															// 最大のサイズ
	bool m_bMoment;																// レベルが上がった瞬間
	bool m_bDrawNumber;
	D3DXVECTOR2 m_pos;															// 位置
	D3DXVECTOR2	m_size;															// サイズ
	D3DXVECTOR2 m_emptyPos;														// 空部分の位置
	D3DXVECTOR2 m_emptySize;													// 空部分のサイズ
	D3DXVECTOR2 m_levelPos;														// 文字の位置
	CUI *m_apUI[MAX_LEVEL_UI];													// UIのポインタ
	CUI *m_apNumber[MAX_LEVEL_DIGIT];											// 数字UIのポインタ
	CUI *m_apIconUI[MAX_LEVEL_ICON_UI];											// アイコンのポインタ
	CUI *m_apIconNumber[MAX_LEVEL_ICON_UI];										// アイコン用の数字UIのポインタ
};
#endif	//_LEVEL_H_