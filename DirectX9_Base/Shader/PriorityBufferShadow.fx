//�v���C�I���e�B�o�b�t�@�V���h�E


//���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 matWorldViewProj;

//���[�J������ˉe��Ԃւ̍��W�ϊ��@���C�g���_
float4x4 matWorldViewProj_L;

//�e�N�X�`�����W�n�ւ̎ˉe
float4x4 matWorldViewProjTex;

//���b�V���̐F
float4 vCol;

//�v���C�I���e�B�ԍ�(�����̔ԍ�)
float4 vId;

//���C�g����
float4 vLightDir;


//�e�N�X�`���T���v��
//Id�}�b�v
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

//�f�J�[���}�b�v
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


//���_�V�F�[�_�o�͍\����
struct VS_OUTPUT
{
	float4 Pos			:POSITION;
	float4 Diffuse		:COLOR0;
	float4 Ambient		:COLOR1;
	float4 ShadowMapUV	:TEXCOORD0;
	float4 ID			:TEXCOORD1;
	float2 UV_Decale	:TEXCOORD2;
};

//�ȉ��V�F�[�_�֐�

//�����p�X�ŕ`��
//=======================================================

//0�p�X�ځ@���_�V�F�[�_
VS_OUTPUT VS_pass0
(
	float4 Pos		: POSITION,
	float3 Normal	: NORMAL
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//�ʒu���W
	Out.Pos = mul(Pos, matWorldViewProj_L);

	//ID��F�Ƃ��ďo��
	Out.Diffuse = vId;

	return Out;
}


//0�p�X�ځ@�s�N�Z���V�F�[�_
PIXELSHADER PS_pass0 = asm
{
	//�A�Z���u���ŋL�q

	ps.1.1

	//�F��Id�Ƃ��ďo��
	mov r0,v0
};

//=======================================================

//1�p�X��(2�p�X��)�@���_�V�F�[�_

VS_OUTPUT VS
(
	float4 Pos:POSITION,
	float3 Normal:NORMAL,
	float2 UV:TEXCOORD
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;


	//���W�ϊ�
	Out.Pos = mul(Pos, matWorldViewProj);

	//�F
	Out.Diffuse = vCol*max(dot(vLightDir.xyz, Normal), 0);//�g�U
	Out.Ambient = vCol*0.3;

	//�e�N�X�`�����W
	float4 uv;
	uv = mul(Pos, matWorldViewProjTex);
	Out.ShadowMapUV = uv;


	//Id�l(�����̔ԍ�)
	Out.ID = vId;


	//�f�J�[��UV���W
	Out.UV_Decale = UV;

	return Out;
}

//1�p�X�ځ@�s�N�Z���V�F�[�_
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

//�e�N�j�b�N
technique TShader
{
	pass P0
	{
		// �V�F�[�_
		VertexShader = compile vs_2_0 VS_pass0();
		PixelShader = <PS_pass0>;
	}
	pass P1
	{
		// �V�F�[�_
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_pass1();
	}
}