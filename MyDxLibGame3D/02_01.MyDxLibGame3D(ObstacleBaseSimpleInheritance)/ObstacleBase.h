// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// ��Q���N���X�̊��
/// </summary>
class ObstacleBase
{
public:
    void Draw() const;					// �`��

    // ���f���n���h���̎擾
    int GetModelHandle() const { return modelHandle; }

    // �|�W�V������getter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// ���f���n���h��
    VECTOR	pos;			// �|�W�V����
};

