float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

float gPI = 3.14159265;

float gRetractSpeed = 0.f;
Texture2D gFoamDiffuse;
float gTotalTime
<
 string UIName = "TimeSpent";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 100.0; 

 float UIStep = 0.1f;
> = 0.f; 


float gDepthBias
<
 string UIName = "Depth Bias";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 0.01; 

 float UIStep = 0.0001f;
> = 0.f; 


float gShadeOpacity
<
 string UIName = "ShadeOpacity";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 1.0; 

 float UIStep = 0.01f;
> = 0.2f; 




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
	DepthEnable = True;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE	;
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
	// wave base formula A * cos(2 * pi * f * t + delta)  
	// Cos to start at the highest point of the wave (from 1 to -1)
	// Amplitude = 1. to only go over half the UV (From 1. to - 1)
	// add 0.5 too the function (from 1. to 0.)
	// PhaseSHift of Pi to start at the bottom of the sine wave (-0.5 to 0)
	float frequency = gRetractSpeed * 0.25f;
	float VSquish = 1.f;
	// make a wave  that goes like a wave from min to max and then instantly resets
	// like a sawtooth wave but with a soft edge instead of a straight one
	// use the sinus to see when it is 
	
	float strength = 0.4f;
	
	if((sin(2.f* gPI * frequency * 2.f *gTotalTime)) < 0.f)
	{
		VSquish -= strength * abs(cos(2 * gPI * frequency * gTotalTime)) + strength;
	}
	else
	{
		VSquish -= strength * abs(cos(2 * gPI * frequency * gTotalTime - gPI * 0.5f )) + strength;
	}

	
	
	//float fullPeriodTime = 1.0f / frequency;
    //float localTime = fmod(gTotalTime, fullPeriodTime);
    
	//VSquish = 1.f * ((localTime / fullPeriodTime));
	
	//VSquish = gTotalTime;
	
	float2 baseTexCoord =  input.texCoord;
	// give some offset to stay behind the water
	baseTexCoord.y -= 0.05f;
	baseTexCoord.y = baseTexCoord.y * VSquish;
	
	float4 baseTextureColor = gFoamDiffuse.Sample(samLinearVClamp,baseTexCoord);

	// only draw the foam
	float opacity = (baseTextureColor.r) * gShadeOpacity;

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
		SetPixelShader( CompileShader( ps_4_0, PS()) );
    }
}
