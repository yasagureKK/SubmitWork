//=============================================================================
// ゲーム中の看板オブジェクト生成処理 [game_signbpard.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _GAME_SIGNBOARD_H_
#define _GAME_SIGNBOARD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModelSingle;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CGameSignboard : public CScene
{
public:
	CGameSignboard();										// コンストラクタ
	~CGameSignboard();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	static CGameSignboard *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nNumPlayer);					// 生成処理

private:
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_size;										// サイズ
	CModelSingle *m_apModelSingle;							// モデル(単体)のポインタ
	int m_nNumPlayer;										// どのプレイヤーに対するものか
};

#endif	// _GAME_SIGNBOARD_H_