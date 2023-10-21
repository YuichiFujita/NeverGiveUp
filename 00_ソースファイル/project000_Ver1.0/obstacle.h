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
		DODGE_NONE,		// �����@����
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

	// ���f�����\����
	struct SModelInfo
	{
		const char *pTextureFile;	// ���f���t�@�C����
		SStatusInfo status;			// �X�e�[�^�X���
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
	static CObstacle *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�

private:
	// �ÓI�����o�ϐ�
	static SModelInfo m_aStatusInfo[];	// ���f�����

	// �����o�ϐ�
	const SStatusInfo m_status;	// �X�e�[�^�X�萔
	const EType m_type;	// ��ޒ萔
	float m_fAngle;		// �Ίp���̊p�x
	float m_fLength;	// �Ίp���̒���
};

#endif	// _OBSTACLE_H_
