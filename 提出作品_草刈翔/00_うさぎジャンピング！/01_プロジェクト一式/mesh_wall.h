//=============================================================================
// メッシュ(ウォ―ル)処理 [mesh_wall.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _MESH_WALL_H_
#define _MESH_WALL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "texture.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CMeshWall : public CScene
{
public:
	// メッシュウォールの動きの種類
	typedef enum
	{
		MESHWALL_TYPE_NONE = 0,
		MESHWALL_TYPE_RIGHT_LOOP,	// X方向の右に合わせてループ
		MESHWALL_TYPE_MAX
	}MESHWALL_TYPE;

	CMeshWall(int nPriority = PRIORITY_NONE);						// コンストラクタ
	~CMeshWall();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nVertical, int nRow,
		MESHWALL_TYPE type, CTexture::TEX_TYPE tex);				// 生成処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// テクスチャ割当処理
	void SetNum(int nNum);											// 番号設定処理

private:
	void TrackingPlayer();											// プレイヤー追尾処理
	LPDIRECT3DTEXTURE9 m_pTexture;									// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// インデックスバッファのポインタ
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3	m_size;												// サイズ
	D3DXVECTOR3 m_rot;												// 向き
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	int m_nVertical;												// 縦の分割数
	int m_nRow;														// 横の分割数
	int m_nNum;														// 番号
	MESHWALL_TYPE m_type;											// 動きの種類
};

#endif // _MESH_FIELD_H_