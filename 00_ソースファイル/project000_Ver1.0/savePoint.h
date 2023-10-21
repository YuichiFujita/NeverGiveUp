//============================================================
//
//	�Z�[�u�|�C���g�w�b�_�[ [savePoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SAVEPOINT_H_
#define _SAVEPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �Z�[�u�|�C���g�N���X
class CSavePoint : public CObjectModel
{
public:
	// �R���X�g���N�^
	CSavePoint();

	// �f�X�g���N�^
	~CSavePoint();

	// �Z�[�u�|�C���g���\����
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CSavePoint *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static SInfo GetSavePointInfo(void);	// �Z�[�u�|�C���g���擾

	// �����o�֐�
	void CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔
	static CSavePoint *m_pCurrentSave;		// ���݂̃Z�[�u�|�C���g�ւ̃|�C���^

	static int m_nNumAll;	// �Z�[�u�|�C���g�̑���

	// �����o�ϐ�
	const int m_nThisSaveID;	// ���g�̃Z�[�u�|�C���g�C���f�b�N�X
};

#endif	// _SAVEPOINT_H_
