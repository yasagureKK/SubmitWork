//=============================================================================
// 2Dオブジェクト処理 [scene2D.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;  // 頂点座標
	float rhw;        // 1.0fで固定
	D3DCOLOR col;     // 頂点カラー
	D3DXVECTOR2 tex;  // テクスチャ画像
} VERTEX_2D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = PRIORITY_NONE);					// コンストラクタ
	~CScene2D();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CScene2D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nPriority);						// 生成処理
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 位置設定処理
	D3DXVECTOR3 GetPos(void);									// 位置取得処理
	D3DXVECTOR3 GetSize(void);									// サイズ取得処理
	void SetTexture(float fU, float fV, int nCutU, int nCutV);	// テクスチャ設定処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャ割当処理
	void SetColor(D3DXCOLOR col);								// カラー設定処理

private:
	LPDIRECT3DTEXTURE9 m_pTexture;								// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR3 m_size;											// サイズ
};

#endif // _SCENE2D_H_