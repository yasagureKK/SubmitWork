//=============================================================================
// ビルボードのエフェクト処理 [effect_billboard.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _EFFECT_BILLBOARD_H_
#define _EFFECT_BILLBOARD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEffectB : public CBillboard
{
public:
	// エフェクトの種類
	typedef enum
	{
		EFFECT_TYPE_NORMAL,		// 通常
		EFFECT_TYPE_VANISH,		// 徐々に消える
		EFFECT_TYPE_SMALL,		// 徐々に小さくなる
		EFFECT_TYPE_MAX
	} EFFECT_TYPE;

	CEffectB();											// コンストラクタ
	~CEffectB();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col, D3DXVECTOR3 move, int nLife);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CEffectB *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife, EFFECT_TYPE type,
		CModel *pModel);								// 生成処理
	int GetLife(void) { return m_nLife; }				// 寿命取得処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3	m_size;									// サイズ
	D3DXVECTOR3	m_defSize;								// 1フレームごとに減らすサイズ
	D3DXVECTOR3 m_mtxPos;								// 保存用のワールドマトリックス座標
	D3DXCOLOR m_col;									// カラー
	CModel *m_pParent;									// 親のモデル
	CBillboard *m_pBillboard;
	EFFECT_TYPE m_type;									// エフェクトの種類
	int m_nLife;										// 寿命
	int m_nBeginLife;									// 1フレーム目の寿命
	float m_fDefAlpha;									// 1フレームごとに減らすアルファ値
	bool m_bModel;										// モデルにくっつけたどうか
};
#endif // _EFFECT_BILLBOARD_H_