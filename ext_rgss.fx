texture Tex;
float4x4 matWVP;
sampler2D TexSampler = sampler_state { Texture = (Tex); };
static const int SKIN_WIDTH = 128, SKIN_HEIGHT = 128;

struct TEX_VS_OUTPUT {
  float4 pos :POSITION;
  float4 diffuse :COLOR0;
  float4 texCoord0 : TEXCOORD0;
};

TEX_VS_OUTPUT TexVertexShader(float4 pos :POSITION, float4 diffuse :COLOR0, float4 inTexCoord0 :TEXCOORD0) {
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
    if(texCoord0.x < 0) {
      pos.x += 12;
      texCoord0.x = 0;
    } else {
      pos.x -= 12;
      texCoord0.x = (texCoord0.x - 12 * 2) / texCoord0.z;
    }
    if(texCoord0.y < 0) {
      pos.y += 12;
      texCoord0.y = 0;
    } else {
      pos.y -= 12;
      texCoord0.y = (texCoord0.y - 12 - 12) / texCoord0.w;
    }
  } else {
    pos.x = -1;
    pos.y = -1;
  }
  outVS.pos = mul(pos, matWVP);
  outVS.diffuse = diffuse;
  outVS.texCoord0 = texCoord0;
  return outVS;
}
float4 WindowContentsPixelShader(float2 texCoord0 :TEXCOORD0) :COLOR0 {
  return tex2D(TexSampler, texCoord0);
}

TEX_VS_OUTPUT WindowVertexShader(
  float4 pos :POSITION, float4 diffuse :COLOR0, float4 texCoord0 :TEXCOORD0, float4 indices :BLENDINDICES) {
  TEX_VS_OUTPUT outVS;
  float closeness = texCoord0.y * (255 - indices[0]) / 510;

  if(texCoord0.x < 0) {
    texCoord0.z = -texCoord0.x - 32;
    texCoord0.x = -16;
  } else {
    texCoord0.z = texCoord0.x - 32;
    texCoord0.x = texCoord0.x - 16;
  }
  if(texCoord0.y < 0) {
    pos.y -= closeness;
    texCoord0.w = -texCoord0.y - 32;
    texCoord0.y = -16;
  } else {
    pos.y -= closeness;
    texCoord0.w = texCoord0.y - 32;
    texCoord0.y = texCoord0.y - 16;
  }
  outVS.pos = mul(pos, matWVP);
  outVS.diffuse = diffuse;
  outVS.texCoord0 = texCoord0;
  return outVS;
}
float4 WindowPixelShader(float4 texCoord0 :TEXCOORD0) :COLOR0 {
  bool inner = false;
  if(texCoord0.x < 0) {
    texCoord0.x = (64 + 16 + texCoord0.x) / SKIN_WIDTH;
  } else {
    if(texCoord0.x >= texCoord0.z) {
      texCoord0.x = (128 - 16 + texCoord0.x - texCoord0.z) / SKIN_WIDTH;
    } else {
      texCoord0.x = (64 + 16 + texCoord0.x % 32) / SKIN_WIDTH;
      inner = true;
    }
  }
  if(texCoord0.y < 0) {
    texCoord0.y = (0 + 16 + texCoord0.y) / SKIN_HEIGHT;
  } else {
    if(texCoord0.y >= texCoord0.w) {
      texCoord0.y = (64 - 16 + texCoord0.y - texCoord0.w) / SKIN_HEIGHT;
    } else if (!inner) {
      texCoord0.y = (16 + texCoord0.y % 32) / SKIN_HEIGHT;
    } else {
     discard;
    }
  }
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
  pass P2 {
    VertexShader = compile vs_2_0 WindowVertexShader();
    PixelShader = compile ps_2_0 WindowPixelShader();
  }
}
