float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float2 gResolution = float2(1280.0f, 720.0f);

Texture2D gDiffuseMap;
Texture2D gShadowMap;

float4 gLightColor :Diffuse <
 string UIName = "LightColor";
 string UIWidget = "Color";
> =float4(1.0f, 1.0f, 1.0f, 1.0f);


float gBrightness 
<
 string UIName = "Brightness";
 string UIWidget = "Slider";
 float UIMin = 0.f;
 float UIMax = 1.f; 

 float UIStep = 0.1f;
> = 0.4f; 


float gDetail 
<
 string UIName = "Detail ";
 string UIWidget = "Slider";
 float UIMin = 0.f;
 float UIMax = 0.1f; 

 float UIStep = 0.01f;
> = 0.1f; 


float gStrength 
<
 string UIName = "Strength ";
 string UIWidget = "Slider";
 float UIMin = 0.f;
 float UIMax = 1.f; 

 float UIStep = 0.1f;
> = 0.5f; 

SamplerComparisonState cmpSampler
{
   // sampler state
   Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
   AddressU = Wrap;
   AddressV = Wrap;
 
   // sampler comparison state
   ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};


DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
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

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
		
    output.pos = mul( float4(input.pos, 1), gWorldViewProj );
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
    // Project our position to light clip space and store it in
    // the texcoord semantic to be interpolated across the surface
	output.texCoord = input.texCoord;
    output.lPos = mul( float4(input.pos, 1), gWorldViewProj_Light );
	
	return output;
}

float2 texOffset(int u, int v)
{
	// Return offseted value (our shadow map has the following dimensions: 1280 * 720)
	return float2(u * 1.0f / gResolution.x, v * 1.0f / gResolution.y);
}

float EvaluateShadowMap(float4 lPos)
{
	//re-homogenize position after interpolation
    lPos.xyz /= lPos.w;
 
    //if position is not visible to the light - dont illuminate it
    //results in hard light frustum
    if( lPos.x < -1.0f || lPos.x > 1.0f ||
        lPos.y < -1.0f || lPos.y > 1.0f ||
        lPos.z < 0.0f  || lPos.z > 1.0f ) 
		{
			return 0;
		}
 
    //transform clip space coords to texture space coords (-1:1 to 0:1)
    lPos.x = lPos.x/2.f + 0.5f;
    lPos.y = lPos.y/-2.f + 0.5f;
	// place the coordinates closer to avoid selfshading
 	lPos.z -= gShadowMapBias;
	
		//PCF sampling for shadow map
	float sum = 0;
	float x, y;

	//perform PCF filtering on a 4 x 4 texel neighborhood
	for (y = -1.5; y <= 1.5; y += 1.0)
	{
		for (x = -1.5; x <= 1.5; x += 1.0)
		{
			sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lPos.xy + texOffset(x, y), lPos.z);
		}
	}

	float shadowFactor = sum / 16.0;
	return min(shadowFactor + 0.8f, 1.5f);
	
	
    //sample shadow map - point sampler
    //float shadowMapDepth = gShadowMap.Sample(samPoint, lpos.xy).r;
 	
    //if clip space z value greater than shadow map value then pixel is in shadow
    //if ( shadowMapDepth < lpos.z) return 0;
 
    // calculate ilumination at fragment
    // return 1.f;
}

float Toon(float3 normal, float3 lightDir)
{
	
	float diffuseStrength = max(0.0f, dot(normalize(normal), normalize(lightDir)));	
	//return floor(diffuseStrength / gDetail);
	return smoothstep( 0.5f - gDetail , 0.5f + gDetail, diffuseStrength);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = Toon(input.normal, -gLightDirection) * gStrength + gBrightness;
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = min(diffuseStrength, 1.5f);
	color_rgb = color_rgb * diffuseStrength * gLightColor;

	return float4( color_rgb * shadowValue , color_a );
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
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

