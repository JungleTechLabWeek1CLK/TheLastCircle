#pragma once

// Windows API
#include <windows.h>

// D3D
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#include <d3d11.h>
#include <d3dcompiler.h>


#include "Math.h"
#include "Shapes.h"


///////////////////////////////////////////////
// Rnederer Class
class URenderer
{
public:
    void Create(HWND hWindow)
    {
        CreateDeviceAndSwapChain(hWindow);
        CreateFrameBuffer();
        CreateRasterizerState();
        CreateShader();
        CreateConstantBuffer();
        CreateVertexBuffers();
    }
    void Release()
    {
        ReleaseVertexBuffers();
        ReleaseConstantBuffer();
        ReleaseShader();
        ReleaseRasterizerState();

        // Reset RenderTarget
        DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

        ReleaseFrameBuffer();
        ReleaseDeviceAndSwapChain();
    }

    void SwapBuffer()
    {
        SwapChain->Present(1, 0); // set syncinterval to 1 for enabling VSync
    }


    // Prepare for rendering
    void Prepare()
    {
        DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
        DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        DeviceContext->RSSetViewports(1, &ViewportInfo);
        DeviceContext->RSSetState(RasterizerState);

        DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
        DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

        PrepareShader();
    }

    void UpdateConstantBuffer(const FVector& Offset, const float Radius, const FVector& Color)
    {
        if (ConstantBuffer)
        {
            D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

            DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame

            FConstants* Constants = (FConstants*)constantbufferMSR.pData;
            Constants->Offset = Offset;
            Constants->Radius = Radius;
            Constants->Color = Color;

            DeviceContext->Unmap(ConstantBuffer, 0);
        }
    }

    void RenderPrimitive(ETypePrimitive PrimitiveType)
    {
        switch (PrimitiveType)
        {
        case EPT_Triangle:
            RenderPrimitive(VertexBufferTriangle, NumVerticesTriangle);
            break;
        case EPT_Cube:
            RenderPrimitive(VertexBufferCube, NumVerticesCube);
            break;
        case EPT_Sphere:
            RenderPrimitive(VertexBufferSphere, NumVerticesSphere);
            break;
        }
    }


    ID3D11Device* Device = nullptr; // for communicating with GPU
    ID3D11DeviceContext* DeviceContext = nullptr; // for managing GPU command execution
private:
    // Device and Swap Chain
    void CreateDeviceAndSwapChain(HWND hWindow)
    {
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

        DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
        swapchaindesc.BufferDesc.Width = 0; // fit to screen size
        swapchaindesc.BufferDesc.Height = 0; // fit to screen size
        swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapchaindesc.SampleDesc.Count = 1; // disable multi-sampling
        swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchaindesc.BufferCount = 2; // double buffering                        
        swapchaindesc.OutputWindow = hWindow;
        swapchaindesc.Windowed = TRUE;
        swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;


        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
            featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
            &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

        SwapChain->GetDesc(&swapchaindesc);

        // Setup viewport
        ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
    }
    void ReleaseDeviceAndSwapChain()
    {
        if (DeviceContext)
        {
            // execute remaining GPU commands
            DeviceContext->Flush();
        }

        if (SwapChain)
        {
            SwapChain->Release();
            SwapChain = nullptr;
        }

        if (Device)
        {
            Device->Release();
            Device = nullptr;
        }

        if (DeviceContext)
        {
            DeviceContext->Release();
            DeviceContext = nullptr;
        }
    }

    // Frame Buffer
    void CreateFrameBuffer()
    {
        // get texture of back buffer from the swap chain
        SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

        D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
        framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
    }
    void ReleaseFrameBuffer()
    {
        if (FrameBuffer)
        {
            FrameBuffer->Release();
            FrameBuffer = nullptr;
        }

        if (FrameBufferRTV)
        {
            FrameBufferRTV->Release();
            FrameBufferRTV = nullptr;
        }
    }

    // Rasterizer
    void CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rasterizerdesc = {};
        rasterizerdesc.FillMode = D3D11_FILL_SOLID;
        rasterizerdesc.CullMode = D3D11_CULL_BACK; // backface culling

        Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
    }
    void ReleaseRasterizerState()
    {
        if (RasterizerState)
        {
            RasterizerState->Release();
            RasterizerState = nullptr;
        }
    }

    // Shader
    void CreateShader()
    {
        ID3DBlob* vertexshaderCSO;
        ID3DBlob* pixelshaderCSO;

        D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

        Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

        D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

        Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

        Stride = sizeof(FVertexSimple);

        vertexshaderCSO->Release();
        pixelshaderCSO->Release();
    }
    void ReleaseShader()
    {
        if (SimpleInputLayout)
        {
            SimpleInputLayout->Release();
            SimpleInputLayout = nullptr;
        }

        if (SimplePixelShader)
        {
            SimplePixelShader->Release();
            SimplePixelShader = nullptr;
        }

        if (SimpleVertexShader)
        {
            SimpleVertexShader->Release();
            SimpleVertexShader = nullptr;
        }
    }
    void PrepareShader()
    {
        DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
        DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
        DeviceContext->IASetInputLayout(SimpleInputLayout);

        // bind constant buffer to vertex shader and pixel shader
        if (ConstantBuffer)
        {
            DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
            DeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
        }
    }

    // Vertex Buffer
    void CreateVertexBuffers()
    {
        NumVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);
        NumVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
        NumVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);

        VertexBufferTriangle = CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
        VertexBufferCube = CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
        VertexBufferSphere = CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
    }
    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
    {
        // Create a vertex buffer
        D3D11_BUFFER_DESC vertexbufferdesc = {};
        vertexbufferdesc.ByteWidth = byteWidth;
        vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
        vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

        ID3D11Buffer* vertexBuffer;

        Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

        return vertexBuffer;
    }
    void ReleaseVertexBuffers()
    {
        ReleaseVertexBuffer(VertexBufferCube);
        ReleaseVertexBuffer(VertexBufferTriangle);
        ReleaseVertexBuffer(VertexBufferSphere);
    }
    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
    {
        vertexBuffer->Release();
    }

    // Constant Buffer
    void CreateConstantBuffer()
    {
        D3D11_BUFFER_DESC constantbufferdesc = {};
        constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
        constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
    }
    void ReleaseConstantBuffer()
    {
        if (ConstantBuffer)
        {
            ConstantBuffer->Release();
            ConstantBuffer = nullptr;
        }
    }

    // Rendering Method
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
    {
        UINT offset = 0;
        DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

        DeviceContext->Draw(numVertices, 0);
    }


    IDXGISwapChain* SwapChain = nullptr; // for swapping frame buffers

    // for managing resources
    ID3D11Texture2D* FrameBuffer = nullptr;
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;
    ID3D11RasterizerState* RasterizerState = nullptr;
    ID3D11Buffer* ConstantBuffer = nullptr;

    // for managing vertex buffers
    ID3D11Buffer* VertexBufferTriangle = nullptr;
    ID3D11Buffer* VertexBufferCube = nullptr;
    ID3D11Buffer* VertexBufferSphere = nullptr;
    UINT NumVerticesTriangle;
    UINT NumVerticesCube;
    UINT NumVerticesSphere;


    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // color of the background
    D3D11_VIEWPORT ViewportInfo;

    // for managing shaders
    ID3D11VertexShader* SimpleVertexShader;
    ID3D11PixelShader* SimplePixelShader;
    ID3D11InputLayout* SimpleInputLayout;
    unsigned int Stride;

    // structure for a constant buffer
    struct FConstants
    {
        FVector Offset;
        float Radius;

        FVector Color;
        float Pad;
    };
};
///////////////////////////////////////////////


// TODO: decrease the number of parameters
class UCharacterPlayer;
class UCharacterEnemy;
class UProjectile;
void DrawCharacters(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectilePlayerList, INT32 ProjectilePlayerListCount, UProjectile** ProjectileEnemyList, INT32 ProjectileEnemyListCount, URenderer* Renderer);