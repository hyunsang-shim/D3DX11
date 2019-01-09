#pragma once

class Program
{
public:
	Program();
	~Program();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	ExecuteValues* values;
	vector<class Execute *> executes;

	D3DXMATRIX view;

	// D3DXVECTOR3		CamPos;			// delete -> replaced by Camera / FreeCam classes
	 D3DXVECTOR3		CamPosDefault;	// delete -> replaced by Camera / FreeCam classes

	 float rotSpeed;
	 float moveSpeed;
};