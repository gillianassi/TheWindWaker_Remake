TextureCube gCubeMap : CubeMap;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState NoCulling
{
    CullMode = NONE;
};
DepthStencilState LessEqualDepth
{
    DepthFunc = LESS_EQUAL;
};
cbuffer cbChangesEveryFrame
{
    matrix matWorldViewProj : WorldViewProjection;
}

struct VS_INPUT
{
    float3 localPos : POSITION;
};

struct VS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;

    // Make sure the sky box is always on the far plane
    // z = w -> z/w =1
    vOut.posH = mul(float4(input.localPos, 0.0f), matWorldViewProj).xyww;
    // use local vertex position as cubemap lookup vector
    vOut.texCoord = input.localPos;

    return vOut;
}
//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return gCubeMap.Sample(samLinear, input.texCoord);
    return float4(1,1,1,1);
}

technique10 Render
{
    pass P0
    {
        SetRasterizerState(NoCulling);
        SetDepthStencilState(LessEqualDepth, 0);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}