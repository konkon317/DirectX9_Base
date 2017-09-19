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
	float3 E	: TEXCOORD2;

	float4 diffuseRate : 	COLOR0;
	//float4 phong 	:	COLOR1;
};

VS_OUTPUT VS( 
	float4 LocalPos : POSITION 	,
	float4 Normal	: NORMAL	,
	float2 Tex      : TEXCOORD0
) 
{
	VS_OUTPUT Out =(VS_OUTPUT)0;
	Out.Pos=mul( LocalPos, matWorldViewProj );

	float3 E =normalize(vEyePos-LocalPos.xyz);
	float3 L = -vLightDir;
	float3 N = normalize(Normal.xyz);
	float3 R = -E +2.0f*dot(N,E)*N;//”½ŽËƒxƒNƒgƒ‹

	Out.E=E;
	Out.N=N;
	
	Out.diffuseRate = K_d * (max(0,dot(N,L)));

	//Out.phong =pow(max(0,dot(L,R)),10);
	//Out.phong.a=1.0f;
	
	Out.Tex =Tex;	

   	return Out;
}

float4 PS( VS_OUTPUT  In) : COLOR0
{
   float4 color= tex2D(Samp,In.Tex);

	float3 L=-vLightDir;
	float3 H=normalize(In.E+L);
	float3 N= normalize(In.N);
   
   	color=//color* 
		((I_a*K_a)+
		(I_d*In.diffuseRate)+
		(pow(max(0,dot(N,H)),10))
		);


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
