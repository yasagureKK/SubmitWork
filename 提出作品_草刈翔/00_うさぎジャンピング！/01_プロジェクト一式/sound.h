//=============================================================================
// サウンド処理 [sound.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

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
	// サウンドファイル
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,		// BGM：タイトル
		SOUND_LABEL_MENU,			// BGM：メニュー
		SOUND_LABEL_TUTORIAL,		// BGM：れんしゅうモード
		SOUND_LABEL_SOLOMENU,		// BGM：ひとりようメニュー
		SOUND_LABEL_CPU,			// BGM：CPUたいせん
		SOUND_LABEL_GAME,			// BGM：スコアアタック
		SOUND_LABEL_PVP,			// BGM：ふたりようモード
		SOUND_LABEL_RESULT,			// BGM：リザルト
		SOUND_LABEL_RANKING,		// BGM：ランキング
		SOUND_LABEL_SOUNDTEST,		// BGM：サウンドテスト
		SOUND_LABEL_TRAILER,		// BGM：動画用BGM
		SOUND_LABEL_TITLE_ENTER,	//  SE：タイトルエンター
		SOUND_LABEL_CURSOR,			//  SE：カーソル
		SOUND_LABEL_DECIDE,			//  SE：決定
		SOUND_LABEL_CANCEL,			//  SE：キャンセル
		SOUND_LABEL_ACCELERATION,	//  SE：加速
		SOUND_LABEL_JUMP,			//  SE：ジャンプ
		SOUND_LABEL_LANDING,		//  SE：着地
		SOUND_LABEL_SPIN,			//  SE：回転
		SOUND_LABEL_RISE,			//  SE：上昇
		SOUND_LABEL_COUNTDOWN,		//  SE：カウントダウン
		SOUND_LABEL_START,			//  SE：スタート
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();												// コンストラクタ
	~CSound();												// デストラクタ
	HRESULT Init(HWND hWnd);								// 初期化処理
	void Uninit(void);										// 終了処理
	HRESULT Play(SOUND_LABEL label);						// セグメント再生(停止)
	void Stop(SOUND_LABEL label);							// 停止
	void Stop(void);										// 停止
	void ControllVoice(SOUND_LABEL label, float fVolume)	// 音量調整(ラベルを指定し、1.0を基本の音量として調節)
	{
		m_apSourceVoice[label]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ
	static PARAM m_aParam[SOUND_LABEL_MAX];							// 各音素材のパラメータ
};

#endif	// _SOUND_H_