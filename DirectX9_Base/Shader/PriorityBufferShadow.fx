//プライオリティバッファシャドウ


//ローカルから射影空間への座標変換
float4x4 matWorldViewProj;

//ローカルから射影空間への座標変換　ライト視点
float4x4 matWorldViewProj_L;

//テクスチャ座標系への射影
float4x4 matWorldViewProjTex;

//メッシュの色
float4 vCol;

//プライオリティ番号(自分の番号)
float4 vId;

//ライト方向
float4 vLightDir;


//テクスチャサンプラ
//Idマップ
texture IdMap;
sampler IdMapSamp = sampler_state
{
	Texture = <IdMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;

};

//デカールマップ
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


//頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 Pos			:POSITION;
	float4 Diffuse		:COLOR0;
	float4 Ambient		:COLOR1;
	float4 ShadowMapUV	:TEXCOORD0;
	float4 ID			:TEXCOORD1;
	float2 UV_Decale	:TEXCOORD2;
};

//以下シェーダ関数

//複数パスで描画
//=======================================================

//0パス目　頂点シェーダ
VS_OUTPUT VS_pass0
(
	float4 Pos		: POSITION,
	float3 Normal	: NORMAL
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//位置座標
	Out.Pos = mul(Pos, matWorldViewProj_L);

	//IDを色として出力
	Out.Diffuse = vId;

	return Out;
}


//0パス目　ピクセルシェーダ
PIXELSHADER PS_pass0 = asm
{
	//アセンブリで記述

	ps.1.1

	//色をIdとして出力
	mov r0,v0
};

//=======================================================

//1パス目(2パス目)　頂点シェーダ

VS_OUTPUT VS
(
	float4 Pos:POSITION,
	float3 Normal:NORMAL,
	float2 UV:TEXCOORD
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;


	//座標変換
	Out.Pos = mul(Pos, matWorldViewProj);

	//色
	Out.Diffuse = vCol*max(dot(vLightDir.xyz, Normal), 0);//拡散
	Out.Ambient = vCol*0.3;

	//テクスチャ座標
	float4 uv;
	uv = mul(Pos, matWorldViewProjTex);
	Out.ShadowMapUV = uv;


	//Id値(自分の番号)
	Out.ID = vId;


	//デカールUV座標
	Out.UV_Decale = UV;

	return Out;
}

//1パス目　ピクセルシェーダ
float4 PS_pass1(VS_OUTPUT In):Color
{
	float4 Color = In.Ambient;

	float4 zero = { 0,0,0,0 };

	float ofset = 0.01f;
	
	float4 id_map = tex2Dproj(IdMapSamp, In.ShadowMapUV);
	float4 decale = tex2D(DecaleMapSamp, In.UV_Decale);

	Color += (id_map.x<In.ID.x + ofset && id_map.x>In.ID.x - ofset) ? In.Diffuse : zero;


	return decale*Color;

}

//テクニック
technique TShader
{
	pass P0
	{
		// シェーダ
		VertexShader = compile vs_2_0 VS_pass0();
		PixelShader = <PS_pass0>;
	}
	pass P1
	{
		// シェーダ
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_pass1();
	}
}