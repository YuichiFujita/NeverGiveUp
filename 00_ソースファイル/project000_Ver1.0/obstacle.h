//============================================================
//
//	��Q���w�b�_�[ [obstacle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// ��Q���N���X
class CObstacle : public CObjectModel
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_BOX = 0,	// �����f��
		TYPE_BENCH,		// �x���`���f��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// ����
	enum EDodge
	{
		DODGE_NONE = 0,	// �����@����
		DODGE_JUMP,		// �W�����v���
		DODGE_SLIDE,	// �X���C�f�B���O���
		DODGE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObstacle(const EType type);

	// �f�X�g���N�^
	~CObstacle();

	// �X�e�[�^�X�\����
	struct SStatusInfo
	{
		D3DXVECTOR3 size;	// �傫��
		EDodge dodge;		// ���@
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	int GetType(void) const override;		// ��ގ擾
	float GetAngle(void) const override;	// �p�x�擾
	float GetLength(void) const override;	// �����擾
	D3DXVECTOR3 GetVec3Sizing(void) const override;	// �傫���擾

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CObstacle *Create		// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static void SetStatusInfo(const SStatusInfo info, const int nID);	// �X�e�[�^�X���ݒ�
	static SStatusInfo GetStatusInfo(const int nID);					// �X�e�[�^�X���擾

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];		// ���f���萔
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���

	// �����o�ϐ�
	const EType m_type;		// ��ޒ萔
	SStatusInfo m_status;	// �X�e�[�^�X
	float m_fAngle;			// �Ίp���̊p�x
	float m_fLength;		// �Ίp���̒���
};

#endif	// _OBSTACLE_H_
