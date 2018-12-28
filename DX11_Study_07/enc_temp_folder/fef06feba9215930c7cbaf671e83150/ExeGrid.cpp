#include "stdafx.h"
#include "ExeGrid.h"

ExeGrid::ExeGrid(ExecuteValues* values)
	: Execute(values)
	, vertexCount(4), indexCount(6)
	, width(5), height(5)
{
	shader = new Shader(ShaderPath + L"003_Color.hlsl");
	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer();

	// create vertices for grid
	{

		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexColor[vertexCount];
		/*
		6-7-8
		3-4-5
		0-1-2
		*/

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;
				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)0.0f;
				vertices[index].Position.z = (float)z;

				vertices[index].Color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
			}
		}
	}

	// create indices for grid
	{
		indexCount = width * height * 6;
		indices = new UINT[indexCount];
		UINT index = 0;
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{				
				indices[index + 0] = (width + 1) * (z + 0) + (x + 0);
				indices[index + 1] = (width + 1) * (z + 1) + (x + 0);
				indices[index + 2] = (width + 1) * (z + 0) + (x + 1);

				indices[index + 3] = (width + 1) * (z + 1) + (x + 1);
				indices[index + 4] = (width + 1) * (z + 0) + (x + 1);
				indices[index + 5] = (width + 1) * (z + 1) + (x + 0);
				

				index += 6;
			}
		}
	}
	

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

	{
		fillModeNumber = 0;
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);

		States::CreateRasterizer(&desc, &fillMode[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &fillMode[1]);
	}

}

ExeGrid::~ExeGrid()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeGrid::Update()
{	
	if (Keyboard::Get()->Down('M'))
		fillModeNumber = fillModeNumber == 0 ? 1 : 0;	
}

void ExeGrid::PreRender()
{
}

void ExeGrid::Render()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	// IA = Input Assembler
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3D::GetDC()->RSSetState(fillMode[fillModeNumber]);

	colorBuffer->SetPSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeGrid::PostRender()
{
	switch (fillModeNumber)
	{
	case 0:
		ImGui::Text("Fill Mode");
		ImGui::SliderInt("Solid", &fillModeNumber, 0, 1);
		break;
	case 1:
		ImGui::Text("Fill Mode");
		ImGui::SliderInt("WireFrame", &fillModeNumber, 0, 1);
		break;
	}
	
}

void ExeGrid::ResizeScreen()
{
}
