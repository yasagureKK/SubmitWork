//=============================================================================
// フラッシュ処理 [flash.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _FLASH_H_
#define _FLASH_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CFlash : public CScene2D
{
public:
	// フラッシュの状態
	typedef enum
	{
		FLASH_NONE = 0,		// 何もしていない
		FLASH_ON,			// フラッシュオン
		FLASH_OFF,			// フラッシュオフ
		FLASH_MAX
	} FLASH;

	CFlash(int nPriority = PRIORITY_FADE);									// コンストラクタ
	~CFlash();																// デストラクタ
	HRESULT Init(D3DXCOLOR col, float fOnSpeed, float fOffSpeed);			// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CFlash *Create(D3DXCOLOR col,  float fOnSpeed, float fOffSpeed);	// 生成処理
	FLASH GetFlash(void) { return m_flash; }								// 取得処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	float m_fOnSpeed;					// フラッシュ中、透明度を下げる速さ
	float m_fOffSpeed;					// フラッシュ後、透明度を上げる速さ
	D3DXCOLOR m_TargetColor;			// 目標の色
	D3DXCOLOR m_color;					// フラッシュの色
	FLASH m_flash;						// フラッシュの状態
};
#endif // _FADE_H_