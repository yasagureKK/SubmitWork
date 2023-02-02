//=============================================================================
// レベルアップ処理 [level_up.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _LEVEL_UP_H_
#define _LEVEL_UP_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LEVELUP_MAX_UI		(6)	// UIの最大数
#define LEVELUP_MAX_SELECT	(3)	// 選択肢の最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLevelUp : public CScene
{
public:
	// レベルアップのフェーズ
	typedef enum
	{
		LEVELUP_PHASE_BG = 0,	// 背景
		LEVELUP_PHASE_SELECT,	// 選択
		LEVELUP_PHASE_FINISH,	// 終了
		LEVELUP_PHASE_MAX,
	} LEVELUP_PHASE;

	CLevelUp(int nPriority = PRIORITY_PAUSE);								// コンストラクタ
	~CLevelUp();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CLevelUp *Create(D3DXVECTOR3 pos);								// 生成処理

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理
	static void SetCorrectionDef(int nDef) { m_nCorrectionDef = nDef; }		// 防御補正値の設定処理
	static void SetCorrectionAtk(int nAtk) { m_nCorrectionAtk = nAtk; }		// 攻撃補正値の設定処理
	static void SetCorrectionSpd(int nSpeed) { m_nCorrectionSpd = nSpeed; }	// 速度補正値の設定処理
	static int GetCorrectionDef(void) { return m_nCorrectionDef; }			// 防御補正値の取得処理
	static int GetCorrectionAtk(void) { return m_nCorrectionAtk; }			// 攻撃補正値の取得処理
	static int GetCorrectionSpd(void) { return m_nCorrectionSpd; }			// 速度補正値の取得処理

private:
	CUI *m_apUI[LEVELUP_MAX_UI];			// UIのポインタ
	CUI *m_apSelectUI[LEVELUP_MAX_SELECT];	// 選択肢のUIのポインタ
	int m_nAlpha;							// アルファ度
	int m_nSelect;							// どの項目を選んでいるか
	int m_nFlash;							// 点滅用のカウント
	static int m_nCorrectionDef;			// 防御の補正値
	static int m_nCorrectionAtk;			// 攻撃の補正値
	static int m_nCorrectionSpd;			// 速度の補正値
	LEVELUP_PHASE m_Phase;					// 現在のフェーズ
};
#endif	// _LEVEL_UP_H_