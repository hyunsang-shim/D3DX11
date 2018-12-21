#include "stdafx.h"
#include "ExeRect.h"

ExeRect::ExeRect(ExecuteValues* values)
	: Execute(values)
	, vertexCount(4), indexCount(6)
	, number(0)
{
	shader = new Shader(defShader + L"003_Color.hlsl");
	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer();

	vertices = new VertexColor[vertexCount];
	vertices[0].Position = D3DXVECTOR3(-1, -1, 0);
	vertices[1].Position = D3DXVECTOR3(-1, 0, 0);
	vertices[2].Position = D3DXVECTOR3(0, -1, 0);
	vertices[3].Position = D3DXVECTOR3(0, 0, 0);
	
	vertices[0].Color = D3DXCOLOR(0, 0, 0, 1);
	vertices[1].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[2].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[3].Color = D3DXCOLOR(0, 0, 1, 1);


	indices = new UINT[indexCount]{ 0, 1, 2, 1, 3, 2 };

	// Create VertexBuffer
	{
		D3D11_BUFFER_DESC  desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexColor) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(
			&desc, &data, &vertexBuffer
		);

		assert(SUCCEEDED(hr));

	}

	// Create IndexBuffer
	{
		D3D11_BUFFER_DESC  desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(
			&desc, &data, &indexBuffer
		);

		assert(SUCCEEDED(hr));
	}

}

ExeRect::~ExeRect()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeRect::Update()
{
	if (Keyboard::Get()->Up(VK_ADD))
	{
		number++;
		if (number > vertexCount)
			number = vertexCount-1;
	}
	else if (Keyboard::Get()->Up(VK_SUBTRACT))
	{
		number--;
		if (number < 0)
			number = 0;
	}


	if (Keyboard::Get()->Press('A'))
	{
		vertices[number].Position.x -= 0.5f * Time::Delta();
	}
	else if (Keyboard::Get()->Press('D'))
	{
		vertices[number].Position.x += 0.5f * Time::Delta();
	}

	D3D::GetDC()->UpdateSubresource(
		vertexBuffer, 
		0, 
		NULL, 
		vertices, 
		sizeof(VertexColor) * vertexCount, 
		0);
}

void ExeRect::PreRender()
{
}

void ExeRect::Render()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	// IA = Input Assembler
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorBuffer->SetPSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	shader->Render();


	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeRect::PostRender()
{
	ImGui::Begin("Color");
	{
		ImGui::SliderFloat3("Color", (float*)&colorBuffer->Data.Color,0,1);
		ImGui::SliderInt("VertexNumber", &number, 0, 3);

	}
	ImGui::End();

	
}

void ExeRect::ResizeScreen()
{
}
