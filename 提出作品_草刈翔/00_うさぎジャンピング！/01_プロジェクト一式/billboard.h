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
#include "texture.h"

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
	CBillboard(int nPriority = PRIORITY_MODEL);					// コンストラクタ
	~CBillboard();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CBillboard *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, CTexture::TEX_TYPE tex,
		int nPriolty);											// 生成処理
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// 位置設定処理
	D3DXVECTOR3 GetPos(void);									// 位置取得処理
	void SetTexture(float fU, float fV, int nCutU, int nCutV);	// テクスチャ設定処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャ割当処理
	void SetColor(D3DXCOLOR col);								// カラー設定処理
	void SetShadow(bool bOn);									// 影の設定処理
	void DeleteShadow(void);									// 影の消去処理

private:
	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR3	m_size;											// サイズ
	CShadow *m_pShadow;											// シャドウのポインタ

protected:
	D3DXVECTOR3 m_move;											// 移動量
};

#endif // _BILLBOARD_H_