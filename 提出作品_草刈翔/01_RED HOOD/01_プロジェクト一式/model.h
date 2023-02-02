//=============================================================================
// モデル管理処理 [model.h]
// Aouthor : 草刈 翔
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL_TEXTURE (2)	// モデルのテクスチャの数
#define MODEL_VTX		  (8)	// 頂点の数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CModel
{
public:
	CModel();											// コンストラクタ
	~CModel();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		const char *aModelName);						// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CModel *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, const char *aModelName);		// 生成処理
	void SetParent(CModel *pModel);						// 親モデル設定処理
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }		// ワールドマトリックス取得処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得処理
	D3DXVECTOR3 GetSize(void) { return m_size; }		// サイズ取得処理
	D3DXVECTOR3 GetMtxPos(void);						// ワールドマトリックス座標取得処理
	void SetColor(D3DXCOLOR col) { m_col = col; }		// カラー設定処理
	//int GetModelType(void) { return m_nType; }			// モデルの種類取得処理

private:
	LPD3DXMESH m_pMesh;									// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;							// マテリアル(材質情報)へのポインタ
	DWORD m_nNumMat;									// マテリアル数
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_MODEL_TEXTURE];	// テクスチャのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3 m_rot;									// 向き
	D3DXMATRIX m_mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3 m_vtxMin;								// モデルの頂点の最小値
	D3DXVECTOR3 m_vtxMax;								// モデルの頂点の最大値
	D3DXVECTOR3 m_vtxPos[MODEL_VTX];					// 8頂点の位置保存用変数
	D3DXVECTOR3 m_vtxRot[MODEL_VTX];					// 8頂点の回転保存用変数
	D3DXMATRIX m_vtxMtxWorld[MODEL_VTX];				// 8頂点のワールドマトリックス保存用変数
	CModel *m_pParent;									// 親のモデル
	D3DXCOLOR m_col;									// カラー
};
#endif // _MODEL_H_