#ifndef __EXTRGSS_GRAPHICS_H__
#define __EXTRGSS_GRAPHICS_H__

#include "d3d9.h"
#include "d3dx9.h"

#define FVF_VERTEX   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
typedef struct
{
  float x, y, z;
  D3DCOLOR diffuse;
  float u, v;
} VERTEX;

extern void Init_ExtGraphics();
extern void Graphics__create_texture(LPDIRECT3DTEXTURE9 *ptr_texture, VERTEX *v, LONG w, LONG h);
extern void Graphics__update_texture(LPDIRECT3DTEXTURE9 texture, DWORD *src, LONG w, LONG h);

extern VALUE mGraphics;

#endif
