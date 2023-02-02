//=============================================================================
// メッシュ(円柱)処理 [mesh_cylinder.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "texture.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMeshCylinder : public CScene
{
public:
	// メッシュシリンダーの種類
	typedef enum
	{
		CYLINDER_TYPE_NONE = 0,		// デフォルト
		CYLINDER_TYPE_BACK_COLOR,	// 背景色
		CYLINDER_TYPE_MAX
	} CYLINDER_TYPE;

	CMeshCylinder(int nPriority = PRIORITY_NONE);					// コンストラクタ
	~CMeshCylinder();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nVertical, int nSide, D3DXCOLOR col,
		bool bOutSide, float fSpinSpeed, CYLINDER_TYPE type,
		CTexture::TEX_TYPE tex);									// 生成処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// テクスチャ割当処理
	void SetNum(int nNum);											// 番号設定処理

private:
	void TrackingPlayer(void);										// プレイヤー追尾処理
	void Spin(void);												// 回転処理
	void ChangeColor(void);											// カラー変更処理
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	D3DXVECTOR3 m_rot;												// 向き
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	D3DXCOLOR m_col;												// カラー
	int m_nSide;													// 横の分割数
	int m_nVertical;												// 縦の分割数
	float m_fRadius;												// 半径
	float m_fAngle;													// 角度
	float m_fAddAngle;												// 加える角度
	float m_fSpinSpeed;												// 回転角度
	float m_fAddSpinSpeed;											// 加える回転角度
	bool m_bOutSide;												// シリンダーの外側に描画するかどうか
	int m_nNum;														// 番号
	CYLINDER_TYPE m_type;											// 種類
};

#endif // _MESH_CYLINDER_H_