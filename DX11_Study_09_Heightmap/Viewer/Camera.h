#pragma once
class Camera
{
public:
	Camera();
	virtual ~Camera();		// used by inheritance

	// Abstract class
	virtual void Update() = 0;
	virtual void	SetRotationSpeed(float speed) = 0;
	virtual float	GetRotationSpeed() = 0;
	virtual void	SetMoveSpeed(float speed) = 0;
	virtual float	GetMoveSpeed() = 0;


protected :
	virtual void Move();
	virtual void Rotation();
	virtual void View();		// refreshed View-matrix

private:
	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;

	// for camera direction
	D3DXVECTOR3	forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;

	D3DXMATRIX	matRotation;
	D3DXMATRIX	matView;

public :
	void Position(OUT D3DXVECTOR3* vec)
	{
		*vec = position;
	}

	void Position(IN float x, IN float y, IN float z)
	{
		position = D3DXVECTOR3(x, y, z);
		View();
	}

	void Rotation(OUT D3DXVECTOR2* vec)
	{
		*vec = D3DXVECTOR2(rotation.x, rotation.y);
	}

	void RotationDegree(OUT D3DXVECTOR2* vec)
	{
		*vec = rotation * 180.0f / (float)D3DX_PI;
	}

	void Rotation(IN float x, IN float y)
	{
		rotation = D3DXVECTOR2(x, y);
		Rotation();
	}

	void RotationDegree(IN float x, IN float y)
	{
		rotation = D3DXVECTOR2(x, y) * 0.01745329f;
	}

	void Matrix(OUT D3DXMATRIX* view)
	{
		*view = matView;
	}

	void Forward(OUT D3DXVECTOR3* vec)
	{
		*vec = forward;
	}

	void Right(OUT D3DXVECTOR3* vec)
	{
		*vec = right;
	}
	
	void Up(OUT D3DXVECTOR3* vec)
	{
		*vec = up;
	}

};

