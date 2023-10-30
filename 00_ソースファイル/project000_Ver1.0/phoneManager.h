//============================================================
//
//	�X�}�z�}�l�[�W���[�w�b�_�[ [phoneManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PHONE_MANAGER_H_
#define _PHONE_MANAGER_H_

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
// �X�}�z�}�l�[�W���[�N���X
class CPhoneManager
{
public:
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_PHONE = 0,	// �X�}�z�̃e�N�X�`��
		TEXTURE_CONTROL,	// ����\���̃e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_PHONE_TAKE,	// �X�}�z�̎��o�����
		STATE_WAIT,			// �ҋ@���
		STATE_PHONE_RETURN,	// �X�}�z�̂��܂����
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPhoneManager();

	// �f�X�g���N�^
	~CPhoneManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetLook(void);	// �X�}�z�\���J�n
	void SetState(const STATE state);			// ��Ԑݒ�
	STATE GetState(void) const;					// ��Ԏ擾
	void SetEnableDisp(const bool bDisp);		// �\���ݒ�
	bool IsDisp(void) const;					// �\���擾
	//void SetMessage(const EMassage massage);	// ���b�Z�[�W�ݒ�

	// �ÓI�����o�֐�
	static CPhoneManager *Create(void);	// ����
	static HRESULT Release(CPhoneManager *&prPhoneManager);	// �j��

private:
	// �����o�֐�
	void UpdateFade(void);			// �t�F�[�h�C��
	void UpdatePhoneTake(void);		// �X�}�z��o
	void UpdateWait(void);			// �ҋ@
	void UpdatePhoneReturn(void);	// �X�}�z���[

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pPhone;	// �X�}�z�̏��
	CObject2D *m_pFade;		// �t�F�[�h�̏��
	STATE m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	float m_fMove;			// �X�}�z�̈ړ���
	bool m_bDisp;			// �\����
};

#endif	// _PHONE_MANAGER_H_
