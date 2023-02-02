//=============================================================================
// 武器UI処理 [weapon.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_WEAPON_UI_POLYGON	(3)	// 最大ポリゴン数
#define MAX_WEAPON_ICON			(2)	// アイコンの数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CWeapon : public CUI
{
public:
	CWeapon();																// コンストラクタ
	~CWeapon();																// デストラクタ
	HRESULT Init(D3DXVECTOR2 pos);												// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CWeapon *Create(D3DXVECTOR2 pos);									// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理
	void SetUIChange(int nType);												// UI変更処理

private:
	D3DXVECTOR2 m_pos;															// 位置
	D3DXVECTOR2	m_size;															// サイズ
	CUI *m_apUI[MAX_WEAPON_UI_POLYGON];											// UIのポインタ
	float m_fTiltCircle;														// 円のUIの傾き
	D3DXVECTOR2 m_posIcon[MAX_WEAPON_ICON];										// アイコンの位置
	float m_fTiltIcon[MAX_WEAPON_ICON];											// アイコンに設定する傾き
	float m_fAddTilt;															// アイコンに追加する傾き
	float m_fObjectiveTilt;														// アイコンの目的の傾き
};
#endif	//_WEAPON_H_