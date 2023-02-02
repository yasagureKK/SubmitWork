//=============================================================================
// �����L���O���� [ranking.h]
// Author : ���� ��
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_BG_UI		(2)	// �w�iUI�̍ő吔
#define RANKING_DATA		(3)	// �f�[�^��
#define MAX_RANKING_DIGIT	(5)	// �ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking(int nPriority = PRIORITY_UI);									// �R���X�g���N�^
	~CRanking();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CRanking *Create(D3DXVECTOR3 pos, bool bPad);					// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	CUI *m_apBgUI[RANKING_BG_UI];											// �w�iUI�̃|�C���^
	CUI *m_apNumber[RANKING_DATA][MAX_RANKING_DIGIT];						// �i���o�[UI�̃|�C���^
	CUI *m_apRankUI[RANKING_DATA];											// �����NUI�̃|�C���^
	bool m_bPad;															// �p�b�h���͂��ꂽ���ǂ���
};
#endif	//_RANKING_H_