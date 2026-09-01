// ShaderW0.hlsl

Texture2D MainTexture : register(t0); 
SamplerState MainSampler : register(s0); 


cbuffer constants : register(b0)
{
    float3 Offset;
    float Radius;
    
    float3 PlayerOffset;
    float CharacterType; // .5 = player, 1.5 = enmey, 2.5 = player projectile, 3.5 enemy projectile
}

struct VS_INPUT
{
    float4 Position : POSITION;      // Input position from vertex buffer
    float4 DummyColor : COLOR;       // Input color from vertex buffer (but will never be used)
    float2 UV : TEXCOORD;            // UV 
};

struct PS_INPUT
{
    float4 Position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 UV : TEXCOORD;
};


PS_INPUT mainVS(VS_INPUT Input)
{
    PS_INPUT Output;
    
    if (CharacterType > 1 && CharacterType < 2)
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
    
    Output.UV = Input.UV;
    
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    float4 TextureColor;
    float2 UvScale;
    UvScale.x = 0.5f;
    UvScale.y = 0.5f;
    float2 UvOffset;
    UvOffset.x = 0.0f;
    UvOffset.y = 0.0f;
    
    if (CharacterType < 1)
    {
        // player
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 1 && CharacterType < 2)
    {
        // enemy
        UvOffset.x = 0.5f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 2 && CharacterType < 3)
    {
        // player projectile
        UvOffset.y = 0.5f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 3 && CharacterType < 4)
    {
        // enemy projectile
        UvOffset.x = 0.5f;
        UvOffset.y = 0.5f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    
    return TextureColor;
}
