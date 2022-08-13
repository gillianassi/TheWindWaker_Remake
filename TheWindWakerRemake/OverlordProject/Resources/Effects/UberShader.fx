/*
******************
* DAE Ubershader *
******************
//FirstName: [Gillian] 
//LastName: [Assi] 
//Class: [2DAE06]

**This Shader Contains:

- Diffuse (Texture & Color)
	- Regular Diffuse
- Specular
	- Specular Level (Texture & Value)
	- Shininess (Value)
	- Models
		- Blinn
		- Phong
- Ambient (Color)
- EnvironmentMapping (CubeMap)
	- Reflection + Fresnel Falloff
	- Refraction
- Normal (Texture)
- Opacity (Texture & Value)

-Techniques
	- WithAlphaBlending
	- WithoutAlphaBlending
*/

//GLOBAL MATRICES
//***************
// The World View Projection Matrix
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
// The ViewInverse Matrix - the third row contains the camera position!
float4x4 gMatrixViewInverse : VIEWINVERSE;
// The World Matrix
float4x4 gMatrixWorld : WORLD;
// Pi
const float gPi = 3.14159265359f;
//STATES
//******
RasterizerState gRS_FrontCulling 
{ 
	CullMode = FRONT; 
};

BlendState gBS_EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

//SAMPLER STATES
//**************
SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//LIGHT
//*****
float3 gLightDirection :DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.577f, 0.577f, 0.577f);

//DIFFUSE
//*******
bool gUseTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Bool";
> = false;

float4 gColorDiffuse
<
	string UIName = "Diffuse Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Texture";
>;

//NORMAL MAPPING
//**************

bool gUseTextureNormal
<
	string UIName = "Normal Mapping";
	string UIWidget = "Bool";
> = false;
bool gFlipGreenChannel
<
	string UIName = "Flip Green Channel";
	string UIWidget = "Bool";
> = false;


Texture2D gTextureNormal
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
>;
//SPECULAR
//********
float4 gColorSpecular
<
	string UIName = "Specular Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Texture";
>;

bool gUseSpecular
<
	string UIName = "Use Specular lighting";
	string UIWidget = "Bool";
> = false;
bool gUseTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Bool";
> = false;



int gShininess
<
	string UIName = "Shininess";
	string UIWidget = "Slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1f;
> = 15;


//SPECULAR MODELS
//***************
bool gUseSpecularBlinn
<
	string UIName = "Specular Blinn";
	string UIWidget = "Bool";
> = false;

bool gUseSpecularPhong
<
	string UIName = "Specular Phong";
	string UIWidget = "Bool";
> = false;
//AMBIENT
//*******
float4 gColorAmbient
<
	string UIName = "Ambient Color";
	string UIWidget = "Color";
> = float4(0,0,0,1);

float gAmbientIntensity
<
	string UIName = "Ambient Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 0.0f;


//ENVIRONMENT MAPPING
//*******************
TextureCube gCubeEnvironment
<
	string UIName = "Environment Cube";
	string ResourceType = "Cube";
>;

bool gUseEnvironmentMapping
<
	string UIName = "Environment Mapping";
	string UIWidget = "Bool";
> = false;

float gReflectionStrength
<
	string UIName = "Reflection Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionStrength
<
	string UIName = "Refraction Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionIndex
<
	string UIName = "Refraction Index";
>  = 0.3f;

//FRESNEL FALLOFF
//***************
bool gUseFresnelFalloff
<
	string UIName = "Fresnel FallOff";
	string UIWidget = "Bool";
> = false;


float4 gColorFresnel
<
	string UIName = "Fresnel Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

float gFresnelPower
<
	string UIName = "Fresnel Power";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0f;

float gFresnelMultiplier
<
	string UIName = "Fresnel Multiplier";
	string UIWidget = "slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0;

float gFresnelHardness
<
	string UIName = "Fresnel Hardness";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 0;

//OPACITY
//***************
float gOpacityIntensity
<
	string UIName = "Opacity Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 1.0f;

bool gUseTextureOpacity
<
	string UIName = "Opacity Map";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureOpacity
<
	string UIName = "Opacity Map";
	string UIWidget = "Texture";
>;

//VS IN & OUT
//***********
struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition: COLOR0;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

float3 CalculateSpecularBlinn(float3 viewDirection, float3 normal, float2 texCoord)
{
	// Calculate the half vector for blinn
	float3 halfvector = - normalize(viewDirection + gLightDirection);
	// Calculate speculat strength
	float specularStrength = saturate(dot(halfvector, normal));
	// Specularity
	float3 specular = pow(specularStrength, gShininess);
	// Set Specular intensity
	float specularIntensity = 1.0f;
	if(gUseTextureSpecularIntensity)
	{
			specularIntensity = gTextureSpecularIntensity.Sample(gTextureSampler, texCoord).r;
	}
	
    float3 specularColor = gColorSpecular * specular * specularIntensity;
    
    return specularColor;
}

float3 CalculateSpecularPhong(float3 viewDirection, float3 normal, float2 texCoord)
{
	// Set Specular intensity
	float specularIntensity = 1.0f;
	if(gUseTextureSpecularIntensity)
	{
			specularIntensity = gTextureSpecularIntensity.Sample(gTextureSampler, texCoord).r;
	}
    float  phongExponent = gShininess / 4.f;
    // Note: normal and lightDir need to pxoint either outwards or inwards for the desired result
	float3 directionToLight = -gLightDirection;
    float3 reflection = reflect(directionToLight,normal);
    // light to point, poin to viewer
    float cos = saturate(dot(reflection, viewDirection));
	

    float phong = pow(cos, phongExponent);
	float3 specularColor = gColorSpecular * phong * specularIntensity;
	return specularColor;
}

float3 CalculateSpecular(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specColor = float3(0,0,0);
	if(gUseSpecular)
	{
		if(gUseSpecularBlinn) specColor += CalculateSpecularBlinn(viewDirection, normal, texCoord);
		if(gUseSpecularPhong) specColor += CalculateSpecularPhong(viewDirection, normal, texCoord);
	}
				
	return specColor;
}

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	if(!gUseTextureNormal)return normal;
	
	float3 binormal = cross(normal, tangent);
	if(gFlipGreenChannel)binormal = -binormal;
	float3x3 tangentSpaceAxis = float3x3(tangent, binormal, normal);
    //// Sample From texture files
    float3 sampledNormal = gTextureNormal.Sample(gTextureSampler, texCoord).rgb;
    //// Remap from [0, 1] to [-1, 1]
    float3 newNormal = (sampledNormal * 2.0f) - float3( 1.f, 1.f, 1.f ); // [-1, 1]
    newNormal = normalize(mul(newNormal, tangentSpaceAxis));
	return newNormal;
}

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float observedArea = max(dot(normal, -gLightDirection), 0.0f);
	float halfObserved = (observedArea * 0.5f + 0.5f) * (observedArea * 0.5f + 0.5f);
    if (observedArea < 0)
    {
        return float3(0.f, 0.f, 0.f);
    }
	float3 diffuseColor = gColorDiffuse;
	if(gUseTextureDiffuse) 
	{
		diffuseColor *= gTextureDiffuse.Sample(gTextureSampler, texCoord).rgb;
	}
	float3 diffColor = diffuseColor * halfObserved;
	return diffColor;
}

float3 CalculateFresnelFalloff(float3 normal, float3 viewDirection, float3 environmentColor)
{
    if (!gUseFresnelFalloff)
    {
        return environmentColor;
    }
    // Do calculate the falloff
    float3 fresnelColor;
    
    if (gUseEnvironmentMapping)
    {
        fresnelColor = environmentColor;
    }
    else
    {
        fresnelColor = gColorFresnel;
    }
	
	// ------ CALCULATE FRESNEL ------------
    // Check how alligned the normal and viewdir are
    float3 fresnel = abs(dot(normal, viewDirection));
	// Cap between 0 and 1 and take the unalligned part
	// the more alligned, the larger the dot, the lower the fresnel
	// parallel = no reflection
	// perpendicular = reflection
    fresnel = 1 - saturate(fresnel);
	// Adjusts the Size of the Falloff
    fresnel = pow(fresnel, gFresnelPower);
	// Adjusts Rim Brightness
    fresnel *= gFresnelMultiplier;
	// ------ CALCULATE MASK ------------
	// Adjusts the shape of the falloff using a second mask (vertical linear gradient).
	// check how alligned the normal is with ground directioon
    float3 mask = dot(float3(0, -1, 0), normal);
	// Cap between 0 and 1 and take the unalligned part
	// facing away from the ground results in a larger values
    mask = 1 - saturate(mask);
	// Adjust the size of the mask
    mask = pow(mask, gFresnelHardness);
	// Calculate the final color
    fresnelColor *= fresnel * mask;
    return fresnelColor;
}

float3 CalculateEnvironment(float3 viewDirection, float3 normal)
{
	if (!gUseEnvironmentMapping)return float3(0,0,0);
	
	float3 reflectionVector = reflect(normal, viewDirection);
	float3 refractionVector = refract(normal, viewDirection, gRefractionIndex);
	
	float3 reflection = gCubeEnvironment.Sample(gTextureSampler, reflectionVector) * gReflectionStrength;
	float3 refraction = gCubeEnvironment.Sample(gTextureSampler, refractionVector) * gRefractionStrength;
	
	float3 environmentColor = reflection + refraction;
	return environmentColor;
}

float CalculateOpacity(float2 texCoord)
{
	if(!gUseTextureOpacity) return gOpacityIntensity;
	float opacity = gTextureOpacity.Sample(gTextureSampler, texCoord).r * gOpacityIntensity;
	
	return opacity;
}

// The main vertex shader
VS_Output MainVS(VS_Input input) {
	
	VS_Output output = (VS_Output)0;
	
	output.Position = mul(float4(input.Position, 1.0), gMatrixWVP);
	output.WorldPosition = mul(float4(input.Position,1.0), gMatrixWorld);
	output.Normal = mul(input.Normal, (float3x3)gMatrixWorld);
	output.Tangent = mul(input.Tangent, (float3x3)gMatrixWorld);
	output.TexCoord = input.TexCoord;
	
	return output;
}

// The main pixel shader
float4 MainPS(VS_Output input) : SV_TARGET {
	// NORMALIZE
	input.Normal = normalize(input.Normal);
	input.Tangent = normalize(input.Tangent);
	
	float3 viewDirection = normalize(input.WorldPosition.xyz - gMatrixViewInverse[3].xyz);
	
	//NORMAL
	float3 newNormal = CalculateNormal(input.Tangent, input.Normal, input.TexCoord);
		
	//SPECULAR
	float3 specColor = CalculateSpecular(viewDirection, newNormal, input.TexCoord);
		
	//DIFFUSE
	float3 diffColor = CalculateDiffuse(newNormal, input.TexCoord);
		
	//AMBIENT
	float3 ambientColor = gColorAmbient * gAmbientIntensity;
		
	//ENVIRONMENT MAPPING
	float3 environmentColor = CalculateEnvironment(viewDirection, newNormal);
	
	//FRESNEL FALLOFF
	environmentColor = CalculateFresnelFalloff(newNormal, viewDirection, environmentColor);
		
	//FINAL COLOR CALCULATION
	float3 finalColor = diffColor + specColor + environmentColor + ambientColor;
	
	//OPACITY
	float opacity = CalculateOpacity(input.TexCoord);
	
	return float4(finalColor,opacity);
}

// Default Technique
technique10 WithAlphaBlending 
{
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetBlendState(gBS_EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

// Default Technique
technique10 WithoutAlphaBlending 
{
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}