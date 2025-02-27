// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// ��Q���N���X�̊��
/// </summary>
class ObstacleBase abstract // abstract��t���邱�ƂŁA���ڎg�p(new��)���֎~����
{
public:
    ObstacleBase(const char* path);     // �R���X�g���N�^
    ~ObstacleBase();                    // �f�X�g���N�^

    void Draw() const;					// �`��

    // �������z�֐�
    // void Update�͒��g���Ȃ��A�h����Œ��g����邱�Ƃ���������
    //virtual void Update() = 0;			// =0�͌Â��������z�֐��錾
    virtual void Update() abstract;			// �X�V

    // ���f���n���h���̎擾
    int GetModelHandle() const { return modelHandle; }

    // �|�W�V������getter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// ���f���n���h��
    VECTOR	pos;			// �|�W�V����
};

