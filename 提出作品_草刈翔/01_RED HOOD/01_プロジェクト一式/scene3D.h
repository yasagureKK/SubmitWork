//=============================================================================
// 3Dオブジェクト処理 [scene3D.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//頂点フォーマット3D

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = PRIORITY_MODEL);								// コンストラクタ
	~CScene3D();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CScene3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot, int nPriolty);					// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }							// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; };								// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }								// 向き取得処理

private:
	D3DXVECTOR3 m_pos;														// 位置
	D3DXVECTOR3 m_rot;														// 向き
	D3DXMATRIX m_mtxWorld;													// ワールドマトリックス
};
#endif // _SCENE3D_H_