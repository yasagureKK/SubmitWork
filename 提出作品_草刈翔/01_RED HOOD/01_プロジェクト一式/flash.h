//=============================================================================
// �t���b�V������ [flash.h]
// Author : ���� ��
//=============================================================================
#ifndef _FLASH_H_
#define _FLASH_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CFlash : public CScene2D
{
public:
	// �t���b�V���̏��
	typedef enum
	{
		FLASH_NONE = 0,		// �������Ă��Ȃ�
		FLASH_ON,			// �t���b�V���I��
		FLASH_OFF,			// �t���b�V���I�t
		FLASH_MAX
	} FLASH;

	CFlash(int nPriority = PRIORITY_FADE);									// �R���X�g���N�^
	~CFlash();																// �f�X�g���N�^
	HRESULT Init(D3DXCOLOR col, float fOnSpeed, float fOffSpeed);			// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CFlash *Create(D3DXCOLOR col,  float fOnSpeed, float fOffSpeed);	// ��������
	FLASH GetFlash(void) { return m_flash; }								// �擾����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	float m_fOnSpeed;					// �t���b�V�����A�����x�������鑬��
	float m_fOffSpeed;					// �t���b�V����A�����x���グ�鑬��
	D3DXCOLOR m_TargetColor;			// �ڕW�̐F
	D3DXCOLOR m_color;					// �t���b�V���̐F
	FLASH m_flash;						// �t���b�V���̏��
};
#endif // _FADE_H_