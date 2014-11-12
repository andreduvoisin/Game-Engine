// Globals
float4x4 gWorld; // World Transform Matrix
float4x4 gViewProj; // Combined View and Projection
texture DiffuseMapTexture;
float4 AmbientColor;
float3 CameraPosition;

struct PointLight
{
	float4 DiffuseColor;
	float4 SpecularColor;
	float3 Position;
	float SpecularPower;
	float InnerRadius;
	float OuterRadius;
};

static const uint MAX_POINT_LIGHTS = 4;
PointLight PointLights[MAX_POINT_LIGHTS];

sampler2D DiffuseMap = sampler_state
{
	Texture = <DiffuseMapTexture>;
	MagFilter = Linear;
	MinFilter = Anisotropic;
	MipFilter = Linear;
	MaxAnisotropy = 16;
};

struct VS_INPUT
{
	float3 vPos : POSITION0;
	float3 vNormal : NORMAL0;
	float2 vUV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 vPos : POSITION0;
    float2 vUV : TEXCOORD0;
    float3 vNormal : TEXCOORD1;
    float3 vWorldPos : TEXCOORD2;
};

VS_OUTPUT VS_Phong(VS_INPUT IN)
{
    // Clear the output structure.
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	// Apply world transform, then view/projection
	OUT.vWorldPos = mul(gWorld, float4(IN.vPos, 1.0f));
	OUT.vPos = mul(gViewProj, float4(OUT.vWorldPos, 1.0f));

	// Set the output normal.
	OUT.vNormal = mul(gWorld, float4(IN.vNormal, 0.0f));
	OUT.vNormal = normalize(OUT.vNormal);
	// Set the output UV.
	OUT.vUV = IN.vUV;
	 
	// Done--return the output.
    return OUT;
}

float4 PS_Phong(VS_OUTPUT IN) : COLOR
{
	float4 LightColor = AmbientColor * tex2D(DiffuseMap, IN.vUV);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		float3 vPixelToLight = PointLights[i].Position - IN.vWorldPos;
		float fDistance = length(vPixelToLight);
		vPixelToLight = normalize(vPixelToLight);
		float fNdotL = dot(IN.vNormal, vPixelToLight);
		if (fNdotL >= 0)
		{
			// Diffuse
			float fLerpFactor = (fDistance - PointLights[i].InnerRadius) / (PointLights[i].OuterRadius - PointLights[i].InnerRadius);
			float4 DiffuseColor = lerp(PointLights[i].DiffuseColor, float4(0.0f, 0.0f, 0.0f, 1.0f), saturate(fLerpFactor));
			LightColor += saturate(DiffuseColor * fNdotL) * tex2D(DiffuseMap, IN.vUV);

			// Specular
			float3 vReflection = normalize(reflect(-vPixelToLight, IN.vNormal));
			float3 vWorldPosToCamera = normalize(CameraPosition - IN.vWorldPos);
			float fRdotV = dot(vReflection, vWorldPosToCamera);
			if(fRdotV >= 0 && fDistance <= PointLights[i].OuterRadius)
			{
				LightColor += saturate(PointLights[i].SpecularColor * pow(fRdotV, PointLights[i].SpecularPower));
			}
		}
	}

    return LightColor;
}

technique DefaultTechnique
{
    pass P0
    {
        vertexShader = compile vs_3_0 VS_Phong();
        pixelShader  = compile ps_3_0 PS_Phong();

		FillMode = Solid; // WireFrame, Solid, ...
    }
}
