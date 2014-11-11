// Let's make this BLUEEEE

// Globals
float4x4 gWorld; // World Transform Matrix
float4x4 gViewProj; // Combined View and Projection
texture DiffuseMapTexture;
float4 AmbientColor;

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

VS_OUTPUT VS_TextureMap(VS_INPUT IN)
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

float4 PS_TextureMap(VS_OUTPUT IN) : COLOR
{
    return AmbientColor * tex2D(DiffuseMap, IN.vUV);
}

technique DefaultTechnique
{
    pass P0
    {
        vertexShader = compile vs_2_0 VS_TextureMap();
        pixelShader  = compile ps_2_0 PS_TextureMap();

		FillMode = Solid; // WireFrame, Solid, ...
    }
}
