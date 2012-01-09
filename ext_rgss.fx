texture Tex;
float4x4 matWVP;
sampler2D TexSampler = sampler_state { Texture = (Tex); };

struct TEX_VS_OUTPUT {
  float4 pos :POSITION;
  float4 diffuse :COLOR0;
  float2 texCoord0 : TEXCOORD0;
};

TEX_VS_OUTPUT TexVertexShader(float4 pos :POSITION, float4 diffuse :COLOR0, float2 inTexCoord0 :TEXCOORD0) {
  TEX_VS_OUTPUT outVS;
  outVS.pos = mul(pos, matWVP);
  outVS.diffuse = diffuse;
  outVS.texCoord0 = inTexCoord0;
  return outVS;
}
float4 TexPixelShader(float2 texCoord0 :TEXCOORD0) :COLOR0 {
  return tex2D(TexSampler, texCoord0);
}

technique ExtRgssTec {
  pass P0 {
    VertexShader = compile vs_2_0 TexVertexShader();
    PixelShader = compile ps_2_0 TexPixelShader();
  }
}
