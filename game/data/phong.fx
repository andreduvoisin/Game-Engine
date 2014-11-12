// Let's make this BLUEEEE

// Globals
float4x4 gWorld; // World Transform Matrix
float4x4 gViewProj; // Combined View and Projection
texture DiffuseMapTexture;
float4 AmbientColor;

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

// Position and UV coordinates for the texture.
struct VS_INPUT
{
	float3 vPos : POSITION0;
	float2 vUV : TEXCOORD0;
};

// Transformed position and UV coordinates for the texture.
struct VS_OUTPUT
{
    float4 vPos : POSITION0;
    float2 vUV : TEXCOORD0;
};

VS_OUTPUT VS_Phong(VS_INPUT IN)
{
    // Clear the output structure.
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	// Apply world transform, then view/projection
	OUT.vPos = mul(gWorld, float4(IN.vPos, 1.0f));
	OUT.vPos = mul(gViewProj, OUT.vPos);

	// Set the output UV.
	OUT.vUV = IN.vUV;
	 
	// Done--return the output.
    return OUT;
}

float4 PS_Phong(VS_OUTPUT IN) : COLOR
{
    return AmbientColor * tex2D(DiffuseMap, IN.vUV);
}

technique DefaultTechnique
{
    pass P0
    {
        vertexShader = compile vs_2_0 VS_Phong();
        pixelShader  = compile ps_2_0 PS_Phong();

		FillMode = Solid; // WireFrame, Solid, ...
    }
}
