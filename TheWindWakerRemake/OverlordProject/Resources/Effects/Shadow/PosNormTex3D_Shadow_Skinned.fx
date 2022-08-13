float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];
float2 gResolution = float2(1280.0f, 720.0f);

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
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

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
		
	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;
	
	// For each bone
	for(int i = 0; i < 4; ++i)
	{
		// Get current index
		float currentBoneIndex = input.blendIndices[i];
		// Make sure it is >= to 0
		if( currentBoneIndex >= 0)
		{
			// Calculate boneSpacePosition and boneSpaceNomal
			float4 boneSpacePosition = mul(originalPosition, gBones[input.blendIndices[i]]); 
			float3 boneSpaceNormal = mul(input.normal, gBones[input.blendIndices[i]]); 
			// Append this position/normal to the transformed position/normal based on the corresponding weight
			transformedPosition += input.blendWeights[i] * boneSpacePosition;
			transformedNormal += input.blendWeights[i] * boneSpaceNormal;
		}
	}
	// Ensure the w-component of the transformed position is 1
	transformedPosition.w = 1.0;
	
	// convert position into float4 and transform the transformed position to clipping space with matWorldViewProj
	output.pos = mul ( transformedPosition, gWorldViewProj );
	// transform the normal to world space = rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld));
	
    // Project our position to light clip space and store it in
    // the texcoord semantic to be interpolated across the surface
	output.texCoord = input.texCoord;
    output.lPos = mul( transformedPosition, mul(  gWorld, gWorldViewProj_Light ) );
	
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
    //sample shadow map - point sampler		//PCF sampling for shadow map
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
	return  min(shadowFactor + 0.8f, 1.f);
		
    //sample shadow map - point sampler
    //float shadowMapDepth = gShadowMap.Sample(samPoint, lpos.xy).r;
 	
    //if clip space z value greater than shadow map value then pixel is in shadow
    //if ( shadowMapDepth < lpos.z) return 0;
 
    // calculate ilumination at fragment
    // return 1.f;}
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
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue, color_a );
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

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

