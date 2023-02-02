//=============================================================================
// ���U���g��� [result.h]
// Author : ���� ��
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_MAX_UI			(8)			// UI�̍ő吔
#define RESULT_MAX_PLAYER		(2)			// �ő�l��
#define RESULT_UI_POSITION_Y	(220.0f)	// ���U���g������Y���W�ʒu
#define RESULT_UI_SIZE_X		(520.0f)	// ���U���g�����̃T�C�YX
#define RESULT_UI_SIZE_Y		(80.0f)		// ���U���g�����̃T�C�YY
#define RESULT_UI_BAND_POS_Y	(75.0f)		// ���U���g�т�Y���W�ʒu
#define RESULT_UI_BAND_SIZE_Y	(150.0f)	// ���U���g�т̃T�C�YY
#define RESULT_UI_TOP_POS_Y		(60.0f)		// ���U���g�g�b�v������Y���W�ʒu
#define RESULT_UI_TOP_SIZE_X	(400.0f)	// ���U���g�g�b�v�����̃T�C�YX
#define RESULT_UI_TOP_SIZE_Y	(110.0f)	// ���U���g�g�b�v�����̃T�C�YY
#define RESULT_UI_1P_POS_X		(360.0f)	// 1P�\���̈ʒu���WX
#define RESULT_UI_1P_POS_Y		(210.0f)	// 1P�\���̈ʒu���WY
#define RESULT_UI_1P_SIZE_X		(190.0f)	// 1P�\���̃T�C�YX
#define RESULT_UI_1P_SIZE_Y		(90.0f)		// 1P�\���̃T�C�YY
#define RESULT_UI_WIN_SIZE_X	(250.0f)	// �u�����I�v�\���̃T�C�YX
#define RESULT_UI_DRAW_SIZE_X	(400.0f)	// �u�Ђ��킯�c�v�\���̃T�C�YX

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUi;
class CBillboard;
class CPlayer;
class CScore;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();											// �R���X�g���N�^
	~CResult();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

private:
	CUi *m_apUi[RESULT_MAX_UI];							// UI�̃|�C���^
	CBillboard *m_pBillboard;							// 2D�I�u�W�F�N�g�̃|�C���^
	CPlayer *m_pPlayer[MAX_PLAYER_NUM];					// �v���C���[�̃|�C���^
	CScore *m_pScore[RESULT_MAX_PLAYER];				// �X�R�A�̃|�C���^
};

#endif	// _RESULT_H_