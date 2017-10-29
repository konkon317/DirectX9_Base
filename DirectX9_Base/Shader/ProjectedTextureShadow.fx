
//ローカルから　射影空間への座標変換
float4x4 matWorldViewProj;

//ローカルからテクスチャ空間への座標変換
float4x4 matWorldViewProjTex;

float4 vLightPos;//ライトの位置

texture DecaleMap;
sampler DecaleMapSamp = sampler_state
{
	Texture = <DecaleMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

//頂点シェーダからピクセルシェーダに渡すデータ
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 DecaleUV	: TEXCOORD0;
	float4 TexShadow: TEXCOORD1;
};

//頂点シェーダ
VS_OUTPUT VS(
	float4 Pos		: POSITION	//頂点位置
	, float4 Normal : NORMAL	//法線ベクトル
	, float2 UV : TEXCOORD0	//テクスチャ座標
)
{
	VS_OUTPUT Out;

	//位置座標
	Out.Pos = mul(Pos, matWorldViewProj);
	
	Out.Color = max(dot(normalize(vLightPos.xyz - Pos.xyz), Normal.xyz), 0);

	//テクスチャ座標
	Out.DecaleUV = UV;

	//テクスチャ座標
	Out.TexShadow = mul(Pos, matWorldViewProjTex);

	return Out;

}

float4 PS(VS_OUTPUT In): COLOR
{
	float4 decale= tex2D(DecaleMapSamp,In.DecaleUV);
	float4 shadow = tex2Dproj(ShadowMapSamp, In.TexShadow);

	return decale*(saturate(In.Color - 0.5f*shadow) + 0.3f);
}

//テクニック
technique  BasicTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}