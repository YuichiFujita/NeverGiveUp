//============================================================
//
//	�Z�[�u�|�C���g���� [savePoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "savePoint.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "scene.h"
#include "player.h"

#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SAVEPOINT_PRIO	(1)	// �Z�[�u�|�C���g�̗D�揇��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 COLL_SIZE = D3DXVECTOR3(280.0f, 0.0f, 280.0f);	// �Z�[�u����̑傫��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CSavePoint::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\POINT\\save000.x",	// �Z�[�u�|�C���g���f��
};
CSavePoint *CSavePoint::m_pCurrentSave = NULL;	// ���݂̃Z�[�u�|�C���g�ւ̃|�C���^
int CSavePoint::m_nNumAll = 0;	// �Z�[�u�|�C���g�̑���

//************************************************************
//	�q�N���X [CSavePoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSavePoint::CSavePoint() : CObjectModel(CObject::LABEL_SAVEPOINT, SAVEPOINT_PRIO), m_nThisSaveID(m_nNumAll)
{
	// �Z�[�u�|�C���g�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSavePoint::~CSavePoint()
{
	// �Z�[�u�|�C���g�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CSavePoint::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_nThisSaveID == 0)
	{ // ���g�̃Z�[�u�|�C���g����ڂ̏ꍇ

		// ���g�̃Z�[�u�|�C���g��ۑ�
		m_pCurrentSave = this;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSavePoint::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSavePoint::Update(void)
{
	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();

	// �f�o�b�O�\��
	if (m_nThisSaveID == 0)
	{
		CManager::GetInstance()->GetDebugProc()->Print("���̃Z�[�u�|�C���g�F%d\n", m_pCurrentSave->m_nThisSaveID);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CSavePoint::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��������
//============================================================
CSavePoint *CSavePoint::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CSavePoint *pSavePoint = NULL;	// �Z�[�u�|�C���g�����p

	if (pSavePoint == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pSavePoint = new CSavePoint;	// �Z�[�u�|�C���g
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pSavePoint != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �Z�[�u�|�C���g�̏�����
		if (FAILED(pSavePoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pSavePoint;
			pSavePoint = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pSavePoint->SetVec3Position(rPos);

		// ������ݒ�
		pSavePoint->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pSavePoint;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Z�[�u�|�C���g���擾����
//============================================================
CSavePoint::SInfo CSavePoint::GetSavePointInfo(void)
{
	// �ϐ���錾
	SInfo info = { VEC3_ZERO, VEC3_ZERO };	// �Z�[�u�|�C���g���

	if (m_pCurrentSave != NULL)
	{ // �Z�[�u�|�C���g�����݂���ꍇ

		// �Z�[�u�|�C���g�̏���ݒ�
		info.pos = m_pCurrentSave->GetVec3Position();	// �ʒu
		info.rot = m_pCurrentSave->GetVec3Rotation();	// ����
	}
	else { assert(false); }	// �Z�[�u�|�C���g����

	// �Z�[�u�|�C���g����Ԃ�
	return info;
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
void CSavePoint::CollisionPlayer(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// �ϐ���錾
	bool  bHit = false;	// �����
	float fPlayerRadius = pPlayer->GetRadius();			// �v���C���[���a
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 posSave = GetVec3Position();			// �Z�[�u�ʒu
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius);	// �v���C���[�傫��

	if (pPlayer == NULL)
	{ // �v���C���[�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �v���C���[�Ƃ̔���
	bHit = collision::Box2D
	( // ����
		posPlayer,	// ����ʒu
		posSave,	// ����ڕW�ʒu
		sizePlayer,	// ����T�C�Y(�E�E��E��)
		sizePlayer,	// ����T�C�Y(���E���E�O)
		COLL_SIZE,	// ����ڕW�T�C�Y(�E�E��E��)
		COLL_SIZE	// ����ڕW�T�C�Y(���E���E�O)
	);
	if (bHit)
	{ // �v���C���[��������̏ꍇ

		// �Z�[�u�|�C���g�����g�ɕύX
		m_pCurrentSave = this;

		// �f�o�b�O�p�[�e�B�N��
		CParticle3D::Create(CParticle3D::TYPE_HEAL, pPlayer->GetVec3Position());
	}
}
