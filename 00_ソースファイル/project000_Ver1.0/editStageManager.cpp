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
#include "effect3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT	= "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g
	const float INIT_MOVE	= 40.0f;	// �z�u���̈ړ���
	const float INIT_ALPHA	= 0.5f;		// �z�u�O�̃��l
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
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_fMove = 0.0f;		// �ʒu�ړ���
	m_thing = THING_BUILDING;	// �z�u��
	memset(&m_building, 0, sizeof(m_building));	// �r���z�u���

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
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_fMove = INIT_MOVE;	// �ʒu�ړ���
	m_thing = THING_BUILDING;	// �z�u��

	// �r���z�u����������
	m_building.type = CBuilding::TYPE_00;	// �r�����

	// �r���̐���	// TODO�F��ނ̐ݒ�ňʒu�ƌ����ȊO�̏��͌��܂�悤�ɕύX
	m_building.pBuilding = CBuilding::Create(m_building.type, m_pos, m_rot, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	if (m_building.pBuilding == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V��OFF�ɂ���
	m_building.pBuilding->SetEnableUpdate(false);

	// �F��ݒ�
	D3DXCOLOR col = m_building.pBuilding->GetCubeColor();	// ���̐F���擾
	m_building.pBuilding->SetCubeColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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

		// �r���z�u�̍X�V
		UpdateBuilding();

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
//	�r���z�u�̍X�V����
//============================================================
void CEditStageManager::UpdateBuilding(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	//--------------------------------------------------------
	//	����̍X�V
	//--------------------------------------------------------
	// ��ނ�ύX
	if (m_pKeyboard->IsTrigger(DIK_1))
	{
		m_building.type = (CBuilding::EType)((m_building.type + 1) % CBuilding::TYPE_MAX);
	}

	// �r����z�u
	if (m_pKeyboard->IsTrigger(DIK_0))
	{
		//----------------------------------------------------
		//	�r���̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V��ON�ɂ���
		m_building.pBuilding->SetEnableUpdate(true);

		// �F��ݒ�
		D3DXCOLOR colSet = m_building.pBuilding->GetCubeColor();	// ���̐F���擾
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colSet.r, colSet.g, colSet.b, 1.0f));

		//----------------------------------------------------
		//	�V�����r���̐���
		//----------------------------------------------------
		// �r���̐���	// TODO�F��ނ̐ݒ�ňʒu�ƌ����ȊO�̏��͌��܂�悤�ɕύX
		m_building.pBuilding = CBuilding::Create(m_building.type, m_pos, m_rot, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
		assert(m_building.pBuilding != NULL);

		// �����X�V��OFF�ɂ���
		m_building.pBuilding->SetEnableUpdate(false);

		// �F��ݒ�
		D3DXCOLOR colNew = m_building.pBuilding->GetCubeColor();	// ���̐F���擾
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colNew.r, colNew.g, colNew.b, INIT_ALPHA));
	}

	//--------------------------------------------------------
	//	�\���̍X�V
	//--------------------------------------------------------
	// �v���C���[�̐i�s�����ɃG�t�F�N�g��\��
	D3DXVECTOR3 posEffect = VEC3_ZERO;	// �G�t�F�N�g�ʒu
	D3DXVECTOR3 sizeBuilding = m_building.pBuilding->GetVec3Sizing();	// �r���傫��

	// �G�t�F�N�g�ʒu���v�Z
	posEffect.x = m_pos.x + sinf(m_rot.y + D3DX_PI) * sizeBuilding.x;
	posEffect.y = m_pos.y + sizeBuilding.y * 2.0f;
	posEffect.z = m_pos.z + cosf(m_rot.y + D3DX_PI) * sizeBuilding.z;

	// �G�t�F�N�g�\��
	CEffect3D::Create(posEffect, 30.0f, CEffect3D::TYPE_NORMAL, 10);

	// �ʒu�𔽉f
	m_building.pBuilding->SetVec3Position(m_pos);

	// �����𔽉f
	m_building.pBuilding->SetVec3Rotation(m_rot);

	// ��ނ𔽉f
	m_building.pBuilding->SetType(m_building.type);
}
