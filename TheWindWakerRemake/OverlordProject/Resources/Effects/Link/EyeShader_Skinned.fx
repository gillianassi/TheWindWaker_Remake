float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70] : BONES;

Texture2D gDiffuseMap;
SamplerState samLinear
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

DepthStencilState DisableDepthWriting
{
	//Enable Depth Rendering
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
	//Disable Depth Writing
};


RasterizerState BackCulling
{
	CullMode = FRONT;
};



BlendState AlphaBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0f;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHTS;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;
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
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	
	return diffuseColor;
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique10 Default
{
    pass P0
    {
		SetRasterizerState(BackCulling);
		SetDepthStencilState(DisableDepthWriting, 0);
		SetBlendState(AlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

