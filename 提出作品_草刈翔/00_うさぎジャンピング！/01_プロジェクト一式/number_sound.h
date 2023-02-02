//=============================================================================
// サウンドナンバー処理 [number_sound.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _NUMBER_SOUND_H_
#define _NUMBER_SOUND_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMBER_SOUND_DIGIT (3)	// 曲の最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CNumberSound : public CScene
{
public:
	CNumberSound(int nPriority = PRIORITY_UI);			// コンストラクタ
	~CNumberSound();									// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CNumberSound *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// 生成処理
	void SetNumber(int nNumber);						// ナンバー設定処理

private:
	void PlaySound(int nNum);							// サウンド再生処理

	CNumber *m_apNumber[MAX_NUMBER_SOUND_DIGIT];		// ナンバーのポインタ
	int m_nSoundNumber;									// 現在の数
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXCOLOR m_col;									// カラー
};

#endif	//_NUMBER_SOUND_H_