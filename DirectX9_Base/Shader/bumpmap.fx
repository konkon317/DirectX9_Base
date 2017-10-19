float4x4 matWorldViewProj;
float4x4 matWorldInverseTranspose;

float4 vLightDir;
float3 vEyePos;

float4 I_a = {0.2f,0.2f,0.2f,1.0f};
float4 I_d = {0.8f,0.8f,0.8f,1.0f};

float4 K_a = {1.0f,1.0f,1.0f,1.0f};
float4 K_d = {1.0f,1.0f,1.0f,1.0f};

texture Tex;
sampler Samp =sampler_state
{
	Texture = <Tex>;
	MinFilter  = LINEAR;
	MagFilter  = LINEAR;
	MipFilter  = NONE;

	AddressU =Clamp;
	AddressV =Clamp;
};

texture NormalMap;
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;//整数区切りごとにテクスチャを繰り返し
};

texture RampMap;
sampler RampSamp = sampler_state
{
	Texture = <RampMap>;
	MinFilter  = LINEAR;
	MagFilter  = LINEAR;
	MipFilter  = NONE;

	AddressU =Clamp;
	AddressV =Clamp;
};

struct VS_OUTPUT
{
	float4 Pos 	: POSITION;	//頂点座標
	//float4 Color : 	COLOR0; //頂点色
	float2 UV	: TEXCOORD0;	//テクスチャ座標
	float3 L	: TEXCOORD1;	//ライトベクトル
	float3 E	: TEXCOORD2;	//視線ベクトル
};

VS_OUTPUT VS(
	float4 Pos :POSITION,
	float4 Normal:Normal,
	float3 Tangent: TANGENT0,		//接ベクトル
	float2 UV : TEXCOORD0	//法線マップのテクスチャ座標
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;//出力データ
	
	//座標変換
	Out.Pos=mul( Pos, matWorldViewProj );
	
	Out.UV = UV;

	//座標系の基底変換
	float3 N=  Normal.xyz;
	float3 T = Tangent.xyz;
	float3 B = cross(N,T);

	//鏡面反射用のベクトル
	float3 E =vEyePos -Pos.xyz;//視線ベクトル
	Out.E.x = dot(E,T);
	Out.E.y = dot(E,B);
	Out.E.z = dot(E,N);
	
	float3 L =-vLightDir.xyz;//ライトベクトル
	Out.L.x = dot(L,T);
	Out.L.y = dot(L,B);
	Out.L.z = dot(L,N);


	return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{

	float3 N =2.0f*tex2D(NormalSamp,In.UV).xyz -1.0f;	//法線マップからの法線
	float3 L =normalize(In.L);												//ライトベクトル
	float3 R = reflect(-normalize(In.E),N);						//視線の反射ベクトル
	
	float4 C   		= tex2D(Samp,In.UV);
	float4 amb 		=(I_a*K_a);
	float4 diffuse=(K_d * I_d*(max(0,dot(N,L))));
	float highLight= pow(max(0,dot(R,L)),8);

	float2 rampUV={ diffuse.x+ highLight+0.1,0};
	float v=tex2D(RampSamp,rampUV).x;
	
	return  C*( amb + v );
							
	
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 VS();
				PixelShader 	= compile ps_3_0 PS();
   }
}

