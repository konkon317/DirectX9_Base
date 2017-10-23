
//���[�J������@�ˉe��Ԃւ̍��W�ϊ�
float4x4 matWorldViewProj;

//���[�J������e�N�X�`����Ԃւ̍��W�ϊ�
float4x4 matWorldViewProjTex;

float4 vLightPos;//���C�g�̈ʒu

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

//���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 DecaleUV	: TEXCOORD0;
	float4 TexShadow: TEXCOORD1;
};

//���_�V�F�[�_
VS_OUTPUT VS(
	float4 Pos		: POSITION	//���_�ʒu
	, float4 Normal : NORMAL	//�@���x�N�g��
	, float2 UV : TEXCOORD0	//�e�N�X�`�����W
)
{
	VS_OUTPUT Out;

	//�ʒu���W
	Out.Pos = mul(Pos, matWorldViewProj);
	
	Out.Color = max(dot(normalize(vLightPos.xyz - Pos.xyz), Normal.xyz), 0);

	//�e�N�X�`�����W
	Out.DecaleUV = UV;

	//�e�N�X�`�����W
	Out.TexShadow = mul(Pos, matWorldViewProjTex);

	return Out;

}

float4 PS(VS_OUTPUT In): COLOR
{
	float4 decale= tex2D(DecaleMapSamp,In.DecaleUV);
	float4 shadow = tex2Dproj(ShadowMapSamp, In.TexShadow);

	return decale*(saturate(In.Color - 0.5f*shadow) + 0.3f);
}

//�e�N�j�b�N
technique  BasicTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}