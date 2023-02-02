//=============================================================================
// サウンド処理 [sound.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SOUND (64)

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	CSound();										// コンストラクタ
	~CSound();										// デストラクタ
	HRESULT Init(HWND hWnd);						// 初期化処理
	void Uninit(void);								// 終了処理
	HRESULT Play(int nLabel);						// セグメント再生(停止)
	void Stop(int nLabel);							// 停止
	void Stop(void);								// 停止
	void ControllVoice(int nLabel, float fVolume)	// 音量調整処理
	{
		m_apSourceVoice[nLabel]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char filename[128];	// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;		// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[MAX_SOUND];	// ソースボイス
	static BYTE *m_apDataAudio[MAX_SOUND];					// オーディオデータ
	static DWORD m_aSizeAudio[MAX_SOUND];					// オーディオデータサイズ
	static PARAM m_aParam[MAX_SOUND];						// 各音素材のパラメータ
};
#endif	// _SOUND_H_