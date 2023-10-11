//============================================================
//
//	�}���`���f���w�b�_�[ [multiModel.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MULTIMODEL_H_
#define _MULTIMODEL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	�N���X��`
//************************************************************
// �}���`���f���N���X
class CMultiModel
{
public:
	// �R���X�g���N�^
	CMultiModel();

	// �f�X�g���N�^
	~CMultiModel();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	void BindModel(const int nModelID);		// ���f������ (�C���f�b�N�X)
	void BindModel(const char *pModelPass);	// ���f������ (�p�X)
	void SetParent(CMultiModel *pModel);	// �e���f���ݒ�
	CMultiModel GetParent(void) const;		// �e���f���擾

	void SetMtxWorld(const D3DXMATRIX& rMtxWorld);	// �}�g���b�N�X�ݒ�
	D3DXMATRIX *GetPtrMtxWorld(void);				// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const;				// �}�g���b�N�X�擾
	void SetVec3Position(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const;		// �����擾
	void SetVec3Scaling(const D3DXVECTOR3& rScale);	// �g�嗦�ݒ�
	D3DXVECTOR3 GetVec3Scaling(void) const;			// �g�嗦�擾

	HRESULT SetOriginMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat);	// ���}�e���A���ݒ�
	void SetMaterial(const D3DXMATERIAL& rMat);			// �}�e���A���ݒ�
	void ResetMaterial(void);							// �}�e���A���Đݒ�
	D3DXMATERIAL GetMaterial(const int nID) const;		// �}�e���A���擾
	void SetAlpha(const float fAlpha);					// �����x�ݒ�
	float GetAlpha(void) const;							// �����x�擾
	float GetMaxAlpha(void) const;						// �ő哧���x�擾
	void SetModelData(const CModel::SModel& rModel);	// ���f�����ݒ�
	CModel::SModel GetModelData(void) const;			// ���f�����擾

	// �ÓI�����o�֐�
	static CMultiModel *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot,				// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);
	static HRESULT Release(CMultiModel *&prMultiModel);	// �j��

private:
	// �����o�ϐ�
	CModel::SModel	m_modelData;	// ���f�����
	CMultiModel		*m_pParent;		// �e���f���ւ̃|�C���^
	D3DXMATERIAL	*m_pMat;		// �}�e���A���ւ̃|�C���^
	D3DXMATRIX		m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;			// �ʒu
	D3DXVECTOR3		m_rot;			// ����
	D3DXVECTOR3		m_scale;		// �g�嗦
	int				m_nModelID;		// ���f���C���f�b�N�X
};

#endif	// _MULTIMODEL_H_
