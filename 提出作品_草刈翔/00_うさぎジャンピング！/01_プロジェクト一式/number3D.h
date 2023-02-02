//=============================================================================
// 3Dオブジェクトでのナンバー管理処理 [number3D.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _NUMBER3D_H_
#define _NUMBER3D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CNumber3D
{
public:
	CNumber3D();								// コンストラクタ
	~CNumber3D();								// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理
	static CNumber3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col);							// 生成処理
	void SetNumber(int nNumber);				// ナンバー設定処理
	void SetPos(D3DXVECTOR3 pos);				// 位置設定処理
	void SetColor(D3DXCOLOR col);				// カラー設定処理

private:
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3	m_size;										// サイズ
	D3DXVECTOR3 m_rot;										// 向き
	D3DXCOLOR m_color;										// カラー
	static LPDIRECT3DTEXTURE9 m_pTexture;					// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;									// ワールドマトリックス
};

#endif	//_NUMBER3D_H_