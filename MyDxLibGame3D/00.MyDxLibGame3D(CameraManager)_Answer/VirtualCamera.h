#pragma once

/// <summary>
/// 仮想カメラ基底
/// </summary>
class VirtualCamera abstract
{
public:
	VirtualCamera(const VECTOR& pos, const VECTOR& target, float fovDegree, int priority, bool isActive);
	
	const VECTOR&	GetPos()		const { return pos; }
	const VECTOR&	GetTarget()		const { return target; }
	float			GetFovDegree()	const { return fovDegree; }
	int				GetPriority()	const { return priority; }
	
	bool IsActive() const { return isActive; }
	
	void SetActive(bool set) { isActive = set; }

	virtual void Update() abstract;

private:
	VECTOR	pos;
	VECTOR	target;
	float	fovDegree;
	int		priority;

	bool	isActive;
};

