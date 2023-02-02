//=============================================================================
// スタートUI処理 [start.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _START_H_
#define _START_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CUIStart : public CUI
{
public:
	// スタートのフェーズ
	typedef enum
	{
		START_PHASE_READY = 0,	// Ready
		START_PHASE_GO,			// Go
		START_PHASE_MAX,
	} START_PHASE;

	CUIStart();																	// コンストラクタ
	~CUIStart();																// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CUIStart *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理
	bool GetStartMoment(void) { return m_bStartMoment; }

private:
	D3DXVECTOR2 m_pos;				// 位置
	D3DXVECTOR2	m_size;				// サイズ
	D3DXVECTOR2	m_expansionSize;	// 拡大した画像サイズ
	CUI *m_apUI;					// UIのポインタ
	bool m_bStartMoment;			// スタートした瞬間
	int m_nWaitTime;				// 待機時間
	float m_fAlpha;					// アルファ度
	float m_fExpansion;				// 拡大率
	START_PHASE m_phase;			// フェーズ
};
#endif	//_START_H_