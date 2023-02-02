//=============================================================================
// ランキング処理 [ranking.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_BG_UI		(2)	// 背景UIの最大数
#define RANKING_DATA		(3)	// データ数
#define MAX_RANKING_DIGIT	(5)	// 最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking(int nPriority = PRIORITY_UI);									// コンストラクタ
	~CRanking();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CRanking *Create(D3DXVECTOR3 pos, bool bPad);					// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	CUI *m_apBgUI[RANKING_BG_UI];											// 背景UIのポインタ
	CUI *m_apNumber[RANKING_DATA][MAX_RANKING_DIGIT];						// ナンバーUIのポインタ
	CUI *m_apRankUI[RANKING_DATA];											// ランクUIのポインタ
	bool m_bPad;															// パッド入力されたかどうか
};
#endif	//_RANKING_H_