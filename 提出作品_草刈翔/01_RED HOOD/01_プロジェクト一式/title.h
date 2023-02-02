//=============================================================================
// タイトル画面 [title.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_MAX_SELECT (4)		// UIの選択肢の最大数
#define TITLE_MAX_UI (3)			// UIの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;
class CRanking;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTitle : public CScene
{
public:
	// タイトルのフェーズ
	typedef enum
	{
		TITLE_PHASE_PRESS_START = 0,	// スタート待ち
		TITLE_PHASE_MODE_SELECT,		// 項目選択
		TITLE_PHASE_HOWTO,				// 遊び方
		TITLE_PHASE_GUIDE,				// 操作ガイド
		TITLE_PHASE_RANKING,			// ランキング
		TITLE_PHASE_MAX,
	} TITLE_PHASE;

	CTitle();																// コンストラクタ
	~CTitle();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CTitle *Create(void);											// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	void UiCreate(TITLE_PHASE phase, bool bPad);	// UI生成処理

	CUI *m_apUI[TITLE_MAX_UI];						// UIのポインタ
	CUI *m_apSelectUI[TITLE_MAX_SELECT];			// 選択肢のUIのポインタ
	CRanking *m_pRanking;							// ランキングのポインタ
	static int m_nSelect;							// 選択肢
	int m_nHowtoTex;								// 遊び方のテクスチャ番号
	TITLE_PHASE m_Phase;							// 現在のフェーズ
};

#endif	// _TITLE_H_