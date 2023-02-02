//=============================================================================
// タイム処理 [time.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

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
#define MAX_TIME		(999)	// 時間の最大数
#define MAX_TIME_DIGIT	(3)		// 最大桁数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTime : public CUI
{
public:
	CTime();																	// コンストラクタ
	~CTime();																	// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CTime *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// 生成処理
	int GetTime(void) { return m_nTime; }										// タイム取得処理
	void SetTimeStop(bool bStop) { m_bTimeStop = bStop; }						// タイムストップ設定処理
	bool GetTimeStop(void) { return m_bTimeStop; }								// タイムストップ取得処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理

private:
	int m_nTime;																// タイム
	int m_nNumber;																// タイム計算用の変数
	bool m_bTimeStop;															// タイム進行を止めるかどうか
	D3DXVECTOR2 m_pos;															// 位置
	D3DXVECTOR2	m_size;															// サイズ
	CUI *m_apUI;																// UIのポインタ
	CUI *m_apNumber[MAX_TIME_DIGIT];											// 数字をつくるUIのポインタ
};
#endif	//_TIME_H_