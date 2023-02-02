//=============================================================================
// ���b�V��(�E�H�\��)���� [mesh_wall.h]
// Authore : ���� ��
//=============================================================================
#ifndef _MESH_WALL_H_
#define _MESH_WALL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "texture.h"

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CMeshWall : public CScene
{
public:
	// ���b�V���E�H�[���̓����̎��
	typedef enum
	{
		MESHWALL_TYPE_NONE = 0,
		MESHWALL_TYPE_RIGHT_LOOP,	// X�����̉E�ɍ��킹�ă��[�v
		MESHWALL_TYPE_MAX
	}MESHWALL_TYPE;

	CMeshWall(int nPriority = PRIORITY_NONE);						// �R���X�g���N�^
	~CMeshWall();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);				// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 rot, int nVertical, int nRow,
		MESHWALL_TYPE type, CTexture::TEX_TYPE tex);				// ��������
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// �e�N�X�`����������
	void SetNum(int nNum);											// �ԍ��ݒ菈��

private:
	void TrackingPlayer();											// �v���C���[�ǔ�����
	LPDIRECT3DTEXTURE9 m_pTexture;									// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;								// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_pos;												// �ʒu
	D3DXVECTOR3	m_size;												// �T�C�Y
	D3DXVECTOR3 m_rot;												// ����
	D3DXMATRIX m_mtxWorld;											// ���[���h�}�g���b�N�X
	int m_nVertical;												// �c�̕�����
	int m_nRow;														// ���̕�����
	int m_nNum;														// �ԍ�
	MESHWALL_TYPE m_type;											// �����̎��
};

#endif // _MESH_FIELD_H_