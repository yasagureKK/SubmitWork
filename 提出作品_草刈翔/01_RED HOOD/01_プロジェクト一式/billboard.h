//=============================================================================
// �r���{�[�h���� [billboard.h]
// Author : ���� ��
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CShadow;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CBillboard : public CScene3D
{
public:
	CBillboard(int nPriority = PRIORITY_MODEL);						// �R���X�g���N�^
	~CBillboard();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nTexType, bool bShadow);								// ��������

	void SetPos(D3DXVECTOR3 pos);									// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; }						// �ʒu�擾����
	void SetSize(D3DXVECTOR3 size);									// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) { return m_size; }					// �T�C�Y�擾����
	void SetTexture(float fU, float fV, int nCutU, int nCutV);		// �e�N�X�`���ݒ菈��
	void SetColor(D3DXCOLOR col);									// �J���[�ݒ菈��
	void TrackingShadow(void);										// �e�̒Ǐ]����
	void DeleteShadow(void);										// �e�̏�������
	void SetVertex(D3DXVECTOR3 Vertex0, D3DXVECTOR3 Vertex1,
		D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3);					// ���_���W�ʒu�ݒ菈��

private:
	D3DXVECTOR3 m_pos;												// �ʒu
	D3DXVECTOR3	m_size;												// �T�C�Y
	CShadow *m_pShadow;												// �V���h�E�̃|�C���^
	bool m_bShadow;													// �V���h�E�����邩�ǂ���

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;									// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;											// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_move;												// �ړ���
};
#endif // _BILLBOARD_H_