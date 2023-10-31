//============================================================
//
//	�`���[�g���A���}�l�[�W���[�w�b�_�[ [tutorialManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A���}�l�[�W���[�N���X
class CTutorialManager
{
public:
	// ������
	enum EExplain
	{
		EXPLAIN_JUMP = 0,	// �W�����v�����e�N�X�`��
		EXPLAIN_SLIDE,		// �X���C�f�B���O�����e�N�X�`��
		EXPLAIN_WALLDASH,	// �Ǒ�������e�N�X�`��
		EXPLAIN_WALLJUMP,	// �ǃW�����v�����e�N�X�`��
		EXPLAIN_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTutorialManager();

	// �f�X�g���N�^
	~CTutorialManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CTutorialManager *Create(void);	// ����
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// �j��

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pExplain;	// �����\���̏��
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
};

#endif	// _TUTORIAL_MANAGER_H_
