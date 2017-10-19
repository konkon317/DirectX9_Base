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
    AddressV = Wrap;//������؂育�ƂɃe�N�X�`�����J��Ԃ�
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
	float4 Pos 	: POSITION;	//���_���W
	//float4 Color : 	COLOR0; //���_�F
	float2 UV	: TEXCOORD0;	//�e�N�X�`�����W
	float3 L	: TEXCOORD1;	//���C�g�x�N�g��
	float3 E	: TEXCOORD2;	//�����x�N�g��
};

VS_OUTPUT VS(
	float4 Pos :POSITION,
	float4 Normal:Normal,
	float3 Tangent: TANGENT0,		//�ڃx�N�g��
	float2 UV : TEXCOORD0	//�@���}�b�v�̃e�N�X�`�����W
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;//�o�̓f�[�^
	
	//���W�ϊ�
	Out.Pos=mul( Pos, matWorldViewProj );
	
	Out.UV = UV;

	//���W�n�̊��ϊ�
	float3 N=  Normal.xyz;
	float3 T = Tangent.xyz;
	float3 B = cross(N,T);

	//���ʔ��˗p�̃x�N�g��
	float3 E =vEyePos -Pos.xyz;//�����x�N�g��
	Out.E.x = dot(E,T);
	Out.E.y = dot(E,B);
	Out.E.z = dot(E,N);
	
	float3 L =-vLightDir.xyz;//���C�g�x�N�g��
	Out.L.x = dot(L,T);
	Out.L.y = dot(L,B);
	Out.L.z = dot(L,N);


	return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{

	float3 N =2.0f*tex2D(NormalSamp,In.UV).xyz -1.0f;	//�@���}�b�v����̖@��
	float3 L =normalize(In.L);												//���C�g�x�N�g��
	float3 R = reflect(-normalize(In.E),N);						//�����̔��˃x�N�g��
	
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

