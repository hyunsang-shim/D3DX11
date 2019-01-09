#include "stdafx.h"
#include "Program.h"
#include "Viewer\FreeCam.h"
#include "Executes\ExeGrid.h"		// << added


Program::Program()
{
	States::Create();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);
	
	values->MainCamera = new FreeCam(1.0f, 2.5f);
	values->MainCamera->Position(2.5f, 1.0f, -3.50f);

	CamPosDefault = D3DXVECTOR3(2.75f, 0.75f, -1.53f);	

	executes.push_back(new ExeGrid(values)); // Enlist item
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values);

	States::Delete();
}

void Program::Update()
{
	values->MainCamera->SetMoveSpeed(moveSpeed);
	values->MainCamera->SetRotationSpeed(rotSpeed);
	values->MainCamera->Update();

	if (Keyboard::Get()->Press('X'))
		values->MainCamera->Position(CamPosDefault.x, CamPosDefault.y, CamPosDefault.z);

	// D3DXVECTOR3 position(-3, 3, -15);
	D3DXVECTOR3 forward(0, 0, 1);
	D3DXVECTOR3 right(1, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);



	// D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);
	//D3DXMatrixLookAtLH(&view, &CamPos, &(CamPos + forward), &up);
	
	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXMATRIX view;
	values->MainCamera->Matrix(&view);

	D3DXMATRIX projection;
	values->Perspective->GetMatrix(&projection);

	values->ViewProjection->SetView(view);
	values->ViewProjection->SetProjection(projection);
	values->ViewProjection->SetVSBuffer(0);

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	for (Execute* exe : executes)
		exe->PostRender();

	ImGui::Text("FPS : %f", Time::Get()->FPS());
	//ImGui::Text("CamPos : %f, %f, %f", );
	ImGui::Text("forward : W \t Backward : S");
	ImGui::Text("Go Left : A \t Go Right : D");
	ImGui::Text("Go Up : Q \t Go Down : S");
	ImGui::Text("Reset to Default Position : X");

	ImGui::Begin("Speed");
	{
		rotSpeed = values->MainCamera->GetRotationSpeed();
		moveSpeed = values->MainCamera->GetMoveSpeed();
		ImGui::SliderFloat("RotationSpeed", &rotSpeed, 0, 3.0f);
		ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0, 20.0f);
	}
	ImGui::End();


}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}