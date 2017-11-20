float4x4 matWorldViewProj;
float4x4 matWorldViewProj_L;
float4x4 matWorldViewProjTex;

float4 vCol;
float4 vLightDir;

texture IdMap;
sampler ShadowMapSamp=sampler_state
{
	Texture =<IdMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture DecaleMap;
sampler DecaleMapSamp =sampler_state
{
	Texture = <DecaleMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU =Clamp;
	AddressV =Clamp;
};

struct VS_OUTPUT
{
	float4 Pos 		: POSITION;
	float4 Diffuse		:COLOR0;
	float4 Ambient 		: COLOR1;
	float4 ShadowMapUV 	: TEXCOORD0;
	float4 Depth		: TEXCOORD1;
	
	float2 DecaleTex 	: TEXCOORD2;	
};

VS_OUTPUT VS_pass0
(
	float4 Pos 	: POSITION,
	float3 Normal 	: NORMAL
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
Pos.w=1.0f;
	float4 pos =mul (Pos,matWorldViewProj_L);
	
	Out.Pos=pos;
	Out.ShadowMapUV =pos;

	return Out; 
}

float4 PS_pass0 (VS_OUTPUT In):COLOR
{
float a=( In.ShadowMapUV.z)/(In.ShadowMapUV.w);

	float4 Out={a,a,a,1};
	


	return Out;	
}

VS_OUTPUT VS 
(
	float4 Pos :POSITION,
	float3 Normal : NORMAL,
	float2 Tex :TEXCOORD0
)
{
	VS_OUTPUT Out =(VS_OUTPUT)0;
	float4 uv;

	Out.Pos = mul(Pos,matWorldViewProj);
	
	Out.Diffuse = vCol*max(dot(vLightDir,Normal),0);
	Out.Ambient = vCol * 0.3f;

	Out.ShadowMapUV = mul(Pos,matWorldViewProjTex);
	
	Out.Depth = mul(Pos,matWorldViewProj_L);
		
	Out.DecaleTex =Tex;
	return Out;
}

float4 PS_pass1 (VS_OUTPUT In):COLOR
{
	float4 Color;
	float4 decale=tex2D(DecaleMapSamp,In.DecaleTex);
	float shadow =tex2Dproj(ShadowMapSamp,In.ShadowMapUV).x;
	Color = In.Ambient + ((shadow* In.Depth.w<In.Depth.z-0.03f)? 0 : In.Diffuse);
	return Color*decale;
}

technique TShader
{
	pass P0
	{
		VertexShader =compile vs_1_1 VS_pass0();
		PixelShader =compile ps_2_0 PS_pass0();
	}
	pass P1
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader =  compile ps_2_0 PS_pass1();
	}	
}









