#ifndef __EXTRGSS_GRAPHICS_H__
#define __EXTRGSS_GRAPHICS_H__

#include <d3d9.h>
#include <d3dx9.h>

#define FVF_VERTEX   (D3DFVF_XYZB1 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE4(0))
typedef struct
{
  float x, y, z;
  unsigned char b1, b2, b3, b4;
  D3DCOLOR diffuse;
  float u, v, w, h;
} VERTEX;

extern void Init_ExtGraphics();
extern void Graphics__create_texture(LPDIRECT3DTEXTURE9 *ptr_texture, LONG w, LONG h);
extern void Graphics__update_texture(LPDIRECT3DTEXTURE9 texture, DWORD *src, LONG w, LONG h);
extern void Graphics__update_vertex(VERTEX *v, LONG x, LONG y, LONG w, LONG h, RgssRect *rect);

extern VALUE mGraphics;

#endif
