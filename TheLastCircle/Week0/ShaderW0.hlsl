// ShaderW0.hlsl

cbuffer constants : register(b0)
{
    float3 Offset;
    float Radius;
    
    float3 Color;
    float Pad;
    
    float3 PlayerOffset;
    float IsEnemy;
}

struct VS_INPUT
{
    float4 Position : POSITION;      // Input position from vertex buffer
    float4 DummyColor : COLOR;       // Input color from vertex buffer (but will never be used)
};

struct PS_INPUT
{
    float4 Position : SV_POSITION; // Transformed position to pass to the pixel shader
};


PS_INPUT mainVS(VS_INPUT Input)
{
    PS_INPUT Output;
    
    if (IsEnemy < 2)
    {
        // enemy
        
        // scale
        float3 Position = Input.Position.xyz * Radius;
        // rotation
        float3 Direction = Offset - PlayerOffset;
        Direction = normalize(Direction);        
        float2x2 RotationMatrix = float2x2(-Direction.y, Direction.x, -Direction.x, -Direction.y);
        float2 Position2D = mul(Position.xy, RotationMatrix);
        Position.xy = Position2D;
        // translation
        Output.Position = float4(Position + Offset, 1.0f);
    }
    else
    {
        // default
        Output.Position = float4((Input.Position.xyz * Radius) + Offset, 1.0f);
    }
    
    
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    // Output the color directly from constant buffer
    return float4(Color, 1.0f);
}
