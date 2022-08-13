float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gFoamColor :Diffuse <
 string UIName = "WaterColor";
 string UIWidget = "Color";
> =float4(0.0f, 0.0f, 0.0f, 1.0f);


bool gUseGradientOpacity = false;
float2 gUVSpeed = float2(0.f,0.f);
Texture2D gFoamDiffuse;
Texture2D gGradient;
float gTotalTime
<
 string UIName = "TimeSpent";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 10.0; 

 float UIStep = 0.01f;
> = 0.f; 


float gDepthBias
<
 string UIName = "Depth Bias";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 0.01; 

 float UIStep = 0.0001f;
> = 0.f; 



float gVOffset
<
 string UIName = "V Offset";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 1.0; 

 float UIStep = 0.01f;
> = 0.0f; 



SamplerState samLinearVClamp
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Clamp;// or Mirror or Clamp or Border
};

SamplerState samLinearVWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{

	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.pos.w += gDepthBias;
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	// Move the texCoord = input
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input, SamplerState samplerState, bool isShoreFoam) : SV_TARGET
{

	float3 color_rgb = gFoamColor.rgb;

	//--float2 movingUVs = input.texCoord + (gUVSpeed * gTotalTime);
	// Calculate the expected offset
	float2 baseUVOffset = (gUVSpeed * gTotalTime);
	baseUVOffset.y +=gVOffset;
	
	if (isShoreFoam)
	{
		// If it is a shore wave, you want the offset to be of range [-1.f, 0] 
		// -> transparent above the wave
		baseUVOffset.y = (baseUVOffset.y * 100.f % 100) * 0.01f - 1.f;
		
	}
	// horizontal reversed uv's
	// You want the foam to go both sides (to flicker
	float2 reversedUVOffset = baseUVOffset;
	reversedUVOffset.x = (-gUVSpeed.x* gTotalTime);

	float2 baseUV = input.texCoord + baseUVOffset;
	float4 baseTextureColor = gFoamDiffuse.Sample(samplerState, input.texCoord + baseUVOffset);
	float4 reversedTextureColor = gFoamDiffuse.Sample(samplerState, input.texCoord + reversedUVOffset);

	//float opacity = gFoamColor.a * (baseTextureColor.a + reversedTextureColor.a);
	// only draw the foam
	float opacity = (baseTextureColor.r + reversedTextureColor.r);
	if (gUseGradientOpacity)
	{
		// use clamp to avoid overshooting UV's
		opacity *= gGradient.Sample(samLinearVClamp, input.texCoord);
	}


	////HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	//color_rgb = color_rgb * diffuseStrength;

	return float4(color_rgb, opacity);
}

// assign sampler state from techniques
float4 PS_Wrap(VS_OUTPUT input) : SV_TARGET
{
	return PS(input, samLinearVWrap, false);
}
float4 PS_Clamp(VS_OUTPUT input) : SV_TARGET
{
	// if it is shore foam
	// change the used sampler state to clamp 
	// -> keep the transparancy above the wave
	return PS(input, samLinearVClamp, true);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique10 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader( ps_4_0, PS_Wrap() ) );
    }
}

technique10 ShoreTech
{
	pass P
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Clamp()));
	}
}

