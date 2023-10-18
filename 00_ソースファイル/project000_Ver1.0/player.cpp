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

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// �v���C���[�Z�b�g�A�b�v

	// �v���C���[��{���
	namespace basic
	{
		const float MOVE		= 2.8f;		// �ړ���
		const float	JUMP		= 20.0f;	// �W�����v�㏸��
		const float	GRAVITY		= 1.0f;		// �d��
		const float	RADIUS		= 20.0f;	// ���a
		const float HEIGHT		= 100.0f;	// �c��
		const float	JUMP_REV	= 0.22f;	// �󒆂̈ړ��ʂ̌����W��
		const float	LAND_REV	= 0.2f;		// �n��̈ړ��ʂ̌����W��
		const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
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
	m_state			= STATE_NORMAL;	// ���
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

	// �ҋ@���[�V������ݒ�
	SetMotion(MOTION_IDOL);

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

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal();

		break;

	case STATE_DEATH:
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
void CPlayer::Hit(const int /*nDmg*/)
{

#if 0	// TODO�FHit����

	// �ϐ���錾
	D3DXVECTOR3 pos = GetVec3Position();	// �v���C���[�ʒu

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL)
		{ // �ʏ��Ԃ̏ꍇ

			// �̗͂���_���[�W�����Z
			m_pLife->AddNum(-nDmg);

			if (m_pLife->GetNum() > 0)
			{ // �����Ă���ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DAMAGE;	// �_���[�W���
			}
			else
			{ // ����ł���ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DEATH;	// ���S���

				// ���[�V�����̐ݒ�
				SetMotion(MOTION_DEATH);	// ���S���[�V����

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_BREAK);	// �j��
			}
		}
	}

#endif

}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	// �����̏�Ԃ�ݒ�
	m_state = (EState)nState;
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
CPlayer *CPlayer::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = rPos;		// ���W�ݒ�p

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

		// �ʒu��ݒ�
		CScene::GetStage()->LimitPosition(pos, basic::RADIUS);	// �X�e�[�W�͈͓��␳
		pPlayer->SetVec3Position(pos);

		// ������ݒ�
		pPlayer->SetVec3Rotation(rRot);
	}

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
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

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, basic::RADIUS);

	// ��Q���Ƃ̓����蔻��
	if (CollisionObstacle(posPlayer))
	{ // ���������ꍇ

		// TODO�F���S��Ԃɂ���

		CParticle3D::Create(CParticle3D::TYPE_HEAL, posPlayer);
	}

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("�v���C���[�ړ��ʁF%f\n", m_fMove);
	CManager::GetInstance()->GetDebugProc()->Print((m_bJump) ? "�W�����v�FON\n" : "�W�����v�FOFF\n");
	CManager::GetInstance()->GetDebugProc()->Print((m_bSlide) ? "�X���C�f�B���O�FON\n" : "�X���C�f�B���O�FOFF\n");

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
	// �ϐ���錾
	D3DXVECTOR3 rotCamera = CManager::GetInstance()->GetCamera()->GetVec3Rotation();	// �J�����̌���

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

#if 0
	if (pKeyboard->IsPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->IsPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(135) + rotCamera.y;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(225) + rotCamera.y;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rotCamera.y) * 2.0f;
			m_move.z += cosf(rotCamera.y) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(180) + rotCamera.y;
		}
	}
	else if (pKeyboard->IsPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->IsPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(45) + rotCamera.y;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(315) + rotCamera.y;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rotCamera.y) * 2.0f;
			m_move.z -= cosf(rotCamera.y) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(0) + rotCamera.y;
		}
	}
	else if (pKeyboard->IsPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rotCamera.y;
	}
	else if (pKeyboard->IsPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + rotCamera.y;
	}
#else
	// �ړ��ʂ��X�V
	m_move.x += m_fMove;

	// �ڕW�������X�V
	m_destRot.y = atan2f(-m_move.x, -m_move.z);

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

			CEffect3D::Create(GetVec3Position(), 80.0f, CEffect3D::TYPE_NORMAL, 20);
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

	if (!m_bJump && !m_bSlide)
	{ // �W�����v�ƃX���C�f�B���O�����Ă��Ȃ��ꍇ

		if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_A))
		{ // �X���C�f�B���O�̑��삪�s��ꂽ�ꍇ

			// �X���C�f�B���O���삪�s��ꂽ����ۑ�
			m_bSlideControl = true;
		}

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

	if (m_bSlide)
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
	else if (CScene::GetField()->LandPosition(rPos, m_move)
	||       CScene::GetStage()->LandPosition(rPos, m_move, 0.0f))
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

		// TODO�F���S��Ԃɂ���

		CEffect3D::Create(rPos, 40.0f, CEffect3D::TYPE_NORMAL, 10);
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

		// TODO�F���S��Ԃɂ���

		CEffect3D::Create(rPos, 40.0f, CEffect3D::TYPE_NORMAL, 10);
	}

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * basic::JUMP_REV;
		m_move.z += (0.0f - m_move.z) * basic::JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * basic::LAND_REV;
		m_move.z += (0.0f - m_move.z) * basic::LAND_REV;
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
				D3DXVECTOR3 posBuild = VEC3_ZERO;		// �r���ʒu
				D3DXVECTOR3 sizeMinBuild = VEC3_ZERO;	// �r���ŏ��傫��
				D3DXVECTOR3 sizeMaxBuild = VEC3_ZERO;	// �r���ő�傫��
				bool bHit = false;	// ������

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ��Q���̈ʒu��ݒ�
				posBuild = pObjCheck->GetVec3Position();

				// ��Q���̍ŏ��̑傫����ݒ�
				sizeMinBuild = pObjCheck->GetVec3Sizing();
				sizeMinBuild.y = 0.0f;	// �c�̑傫����������

				// ��Q���̍ő�̑傫����ݒ�
				sizeMaxBuild = pObjCheck->GetVec3Sizing();
				sizeMaxBuild.y *= 2.0f;	// �c�̑傫����{�ɂ���

				switch (pObjCheck->GetType())
				{ // ���@���Ƃ̏���
				case CObstacle::DODGE_JUMP:		// �W�����v���

					if (!m_bJump)
					{ // �W�����v���ł͂Ȃ��ꍇ

						// �񎲂̋�`�̏Փ˔���
						bHit = collision::Box2D
						( // ����
							rPos,			// ����ʒu
							posBuild,		// ����ڕW�ʒu
							sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
							sizeMinPlayer,	// ����T�C�Y(���E���E�O)
							sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
							sizeMinBuild	// ����ڕW�T�C�Y(���E���E�O)
						);
					}

					break;

				case CObstacle::DODGE_SLIDE:	// �X���C�f�B���O���

					if (!m_bSlide)
					{ // �X���C�f�B���O���ł͂Ȃ��ꍇ

						// �񎲂̋�`�̏Փ˔���
						bHit = collision::Box2D
						( // ����
							rPos,			// ����ʒu
							posBuild,		// ����ڕW�ʒu
							sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
							sizeMinPlayer,	// ����T�C�Y(���E���E�O)
							sizeMaxBuild,	// ����ڕW�T�C�Y(�E�E��E��)
							sizeMinBuild	// ����ڕW�T�C�Y(���E���E�O)
						);
					}

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				if (bHit)
				{ // �������Ă����ꍇ

					// �������Ă������Ԃ�
					return true;
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
