//=============================================================================
// �I�u�W�F�N�g�Ǘ� [scene.h]
// Author : ���� ��
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene
{
public:
	// �`�揇�̎��(�Ⴂ���̂���`��)
	typedef enum
	{
		PRIORITY_NONE = 0,			// �Ȃ�
		PRIORITY_PLAYER,			// �v���C���[
		PRIORITY_MODEL,				// ���f��
		PRIORITY_EFFECT,			// �G�t�F�N�g
		PRIORITY_UI,				// UI
		PRIORITY_PAUSE,				// �|�[�Y
		PRIORITY_FADE,				// �t�F�[�h
		PRIORITY_MAX
	} PRIORITY;

	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJECT_TYPE_NONE = 0,			// �Ȃ�
		OBJECT_TYPE_PLAYER,				// �v���C���[
		OBJECT_TYPE_MODEL,				// ���f��
		OBJECT_TYPE_WALL,				// ��
		OBJECT_TYPE_ENEMY,				// �G
		OBJECT_TYPE_UI,					// UI
		OBJECT_TYPE_MAX
	} OBJECT_TYPE;

	CScene(int nPriority = PRIORITY_NONE);		// �R���X�g���N�^
	virtual ~CScene();							// �f�X�g���N�^
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;	// ����������
	virtual void Uninit(void) = 0;				// �I������
	virtual void Update(void) = 0;				// �X�V����
	virtual void Draw(void) = 0;				// �`�揈��
	static void ReleaseAll(void);				// �S�ẴI�u�W�F�N�g�̊J������
	static void UpdateAll(void);				// �S�ẴI�u�W�F�N�g�̍X�V����
	static void DrawAll(void);					// �S�ẴI�u�W�F�N�g�̕`�揈��

	CScene *GetObjNext(CScene *pObject);		// �����̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�̏��擾����
	static CScene *GetTopObj(int nPriority);	// �����̃v���C�I���e�B�̐擪�I�u�W�F�N�g�̎擾����

	virtual OBJECT_TYPE GetObjType(void) = 0;	// �I�u�W�F�N�g�̎�ނ̎擾����
	virtual void SetPos(D3DXVECTOR3 pos) = 0;	// �ʒu�ݒ菈��
	virtual D3DXVECTOR3 GetPos(void) = 0;		// �ʒu�擾����
	virtual D3DXVECTOR3 GetPosOld(void) = 0;	// 1�t���[���O�̈ʒu�擾����
	virtual D3DXVECTOR3 GetSize(void) = 0;		// �T�C�Y�擾����
	virtual D3DXVECTOR3 GetRot(void) = 0;		// �����擾����

private:
	static CScene *m_pTop[PRIORITY_MAX];		// �擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];		// �Ō���̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pPrev;							// �O�̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pNext;							// ���̃I�u�W�F�N�g�̃|�C���^
	int m_nPriority;							// �`��D�揇�ʂ̔ԍ�
	bool m_bDeath;								// ���S�t���O

protected:
	void Release();								// �P�̂̊J������
};
#endif // _SCENE_H_