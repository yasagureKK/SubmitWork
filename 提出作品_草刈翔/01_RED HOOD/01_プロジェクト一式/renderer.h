//=============================================================================
// レンダリング処理 [renderer.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();												// コンストラクタ
	~CRenderer();												// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// デバイスの取得処理

private:
#ifdef _DEBUG
	void DrawFPS(void);											// FPS描画処理
#endif

	LPDIRECT3D9	m_pD3D;											// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;								// Deviceオブジェクト(描画に必要)
	LPD3DXFONT m_pFont;											// フォントへのポインタ
};
#endif // _RENDERER_H_