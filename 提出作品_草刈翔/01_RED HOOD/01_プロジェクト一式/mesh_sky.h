//=============================================================================
// ���b�V��(��)���� [mesh_sky.h]
// Author : ���� ��
//=============================================================================
#ifndef _MESH_SKY_H_
#define _MESH_SKY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CMeshSky : public CScene3D
{
public:
	CMeshSky(int nPriority = PRIORITY_NONE);						// �R���X�g���N�^
	~CMeshSky();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CMeshSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		float fSpin, int nTexType);									// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }		// �I�u�W�F�N�g�̎�ނ̎擾����
	D3DXVECTOR3 GetPos(void) { return m_pos; };						// �ʒu�擾����

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// �e�N�X�`����������

private:
	LPDIRECT3DTEXTURE9 m_pTexture;									// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;												// �ʒu
	D3DXVECTOR3	m_size;												// �T�C�Y
	D3DXVECTOR3 m_rot;												// ����
	D3DXMATRIX m_mtxWorld;											// ���[���h�}�g���b�N�X
	int m_nVertical;												// �c�̕�����
	int m_nSide;													// ���̕�����
	float m_fSpin;													// ��]�̑���
};
#endif // _MESH_SKY_H_