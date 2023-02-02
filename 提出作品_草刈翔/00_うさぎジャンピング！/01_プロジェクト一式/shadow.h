//=============================================================================
// シャドウ処理 [shadow.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CShadow : public CScene3D
{
public:
	CShadow(int nPriority = PRIORITY_MODEL);			// コンストラクタ
	~CShadow();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CShadow *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot);				// 生成処理
	void SetShadow(D3DXVECTOR3 pos, float fRange);		// 影の設定処理
	void SetUseShadow(bool bUse);						// 使用状態の設定処理

private:
	D3DXVECTOR3 m_move;									// ポリゴンの移動量
	D3DXVECTOR3 m_Defaultsize;							// デフォルトの影のサイズ
	bool m_bUse;										// 使用しているかどうか
};

#endif // _SHADOW_H_