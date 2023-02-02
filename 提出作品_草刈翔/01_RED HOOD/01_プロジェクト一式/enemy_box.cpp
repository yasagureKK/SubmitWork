//=============================================================================
// エネミーボックス処理 [enemy_box.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "enemy_box.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"
#include "enemy.h"
#include "mesh_wall.h"
#include "flash.h"
#include "sound.h"
#include "game.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyBox::CEnemyBox(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = ENEMY_BOX_TYPE_0;
	m_nEnemyNum = 0;
	m_bInside = false;
	m_bDefeat = false;
	memset(&m_pEnemy, NULL, sizeof(m_pEnemy));
	memset(&m_pMeshWall, NULL, sizeof(m_pMeshWall));
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyBox::~CEnemyBox()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyBox::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_bInside = false;
	m_bDefeat = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyBox::Uninit(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEBOX_ENEMY; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{
			m_pEnemy[nCntEnemy] = NULL;
		}
	}

	for (int nCntWall = 0; nCntWall < MAX_ENEBOX_WALL; nCntWall++)
	{
		if (m_pMeshWall[nCntWall] != NULL)
		{
			m_pMeshWall[nCntWall]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyBox::Update(void)
{
	InsidePlayer();

	// プレイヤーが中にいる状態で
	if (m_bInside == true)
	{
		for (int nCntEnemy = 0; nCntEnemy < m_nEnemyNum; nCntEnemy++)
		{
			// 1体でもライフが残っていれば終了する
			if (m_pEnemy[nCntEnemy] != NULL && m_pEnemy[nCntEnemy]->GetLife() > 0)
			{
				break;
			}

			// 最後の敵までfor文がループできたら
			if (nCntEnemy == m_nEnemyNum - 1)
			{
				// 全ての敵がNULL(倒された)場合、全滅した状態にする
				m_bDefeat = true;
			}
		}
	}

	// 敵が全滅したら
	if (m_bDefeat == true)
	{
		// エネミーボックスのクリアSEの再生
		CSound *pSound = CManager::GetSound();
		pSound->Play(26);
		pSound->ControllVoice(26, 0.8f);

		// 白くフラッシュさせる
		CFlash::Create(D3DCOLOR_RGBA(255, 255, 255, 200), 0.5f, 0.01f);

		// 番号が2(ボスを生成するボックス)だったら
		if (m_type == ENEMY_BOX_TYPE_BOSS)
		{
			// ボス戦フラグをfalseに
			CManager::GetGame()->SetBossBattle(false);

			// ゲームのボス撃破フラグをtrueに
			CManager::GetGame()->SetDefeatBoss(true);
		}

		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyBox::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemyBox* CEnemyBox::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// クラスの生成
	CEnemyBox *pEnemyBox = NULL;
	pEnemyBox = new CEnemyBox;

	if (pEnemyBox != NULL)
	{
		// 変数の初期化
		pEnemyBox->m_type = (CEnemyBox::ENEMY_BOX_TYPE)nType;

		// 初期化処理
		pEnemyBox->Init(pos, size);
	}
	return pEnemyBox;
}

//=============================================================================
// プレイヤーが中に入ったかを判別する処理
//=============================================================================
void CEnemyBox::InsidePlayer(void)
{
	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			D3DXVECTOR3 wallSize = m_size * 2 * 0.98f / 2;
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// プレイヤーの位置
			D3DXVECTOR3 posPlyOld = pPlayer->GetPosOld();	// プレイヤーの1フレーム前の位置
			D3DXVECTOR3 sizePlayer = pPlayer->GetSize();	// プレイヤーのサイズ

			if (m_pos.y + m_size.y > posPlayer.y &&
				m_pos.y < posPlayer.y + sizePlayer.y &&
				m_pos.z + wallSize.z >= posPlayer.z + sizePlayer.z / 2.0f &&
				m_pos.z - wallSize.z <= posPlayer.z - sizePlayer.z / 2.0f &&
				m_pos.x + wallSize.x >= posPlayer.x + sizePlayer.x / 2.0f &&
				m_pos.x - wallSize.x <= posPlayer.x - sizePlayer.x / 2.0f)
			{
				// 敵・壁を配置
				if (m_bInside == false)
				{
					// エネミーボックスのインSEの再生
					CSound *pSound = CManager::GetSound();
					pSound->Play(25);
					pSound->ControllVoice(25, 0.8f);

					// ボス戦のエリアに入ったら
					if (m_type == ENEMY_BOX_TYPE_BOSS)
					{
						// プレイヤーを上空の足場に移動させる
						pPlayer->SetPos(D3DXVECTOR3(posPlayer.x, posPlayer.y + 5000.0f, posPlayer.z));
					}

					// 赤くフラッシュさせる
					CFlash::Create(D3DCOLOR_RGBA(178, 0, 0, 200), 0.075f, 0.02f);

					// オブジェクトの配置
					SetObject();
				}
				// プレイヤーが中に入った状態
				m_bInside = true;
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 敵・壁の配置処理
//=============================================================================
void CEnemyBox::SetObject(void)
{
	if (m_type != ENEMY_BOX_TYPE_BOSS)
	{
		// 壁の配置
		if (m_pMeshWall[0] == NULL)
		{
			// テクスチャが重なってしまうため、フィールドの壁から少し離して配置
			m_pMeshWall[0] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 3);
			m_pMeshWall[1] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 1, 1, 3);
			m_pMeshWall[2] = CMeshWall::Create(D3DXVECTOR3(m_pos.x + m_size.x * 0.98f, m_pos.y, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 1, 1, 3);
			m_pMeshWall[3] = CMeshWall::Create(D3DXVECTOR3(m_pos.x - m_size.x * 0.98f, m_pos.y, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 1, 1, 3);
			m_pMeshWall[4] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z + m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 4);
			m_pMeshWall[5] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z - m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 1, 1, 4);
			m_pMeshWall[6] = CMeshWall::Create(D3DXVECTOR3(m_pos.x + m_size.x * 0.98f, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 1, 1, 4);
			m_pMeshWall[7] = CMeshWall::Create(D3DXVECTOR3(m_pos.x - m_size.x * 0.98f, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 1, 1, 4);
		}
	}

	// 敵の配置
	if (m_pEnemy[0] == NULL)
	{
		switch (m_type)
		{
		case ENEMY_BOX_TYPE_0:
			m_nEnemyNum = 4;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 250.0f, m_pos.y, m_pos.z - 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 350.0f, m_pos.y, m_pos.z - 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			break;

		case ENEMY_BOX_TYPE_1:
			m_nEnemyNum = 6;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 15.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 15.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 500.0f, m_pos.y, m_pos.z),			D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 15.0f);
			m_pEnemy[3] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 500.0f, m_pos.y, m_pos.z),			D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 15.0f);
			m_pEnemy[4] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 350.0f, m_pos.y, m_pos.z + 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 17.0f);
			m_pEnemy[5] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 350.0f, m_pos.y, m_pos.z + 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 18.0f);
			break;

		case ENEMY_BOX_TYPE_2:
			m_nEnemyNum = 4;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[3] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			break;

		case ENEMY_BOX_TYPE_BOSS:
			// ボス戦フラグをtrueに
			CManager::GetGame()->SetBossBattle(true);
			m_nEnemyNum = 1;

			// 上空の足場に配置
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 5000.0f, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_BOSS, 0.0f);
			break;

		default:
			break;
		}
	}
}