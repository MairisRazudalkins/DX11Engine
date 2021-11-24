//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ModelConstBuffer : register(b0) // Object buffer
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer LightingConstBuffer : register(b1) // Light buffer
{
    float4 DiffuseLight;
    float4 AmbientLight;
    float4 SpecularLight;
    float3 LightVecW;
}

cbuffer MaterialConstBuffer : register(b2) // Mtrl buffer
{
    float4 DiffuseMtrl;
    float4 AmbientMtrl;
    float4 SpecularMtrl;
    float SpecularPower;
    float3 EyePosW;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    //float4 Pos : SV_POSITION;
    //float3 NormalL : NORMAL0;
    //float4 Color : COLOR0;
    
    float4 PosH : SV_POSITION; // TIDY THIS UP!
    float3 NormalL : NORMAL0;
    float2 Tex : TEXCOORD0;
    float3 PosW : POSITION;
    float4 Color : COLOR0;
};

Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
Texture2D txNorm : register(t2);

SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
        
    output.PosH = mul(Pos, World);
    output.PosH = mul(output.PosH, View);
    output.PosH = mul(output.PosH, Projection);

    float3 normalW = mul(float4(NormalL, 0.0f), World).xyz;
    normalW = normalize(normalW);

    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
    output.Color.rgb = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
    output.Color.a = DiffuseMtrl.a;
    
    output.Tex = TexCoord;
    
    //output.NormalL = mul(float4(NormalL, 0.f), World).xyz;
    //output.NormalL = normalize(output.NormalL);
    //
    //output.PosW = mul(float4(Pos.xyz, 1.f), World).xyz;
    //
    //output.PosH = mul(Pos, World);
    //output.PosH = mul(output.PosH, View);
    //output.PosH = mul(output.PosH, Projection);

    
    // --------- ORIGINAL --------- \\
    //output.Pos = mul( Pos, World );
    //output.Pos = mul( output.Pos, View );
    //output.Pos = mul( output.Pos, Projection );
    //output.Color.rgba = 1.f;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    normalize(input.NormalL);
    
    float3 diffuseMap = txDiffuse.Sample(samLinear, input.Tex);
    float3 specularMap = txSpec.Sample(samLinear, input.Tex);
    float3 normalMap = (txNorm.Sample(samLinear, input.Tex) * 2.f) - 1.f;
    
    float3 bumpNormal = normalize((normalMap.x * 0.5f) + (normalMap.y * 0.1f) + (normalMap.z * input.NormalL));
    float lightIntensity = max(saturate(dot(bumpNormal, -LightVecW)) * SpecularPower, 1.f); // if normal map isnt assinged default the light intensity to 1.f so object isnt dark
    
    float3 reflection = normalize(2 * lightIntensity * bumpNormal - LightVecW);
    float4 specular = pow(saturate(dot(reflection, EyePosW)), SpecularPower);
    specular = specular * float4(specularMap, 1.f);
    
    float4 color = saturate((DiffuseMtrl + float4(diffuseMap, 1.f)) * lightIntensity);
    
    return color;
    
    //input.NormalL = normalize(input.NormalL);
    //
    //float4 norm = txNorm.Sample(samLinear, input.Tex);
    //norm = (norm * 2.f) - 1.f;
    //
    //float3 bumpNormal = (norm.x * 0.2f) + (norm.y * .1f) + (norm.z * input.NormalL);
    //bumpNormal = normalize(bumpNormal);
    //
    //float lightIntensity = saturate(dot(bumpNormal, LightVecW)) * SpecularPower;
    //
    //float4 textureColor = txDiffuse.Sample(samLinear, input.Tex);
    //
    //float4 color = saturate(textureColor * lightIntensity);
    //color = color * textureColor;
    //
    //return input.Color;
}
