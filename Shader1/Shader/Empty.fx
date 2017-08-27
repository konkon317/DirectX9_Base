float4x4 matWorldViewProj;

float4 BasicTransform( float4 LocalPos : POSITION ) : POSITION
{

   return mul( LocalPos, matWorldViewProj );
}

float4 NoWorkingPixelShader( float4 ScreenColor : COLOR0 ) : COLOR0
{

   return ScreenColor;
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 BasicTransform();
   	PixelShader 	= compile ps_2_0 NoWorkingPixelShader();
   }
}
