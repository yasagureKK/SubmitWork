//=============================================================================
// メッシュ(空)処理 [mesh_sky.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _MESH_SKY_H_
#define _MESH_SKY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CMeshSky : public CScene3D
{
public:
	CMeshSky(int nPriority = PRIORITY_NONE);						// コンストラクタ
	~CMeshSky();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CMeshSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		float fSpin, int nTexType);									// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }		// オブジェクトの種類の取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; };						// 位置取得処理

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// テクスチャ割当処理

private:
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	D3DXVECTOR3 m_rot;												// 向き
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	int m_nVertical;												// 縦の分割数
	int m_nSide;													// 横の分割数
	float m_fSpin;													// 回転の速さ
};
#endif // _MESH_SKY_H_