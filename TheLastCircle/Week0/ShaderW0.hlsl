// ShaderW0.hlsl

Texture2D MainTexture : register(t0);
Texture2D BackgroundTexture : register(t1);
SamplerState MainSampler : register(s0);


cbuffer constants : register(b0)
{
    float3 Offset;
    float Radius;
    
    float3 PlayerOffset;
    
    // .5 = player, 2.5 = player projectile, 3.5 = enemy projectile
    // 4.5 = EXP, 5.5 = bomb, 6.5 = heal, 7.5 = magnet
    // 10.5 = enemy_walker, 11.5 = enemy_runner, 12.5 = enemy_ranger
    // 100.5 = background, 99.5 = health bar, 98.5 = exp bar
    float CharacterType;
    
    float InvincibleTime;
}

struct VS_INPUT
{
    float4 Position : POSITION; // Input position from vertex buffer
    float4 DummyColor : COLOR; // Input color from vertex buffer (but will never be used)
    float2 UV : TEXCOORD; // UV 
};

struct PS_INPUT
{
    float4 Position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 UV : TEXCOORD;
};


PS_INPUT mainVS(VS_INPUT Input)
{
    PS_INPUT Output;
    
    if (CharacterType > 99 && CharacterType < 100)
    {
        // health bar
        float Width = 0.3f;
        float Height = 0.04f;
        
        float3 Box = float3(Input.Position.x * Width, Input.Position.y * Height, 0.f);
        
        Output.Position = float4(Box + Offset, 1.0f);
    }
    else if (CharacterType > 98 && CharacterType < 99)
    {
        // exp bar
        float Width = 0.3f;
        float Height = 0.04f;
        
        float3 Box = float3(Input.Position.x * Width, Input.Position.y * Height, 0.f);
        
        Output.Position = float4(Box + Offset, 1.0f);
    }
    else if (CharacterType > 10 && CharacterType < 13)
    {
        // enemy
        
        // scale
        float3 Position = Input.Position.xyz * Radius;
        // rotation
        float3 Direction = PlayerOffset;
        if (Direction.x * Direction.x > 0.000001f || Direction.y * Direction.y > 0.000001f)
            Direction = Offset - PlayerOffset;
        else
            Direction = Offset;
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
    UvScale.x = 0.33f;
    UvScale.y = 0.33f;
    float2 UvOffset;
    UvOffset.x = 0.0f;
    UvOffset.y = 0.0f;
    
    if (CharacterType < 1)
    {
        // player
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
        if (InvincibleTime > 0.0f)
        {
            float Flash = 0.5f + 0.5f * sin(InvincibleTime * 20.0f);

            TextureColor.rgb = lerp(TextureColor.rgb, float3(1.0f, 1.0f, 1.0f), Flash);
        }
    }
    else if (CharacterType > 2 && CharacterType < 3)
    {
        // player projectile
        UvOffset.y = 0.33f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 3 && CharacterType < 4)
    {
        // enemy projectile
        UvOffset.x = 0.33f;
        UvOffset.y = 0.33f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 4 && CharacterType < 5)
    {
        // EXP
        float3 Color = { 0.9f, 0.9f, 0.15f };
        TextureColor = float4(Color, 1.f);
    }
    else if (CharacterType > 5 && CharacterType < 6)
    {
        // bomb
        UvOffset.y = 0.66f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 6 && CharacterType < 7)
    {
        // heal
        UvOffset.x = 0.33f;
        UvOffset.y = 0.66f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 7 && CharacterType < 8)
    {
        // magnet
        UvOffset.x = 0.66f;
        UvOffset.y = 0.66f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 10 && CharacterType < 11)
    {
        // enemy walker
        UvOffset.x = 0.33f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 11 && CharacterType < 12)
    {
        // enemy runner
        UvOffset.x = 0.66f;
        UvOffset.y = 0.33f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 12 && CharacterType < 13)
    {
        // enemy ranger
        UvOffset.x = 0.66f;
        TextureColor = MainTexture.Sample(MainSampler, Input.UV * UvScale + UvOffset);
    }
    else if (CharacterType > 100)
    {
        // Background
        float ScrollSpeed = 5.f;
        
        float2 UVScroll;
        UVScroll.x = PlayerOffset.x * ScrollSpeed;

        UVScroll.y = -PlayerOffset.y * ScrollSpeed;
        
        TextureColor = BackgroundTexture.Sample(MainSampler, Input.UV + UVScroll);
    }
    else if (CharacterType > 99 && CharacterType < 100)
    {
        // health bar
        float BorderWidth = 0.02f;
        float BorderHeight = 0.1f;
        
        if (Input.UV.x < BorderWidth || Input.UV.x > 1.f - BorderWidth || Input.UV.y < BorderHeight || Input.UV.y > 1.f - BorderHeight)
            TextureColor = float4(1.f, 1.f, 1.f, 1.f);
        else if (Input.UV.x < (PlayerOffset.x / PlayerOffset.y))
            TextureColor = float4(0.9f, 0.15f, 0.15f, 1.f);
        else
            TextureColor = float4(0.15f, 0.15f, 0.15f, 1.f);
    }
    else if (CharacterType > 98 && CharacterType < 99)
    {
        // exp bar
        float BorderWidth = 0.02f;
        float BorderHeight = 0.1f;
        
        if (Input.UV.x < BorderWidth || Input.UV.x > 1.f - BorderWidth || Input.UV.y < BorderHeight || Input.UV.y > 1.f - BorderHeight)
            TextureColor = float4(1.f, 1.f, 1.f, 1.f);
        else if (Input.UV.x < (PlayerOffset.x / PlayerOffset.y))
            TextureColor = float4(0.9f, 0.9f, 0.15f, 1.f);
        else
            TextureColor = float4(0.15f, 0.15f, 0.15f, 1.f);
    }
    
    return TextureColor;
}
