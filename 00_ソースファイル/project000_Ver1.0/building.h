//============================================================
//
//	�r���w�b�_�[ [building.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BUILDING_H_
#define _BUILDING_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �r���N���X
class CBuilding : public CObjectMeshCube
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_00 = 0,	// �r��00�e�N�X�`��
		TYPE_01,		// �r��01�e�N�X�`��
		TYPE_02,		// �r��02�e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBuilding();

	// �f�X�g���N�^
	~CBuilding();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;			// ������
	void Uninit(void) override;				// �I��
	void Update(void) override;				// �X�V
	void Draw(void) override;				// �`��
	void SetType(const int nType) override;	// ��ސݒ�

	// �ÓI�����o�֐�
	static CBuilding *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXCOLOR& rCol		// �F
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[][6];	// �e�N�X�`���萔
};

#endif	// _BUILDING_H_
