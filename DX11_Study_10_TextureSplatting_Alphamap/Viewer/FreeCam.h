#pragma once
#include "Camera.h"


class FreeCam :
	public Camera
{
public :
	FreeCam(float moveSpeed = 20.0f, float rotationSpeed = 2.5f);
	~FreeCam();
	void Update();
	D3DXVECTOR3 GetPosition();
	void	SetRotationSpeed(float speed);
	float	GetRotationSpeed();
	void	SetMoveSpeed(float speed);
	float	GetMoveSpeed();


private:
	float moveSpeed;
	float rotationSpeed;

};