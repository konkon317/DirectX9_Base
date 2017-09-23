float4x4 matWorldViewProj;
float4x4 matWorldInverseTranspose;

float3 vLightDir;
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

struct VS_OUTPUT
{
	float4 Pos 	: POSITION;
	float2 Tex 	: TEXCOORD0;
	float3 N	: TEXCOORD1;
	float3 X	: TEXCOORD2;

	float4 Color : 	COLOR0;
};

VS_OUTPUT VS( 
	float4 LocalPos : POSITION 	,
	float4 Normal	: NORMAL	,
	float2 Tex      : TEXCOORD0
) 
{
	VS_OUTPUT Out =(VS_OUTPUT)0;
	
	//座標変換
	Out.Pos=mul( LocalPos, matWorldViewProj );

	float3 E =normalize(vEyePos-LocalPos.xyz);
	float3 L = -vLightDir;
	float3 N = normalize(Normal.xyz);

	//拡散光
	Out.Color =(I_a*K_a)+ (K_d * I_d*(max(0,dot(N,L))));

	//鏡面反射用のベクトル
	Out.X= LocalPos.xyz;
	Out.N= N;
	
	//テクスチャ座標
	Out.Tex =Tex;	

   	return Out;
}

float4 PS( VS_OUTPUT  In) : COLOR0
{
	//テクスチャ色　サンプリング
  float4 color= tex2D(Samp,In.Tex);

	//鏡面反射の計算　クック・トランスモデル
	float3 L=-vLightDir;					//ライトベクトル
	float3 N = normalize(In.N);				//法線ベクトル
	float3 V = normalize(vEyePos - In.X);	//視線ベクトル
	float3 H = normalize(V + L);			//視線とライトのハーフベクトル

	//計算に使うそれぞれの角度
	float NV = dot(N, V);
	float NH = dot(N, H);
	float VH = dot(V, H);
	float NL = dot(N, L);
	float LH = dot(L, H);

	//マイクロファセットの分布関数
	//Beckmann分布関数
	const float m = 0.35f;//荒さ
	float NH2 =  NH*NH;
	float D = exp(-(1 - NH2) / (NH2*m*m)) /
				 (4 * m *m * NH2 * NH2);

	//幾何学減数率　
	float G = min(1,min(2 * NH*NV / VH, 2 * NH*NL / VH));

	//フレネル
	float n = 3.0f;//複素屈折率の実数部
	float g = sqrt(n*n + LH*LH - 1);
	float gpc = g + LH;
	float gnc = g - LH;
	float cgpc = LH*gpc - 1;
	float cgnc = LH*gnc + 1;
	float F = 0.5f* ((gnc * gnc) / (gpc*gpc)) *(1 + (cgpc*cgpc) / (cgnc*cgnc));
	float F0 =((n-1)*(n-1))/((n+1)*(n+1));

	//金属の色
	float4 light_color ={0.296f,0.304f,1.000f,1.0f};
	float4 ks = {light_color.x*0.484f,
							 light_color.y*0.433f,
							 light_color.z*0.185f,
								1.0f};
	ks = ks+(light_color-ks)*max(0,F-F0)/(1-F0);
		ks=ks*max(0,(F*G*D/NV));
   
   	color=color* In.Color;

	color=color+ks;


   return color;
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 VS();
				PixelShader 	= compile ps_2_0 PS();
   }
}
