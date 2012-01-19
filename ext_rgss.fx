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

TEX_VS_OUTPUT WindowContentsVertexShader(
  float4 pos :POSITION, float4 diffuse :COLOR0, float4 texCoord0 :TEXCOORD0, float4 indices :BLENDINDICES) {
  TEX_VS_OUTPUT outVS;

  if(indices[0] == 255) {
    if(texCoord0.x == 0) {
      pos.x += 12;
    } else {
      pos.x -= 12;
    }
    if(texCoord0.y == 0) {
      pos.y += 12;
    } else {
      pos.y -= 12;
    }
  } else {
    pos.x = -1;
    pos.y = -1;
  }
  outVS.pos = mul(pos, matWVP);
  outVS.diffuse = diffuse;
  outVS.texCoord0 = texCoord0.xy;
  return outVS;
}
float4 WindowContentsPixelShader(float2 texCoord0 :TEXCOORD0) :COLOR0 {
  return tex2D(TexSampler, texCoord0);
}

technique ExtRgssTec {
  pass P0 {
    VertexShader = compile vs_2_0 TexVertexShader();
    PixelShader = compile ps_2_0 TexPixelShader();
  }
  pass P1 {
    VertexShader = compile vs_2_0 WindowContentsVertexShader();
    PixelShader = compile ps_2_0 WindowContentsPixelShader();
  }
}
