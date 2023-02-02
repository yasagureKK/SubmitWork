//=============================================================================
// アロー処理 [arrow.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "arrow.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "sound.h"
#include "collision_sphere.h"
#include "effect_billboard.h"
#include "data_attack.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CArrow::CArrow(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	m_pCollisionS = NULL;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CArrow::~CArrow()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CArrow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_pos = pos;
	m_posOld = m_pos;
	m_size = size;
	m_nLife = SUSTAIN_TIME;

	// 位置の設定
	SetPos(m_pos);

	// 矢のモデルを生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/arrow.x");

	// サイズを取得
	m_size = m_pModel->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	// 攻撃情報取得処理
	CDataAttack *pdataAtk = CManager::GetDataAtk();

	// コリジョン(武器判定)をつける
	m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size.x * 1.5f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_WEAPON, pdataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));

	// ダメージ量×倍率で攻撃力を設定
	m_pCollisionS->SetDamageValue(m_fAttackValue * pdataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// ボウガンのショットSEの再生
	pSound->Play(9);
	pSound->ControllVoice(9, 0.8f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CArrow::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CArrow::Update(void)
{
	// 寿命を減らす
	m_nLife--;

	// サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// 壁に当たっていないなら
	if (CMeshWall::Collision(this) == false)
	{
		// 軌跡にエフェクトをつける
		CEffectB::Create(D3DXVECTOR3(m_pos.x - (sinf(m_rot.y) * 20.0f), m_pos.y, m_pos.z - (cosf(m_rot.y) * 20.0f)), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			35, CEffectB::EFFECT_TYPE_SMALL, NULL);

		// 位置の取得
		D3DXVECTOR3 pos = GetPos();
		m_pos = pos;

		// 1フレーム前の位置の更新
		m_posOld = m_pos;

		// 直進させる
		m_pos.x -= sinf(m_rot.y) * 16.0f;
		m_pos.z -= cosf(m_rot.y) * 16.0f;

		// 位置反映
		SetPos(m_pos);

		if (m_pCollisionS != NULL)
		{
			// 球体コリジョンを追従させる
			m_pCollisionS->SetPosCollision(m_pos);
		}

	}
	// 壁に当たったら
	else if (CMeshWall::Collision(this) == true)
	{
		if (m_pCollisionS != NULL)
		{
			// サウンド取得処理
			CSound *pSound = CManager::GetSound();

			// ボウガンの突き刺さりSEの再生
			pSound->Play(10);
			pSound->ControllVoice(10, 0.8f);

			// 当たり判定を消す
			m_pCollisionS->Uninit();
			m_pCollisionS = NULL;
		}
	}

	// 他のコリジョンと接触した時の処理
	TouchCollision();

	// 寿命がなくなったら
	if (m_nLife <= 0)
	{
		// 当たり判定を消す
		if (m_pCollisionS != NULL)
		{
			m_pCollisionS->Uninit();
			m_pCollisionS = NULL;
		}

		// 消滅
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CArrow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

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

	// モデルの描画
	m_pModel->Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CArrow *CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAttackValue)
{
	// クラスの生成
	CArrow *pArrow = NULL;
	pArrow = new CArrow;
	
	if (pArrow != NULL)
	{
		// 変数の初期化
		pArrow->m_rot = rot;
		pArrow->m_fAttackValue = fAttackValue;

		// 初期化処理
		pArrow->Init(pos, pArrow->m_size);
	}
	return pArrow;
}

//=============================================================================
// それぞれのコリジョンの接触処理
//=============================================================================
void CArrow::TouchCollision(void)
{
	if (m_pCollisionS != NULL)
	{
		// 敵判定のコリジョンと触れた瞬間なら
		if (m_pCollisionS->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_ENEMY) == true)
		{
			// サウンド取得処理
			CSound *pSound = CManager::GetSound();

			// 斧のヒットSEの再生
			pSound->Play(8);
			pSound->ControllVoice(8, 0.8f);

			m_pCollisionS->Uninit();
			Uninit();
		}
	}
}