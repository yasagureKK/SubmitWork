//=============================================================================
// �t�F�[�h���� [fade.h]
// Author : ���� ��
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "scene.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CFade : public CScene
{
public:
	// �t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,		// �������Ă��Ȃ�
		FADE_IN,			// �t�F�[�h�C��
		FADE_OUT,			// �t�F�[�h�A�E�g
		FADE_MAX
	} FADE;

	CFade(int nPriority = PRIORITY_FADE);					// �R���X�g���N�^
	~CFade();												// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);							// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	void SetFade(CManager::MODE modeNext, D3DXCOLOR col);	// �ݒ菈��
	static FADE GetFade(void) { return m_fade; }			// �擾����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	CManager::MODE m_modeNext;								// ���̉��
	D3DXCOLOR m_color;										// �t�F�[�h�̐F
	static FADE m_fade;										// �t�F�[�h�̏��
};
#endif // _FADE_H_