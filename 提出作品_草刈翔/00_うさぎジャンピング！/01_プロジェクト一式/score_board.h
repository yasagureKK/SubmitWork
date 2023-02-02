//=============================================================================
// 看板に表示させるスコア処理 [score_board.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _SCORE_BOARD_H_
#define _SCORE_BOARD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_BOARD_DIGIT	(4)	// 看板に表示させるスコアの最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber3D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScoreBoard : public CScene3D
{
public:
	CScoreBoard(int nPriority = PRIORITY_MODEL);	// コンストラクタ
	~CScoreBoard();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DCOLOR col);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static CScoreBoard *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DCOLOR col);			// 生成処理
	void SetPos(D3DXVECTOR3 pos);					// 位置設定処理

private:
	CNumber3D *m_apNumber3D[MAX_SCORE_BOARD_DIGIT];	// 3Dオブジェクトのナンバーのポインタ
	int m_nScore;									// スコア
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_size;								// サイズ
	D3DXVECTOR3 m_rot;								// 向き
};

#endif	//_SCORE_BOARD_H_