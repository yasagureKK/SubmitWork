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
	CScene2D(int nPriority = PRIORITY_NONE);									// コンストラクタ
	~CScene2D();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);												// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static CScene2D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR2 size, int nPriority);										// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }					// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos);												// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }									// 位置取得処理
	void SetSize(D3DXVECTOR2 size);												// サイズ設定処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// サイズ取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// 向き取得処理

	void SetTexture(float fU, float fV, int nCutU, int nCutV);					// テクスチャ座標設定処理
	void BindTexture(int nTexType);												// テクスチャ割当処理
	void SetColor(D3DXCOLOR col);												// カラー設定処理
	void SetBesideBarScale(float fScale, bool bReverse);						// ライフゲージ(横向き)のスケール設定処理
	void SetTilt(float fAngle);													// 傾き設定処理

private:
	LPDIRECT3DTEXTURE9 m_pTexture;												// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;											// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;															// 位置
	D3DXVECTOR2 m_size;															// サイズ
	D3DXCOLOR m_col;															// カラー
	float m_fRadius;															// 頂点までの半径
	float m_fTilt;																// 傾き
};
#endif // _SCENE2D_H_