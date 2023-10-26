//============================================================
//
//	���U���g�}�l�[�W���[�w�b�_�[ [resultManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
#define NUM_RESULT	(2)	// ���U���g�\���ɕK�v�ȃ|���S����

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g�}�l�[�W���[�N���X
class CResultManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_MISSION = 0,	// MISSION�e�N�X�`��
		TEXTURE_CLEAR,			// CLEAR�e�N�X�`��
		TEXTURE_FAILED,			// FAILED�e�N�X�`��
		TEXTURE_SCORE,			// �X�R�A�\���e�N�X�`��
		TEXTURE_TIME,			// �^�C���\���e�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_RESULT,		// ���U���g�\�����
		STATE_TIME_WAIT,	// �^�C���\���ҋ@���
		STATE_TIME,			// �^�C���\�����
		STATE_WAIT,			// �J�ڑҋ@���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CResultManager();

	// �f�X�g���N�^
	~CResultManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CResultManager *Create(void);	// ����
	static HRESULT Release(CResultManager *&prResultManager);	// �j��

private:
	// �����o�֐�
	void UpdateFade(void);		// �t�F�[�h�C��
	void UpdateResult(void);	// ���U���g�\��
	void UpdateTime(void);		// �^�C���\��
	void UpdateBack(void);		// �J�ڌ���
	void SkipStaging(void);		// ���o�X�L�b�v
	void SetTexResult(void);	// ���U���g�\���̃e�N�X�`���ݒ�
	bool UpdateDrawWait(const int nWait);	// �\���ҋ@

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_apResult[NUM_RESULT];	// ���U���g�\���̏��
	CObject2D *m_pTimeLogo;		// �^�C�����S�̏��
	CObject2D *m_pFade;			// �t�F�[�h�̏��
	CTimerManager *m_pTime;		// �^�C���̏��
	EState m_state;				// ���
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	float m_fScale;				// �|���S���g�嗦
};

#endif	// _RESULTMANAGER_H_
