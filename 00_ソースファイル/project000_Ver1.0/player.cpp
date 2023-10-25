//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"

#include "multiModel.h"
#include "shadow.h"
#include "stage.h"
#include "field.h"
#include "building.h"
#include "obstacle.h"
#include "savePoint.h"

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g

	const int PRIORITY = 3;	// �v���C���[�̗D�揇��

	// �v���C���[��{���
	namespace basic
	{
		const float	MOVE		= 2.8f;		// �ړ���
		const float	JUMP		= 20.0f;	// �W�����v�㏸��
		const float	GRAVITY		= 1.0f;		// �d��
		const float	RADIUS		= 20.0f;	// ���a
		const float	HEIGHT		= 100.0f;	// �c��
		const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
		const float	BLOW_SIDE	= 40.0f;	// ������ю��̉��ړ���
		const float	BLOW_UP		= 30.0f;	// ������ю��̏c�ړ���

		const float	JUMPPAD_MOVE	= 60.0f;	// �W�����v�p�b�h�̏�ړ���
		const float	NOR_JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
		const float	NOR_LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
		const float	DMG_JUMP_REV	= 0.01f;	// �_���[�W��Ԏ��̋󒆂̈ړ��ʂ̌����W��
		const float	DMG_LAND_REV	= 0.5f;		// �_���[�W��Ԏ��̒n��̈ړ��ʂ̌����W��
		const float DMG_SUB_ALPHA	= 0.025f;	// �_���[�W��Ԏ��̓����x�̌��Z��
		const float	SPAWN_ADD_ALPHA	= 0.03f;	// �X�|�[����Ԏ��̓����x�̉��Z��

		const D3DXVECTOR3 DMG_ADDROT = D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	}

	// �X���C�f�B���O���
	namespace slide
	{
		const float	SLIDE_CONTROL_MIN	= 2.4f;		// �X���C�f�B���O���\�ɂȂ�ړ���
		const float	PLUS_SUB_MOVE		= 0.0035f;	// �ړ��ʂ̍ő�ǉ����Z��

		const int	MIN_END_CNT	= 30;		// �X���C�f�B���O�̉������삪�ł���悤�ɂȂ�J�E���g
		const int	MAX_END_CNT	= 80;		// �X���C�f�B���O�������I������J�E���g
		const float	MIN_MOVE	= 1.25f;	// �ړ��ʂ̍Œᑬ�x
		const float	SUB_MOVE	= 0.01f;	// �X���C�f�B���O���̑��x���Z��
		const float	ADD_MOVE	= 0.08f;	// ��X���C�f�B���O���̑��x���Z��
	}

	// �v���C���[���N���X���
	namespace other
	{
		const D3DXVECTOR3 SHADOW_SIZE = D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\PLAYER\\00_waist.x",	// ��
	"data\\MODEL\\PLAYER\\01_body.x",	// ��
	"data\\MODEL\\PLAYER\\02_head.x",	// ��
	"data\\MODEL\\PLAYER\\03_armUL.x",	// ����r
	"data\\MODEL\\PLAYER\\04_armUR.x",	// �E��r
	"data\\MODEL\\PLAYER\\05_armDL.x",	// �����r
	"data\\MODEL\\PLAYER\\06_armDR.x",	// �E���r
	"data\\MODEL\\PLAYER\\07_handL.x",	// ����
	"data\\MODEL\\PLAYER\\08_handR.x",	// �E��
	"data\\MODEL\\PLAYER\\09_legUL.x",	// ��������
	"data\\MODEL\\PLAYER\\10_legUR.x",	// �E������
	"data\\MODEL\\PLAYER\\11_legDL.x",	// ����
	"data\\MODEL\\PLAYER\\12_legDR.x",	// �E��
	"data\\MODEL\\PLAYER\\13_footL.x",	// ����
	"data\\MODEL\\PLAYER\\14_footR.x",	// �E��
};

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, PRIORITY)
{
	// �����o�ϐ����N���A
	m_pShadow		= NULL;			// �e�̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterSlide	= 0;			// �X���C�f�B���O�Ǘ��J�E���^�[
	m_fMove			= 0.0f;			// �ړ���
	m_bJump			= false;		// �W�����v��
	m_bSlide		= false;		// �X���C�f�B���O��
	m_bSlideControl	= false;		// �X���C�f�B���O�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	m_pShadow		= NULL;			// �e�̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterSlide = 0;			// �X���C�f�B���O�Ǘ��J�E���^�[
	m_fMove			= basic::MOVE;	// �ړ���
	m_bJump			= true;			// �W�����v��
	m_bSlide		= false;		// �X���C�f�B���O��
	m_bSlideControl	= false;		// �X���C�f�B���O�����

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ���f�����̐ݒ�
	SetModelInfo();

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, other::SHADOW_SIZE, this);
	if (m_pShadow == NULL)
	{ // ��g�p���̏ꍇ

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
void CPlayer::Uninit(void)
{
	// �e�̏I��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal();

		break;

	case STATE_DAMAGE:

		// �_���[�W��Ԏ��̍X�V
		currentMotion = UpdateDamage();

		break;

	case STATE_CLEAR:
		break;

	case STATE_OVER:
		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	m_pShadow->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL)
		{ // �ʏ��Ԃ̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ������уx�N�g����ݒ�
			m_move.x = sinf(rotPlayer.y) * basic::BLOW_SIDE;
			m_move.y = basic::BLOW_UP;
			m_move.z = cosf(rotPlayer.y) * basic::BLOW_SIDE;

			// ��Ԃ�ύX
			SetState(STATE_DAMAGE);	// �_���[�W���
		}
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	if (m_state != CPlayer::STATE_CLEAR
	&&  m_state != CPlayer::STATE_OVER)
	{ // �Q�[���I���Ɋւ����Ԃł͂Ȃ��ꍇ

		// �����̏�Ԃ�ݒ�
		m_state = (EState)nState;
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return basic::RADIUS;
}

//============================================================
//	�c���擾����
//============================================================
float CPlayer::GetHeight(void) const
{
	// �c����Ԃ�
	return basic::HEIGHT;
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = NULL;	// �v���C���[�����p

	if (pPlayer == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPlayer = new CPlayer;	// �v���C���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pPlayer != NULL)
	{ // �g�p����Ă���ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPlayer;
			pPlayer = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
}

//============================================================
//	�o���̐ݒ菈��
//============================================================
void CPlayer::SetSpawn(void)
{
	if (CSavePoint::GetNumAll() <= 0)
	{ // �Z�[�u�|�C���g������Ȃ��ꍇ

		// �����𔲂���
		return;
	}

	// �ϐ���錾
	CSavePoint::SInfo SavePointInfo = CSavePoint::GetSavePointInfo();	// �Z�[�u�|�C���g�̏��

	// ����������
	SetState(STATE_SPAWN);	// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�
	m_nCounterState = 0;	// �J�E���^�[��������

	// �ʒu��ݒ�
	SetVec3Position(SavePointInfo.pos);

	// ������ݒ�
	SetVec3Rotation(SavePointInfo.rot);
	m_destRot = SavePointInfo.rot;

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �Ǐ]�J�����̖ڕW�ʒu�̐ݒ�
	CManager::GetInstance()->GetCamera()->SetDestFollow();
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	if (UpdateFadeOut(basic::SPAWN_ADD_ALPHA))
	{ // �s�����ɂȂ�؂����ꍇ

		// ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return MOTION_IDOL;
	}

	// �ړ�����
	currentMotion = UpdateMove();

	// �W�����v����
	UpdateJump();

	// �X���C�f�B���O����
	UpdateSliding();

	// �����X�V
	UpdateRotation(rotPlayer);

	// ���n����
	UpdateLanding(posPlayer);

	// �����ύX�̓����蔻��
	CollisionRotationBuilding(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, basic::RADIUS);

	// ��Q���Ƃ̓����蔻��
	if (CollisionObstacle(posPlayer))
	{ // ���������ꍇ

		// �q�b�g
		Hit();
	}

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�v���C���[�ړ����x]�F%f\n", m_fMove);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, (m_bJump) ? "[�W�����v]�FON\n" : "[�W�����v]�FOFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, (m_bSlide) ? "[�X���C�f�B���O]�FON\n" : "[�X���C�f�B���O]�FOFF\n");

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�_���[�W��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateDamage(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return MOTION_IDOL;
	}

	// ���������Z
	rotPlayer += basic::DMG_ADDROT;

	// ���n����
	UpdateLanding(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, basic::RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �t�F�[�h�C����Ԏ��̍X�V
	if (UpdateFadeIn(basic::DMG_SUB_ALPHA))
	{ // �����ɂȂ�؂����ꍇ

		// �o���̐ݒ�
		SetSpawn();
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�v���C���[�ړ���]�F%f %f %f\n", m_move.x, m_move.y, m_move.z);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(void)
{
#if 0
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h

	if (pKeyboard->IsPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->IsPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf((D3DX_PI * 0.25f)) * 2.0f;
			m_move.z -= cosf((D3DX_PI * 0.25f)) * 2.0f;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf((D3DX_PI * 0.75f)) * 2.0f;
			m_move.z += cosf((D3DX_PI * 0.75f)) * 2.0f;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(0.0f) * 2.0f;
			m_move.z -= cosf(0.0f) * 2.0f;
		}
	}
	else if (pKeyboard->IsPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->IsPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf((D3DX_PI * 0.75f)) * 2.0f;
			m_move.z -= cosf((D3DX_PI * 0.75f)) * 2.0f;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf((D3DX_PI * 0.25f)) * 2.0f;
			m_move.z += cosf((D3DX_PI * 0.25f)) * 2.0f;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(0.0f) * 2.0f;
			m_move.z += cosf(0.0f) * 2.0f;
		}
	}
	else if (pKeyboard->IsPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x -= sinf((D3DX_PI * 0.5f)) * 2.0f;
		m_move.z -= cosf((D3DX_PI * 0.5f)) * 2.0f;
	}
	else if (pKeyboard->IsPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf((D3DX_PI * 0.5f)) * 2.0f;
		m_move.z += cosf((D3DX_PI * 0.5f)) * 2.0f;
	}

	// �ڕW�������X�V
	//m_destRot.y = atan2f(-m_move.x, -m_move.z);
#else
	// �ړ��ʂ��X�V
	m_move.x += sinf(m_destRot.y + D3DX_PI) * m_fMove;
	m_move.z += cosf(m_destRot.y + D3DX_PI) * m_fMove;

	if (!m_bSlide)
	{ // �X���C�f�B���O���ł͂Ȃ��ꍇ

		if (m_fMove < basic::MOVE)
		{ // �ړ��ʂ��ō����x�ł͂Ȃ��ꍇ

			// �ړ��ʂ����Z
			m_fMove += slide::ADD_MOVE;

			if (m_fMove > basic::MOVE)
			{ // �ړ��ʂ��ō����x�𒴂����ꍇ

				// �ړ��ʂ�␳
				m_fMove = basic::MOVE;
			}
		}
	}
#endif

	// �ړ����[�V������Ԃ�
	return MOTION_MOVE;
}

//============================================================
//	�W�����v�̍X�V����
//============================================================
void CPlayer::UpdateJump(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (!m_bJump && !m_bSlide)
	{ // �W�����v�ƃX���C�f�B���O�����Ă��Ȃ��ꍇ

		if (pKeyboard->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_B))
		{ // �W�����v�̑��삪�s��ꂽ�ꍇ

			// ��ړ���
			m_move.y += basic::JUMP;

			// �W�����v���Ă����Ԃɂ���
			m_bJump = true;

			// �W�����v���[�V������ݒ�
			SetMotion(MOTION_JUMP);
		}
	}
}

//============================================================
//	�X���C�f�B���O�̍X�V����
//============================================================
void CPlayer::UpdateSliding(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (!m_bSlide)
	{ // �X���C�f�B���O���Ă��Ȃ��ꍇ

		if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_A))
		{ // �X���C�f�B���O�̑��삪�s��ꂽ�ꍇ

			// �X���C�f�B���O���삪�s��ꂽ����ۑ�
			m_bSlideControl = true;
		}

		if (!m_bJump)
		{ // �W�����v���Ă��Ȃ��ꍇ

			if (m_fMove >= slide::SLIDE_CONTROL_MIN)
			{ // �X���C�f�B���O���\�Ȉړ����x�̏ꍇ

				if (m_bSlideControl)
				{ // �X���C�f�B���O�̑��삪�s��ꂽ�ꍇ

					// �X���C�f�B���O���삪�s��ꂽ����������
					m_bSlideControl = false;

					// �X���C�f�B���O���Ă����Ԃɂ���
					m_bSlide = true;

					// �X���C�f�B���O���[�V������ݒ�
					SetMotion(MOTION_SLIDE);
				}
			}
		}
	}
	else
	{ // �X���C�f�B���O���Ă���ꍇ

		// �ϐ���錾
		float fPlusSubMove = (slide::PLUS_SUB_MOVE / (float)slide::MAX_END_CNT) * (float)(slide::MAX_END_CNT - m_nCounterSlide);	// �ړ��ʂ̒ǉ����Z��

		// �J�E���^�[�����Z
		m_nCounterSlide++;

		if (m_fMove > slide::MIN_MOVE)
		{ // �ړ��ʂ��Œᑬ�x�ł͂Ȃ��ꍇ

			// �ړ��ʂ����Z
			m_fMove -= slide::SUB_MOVE + fPlusSubMove;

			if (m_fMove < slide::MIN_MOVE)
			{ // �ړ��ʂ��Œᑬ�x�𒴂����ꍇ

				// �ړ��ʂ�␳
				m_fMove = slide::MIN_MOVE;
			}
		}

		if (m_nCounterSlide > slide::MIN_END_CNT)
		{ // �X���C�f�B���O�̉������삪�ł���J�E���g�ɓ��B�����ꍇ

			if (!(pKeyboard->IsPress(DIK_RETURN) || pPad->IsPress(CInputPad::KEY_A)))
			{ // �X���C�f�B���O�����̑��삪�s��ꂽ�ꍇ

				// �J�E���^�[��������
				m_nCounterSlide = 0;

				// �X���C�f�B���O���Ă��Ȃ���Ԃɂ���
				m_bSlide = false;
			}
		}

		if (m_nCounterSlide > slide::MAX_END_CNT)
		{ // �X���C�f�B���O�������I������J�E���g�ɓ��B�����ꍇ

			// �J�E���^�[��������
			m_nCounterSlide = 0;

			// �X���C�f�B���O���Ă��Ȃ���Ԃɂ���
			m_bSlide = false;
		}
	}
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// ���n����
	if (CollisionBuilding(rPos))
	{ // �u���b�N�ɒ��n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
	else if (CScene::GetStage()->LandFieldPosition(rPos, m_move)
	||       CScene::GetStage()->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
	else
	{ // ���n���Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		m_bJump = true;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * basic::REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	// �ϐ���錾
	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����

	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop(nAnimMotion))
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	// ���[�V�����̑J��
	if (IsMotionFinish())
	{ // ���[�V�������I�����Ă����ꍇ

		switch (GetMotionType())
		{ // ���[�V�����̎�ނ��Ƃ̏���
		case MOTION_JUMP:	// �W�����v���[�V����

			if (!m_bJump)
			{ // �W�����v���ł͂Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}

			// �����𔲂���
			break;

		case MOTION_SLIDE:	// �X���C�f�B���O���[�V����

			if (!m_bSlide)
			{ // �X���C�f�B���O���ł͂Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�l�����ϊ�����
//============================================================
CPlayer::EDirection CPlayer::FourDirection
(
	const float fRot,		// ����
	const float fTolerance	// ���e�l
)
{
	if (fabsf(fRot) <= fTolerance)
	{ // ���e�l�ȉ��̌덷�̏ꍇ

		// �����F0����Ԃ�
		return DIRECTION_DOWN;
	}
	else if (fabsf(fRot - HALF_PI_TWO_DP) <= fTolerance)
	{ // ���e�l�ȉ��̌덷�̏ꍇ

		// �����F90����Ԃ�
		return DIRECTION_RIGHT;
	}
	else if (fabsf(fRot - PI_TWO_DP) <= fTolerance)
	{ // ���e�l�ȉ��̌덷�̏ꍇ

		// �����F180����Ԃ�
		return DIRECTION_UP;
	}
	else if (fabsf(fRot - -HALF_PI_TWO_DP) <= fTolerance)
	{ // ���e�l�ȉ��̌덷�̏ꍇ

		// �����F270����Ԃ�
		return DIRECTION_LEFT;
	}

	// �����Ȃ���Ԃ�
	return DIRECTION_NONE;
}

//============================================================
//	�����ύX�̓����蔻��
//============================================================
void CPlayer::CollisionRotationBuilding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(basic::RADIUS, 0.0f, basic::RADIUS);			// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(basic::RADIUS, basic::HEIGHT, basic::RADIUS);	// �v���C���[�ő�傫��
	float fDisPosY = 0.0f;	// �v���C���[�ƃr����Y����
	bool bInitDis = false;	// Y�����̏�������

	// �|�C���^��錾
	CObject *pCurrentRotObj = NULL;	// ���݂̌����ݒ�I�u�W�F�N�g

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 posBuild = VEC3_ZERO;		// �r���ʒu
				D3DXVECTOR3 rotBuild = VEC3_ZERO;		// �r������
				D3DXVECTOR3 sizeMinBuild = VEC3_ZERO;	// �r���ŏ��傫��
				D3DXVECTOR3 sizeMaxBuild = VEC3_ZERO;	// �r���ő�傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BUILDING)
				{ // �I�u�W�F�N�g���x�����r���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() != CBuilding::COLLISION_GROUND)
				{ // ��Ԃ��n�ʂł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �r���̈ʒu��ݒ�
				posBuild = pObjCheck->GetVec3Position();

				// �r���̌�����ݒ�
				rotBuild = pObjCheck->GetVec3Rotation();

				// �r���̍ŏ��̑傫����ݒ�
				sizeMinBuild = pObjCheck->GetVec3Sizing();
				sizeMinBuild.y = 0.0f;	// �c�̑傫����������

				// �r���̍ő�̑傫����ݒ�
				sizeMaxBuild = pObjCheck->GetVec3Sizing();
				sizeMaxBuild.y *= 2.0f;	// �c�̑傫����{�ɂ���

				// �����ύX�̓����蔻��
				if (collision::Box2D
				( // ����
					rPos,			// ����ʒu
					posBuild,		// ����ڕW�ʒu
					sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
					sizeMinPlayer,	// ����T�C�Y(���E���E�O)
					sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
					sizeMinBuild	// ����ڕW�T�C�Y(���E���E�O)
				))
				{ // ������������ꍇ

					// �ϐ���錾
					float fDis = rPos.y - (posBuild.y + sizeMaxBuild.y);	// �v���C���[�ƃr����Y����

					if (fDis >= 0.0f)
					{ // �v���C���[�ƃr����Y�������v���X�̏ꍇ

						if (!bInitDis)
						{ // ���������Ă��Ȃ��ꍇ

							// ���݂̋�������
							fDisPosY = fDis;

							// ���݂̌����ݒ�I�u�W�F�N�g����
							pCurrentRotObj = pObjCheck;

							// �������ς݂ɂ���
							bInitDis = true;
						}
						else
						{ // ���������Ă���ꍇ

							if (fDis < fDisPosY)
							{ // ���߂��r���̏ꍇ

								// ���݂̋�������
								fDisPosY = fDis;

								// ���݂̌����ݒ�I�u�W�F�N�g����
								pCurrentRotObj = pObjCheck;
							}
						}
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	if (pCurrentRotObj != NULL)
	{ // �r�����������ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posBuild = VEC3_ZERO;	// �r���ʒu
		D3DXVECTOR3 rotBuild = VEC3_ZERO;	// �r������
		D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// �r���傫��
		D3DXVECTOR3 posLeft = VEC3_ZERO;	// �v���C���[���猩���r���̍��ʒu
		D3DXVECTOR3 posRight = VEC3_ZERO;	// �v���C���[���猩���r���̉E�ʒu
		
		// �r���̈ʒu��ݒ�
		posBuild = pCurrentRotObj->GetVec3Position();

		// �r���̌�����ݒ�
		rotBuild = pCurrentRotObj->GetVec3Rotation();

		// �r���̑傫����ݒ�
		sizeBuild = pCurrentRotObj->GetVec3Sizing();
		sizeBuild.y *= 2.0f;	// �c�̑傫����{�ɂ���

		float fRotLeft = m_destRot.y - HALF_PI;		// �v���C���[��90�x������
		useful::NormalizeRot(fRotLeft);				// ���������K��
		float fRotRight = m_destRot.y + HALF_PI;	// �v���C���[��90�x�E����
		useful::NormalizeRot(fRotRight);			// �E�������K��

		if (FourDirection(m_destRot.y, QRTR_PI * 0.5f) == FourDirection(rotBuild.y, QRTR_PI * 0.5f))
		{ // �������Ⴄ�ꍇ

			// �֐��𔲂���
			return;
		}

		// �r���̍��ʒu���v�Z
		posLeft.x = posBuild.x - sinf(fRotLeft) * sizeBuild.x;
		posLeft.y = posBuild.y + sizeBuild.y;
		posLeft.z = posBuild.z - cosf(fRotLeft) * sizeBuild.z;

		// �r���̉E�ʒu���v�Z
		posRight.x = posBuild.x - sinf(fRotRight) * sizeBuild.x;
		posRight.y = posBuild.y + sizeBuild.y;
		posRight.z = posBuild.z - cosf(fRotRight) * sizeBuild.z;

		if (collision::LineOuterProduct(posLeft, posRight, rPos) <= 0.0f)
		{ // �v���C���[�̐i�s�������猩�ăr���̒��S��艜���̏ꍇ

			// �v���C���[�̈ʒu�Ƀr���̒��S�ʒu��ݒ�
			rPos.x = posBuild.x;
			rPos.z = posBuild.z;

			// �v���C���[�̈ړ��ʂ�������
			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// �v���C���[�̖ڕW�����Ƀr���̌�����ݒ�
			m_destRot.y = rotBuild.y;
		}
	}
}

//============================================================
//	�r���Ƃ̈ꎲ���Ƃ̓����蔻��
//============================================================
bool CPlayer::ResponseSingleBuilding(const EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(basic::RADIUS, 0.0f, basic::RADIUS);			// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(basic::RADIUS, basic::HEIGHT, basic::RADIUS);	// �v���C���[�ő�傫��
	bool bMin = false;	// �s�̕����̔����
	bool bMax = false;	// ���̕����̔����
	bool bHit = false;	// �e���̔�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 posBuild = VEC3_ZERO;		// �r���ʒu
				D3DXVECTOR3 rotBuild = VEC3_ZERO;		// �r������
				D3DXVECTOR3 sizeMinBuild = VEC3_ZERO;	// �r���ŏ��傫��
				D3DXVECTOR3 sizeMaxBuild = VEC3_ZERO;	// �r���ő�傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BUILDING)
				{ // �I�u�W�F�N�g���x�����r���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �r���̈ʒu��ݒ�
				posBuild = pObjCheck->GetVec3Position();

				// �r���̌�����ݒ�
				rotBuild = pObjCheck->GetVec3Rotation();

				// �r���̍ŏ��̑傫����ݒ�
				sizeMinBuild = pObjCheck->GetVec3Sizing();
				sizeMinBuild.y = 0.0f;	// �c�̑傫����������

				// �r���̍ő�̑傫����ݒ�
				sizeMaxBuild = pObjCheck->GetVec3Sizing();
				sizeMaxBuild.y *= 2.0f;	// �c�̑傫����{�ɂ���

				switch (axis)
				{ // ���莲���Ƃ̏���
				case AXIS_X:	// X��

					// X���̏Փ˔���
					collision::ResponseSingleX
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posBuild,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinBuild,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						&bMin,			// ������̔���
						&bMax			// �E����̔���
					);

					if (bMin || bMax)
					{ // �����瓖�����Ă����ꍇ

						// �Ԃ���󋵂ɂ���
						bHit = true;
					}

					break;

				case AXIS_Y:	// Y��

					// Y���̏Փ˔���
					collision::ResponseSingleY
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posBuild,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinBuild,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						&bMin,			// ������̔���
						&bMax			// �ォ��̔���
					);

					if (bMax)
					{ // �ォ�瓖�����Ă����ꍇ

						// ���n���Ă���󋵂ɂ���
						bHit = true;
					}

					break;

				case AXIS_Z:	// Z��

					// Z���̏Փ˔���
					collision::ResponseSingleZ
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posBuild,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinBuild,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						&bMin,			// �O����̔���
						&bMax			// �ォ��̔���
					);

					if (bMin || bMax)
					{ // �����瓖�����Ă����ꍇ

						// �Ԃ���󋵂ɂ���
						bHit = true;
					}

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �e���̔������Ԃ�
	return bHit;
}

//============================================================
//	�r���Ƃ̓����蔻��
//============================================================
bool CPlayer::CollisionBuilding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �d�͂����Z
	m_move.y -= basic::GRAVITY;

	// �ړ��ʂ����Z
	rPos.x += m_move.x;

	// X���̓����蔻��
	if (ResponseSingleBuilding(AXIS_X, rPos))
	{ // ���ɓ������Ă����ꍇ

		// �q�b�g
		Hit();
	}

	// �ړ��ʂ����Z
	rPos.y += m_move.y;

	// Y���̓����蔻��
	if (ResponseSingleBuilding(AXIS_Y, rPos))
	{ // ��ɓ������Ă����ꍇ

		// ���n���Ă���󋵂ɂ���
		bLand = true;
	}

	// �ړ��ʂ����Z
	rPos.z += m_move.z;

	// Z���̓����蔻��
	if (ResponseSingleBuilding(AXIS_Z, rPos))
	{ // ���ɓ������Ă����ꍇ

		// �q�b�g
		Hit();
	}

	if (m_state == STATE_DAMAGE)
	{ // �_���[�W��Ԃ̏ꍇ

		// �ړ��ʂ�����
		if (m_bJump)
		{ // �󒆂̏ꍇ

			m_move.x += (0.0f - m_move.x) * basic::DMG_JUMP_REV;
			m_move.z += (0.0f - m_move.z) * basic::DMG_JUMP_REV;
		}
		else
		{ // �n��̏ꍇ

			m_move.x += (0.0f - m_move.x) * basic::DMG_LAND_REV;
			m_move.z += (0.0f - m_move.z) * basic::DMG_LAND_REV;
		}
	}
	else
	{ // �_���[�W��Ԃł͂Ȃ��ꍇ

		// �ړ��ʂ�����
		if (m_bJump)
		{ // �󒆂̏ꍇ

			m_move.x += (0.0f - m_move.x) * basic::NOR_JUMP_REV;
			m_move.z += (0.0f - m_move.z) * basic::NOR_JUMP_REV;
		}
		else
		{ // �n��̏ꍇ

			m_move.x += (0.0f - m_move.x) * basic::NOR_LAND_REV;
			m_move.z += (0.0f - m_move.z) * basic::NOR_LAND_REV;
		}
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	��Q���Ƃ̓����蔻��
//============================================================
bool CPlayer::CollisionObstacle(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(basic::RADIUS, 0.0f, basic::RADIUS);			// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(basic::RADIUS, basic::HEIGHT, basic::RADIUS);	// �v���C���[�ő�傫��

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 pos0, pos1, pos2, pos3;	// ���_���W����p
				D3DXVECTOR3 posObs  = VEC3_ZERO;	// ��Q���ʒu
				D3DXVECTOR3 rotObs  = VEC3_ZERO;	// ��Q������
				D3DXVECTOR3 sizeObs = VEC3_ZERO;	// ��Q���傫��
				float fAngle  = 0.0f;	// �Ίp���̊p�x
				float fLength = 0.0f;	// �Ίp���̒���
				bool  bHit = false;		// ������

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ��Q���̏����擾
				posObs  = pObjCheck->GetVec3Position();	// �ʒu
				rotObs  = pObjCheck->GetVec3Rotation();	// ����
				sizeObs = pObjCheck->GetVec3Sizing();	// �傫��
				fAngle  = pObjCheck->GetAngle();		// �Ίp���p�x
				fLength = pObjCheck->GetLength();		// �Ίp������

				// ���_���W���v�Z
				pos0.x = posObs.x + sinf(rotObs.y + (D3DX_PI + fAngle)) * fLength;
				pos0.y = 0.0f;
				pos0.z = posObs.z + cosf(rotObs.y + (D3DX_PI + fAngle)) * fLength;
				pos1.x = posObs.x + sinf(rotObs.y + (D3DX_PI - fAngle)) * fLength;
				pos1.y = 0.0f;
				pos1.z = posObs.z + cosf(rotObs.y + (D3DX_PI - fAngle)) * fLength;
				pos3.x = posObs.x + sinf(rotObs.y - fAngle) * fLength;
				pos3.y = 0.0f;
				pos3.z = posObs.z + cosf(rotObs.y - fAngle) * fLength;
				pos2.x = posObs.x + sinf(rotObs.y + fAngle) * fLength;
				pos2.y = 0.0f;
				pos2.z = posObs.z + cosf(rotObs.y + fAngle) * fLength;

				switch (pObjCheck->GetState())
				{ // ���@���Ƃ̏���
				case CObstacle::DODGE_JUMP:		// �W�����v���

					if (!m_bJump)
					{ // �W�����v���ł͂Ȃ��ꍇ

						if (rPos.y + basic::HEIGHT >= posObs.y
						&&  rPos.y <= posObs.y + sizeObs.y)
						{ // Y���W���͈͓��̏ꍇ

							// �񎲂̋�`�̏Փ˔���
							bHit = collision::BoxOuterPillar
							( // ����
								pos0,	// �l�p�̊e���_
								pos1,	// �l�p�̊e���_
								pos2,	// �l�p�̊e���_
								pos3,	// �l�p�̊e���_
								rPos	// ����ʒu
							);
						}
					}

					break;

				case CObstacle::DODGE_SLIDE:	// �X���C�f�B���O���

					if (!m_bSlide)
					{ // �X���C�f�B���O���ł͂Ȃ��ꍇ

						if (rPos.y + basic::HEIGHT >= posObs.y
						&&  rPos.y <= posObs.y + sizeObs.y)
						{ // Y���W���͈͓��̏ꍇ

							// �񎲂̋�`�̏Փ˔���
							bHit = collision::BoxOuterPillar
							( // ����
								pos0,	// �l�p�̊e���_
								pos1,	// �l�p�̊e���_
								pos2,	// �l�p�̊e���_
								pos3,	// �l�p�̊e���_
								rPos	// ����ʒu
							);
						}
					}

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				if (bHit)
				{ // �������Ă����ꍇ

					switch (pObjCheck->GetType())
					{ // ��ނ��Ƃ̏���
					case CObstacle::TYPE_JUMPPAD:	// �W�����v�p�b�h

						// �ړ��ʂ�ǉ�
						m_move.y += basic::JUMPPAD_MOVE;

						// �W�����v���[�V������ݒ�
						SetMotion(MOTION_JUMP);

						break;

					default:	// ��L�ȊO

						// �������Ă������Ԃ�
						return true;
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �������Ă��Ȃ�����Ԃ�
	return false;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::SMotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// �p�[�c���̐ݒ�
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�v���C���[�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
