//============================================================
//
//	�����_���[�w�b�_�[ [renderer.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����_���[�N���X
class CRenderer
{
public:
	// �R���X�g���N�^
	CRenderer();

	// �f�X�g���N�^
	~CRenderer();

	// �����o�֐�
	HRESULT Init(HWND hWnd, BOOL bWindow);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	LPDIRECT3DDEVICE9 GetDevice(void) const;	// �f�o�C�X�擾

	// �ÓI�����o�֐�
	static CRenderer *Create(HWND hWnd, BOOL bWindow);	// ����
	static HRESULT Release(CRenderer *&prRenderer);		// �j��

private:
	// �����o�֐�
	HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// �f�o�C�X����

	// �����o�ϐ�
	LPDIRECT3D9			m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Direct3D�f�o�C�X
};

#endif	// _RENDERER_H_
