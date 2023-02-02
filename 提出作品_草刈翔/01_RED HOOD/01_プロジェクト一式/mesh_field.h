//=============================================================================
// メッシュフィールド処理 [mesh_field.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CMeshField : public CScene3D
{
public:
	CMeshField(int nPriority = PRIORITY_NONE);						// コンストラクタ
	~CMeshField();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nRow, int nLine, int nTexType);		// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_MODEL; }		// オブジェクトの種類の取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }						// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return m_size; }					// サイズ取得処理

	void SetColor(D3DXCOLOR col);									// カラー設定処理
	static bool Collision(CScene *pScene);							// 当たり判定処理

private:
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// インデックスバッファのポインタ
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	D3DXVECTOR3 m_rot;												// 向き
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	int m_nRow;														// 横の分割数
	int m_nLine;													// 縦の分割数
};
#endif // _MESH_FIELD_H_