//============================================================
//
//	�X�R�A�w�b�_�[ [score.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_SCORE	(8)	// �X�R�A�̌���

//************************************************************
//	�O���錾
//************************************************************
class CValue;	// �����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�R�A�N���X
class CScore : public CObject
{
public:
	// �R���X�g���N�^
	CScore();

	// �f�X�g���N�^
	~CScore();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetPriority(const int nPriority) override;			// �D�揇�ʐݒ�
	void SetEnableUpdate(const bool bUpdate) override;		// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// �傫���擾

	// �ÓI�����o�֐�
	static CScore *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR3& rSpace	// ��
	);

	// �����o�֐�
	void Add(const int nNum);	// ���Z
	void Set(const int nNum);	// �ݒ�
	int  Get(void);				// �擾
	void SetSpace(const D3DXVECTOR3& rSpace);	// �󔒐ݒ�
	D3DXVECTOR3 GetSpace(void) const;			// �󔒎擾

private:
	// �����o�֐�
	void SetDrawValue(void);	// �����̕\���ݒ�
	void SetTexNum(void);		// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CValue *m_apValue[MAX_SCORE];	// ���l�̏��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_size;		// �傫��
	D3DXVECTOR3 m_space;	// ��
	int m_nNum;				// �X�R�A
};

#endif	// _SCORE_H_
