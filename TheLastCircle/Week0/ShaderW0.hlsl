// ShaderW0.hlsl

cbuffer constants : register(b0)
{
    float3 Offset;
    float Radius;
    
    float3 Color;
    float Pad;
}

struct VS_INPUT
{
    float4 position : POSITION;      // Input position from vertex buffer
    float4 DummyColor : COLOR;       // Input color from vertex buffer (but will never be used)
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
};


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    output.position = float4((input.position.xyz * Radius) + Offset, 1.0f);
        
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // Output the color directly from constant buffer
    return float4(Color, 1.0f);
}
