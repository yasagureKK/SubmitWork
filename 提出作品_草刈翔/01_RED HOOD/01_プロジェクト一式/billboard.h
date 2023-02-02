//=============================================================================
// ビルボード処理 [billboard.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CShadow;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CBillboard : public CScene3D
{
public:
	CBillboard(int nPriority = PRIORITY_MODEL);						// コンストラクタ
	~CBillboard();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nTexType, bool bShadow);								// 生成処理

	void SetPos(D3DXVECTOR3 pos);									// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }						// 位置取得処理
	void SetSize(D3DXVECTOR3 size);									// サイズ設定処理
	D3DXVECTOR3 GetSize(void) { return m_size; }					// サイズ取得処理
	void SetTexture(float fU, float fV, int nCutU, int nCutV);		// テクスチャ設定処理
	void SetColor(D3DXCOLOR col);									// カラー設定処理
	void TrackingShadow(void);										// 影の追従処理
	void DeleteShadow(void);										// 影の消去処理
	void SetVertex(D3DXVECTOR3 Vertex0, D3DXVECTOR3 Vertex1,
		D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3);					// 頂点座標位置設定処理

private:
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	CShadow *m_pShadow;												// シャドウのポインタ
	bool m_bShadow;													// シャドウをつけるかどうか

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	D3DXVECTOR3 m_move;												// 移動量
};
#endif // _BILLBOARD_H_