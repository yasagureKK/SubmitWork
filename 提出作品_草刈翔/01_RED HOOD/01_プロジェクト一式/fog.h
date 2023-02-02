//=============================================================================
// �t�H�O(��)���� [fog.h]
// Author : ���� ��
//=============================================================================
#ifndef _FOG_H_
#define _FOG_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CFog : public CScene
{
public:
	CFog(int nPriority = PRIORITY_NONE);									// �R���X�g���N�^
	~CFog();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CFog *Create(D3DXCOLOR col, float fStart, float fEnd);			// ��������
	void SetFogLine(float fStart, float fEnd);								// �v�Z�p�̕ϐ��̐ݒ�

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	D3DXCOLOR m_col;									// �J���[
	float m_fStartFog;									// �����ʂ��J�n����鋗��
	float m_fEngFog;									// �����ʂ��������Ȃ��Ȃ鋗��
};
#endif // _FOG_H_