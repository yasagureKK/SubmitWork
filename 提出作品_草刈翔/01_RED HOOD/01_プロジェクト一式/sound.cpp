//=============================================================================
// サウンド処理 [sound.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOUND_FILE	"data/TEXT/sound_label.txt"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
IXAudio2 *CSound::m_pXAudio2 = NULL;
IXAudio2MasteringVoice *CSound::m_pMasteringVoice = NULL;
IXAudio2SourceVoice *CSound::m_apSourceVoice[MAX_SOUND] = { NULL };
BYTE *CSound::m_apDataAudio[MAX_SOUND] = { NULL };
DWORD CSound::m_aSizeAudio[MAX_SOUND] = { NULL };
CSound::PARAM CSound::m_aParam[MAX_SOUND] = { NULL };

//=============================================================================
// コンストラクタ
//=============================================================================
CSound::CSound()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen(SOUND_FILE, "r");

	if (pFile != NULL)
	{
		int nCntSound = 0, nMaxSound = 0;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "NUM_SOUND") == 0)	// NUM_SOUNDの文字列
			{
				// サウンドの最大数を取得
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &nMaxSound);

				if (nMaxSound >= MAX_SOUND)
				{
					nMaxSound = MAX_SOUND;
				}
			}

			// オーディオのパス・ループ情報を取得
			if (strcmp(&cString[0], "SOUND_NAME") == 0)
			{
				int nLoop = 0;

				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cString[0]);
				strcpy(&m_aParam[nCntSound].filename[0], &cString[0]);

				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &nLoop);
				m_aParam[nCntSound].nCntLoop = nLoop;

				nCntSound++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPTの文字列
			{
				break;
			}

			// 最大数になったらパスの読み込みを終了
			if (nCntSound >= MAX_SOUND)
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
		}

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < MAX_SOUND; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aParam[nCntSound].filename, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		if (filetype != 'EVAW')
		{
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));

		// バッファの設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < MAX_SOUND; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();

	if (m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
HRESULT CSound::Play(int nLabel)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER       buffer;

	// バッファの設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[nLabel];
	buffer.pAudioData = m_apDataAudio[nLabel];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[nLabel].nCntLoop;

	if (m_apSourceVoice[nLabel] != NULL)
	{
		// 状態取得
		m_apSourceVoice[nLabel]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
		 // 一時停止
			m_apSourceVoice[nLabel]->Stop(0);

			// クリア
			m_apSourceVoice[nLabel]->FlushSourceBuffers();
		}

		// 登録
		m_apSourceVoice[nLabel]->SubmitSourceBuffer(&buffer);

		// 再生
		m_apSourceVoice[nLabel]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(int nLabel)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[nLabel]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[nLabel]->Stop(0);

		// クリア
		m_apSourceVoice[nLabel]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(void)
{
	for (int nCntSound = 0; nCntSound < MAX_SOUND; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// クリア
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}