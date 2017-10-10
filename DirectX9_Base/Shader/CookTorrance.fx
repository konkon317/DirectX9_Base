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
	float2 uv	: TEXCOORD0;
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
	Out.uv =Tex;	

   	return Out;
}

float4 PS( VS_OUTPUT  In) : COLOR0
{
float4 color = tex2D(Samp,In.uv);

	//クック・トランスモデル (金属鏡面反射)

	//ライトベクトル
	float3 L = -vLightDir;

	//法線ベクトル
	float3  N = normalize(In.N);
	//頂点シェーダからピクセルシェーダに渡さる過程で補間されて
	//長さが1でなくなっているので再度標準化ベクトルに直す
	
	//視線ベクトル
	//視点の座標　-　そのピクセルに対応するローカル座標
	//座標から視点へのベクトル
	float3 E = normalize(vEyePos - In.X);//参考書だとEでなくVとしている

	//視線ベクトルとライトベクトルのハーフベクトル
	float3 H = normalize(E + L);

	//計算に使用するベクトル同士のなす角の角度(cos)を計算
	float NE = dot(N, E);
	float NH = dot(N, H);
	float EH = dot(E, H);
	float NL = dot(N, L);
	float LH = dot(L, H);

	//クックトランスの反射モデル
	// 色 *((D * G * F )/ (N・V));

	//D マイクロファセットの分布関数
	//視線の反射ベクトルがライトの方向を向く割合
	//Becmann分布を使用
	const float m = 0.35;//表面の粗さ　0に近いほうがつるつる
	float NH2 = NH * NH;
	float D = exp(-(1 - NH2) / (NH2 * m * m)) / (4 * m * m * NH2 * NH2);
	//exp (exponential) 指数関数 自然対数の底の何乗であるか
	//e^(X) と表現される物と一緒

	//G 幾何学減衰率
	//マイクロファセットに入射した光が別のマイクロファセットに影を作る効果
	//反射光が遮蔽される場合と入射光が遮蔽される場合に二つがある
	float Ga = 2 * (NH*NE) / (EH); //反射光が視線に届く割合								   
	float Gb = 2 * (NH*NL) / (EH); //入射光が遮られなかった割合
	//Ga,Gbは遮蔽された割合が1からマイナスされている
	float G = min(1, min(Ga, Gb));//Ga,Gb,1の内の最低値を採用する

	//F フレネル (公式をそのまま使用)
	//電磁気学的な現象
	//面と光の入射と屈折率によって反射する光の強さが変わる
	float n = 3.0f;//(複素)屈折率の実数部 //金属の場合は屈折率は複素数になるらしい
	float g = sqrt( n * n + LH * LH - 1);
	float gpc = g + LH;
	float gnc = g - LH;
	float cgpc = LH*gpc - 1;
	float cgnc = LH*gnc + 1;
	float F = 0.5f * gnc*gnc * (1 + (cgpc*cgpc) / (cgnc*cgnc)) / (gpc*gpc);

	//金属の色
	float4 ks = color;
	//光の色
	float4 light_color = { 1.0f,1.0f,1.0f,1.0f };

	float4 color0 = { ks.x*light_color.x,
					 ks.y*light_color.y,
					 ks.z*light_color.z,
					 1.0f};

	//カラーシフト　
	//反射光に金属の色がふくまれること
	//ただし入射方向と視線の方向が真反対の場合は全反射が起きるため
	//入射光の色がそのまま返る
	//視線のほぼ正面から照らしたときとの中間を線形補間で計算する

	//視線と光のベクトルが重なっているときの反射光色
	//上のFの式のLHを1と見た時の式 (cos(0)=1より)
	float F0 = ((n - 1) / (n + 1))*((n - 1) / (n + 1));

	float4 colorR = color0;	

	//t が補間の変数
	float t = max(0, F - F0) / (1 - F0);
	colorR = (1 - t)*color0 + (t*light_color);
	
	
	//4つの項をまとめて鏡面反射計算の完成
    colorR = colorR * max(0, F * G * D / (NE));

	// *  1 / NV ; 観測者から見える面の見た目上のマイクロファセットの濃度
	//             縁の部分に鏡面反射が出やすくなる効果

	//環境光　拡散光
	color = color* In.Color;

	//鏡面反射
	color = color+colorR;

	return color;
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 VS();
				PixelShader 	= compile ps_3_0 PS();
   }
}
