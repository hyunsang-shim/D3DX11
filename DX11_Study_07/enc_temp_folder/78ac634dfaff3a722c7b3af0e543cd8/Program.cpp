#include "stdafx.h"
#include "Program.h"
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
	
	CamPosDefault = CamPos = D3DXVECTOR3(2.75f, 0.75f, -1.53f);	

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

	if (Keyboard::Get()->Press('A'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.x -= 5.0f * Time::Delta();
		else
			CamPos.x -= 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('D'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.x += 5.0f * Time::Delta();
		else
			CamPos.x += 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('W'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.z += 5.0f * Time::Delta();
		else
			CamPos.z += 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('S'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.z -= 5.0f * Time::Delta();
		else
			CamPos.z -= 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('Q'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.y -= 5.0f * Time::Delta();
		else
			CamPos.y -= 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('E'))
		if (Keyboard::Get()->Press(VK_SHIFT))
			CamPos.y += 5.0f * Time::Delta();
		else
			CamPos.y += 10.0f * Time::Delta();

	if (Keyboard::Get()->Press('X'))
		CamPos = CamPosDefault;

	// D3DXVECTOR3 position(-3, 3, -15);
	D3DXVECTOR3 forward(0, 0, 1);
	D3DXVECTOR3 right(1, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);



	// D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);
	D3DXMatrixLookAtLH(&view, &CamPos, &(CamPos + forward), &up);
	
	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
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
	ImGui::Text("CamPos : %f, %f, %f", CamPos.x, CamPos.y, CamPos.z);

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