//=============================================================================
// ガイド処理 [jump_guide.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _JUMP_GUIDE_H_
#define _JUMP_GUIDE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_JUMP_GUIDE_DIGIT (3)	// 最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CJumpGuide : public CScene
{
public:
	CJumpGuide(int nPriority = PRIORITY_UI);			// コンストラクタ
	~CJumpGuide();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CJumpGuide *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// 生成処理
	void SetPosValue(int nPos);							// 位置座標の値設定処理
	int GetPosValue(void);								// 位置座標の値取得処理
	void SetNum(int nNum);								// スコア番号設定処理
	int GetNum(void);									// スコア番号取得処理

private:
	CNumber *m_apNumber[MAX_SCORE_JUMP_GUIDE_DIGIT];	// 2Dオブジェクトのナンバーのポインタ
	CUi *m_apUi[2];										// UIのポインタ
	int m_nPos;											// 現在の位置座標X
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXCOLOR m_col;									// カラー
	int m_nNum;											// 番号
	bool m_bChangeTex;									// 画像を切り替えたかどうか
};

#endif	//_JUMP_GUIDE_H_