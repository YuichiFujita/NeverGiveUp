//============================================================
//
//	��Q������ [obstacle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define OBSTACLE_PRIO	(1)	// ��Q���̗D�揇��

#define SIZE_OBSTACLE	(D3DXVECTOR3(60.0f, 35.0f, 160.0f))	// ��Q���̑傫��	// TODO�F��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CObstacle::SModelInfo CObstacle::m_aStatusInfo[] =	// ���f�����
{
	{ // �����f��
		"data\\MODEL\\OBSTACLE\\obstacle000.x",	// ���f���t�@�C����
		{ // �X�e�[�^�X���
			SIZE_OBSTACLE,	// �傫��
			DODGE_JUMP,		// ���@
		},
	},

	{ // �x���`���f��
		"data\\MODEL\\OBSTACLE\\obstacle001.x",	// ���f���t�@�C����
		{ // �X�e�[�^�X���
			SIZE_OBSTACLE,	// �傫��
			DODGE_SLIDE,	// ���@
		},
	},
};

//************************************************************
//	�q�N���X [CObstacle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObstacle::CObstacle(const EType type) : CObjectModel(CObject::LABEL_OBSTACLE, OBSTACLE_PRIO), m_status(m_aStatusInfo[type].status), m_type(type)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CObstacle::~CObstacle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObstacle::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObstacle::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CObstacle::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CObstacle::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��ގ擾����
//============================================================
int CObstacle::GetType(void) const
{
	// ��Q���̉��@��Ԃ�
	return m_status.dodge;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObstacle::GetVec3Sizing(void) const
{
	// ��Q���̑傫����Ԃ�
	return m_status.size;
}

//============================================================
//	��������
//============================================================
CObstacle *CObstacle::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CObstacle *pObstacle = NULL;	// ��Q�������p

	if (pObstacle == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObstacle = new CObstacle(type);	// ��Q��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObstacle != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ��Q���̏�����
		if (FAILED(pObstacle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObstacle;
			pObstacle = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ��ނ�ݒ�
		pObstacle->SetType(type);

		// �ʒu��ݒ�
		pObstacle->SetVec3Position(rPos);

		// ������ݒ�
		pObstacle->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObstacle;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CObstacle::SetType(const EType type)
{
	// �����̎�ނ̏���ݒ�
	if (type < TYPE_MAX)
	{ // ��ނ�����ꍇ

		// ���f����o�^�E����
		BindModel(m_aStatusInfo[type].pTextureFile);
	}
	else { assert(false); }	// ��ރI�[�o�[
}
