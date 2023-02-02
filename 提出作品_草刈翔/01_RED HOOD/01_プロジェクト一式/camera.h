//=============================================================================
// �J�������� [camera.h]
// Author : ���� ��
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CCamera
{
public:
	// �J�����̓����̐ݒ�
	typedef enum
	{
		CAMERA_MOVE_TYPE_PLAYER = 0,	// �v���C���[�̒Ǐ]
		CAMERA_MOVE_TYPE_BOSS,			// �{�X��
		CAMERA_MOVE_TYPE_CLEAR,			// �N���A��
		CAMERA_MOVE_TYPE_DEBUG,			// �f�o�b�O�p
		PLAYER_STATE_MAX
	} CAMERA_MOVE_TYPE;

	CCamera();															// �R���X�g���N�^
	~CCamera();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);						// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	static CCamera *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);						// ��������
	void Set(void);														// �J�����ݒ菈��
	void SetPosV(D3DXVECTOR3 pos) { m_posV = pos; }						// ���_�ݒ菈��
	D3DXVECTOR3 GetPosV(void) { return m_posV; }						// ���_�擾����
	void SetPosR(D3DXVECTOR3 pos) { m_posR = pos; }						// �����_�ݒ菈��
	D3DXVECTOR3 GetPosR(void) { return m_posR; }						// �����_�擾����
	void SetRotV(D3DXVECTOR3 rot) { m_rot = rot; }						// ���_�̌����ݒ菈��
	D3DXVECTOR3 GetRotV(void) { return m_rot; }							// ���_�̌����擾����
	D3DVIEWPORT9 *GetViewPort(void) { return &m_viewport; }				// �r���[�|�[�g�擾����
	void SetCameraMove(CAMERA_MOVE_TYPE type);							// �J�����̓����̐ݒ菈��
	static void SetControl(bool bControl) { m_bControl = bControl; }	// ����ł��邩�ݒ菈��

private:
	D3DXVECTOR3 MathFront(void);			// ���ʂ̌������v�Z���鏈��
	void MovePlayer(void);					// �v���C���[�p�̈ړ�����
	void MoveBoss(void);					// �{�X��p�̈ړ�����
	void MoveClear(void);					// �N���A���̈ړ�����
#ifdef _DEBUG
	void DebugMove(void);					// �f�o�b�O�Ŏg���ړ�����
#endif
	D3DXVECTOR3 m_posV;						// ���݂̎��_
	D3DXVECTOR3 m_posR;						// ���݂̒����_
	D3DXVECTOR3 m_vecU;						// ������x�N�g��
	D3DXVECTOR3 m_rot;						// ���_�̌���
	D3DXVECTOR3 m_rotAsk;					// �ړI�̌���
	float m_fSpeed;							// �ړ����x(��Ƀf�o�b�O�Ŏg�p)
	float m_fRotateSpeed;					// ���񑬓x
	float m_fHeight;						// ���_���璍���_�܂ł̍���
	float m_fSaveHeight;					// ���_���璍���_�܂ł̍���
	float m_fDistance_PlayerPosR;			// �v���C���[�̈ʒu�ƒ����_�̋���
	float m_fDistance_posVposR;				// ���_�ƒ����_�̋���
	float m_fSaveDistance_PlayerPosR;		// �v���C���[�̈ʒu�ƒ����_�̖ړI�̋���
	float m_fSaveDistance_posVposR;			// ���_�ƒ����_�̖ړI�̋���
	float m_fObjectiveRot;					// �ړI�̌���
	float m_fObjectiveDistance;				// �ړI�̋���
	float m_fNumRot;						// ������ς����
	D3DXMATRIX m_mtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;					// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;				// �r���[�|�[�g
	bool m_bRotate;							// ��]���Ă��邩�ǂ���
	static bool m_bControl;					// ����ł��邩�ǂ���
	CAMERA_MOVE_TYPE m_type;				// �����̎��
};
#endif // _CAMERA_H_