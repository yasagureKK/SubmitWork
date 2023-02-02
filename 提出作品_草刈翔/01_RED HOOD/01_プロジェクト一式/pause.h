//=============================================================================
// ポーズ処理 [pause.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MAX_UI		(5)	// UIの最大数
#define PAUSE_MAX_SELECT_UI	(3)	// 選択肢のUIの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPause : public CScene
{
public:
	// ポーズのフェーズ
	typedef enum
	{
		PAUSE_PHASE_NORMAL = 0,	// 通常
		PAUSE_PHASE_GUIDE,		// 操作ガイド
		PAUSE_PHASE_MAX,
	} PAUSE_PHASE;

	CPause(int nPriority = PRIORITY_PAUSE);									// コンストラクタ
	~CPause();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CPause *Create(D3DXVECTOR3 pos);									// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

	static void SetPossiblePause(bool bPause);								// ポーズできるか設定処理

private:
	void UICreate(PAUSE_PHASE phase, float fTex);	// UI生成処理
	void CancelPause(void);							// ポーズ解除時の処理

	CUI *m_apUi[PAUSE_MAX_UI];						// UIのポインタ
	CUI *m_apSelectUi[PAUSE_MAX_SELECT_UI];			// 選択肢のUIのポインタ
	static bool m_bCanPause;						// ポーズできるかどうか
	bool m_bPause;									// ポーズしているかどうか
	static int m_nSelect;							// どの選択肢を選んでいるか
	PAUSE_PHASE m_Phase;							// 現在のフェーズ
};
#endif	// _PAUSE_H_