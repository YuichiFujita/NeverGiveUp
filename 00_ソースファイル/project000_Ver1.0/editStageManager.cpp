//============================================================
//
//	�G�f�B�b�g�X�e�[�W�}�l�[�W���[���� [editStageManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStageManager.h"
#include "manager.h"
#include "input.h"
#include "editBuilding.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT	= "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g
	const float INIT_MOVE	= 40.0f;	// �z�u���̈ړ���
}

//************************************************************
//	�e�N���X [CEditStageManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStageManager::CEditStageManager()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pBuilding = NULL;			// �G�f�B�b�g�r���̏��
	m_pos	= VEC3_ZERO;		// �ʒu
	m_rot	= VEC3_ZERO;		// ����
	m_fMove	= 0.0f;				// �ʒu�ړ���
	m_thing	= THING_BUILDING;	// �z�u��

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditStageManager::~CEditStageManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditStageManager::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pBuilding = NULL;			// �G�f�B�b�g�r���̏��
	m_pos	= VEC3_ZERO;		// �ʒu
	m_rot	= VEC3_ZERO;		// ����
	m_fMove	= INIT_MOVE;		// �ʒu�ړ���
	m_thing	= THING_BUILDING;	// �z�u��

	// �G�f�B�b�g�r���̐���
	m_pBuilding = CEditBuilding::Create(this);
	if (m_pBuilding == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEditStageManager::Uninit(void)
{
#if _DEBUG

	if (m_pBuilding != NULL)
	{ // �G�f�B�b�g�r�����g�p����Ă���ꍇ

		// �G�f�B�b�g�r���̔j��
		CEditBuilding::Release(m_pBuilding);
	}
	else { assert(false); }	// ��g�p��

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditStageManager::Update(void)
{
#if _DEBUG

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �ʒu��ύX
	if (m_pKeyboard->IsTrigger(DIK_S))
	{
		m_pos.z += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_W))
	{
		m_pos.z -= m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_A))
	{
		m_pos.x += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_D))
	{
		m_pos.x -= m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_E))
	{
		m_pos.y += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_Q))
	{
		m_pos.y -= m_fMove;
	}

	// ������ύX
	if (m_pKeyboard->IsTrigger(DIK_Z))
	{
		m_rot.y += HALF_PI;
		useful::Vec3NormalizeRot(m_rot);
	}
	if (m_pKeyboard->IsTrigger(DIK_C))
	{
		m_rot.y -= HALF_PI;
		useful::Vec3NormalizeRot(m_rot);
	}

	switch (m_thing)
	{ // �z�u�����Ƃ̏���
	case THING_BUILDING:	// �r��

		if (m_pBuilding != NULL)
		{ // �G�f�B�b�g�r�����g�p����Ă���ꍇ

			// �G�f�B�b�g�r���̍X�V
			m_pBuilding->Update();
		}
		else { assert(false); }	// ��g�p��

		break;

#if 0

	case THING_SIGNBOARD:	// �Ŕ�
		break;
	case THING_OBSTACLE:	// ��Q��
		break;
	case THING_SAVEPOINT:	// �Z�[�u�|�C���g
		break;
	case THING_GOALPOINT:	// �S�[���|�C���g
		break;

#endif

	default:	// ��O����
		assert(false);
		break;
	}

#endif	// _DEBUG
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	��������
//============================================================
CEditStageManager *CEditStageManager::Create(void)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStageManager *pEditStageManager = NULL;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�����p

	if (pEditStageManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEditStageManager = new CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditStageManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�̏�����
		if (FAILED(pEditStageManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditStageManager;
			pEditStageManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditStageManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s

#else	// NDEBUG

	// NULL��Ԃ�
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
HRESULT CEditStageManager::Release(CEditStageManager *&prEditStageManager)
{
#if _DEBUG

	if (prEditStageManager != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�̏I��
		prEditStageManager->Uninit();

		// �������J��
		delete prEditStageManager;
		prEditStageManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}
