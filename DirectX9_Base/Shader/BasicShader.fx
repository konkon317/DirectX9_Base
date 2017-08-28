float4x4 matWorldViewProj;

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
	float4 Pos :POSITION;
	float2 Tex :TEXCOORD0;
};

VS_OUTPUT VS( 
	float4 LocalPos : POSITION ,
	float2 Tex      : TEXCOORD 
) 
{
	VS_OUTPUT Out =(VS_OUTPUT)0;
	Out.Pos=mul( LocalPos, matWorldViewProj );

	Out.Tex =Tex;

   	return Out;
}

float4 PS( VS_OUTPUT  In) : COLOR0
{

   return tex2D(Samp,In.Tex);
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 VS();
		PixelShader 	= compile ps_2_0 PS();
   }
}
