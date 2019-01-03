#include "stdafx.h"
#include "FreeCam.h"

FreeCam::FreeCam(float moveSpeed, float rotationSpeed)
	: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{
}

FreeCam::~FreeCam()
{
}

void FreeCam::Update() 
{
	D3DXVECTOR3 forward, right, up;		// local use only;

	Forward(&forward);
	Right(&right);
	Up(&up);

	// for Camera Movement
	{
		D3DXVECTOR3 position;
		Position(&position);

		if (Mouse::Get()->Press(1))
		{
			if (Keyboard::Get()->Press('W'))
				position += forward * moveSpeed * Time::Delta();
			else if(Keyboard::Get()->Press('S'))
				position += -forward * moveSpeed * Time::Delta();
			
			if (Keyboard::Get()->Press('A'))
				position += -right * moveSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('D'))
				position += right * moveSpeed * Time::Delta();

			if (Keyboard::Get()->Press('E'))
				position += up * moveSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('Q'))
				position += -up * moveSpeed * Time::Delta();


			Position(position.x, position.y, position.z);
		}
	}

	// for Camera Rotation
	{
		D3DXVECTOR2 rotation;
		Rotation(&rotation);

		if (Mouse::Get()->Press(1))
		{
			D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();
			rotation.x += val.y * rotationSpeed * Time::Delta();
			rotation.y += val.x * rotationSpeed * Time::Delta();
		}

		Rotation(rotation.x, rotation.y);
	}
}

D3DXVECTOR3 FreeCam::GetPosition()
{
	D3DXVECTOR3 position;
	Position(&position);

	return position;
}

void FreeCam::SetRotationSpeed(float speed)
{
	rotationSpeed = speed;
}

float FreeCam::GetRotationSpeed()
{
	return rotationSpeed;
}

void FreeCam::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
}

float FreeCam::GetMoveSpeed()
{
	return moveSpeed;
}
