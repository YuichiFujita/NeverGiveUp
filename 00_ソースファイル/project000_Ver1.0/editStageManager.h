//============================================================
//
//	�G�f�B�b�g�X�e�[�W�}�l�[�W���[�w�b�_�[ [editStageManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_STAGEMANAGER_H_
#define _EDIT_STAGEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "building.h"

//************************************************************
//	�O���錾
//************************************************************
class CBuilding;	// �r���N���X
class CSignboard;	// �ŔN���X
class CObstacle;	// ��Q���N���X
class CSavePoint;	// �Z�[�u�|�C���g�N���X
class CGoalPoint;	// �S�[���|�C���g�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X
class CEditStageManager
{
public:
	// �z�u����
	enum EThing
	{
		THING_BUILDING = 0,	// �r��
		THING_SIGNBOARD,	// �Ŕ�
		THING_OBSTACLE,		// ��Q��
		THING_SAVEPOINT,	// �Z�[�u�|�C���g
		THING_GOALPOINT,	// �S�[���|�C���g
		THING_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditStageManager();

	// �f�X�g���N�^
	~CEditStageManager();

	// �r�����\����
	struct SBuildingInfo
	{
		CBuilding *pBuilding;	// �r�����
		CBuilding::EType type;	// �r�����
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CEditStageManager *Create(void);	// ����
	static HRESULT Release(CEditStageManager *&prEditStageManager);	// �j��

private:
	// �����o�֐�
	void UpdateBuilding(void);	// �r���z�u�X�V

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	EThing m_thing;		// �z�u��
	SBuildingInfo m_building;	// �r���z�u���
	float m_fMove;	// �ʒu�ړ���
};

#endif	// _EDIT_STAGEMANAGER_H_
