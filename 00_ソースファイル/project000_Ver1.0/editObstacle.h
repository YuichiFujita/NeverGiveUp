//============================================================
//
//	�G�f�B�b�g��Q���w�b�_�[ [editObstacle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "obstacle.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g��Q���N���X
class CEditObstacle
{
public:
	// �R���X�g���N�^
	CEditObstacle();

	// �f�X�g���N�^
	~CEditObstacle();

	// ��Q�����\����
	struct SInfo
	{
		CObstacle *pObstacle;	// ��Q�����
		CObstacle::EType type;	// ��Q�����
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetDisp(const bool bDisp);	// �\���ݒ�
	void DrawDebugControl(void);	// ����\���`��
	void DrawDebugInfo(void);		// ���\���`��
	void Save(FILE *pFile);			// �ۑ�

	// �ÓI�����o�֐�
	static CEditObstacle *Create(CEditStageManager *pEdit);	// ����
	static HRESULT Release(CEditObstacle *&prEditObstacle);	// �j��

private:
	// �����o�֐�
	void UpdateChangeType(void);	// ��ޕύX�̍X�V
	void CreateObstacle(void);		// ��Q������
	void ReleaseObstacle(void);		// ��Q���j��

	void DeleteCollisionObstacle(const bool bRelase);	// ��Q���̍폜����
	void InitAllColorObstacle(void);					// ��Q���̐F�S������

	// �����o�ϐ�
	CEditStageManager *m_pEdit;	// �G�f�B�b�g�X�e�[�W�̏��
	SInfo m_obstacle;	// ��Q���z�u���
};

#endif	// _EDIT_OBSTACLE_H_
