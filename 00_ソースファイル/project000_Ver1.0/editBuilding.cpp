//============================================================
//
//	�G�f�B�b�g�r������ [editBuilding.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editBuilding.h"
#include "manager.h"
#include "input.h"
#include "editStageManager.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	INIT_ALPHA		= 0.5f;		// �z�u�O�̃��l
	const float	EFFECT_RADIUS	= 30.0f;	// �����\���G�t�F�N�g�̔��a
	const int	EFFECT_LIFE		= 10;		// �����\���G�t�F�N�g�̎���
}

//************************************************************
//	�e�N���X [CEditBuilding] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditBuilding::CEditBuilding()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEdit = NULL;	// �G�f�B�b�g�X�e�[�W�̏��
	memset(&m_building, 0, sizeof(m_building));	// �r���z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditBuilding::~CEditBuilding()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditBuilding::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pEdit = NULL;	// �G�f�B�b�g�X�e�[�W�̏��
	m_building.pBuilding = NULL;			// �r�����
	m_building.type = CBuilding::TYPE_00;	// �r�����

	// �r���̐���	// TODO�F��ނ̐ݒ�ňʒu�ƌ����ȊO�̏��͌��܂�悤�ɕύX
	m_building.pBuilding = CBuilding::Create(m_building.type, VEC3_ZERO, VEC3_ZERO, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	if (m_building.pBuilding == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V�E�����`���OFF�ɂ���
	m_building.pBuilding->SetEnableUpdate(false);
	m_building.pBuilding->SetEnableDraw(false);

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
void CEditBuilding::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditBuilding::Update(void)
{
#if _DEBUG

	if (m_pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// ��ޕύX�̍X�V
	UpdateChangeType();

	// �r���̐���
	CreateBuilding();

	// �r���̔j��
	ReleaseBuilding();

	// �����\���G�t�F�N�g�̐���
	CreateRotaEffect();

	// �ʒu�𔽉f
	m_building.pBuilding->SetVec3Position(m_pEdit->GetVec3Position());

	// �����𔽉f
	m_building.pBuilding->SetVec3Rotation(m_pEdit->GetVec3Rotation());

	// ��ނ𔽉f
	m_building.pBuilding->SetType(m_building.type);

#endif
}

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CEditBuilding::SetDisp(const bool bDisp)
{
	// �����X�V�E�����`���\���󋵂ɍ��킹��
	m_building.pBuilding->SetEnableUpdate(bDisp);	// �X�V
	m_building.pBuilding->SetEnableDraw(bDisp);		// �`��
}

//============================================================
//	��������
//============================================================
CEditBuilding *CEditBuilding::Create(CEditStageManager *pEdit)
{
#if _DEBUG

	// �|�C���^��錾
	CEditBuilding *pEditBuilding = NULL;	// �G�f�B�b�g�r�������p

	if (pEditBuilding == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEditBuilding = new CEditBuilding;	// �G�f�B�b�g�r��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditBuilding != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G�f�B�b�g�r���̏�����
		if (FAILED(pEditBuilding->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditBuilding;
			pEditBuilding = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �G�f�B�b�g�X�e�[�W�̏���ݒ�
		pEditBuilding->m_pEdit = pEdit;

		// �m�ۂ����A�h���X��Ԃ�
		return pEditBuilding;
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
HRESULT CEditBuilding::Release(CEditBuilding *&prEditBuilding)
{
#if _DEBUG

	if (prEditBuilding != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�r���̏I��
		prEditBuilding->Uninit();

		// �������J��
		delete prEditBuilding;
		prEditBuilding = NULL;

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
//	��ޕύX�̍X�V����
//============================================================
void CEditBuilding::UpdateChangeType(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��ނ�ύX
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_building.type = (CBuilding::EType)((m_building.type + 1) % CBuilding::TYPE_MAX);
	}
}

//============================================================
//	�r���̐�������
//============================================================
void CEditBuilding::CreateBuilding(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit = m_pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���
	D3DXCOLOR colBuild = XCOL_WHITE;	// �F�ۑ��p

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �r����z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�r���̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_building.pBuilding->SetEnableUpdate(true);
		m_building.pBuilding->SetEnableDraw(true);

		// �F��ݒ�
		colBuild = m_building.pBuilding->GetCubeColor();	// ���̐F���擾
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, 1.0f));

		//----------------------------------------------------
		//	�V�����r���̐���
		//----------------------------------------------------
		// �r���̐���	// TODO�F��ނ̐ݒ�ňʒu�ƌ����ȊO�̏��͌��܂�悤�ɕύX
		m_building.pBuilding = CBuilding::Create(m_building.type, posEdit, rotEdit, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
		assert(m_building.pBuilding != NULL);

		// �F��ݒ�
		colBuild = m_building.pBuilding->GetCubeColor();	// ���̐F���擾
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, INIT_ALPHA));
	}
}

//============================================================
//	�r���̔j������
//============================================================
void CEditBuilding::ReleaseBuilding(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �r����z�u
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{

	}
}

//============================================================
//	�����\���G�t�F�N�g�̐�������
//============================================================
void CEditBuilding::CreateRotaEffect(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEffect = VEC3_ZERO;								// �G�t�F�N�g�ʒu
	D3DXVECTOR3 posEdit   = m_pEdit->GetVec3Position();				// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit   = m_pEdit->GetVec3Rotation();				// �G�f�B�b�g�̌���
	D3DXVECTOR3 sizeBuild = m_building.pBuilding->GetVec3Sizing();	// �r���傫��

	// �G�t�F�N�g�ʒu���v�Z
	posEffect.x = posEdit.x + sinf(rotEdit.y + D3DX_PI) * sizeBuild.x;
	posEffect.y = posEdit.y + sizeBuild.y * 2.0f;
	posEffect.z = posEdit.z + cosf(rotEdit.y + D3DX_PI) * sizeBuild.z;

	// �����\���G�t�F�N�g�𐶐�
	CEffect3D::Create(posEffect, EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE);
}
