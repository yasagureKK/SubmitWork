//=============================================================================
// ���f���Ǘ����� [model.h]
// Aouthor : ���� ��
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODEL_TEXTURE (2)	// ���f���̃e�N�X�`���̐�
#define MODEL_VTX		  (8)	// ���_�̐�

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModel
{
public:
	CModel();											// �R���X�g���N�^
	~CModel();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		const char *aModelName);						// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CModel *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, const char *aModelName);		// ��������
	void SetParent(CModel *pModel);						// �e���f���ݒ菈��
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }		// ���[���h�}�g���b�N�X�擾����
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�擾����
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾����
	D3DXVECTOR3 GetSize(void) { return m_size; }		// �T�C�Y�擾����
	D3DXVECTOR3 GetMtxPos(void);						// ���[���h�}�g���b�N�X���W�擾����
	void SetColor(D3DXCOLOR col) { m_col = col; }		// �J���[�ݒ菈��
	//int GetModelType(void) { return m_nType; }			// ���f���̎�ގ擾����

private:
	LPD3DXMESH m_pMesh;									// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;							// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD m_nNumMat;									// �}�e���A����
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_MODEL_TEXTURE];	// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	D3DXVECTOR3 m_rot;									// ����
	D3DXMATRIX m_mtxWorld;								// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMin;								// ���f���̒��_�̍ŏ��l
	D3DXVECTOR3 m_vtxMax;								// ���f���̒��_�̍ő�l
	D3DXVECTOR3 m_vtxPos[MODEL_VTX];					// 8���_�̈ʒu�ۑ��p�ϐ�
	D3DXVECTOR3 m_vtxRot[MODEL_VTX];					// 8���_�̉�]�ۑ��p�ϐ�
	D3DXMATRIX m_vtxMtxWorld[MODEL_VTX];				// 8���_�̃��[���h�}�g���b�N�X�ۑ��p�ϐ�
	CModel *m_pParent;									// �e�̃��f��
	D3DXCOLOR m_col;									// �J���[
};
#endif // _MODEL_H_