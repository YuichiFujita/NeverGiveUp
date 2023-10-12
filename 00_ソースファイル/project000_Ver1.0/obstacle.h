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
		TYPE_NORMAL = 0,	// �ʏ탂�f��
		TYPE_MAX			// ���̗񋓌^�̑���
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
	CObstacle();

	// �f�X�g���N�^
	~CObstacle();

	// ���f�����\����
	struct SModelInfo
	{
		const char *pTextureFile;	// ���f���t�@�C����
		EDodge dodge;	// ���@
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;			// ������
	void Uninit(void) override;				// �I��
	void Update(void) override;				// �X�V
	void Draw(void) override;				// �`��
	void SetType(const int nType) override;	// ��ސݒ�

	// �ÓI�����o�֐�
	static CObstacle *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

private:
	// �����o�ϐ�
	EDodge m_dodge;	// ���@

	// �ÓI�����o�ϐ�
	static SModelInfo m_aStatusInfo[];	// ���f�����
};

#endif	// _OBSTACLE_H_
