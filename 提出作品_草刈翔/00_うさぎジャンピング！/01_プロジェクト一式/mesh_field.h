//=============================================================================
// ���b�V��(�t�B�[���h)���� [mesh_field.h]
// Authore : ���� ��
//=============================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "texture.h"

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CMeshField : public CScene
{
public:
	// ���b�V���t�B�[���h�̓����̎��
	typedef enum
	{
		MESHFIELD_TYPE_NONE = 0,
		MESHFIELD_TYPE_RIGHT_LOOP,			// X�����̉E�ɍ��킹�ă��[�v
		MESHFIELD_TYPE_MAX
	}MESHFIELD_TYPE;

	CMeshField(int nPriority = PRIORITY_NONE);						// �R���X�g���N�^
	~CMeshField();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nRow, int nLine,
		MESHFIELD_TYPE type, CTexture::TEX_TYPE tex);				// ��������
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// �e�N�X�`����������
	void SetNum(int nNum);											// �ԍ��ݒ菈��
	static bool Collision(CScene *pScene);							// �����蔻��

private:
	void TrackingPlayer();											// �v���C���[�ǔ�����
	LPDIRECT3DTEXTURE9 m_pTexture;									// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_pos;												// �ʒu
	D3DXVECTOR3	m_size;												// �T�C�Y
	D3DXVECTOR3 m_rot;												// ����
	D3DXMATRIX m_mtxWorld;											// ���[���h�}�g���b�N�X
	int m_nRow;														// ���̕�����
	int m_nLine;													// ���̕�����
	int m_nNum;														// �ԍ�
	MESHFIELD_TYPE m_type;											// �����̎��
};

#endif // _MESH_FIELD_H_