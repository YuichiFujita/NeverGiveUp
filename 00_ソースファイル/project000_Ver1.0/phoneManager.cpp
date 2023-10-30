//============================================================
//
//	�X�}�z�}�l�[�W���[���� [phoneManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "phoneManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PHONE_PRIO	(6)	// �X�}�z�̗D�揇��

#define POS_PHONE	(D3DXVECTOR3(SCREEN_CENT.x, 1200.0f, 0.0f))	// �X�}�z�̏������W
#define SIZE_PHONE	(D3DXVECTOR3(394.0f, 653.0f, 0.0f))			// �X�}�z�̑傫��

#define COL_PHONE	(XCOL_WHITE)	// �X�}�z�̐F
#define COL_FADE	(XCOL_WHITE)	// �t�F�[�h�̐F
#define PHONE_MOVE	(0.5f)			// �X�}�z�̈ʒu�̍X�V��
#define PHONE_STOP	(SCREEN_CENT.y)	// �X�}�z�̒�~Y�ʒu

#define FADE_CHAN	(0.02f)	// �t�F�[�h�̃��l�ύX��
#define FADE_STOP	(0.6f)	// �t�F�[�h�̍ő僿�l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPhoneManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\phone000.png",	// �X�}�z�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\phone001.png",	// ����\���̃e�N�X�`�����΃p�X
};

//************************************************************
//	�e�N���X [CPhoneManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPhoneManager::CPhoneManager()
{
	// �����o�ϐ����N���A
	m_pPhone	= NULL;			// �X�}�z�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_state		= STATE_NONE;	// ���
	m_fMove		= 0.0f;			// �X�}�z�̈ړ���
	m_bDisp		= false;		// �\����
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPhoneManager::~CPhoneManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPhoneManager::Init(void)
{
	// �����o�ϐ���������
	m_pPhone	= NULL;			// �X�}�z�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_state		= STATE_NONE;	// ���
	m_fMove		= 0.0f;			// �X�}�z�̈ړ���
	m_bDisp		= false;		// �\����
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		COL_FADE		// �F
	);
	if (m_pFade == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PHONE_PRIO);

	// �����`���OFF�ɐݒ�
	m_pFade->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�X�}�z�̐����E�ݒ�
	//--------------------------------------------------------
	// �X�}�z�̐���
	m_pPhone = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SIZE_PHONE,		// �傫��
		VEC3_ZERO,		// ����
		COL_PHONE		// �F
	);
	if (m_pPhone == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pPhone->BindTexture(mc_apTextureFile[TEXTURE_PHONE]);

	// �D�揇�ʂ�ݒ�
	m_pPhone->SetPriority(PHONE_PRIO);

	// �����`���OFF�ɐݒ�
	m_pPhone->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPhoneManager::Uninit(void)
{
	// �X�}�z�̏I��
	m_pPhone->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPhoneManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:			// �������Ȃ����

		// ����

		// �����𔲂���
		break;

	case STATE_FADEIN:			// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFade();

		// �����𔲂���
		break;

	case STATE_PHONE_TAKE:		// �X�}�z�̎��o�����

		// �X�}�z��o�̍X�V
		UpdatePhoneTake();

		// �����𔲂���
		break;

	case STATE_WAIT:			// �ҋ@���

		// �ҋ@�̍X�V
		UpdateWait();

		// �����𔲂���
		break;

	case STATE_PHONE_RETURN:	// �X�}�z�̂��܂����

		// �X�}�z���[�̍X�V
		UpdatePhoneReturn();

		// �����𔲂���
		break;

	case STATE_END:				// �I�����

		// ����

		// �����𔲂���
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �X�}�z�̍X�V
	m_pPhone->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

//============================================================
//	�X�}�z�\���̊J�n����
//============================================================
void CPhoneManager::SetLook(void)
{
	// �X�}�z�̈ʒu��������
	m_pPhone->SetVec3Position(POS_PHONE);

	// �|���S���̐F����������
	m_pPhone->SetColor(COL_PHONE);	// �X�}�z
	m_pFade->SetColor(COL_FADE);	// �t�F�[�h

	// ����������
	m_fMove = 0.0f;			// �X�}�z�̈ړ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �\����ON�ɂ���
	SetEnableDisp(true);

	// ��Ԃ�ݒ�
	m_state = STATE_FADEIN;	// �t�F�[�h�C�����
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPhoneManager::SetState(const STATE state)
{
	// �����̏�Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	��Ԏ擾����
//============================================================
CPhoneManager::STATE CPhoneManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CPhoneManager::SetEnableDisp(const bool bDisp)
{
	// �����̕\���󋵂�ݒ�
	m_bDisp = bDisp;

	// �`��󋵂�\���󋵂ƈ�v������
	m_pFade->SetEnableDraw(bDisp);
	m_pPhone->SetEnableDraw(bDisp);
}

//============================================================
//	�\���擾����
//============================================================
bool CPhoneManager::IsDisp(void) const
{
	// �\���󋵂�Ԃ�
	return m_bDisp;
}

//============================================================
//	��������
//============================================================
CPhoneManager *CPhoneManager::Create(void)
{
	// �|�C���^��錾
	CPhoneManager *pPhoneManager = NULL;	// �X�}�z�}�l�[�W���[�����p

	if (pPhoneManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPhoneManager = new CPhoneManager;	// �X�}�z�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pPhoneManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �X�}�z�}�l�[�W���[�̏�����
		if (FAILED(pPhoneManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPhoneManager;
			pPhoneManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPhoneManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CPhoneManager::Release(CPhoneManager *&prPhoneManager)
{
	if (prPhoneManager != NULL)
	{ // �g�p���̏ꍇ

		// �X�}�z�}�l�[�W���[�̏I��
		prPhoneManager->Uninit();

		// �������J��
		delete prPhoneManager;
		prPhoneManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CPhoneManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	// �t�F�[�h�̃��l�����Z
	colFade.a += FADE_CHAN;
	
	if (colFade.a >= FADE_STOP)
	{ // �t�F�[�h�̃��l�����l�ȏ�̏ꍇ

		// �t�F�[�h�̃��l��␳
		colFade.a = FADE_STOP;

		// ��Ⳃ̎��o����Ԃɂ���
		m_state = STATE_PHONE_TAKE;

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAPER);	// ��Ⳃ߂��艹
	}

	// �t�F�[�h�̐F�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�X�}�z��o�̍X�V����
//============================================================
void CPhoneManager::UpdatePhoneTake(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPhone = m_pPhone->GetVec3Position();	// �X�}�z�̈ʒu

	// �X�}�z�̈ʒu���Z�ʂ����Z
	m_fMove += PHONE_MOVE;

	// �X�}�z�̈ʒu�����Z
	posPhone.y -= m_fMove;

	if (posPhone.y <= PHONE_STOP)
	{ // �X�}�z�̈ʒu�����l�ȉ��̏ꍇ

		// �X�}�z�̈ʒu��␳
		posPhone.y = PHONE_STOP;

		// �X�}�z�̈ړ��ʂ�������
		m_fMove = 0.0f;

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}

	// �X�}�z�̈ʒu�𔽉f
	m_pPhone->SetVec3Position(posPhone);
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CPhoneManager::UpdateWait(void)
{
	//m_state = STATE_PHONE_RETURN;
}

//============================================================
//	�X�}�z���[�̍X�V����
//============================================================
void CPhoneManager::UpdatePhoneReturn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPhone = m_pPhone->GetVec3Position();	// ��Ⳃ̈ʒu
	D3DXCOLOR colFade = m_pFade->GetColor();			// �t�F�[�h�̐F

	//--------------------------------------------------------
	//	�t�F�[�h�̓�����
	//--------------------------------------------------------
	if (colFade.a > 0.0f)
	{ // �t�F�[�h�̃��l�����l���傫���ꍇ

		// �t�F�[�h�̃��l�����Z
		colFade.a -= FADE_CHAN;

		if (colFade.a <= 0.0f)
		{ // �t�F�[�h�̃��l�����l�ȉ��̏ꍇ

			// �t�F�[�h�̃��l��␳
			colFade.a = 0.0f;
		}

		// �t�F�[�h�̐F�𔽉f
		m_pFade->SetColor(colFade);
	}

	//--------------------------------------------------------
	//	�X�}�z�̈ړ�
	//--------------------------------------------------------
	if (posPhone.y < POS_PHONE.y)
	{ // �X�}�z�̈ʒu�����l��菬�����ꍇ

		// �X�}�z�̈ړ��ʂ����Z
		m_fMove += PHONE_MOVE;

		// �X�}�z�̈ʒu�����Z
		posPhone.y += m_fMove;
	}
	else if (posPhone.y >= POS_PHONE.y)
	{ // �X�}�z�̈ʒu�����l�ȏ�̏ꍇ

		// �X�}�z�̈ʒu��␳
		posPhone.y = POS_PHONE.y;

		// �X�}�z�̈ړ��ʂ�������
		m_fMove = 0.0f;

		if (colFade.a <= 0.0f)
		{ // ���l��������؂��Ă���ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;

			// �\����OFF�ɂ���
			SetEnableDisp(false);
		}
	}

	// �X�}�z�̈ʒu�𔽉f
	m_pPhone->SetVec3Position(posPhone);
}
