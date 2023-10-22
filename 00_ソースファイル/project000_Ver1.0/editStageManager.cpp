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
//	�}�N����`
//************************************************************
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��

#define KEY_CHANGE_THING	(DIK_1)	// �z�u���ύX�L�[
#define NAME_CHANGE_THING	("1")	// �z�u���ύX�\��

#define KEY_MOVE_UP		(DIK_UP)	// �ړ��ʏ㏸�L�[
#define NAME_MOVE_UP	("��")		// �ړ��ʏ㏸�\��
#define KEY_MOVE_DOWN	(DIK_DOWN)	// �ړ��ʉ��~�L�[
#define NAME_MOVE_DOWN	("��")		// �ړ��ʉ��~�\��

#define KEY_FAR		(DIK_W)	// ���ړ��L�[
#define NAME_FAR	("W")	// ���ړ��\��
#define KEY_NEAR	(DIK_S)	// ��O�ړ��L�[
#define NAME_NEAR	("S")	// ��O�ړ��\��
#define KEY_RIGHT	(DIK_D)	// �E�ړ��L�[
#define NAME_RIGHT	("D")	// �E�ړ��\��
#define KEY_LEFT	(DIK_A)	// ���ړ��L�[
#define NAME_LEFT	("A")	// ���ړ��\��
#define KEY_UP		(DIK_Q)	// ��ړ��L�[
#define NAME_UP		("Q")	// ��ړ��\��
#define KEY_DOWN	(DIK_E)	// ���ړ��L�[
#define NAME_DOWN	("E")	// ���ړ��\��

#define KEY_ROTA_RIGHT	(DIK_Z)	// �E��]�L�[
#define NAME_ROTA_RIGHT	("Z")	// �E��]�\��
#define KEY_ROTA_LEFT	(DIK_C)	// ����]�L�[
#define NAME_ROTA_LEFT	("C")	// ����]�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT	= "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g
	const float INIT_MOVE	= 40.0f;	// �z�u���̏����ړ���
	const float CHANGE_MOVE = 10.0f;	// �z�u���̈ړ��ʂ̕ϓ���
	const float MIN_MOVE	= 10.0f;	// �z�u���̍ŏ��ړ���
	const float MAX_MOVE	= 200.0f;	// �z�u���̍ő�ړ���
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
	m_bEdit	= false;			// �G�f�B�b�g��

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
	m_bEdit	= false;			// �G�f�B�b�g��

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

	if (!m_bEdit)
	{ // �G�f�B�b�g���[�h�ł͂Ȃ��ꍇ

		// �����𔲂���
		return;
	}

	// �z�u���ύX�̍X�V
	UpdateChangeThing();

	// �ړ��ʕύX�̍X�V
	UpdateChangeMove();

	// �ʒu�̍X�V
	UpdatePosition();

	// �����̍X�V
	UpdateRotation();

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
//	�G�f�B�b�g�󋵂̐ݒ菈��
//============================================================
void CEditStageManager::SetEnableEdit(const bool bEdit)
{
	// �����̃G�f�B�b�g�󋵂ɂ���
	m_bEdit = bEdit;

	switch (m_thing)
	{ // �z�u�����Ƃ̏���
	case THING_BUILDING:	// �r��

		if (m_pBuilding != NULL)
		{ // �G�f�B�b�g�r�����g�p����Ă���ꍇ

			// �G�f�B�b�g�r���̕\���̐ݒ�
			m_pBuilding->SetDisp(m_bEdit);
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
}

//============================================================
//	�G�f�B�b�g�󋵎擾����
//============================================================
bool CEditStageManager::IsEdit(void) const
{
	// �G�f�B�b�g�󋵂�Ԃ�
	return m_bEdit;
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

//============================================================
//	�z�u���ύX�̍X�V����
//============================================================
void CEditStageManager::UpdateChangeThing(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �z�u����ύX
	if (m_pKeyboard->IsPress(KEY_CHANGE_THING))
	{
		switch (m_thing)
		{ // �z�u�����Ƃ̏���
		case THING_BUILDING:	// �r��

			if (m_pBuilding != NULL)
			{ // �G�f�B�b�g�r�����g�p����Ă���ꍇ

				// �G�f�B�b�g�r���̕\���̐ݒ�
				m_pBuilding->SetDisp(false);
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

		// �z�u���̕ύX
		m_thing = (EThing)((m_thing + 1) % THING_MAX);

		switch (m_thing)
		{ // �z�u�����Ƃ̏���
		case THING_BUILDING:	// �r��

			if (m_pBuilding != NULL)
			{ // �G�f�B�b�g�r�����g�p����Ă���ꍇ

				// �G�f�B�b�g�r���̕\���̐ݒ�
				m_pBuilding->SetDisp(true);
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
	}
}

//============================================================
//	�ړ��ʕύX�̍X�V����
//============================================================
void CEditStageManager::UpdateChangeMove(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �ړ��ʂ�ύX
	if (m_pKeyboard->IsPress(KEY_MOVE_UP))
	{
		m_fMove += CHANGE_MOVE;
	}
	if (m_pKeyboard->IsPress(KEY_MOVE_DOWN))
	{
		m_fMove -= CHANGE_MOVE;
	}

	// �ړ��ʂ�␳
	useful::LimitNum(m_fMove, MIN_MOVE, MAX_MOVE);
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEditStageManager::UpdatePosition(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �ʒu��ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void CEditStageManager::UpdateRotation(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ������ύX
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		m_rot.y += HALF_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		m_rot.y -= HALF_PI;
	}

	// �����𐳋K��
	useful::Vec3NormalizeRot(m_rot);
}
