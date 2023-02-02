//=============================================================================
// フェード処理 [fade.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "manager.h"
#include "scene2D.h"

//*****************************************************************************
// フェードクラス
//*****************************************************************************
class CFade : public CScene2D
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない
		FADE_IN,			// フェードイン
		FADE_OUT,			// フェードアウト
		FADE_MAX
	} FADE;

	CFade(int nPriority = PRIORITY_FADE);					// コンストラクタ
	~CFade();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void SetFade(CManager::MODE modeNext, D3DXCOLOR col);	// 設定処理
	static FADE GetFade(void);								// 取得処理
	void SetFlash(D3DXCOLOR col);							// フラッシュ設定処理(モード遷移なし)
	void SetScreenCol(D3DXCOLOR col);						// 画面カラー設定処理(モード遷移なし)

private:
	static FADE m_fade;										// フェードの状態
	CManager::MODE m_modeNext;								// 次の画面
	D3DXCOLOR m_color;										// フェードの色
};
#endif // _FADE_H_