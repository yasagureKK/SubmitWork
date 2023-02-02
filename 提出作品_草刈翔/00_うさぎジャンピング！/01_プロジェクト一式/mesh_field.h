//=============================================================================
// メッシュ(フィールド)処理 [mesh_field.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "texture.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CMeshField : public CScene
{
public:
	// メッシュフィールドの動きの種類
	typedef enum
	{
		MESHFIELD_TYPE_NONE = 0,
		MESHFIELD_TYPE_RIGHT_LOOP,			// X方向の右に合わせてループ
		MESHFIELD_TYPE_MAX
	}MESHFIELD_TYPE;

	CMeshField(int nPriority = PRIORITY_NONE);						// コンストラクタ
	~CMeshField();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nRow, int nLine,
		MESHFIELD_TYPE type, CTexture::TEX_TYPE tex);				// 生成処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// テクスチャ割当処理
	void SetNum(int nNum);											// 番号設定処理
	static bool Collision(CScene *pScene);							// 当たり判定

private:
	void TrackingPlayer();											// プレイヤー追尾処理
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// インデックスバッファのポインタ
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	D3DXVECTOR3 m_rot;												// 向き
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	int m_nRow;														// 横の分割数
	int m_nLine;													// 奥の分割数
	int m_nNum;														// 番号
	MESHFIELD_TYPE m_type;											// 動きの種類
};

#endif // _MESH_FIELD_H_