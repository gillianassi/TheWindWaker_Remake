float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

float2 gUVSpeed = float2(0.f,0.f);
Texture2D gShadeDiffuse;

float gTotalTime
<
 string UIName = "TimeSpent";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 10.0; 

 float UIStep = 0.01f;
> = 0.f; 

float gShadeOpacity
<
 string UIName = "ShadeOpacity";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 1.0; 

 float UIStep = 0.01f;
> = 0.2f; 


float gVOffset
<
 string UIName = "V Offset";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 1.0; 

 float UIStep = 0.01f;
> = 0.5f; 


float gDepthBias
<
 string UIName = "Depth Bias";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 0.01; 

 float UIStep = 0.0001f;
> = 0.f; 



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
		// place the coordinates closer in depth to avoid z-fighting
 	output.pos.w += gDepthBias;
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	// Move the texCoord = input
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float3 color_rgb = float3(0.f ,0.f,0.f);
	// Calculate the expected offset
	float2 baseUVOffset = (gUVSpeed * gTotalTime);
	baseUVOffset.y = +gVOffset;
	// horizontal reversed uv's
	// You want the foam to go both sides (to flicker
	float2 reversedUVOffset = baseUVOffset;
	reversedUVOffset.x = (-gUVSpeed.x* gTotalTime) - 0.5f;
	
	float2 baseUV = input.texCoord + baseUVOffset;
	float4 baseTextureColor = gShadeDiffuse.Sample(samLinearVClamp, input.texCoord + baseUVOffset);
	float4 reversedTextureColor = gShadeDiffuse.Sample(samLinearVClamp, input.texCoord + reversedUVOffset);

	// only draw the foam
	float opacity = saturate(baseTextureColor.r + reversedTextureColor.r) * gShadeOpacity;
	return float4(color_rgb, opacity);
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
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}


