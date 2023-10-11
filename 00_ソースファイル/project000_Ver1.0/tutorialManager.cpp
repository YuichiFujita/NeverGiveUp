//============================================================
//
//	�`���[�g���A���}�l�[�W���[���� [tutorialManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TUTORIAL_PRIO	(14)	// �`���[�g���A���̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\tutorial000.png",	// �����e�N�X�`��
};

//************************************************************
//	�e�N���X [CTutorialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTutorialManager::CTutorialManager()
{
	// �����o�ϐ����N���A
	m_pExplain	= NULL;			// �����\���̏��
	m_state		= STATE_NONE;	// ���
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nCounterExplain = 0;		// �����Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pExplain	= NULL;					// �����\���̏��
	m_state		= STATE_NORMAL;			// ���
	m_nCounterState = 0;				// ��ԊǗ��J�E���^�[
	m_nCounterExplain = EXPLAIN_NORMAL;	// �����Ǘ��J�E���^�[

	// �I��w�i�̐���
	m_pExplain = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE		// �傫��
	);
	if (m_pExplain == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pExplain->SetPriority(TUTORIAL_PRIO);

	// �����e�N�X�`����o�^�E����
	m_pExplain->BindTexture(pTexture->Regist(mc_apTextureFile[m_nCounterExplain]));

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CTutorialManager::Uninit(void)
{
	// �����\���̏I��
	m_pExplain->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_NORMAL:	// �ʏ���

		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN))
		{
			// �V�[���̐ݒ�
			CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// �^�C�g�����
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �����\���̍X�V
	m_pExplain->Update();
}

//============================================================
//	��������
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// �|�C���^��錾
	CTutorialManager *pTutorialManager = NULL;	// �`���[�g���A���}�l�[�W���[�����p

	if (pTutorialManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTutorialManager = new CTutorialManager;	// �`���[�g���A���}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pTutorialManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �`���[�g���A���}�l�[�W���[�̏�����
		if (FAILED(pTutorialManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTutorialManager;
			pTutorialManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTutorialManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	if (prTutorialManager != NULL)
	{ // �g�p���̏ꍇ

		// �`���[�g���A���}�l�[�W���[�̏I��
		if (FAILED(prTutorialManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prTutorialManager;
			prTutorialManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prTutorialManager;
		prTutorialManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
