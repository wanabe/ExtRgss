#include "ext_rgss.h"
#include "graphics.h"
#include "bitmap.h"
#include "sprite.h"
#include "window.h"

LPDIRECT3DDEVICE9 pD3DDevice;
LPD3DXEFFECT pEffect;
VALUE mGraphics, mOldGraphics;

struct hWndFinder {
  HWND hWnd;
  DWORD pId;
};

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lp) {
  struct hWndFinder *pFinder = (struct hWndFinder *)lp;
  DWORD pId;
  GetWindowThreadProcessId(hWnd, &pId);
  if(pId == pFinder->pId) {
    pFinder->hWnd = hWnd;
    return FALSE;
  }
  return TRUE;
}

static VALUE Graphics_s_init(VALUE self) {
  HRESULT hr;
  LPDIRECT3D9 pD3D;
  D3DPRESENT_PARAMETERS D3DPP = {0,0,D3DFMT_UNKNOWN,0,D3DMULTISAMPLE_NONE,0,
    D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D24S8,D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL,
    D3DPRESENT_RATE_DEFAULT,D3DPRESENT_INTERVAL_DEFAULT};
  struct hWndFinder finder;
  ID3DXBuffer *pErrorMsgs;

  finder.pId = GetCurrentProcessId();
  EnumWindows(EnumWindowsProc, (LPARAM)&finder);
  if((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
    rb_raise(rb_eRuntimeError, "Direct3DCreate9 failed");
  }

  hr = pD3D->lpVtbl->CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, finder.hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice);
  if(FAILED(hr)) {
    hr = pD3D->lpVtbl->CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, finder.hWnd,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice);
    if(FAILED(hr)) {
      rb_raise(rb_eRuntimeError, "IDirect3D9::CreateDevice failed");
    }
  }

  hr = D3DXCreateEffectFromFile(
    pD3DDevice, "ext_rgss.fx", NULL, NULL, 0, NULL, &pEffect, &pErrorMsgs);
  if(FAILED(hr)) {
    if(pErrorMsgs) {
      rb_raise(rb_eRuntimeError, (char*)(pErrorMsgs->lpVtbl->GetBufferPointer(pErrorMsgs)));
    } else {
      rb_raise(rb_eRuntimeError, "Can't load effect file");
    }
  }
  return self;
}

static VALUE Graphics_s_update(VALUE self) {
  D3DXMATRIX mat = {{{1.0/544,0,0,0, 0,-1.0/416,0,0, 1,-1,-1,-1, -1.0/2,1.0/2,1.0/2,1.0/2}}};
  VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));
  VALUE windows = rb_ivar_get(mGraphics, rb_intern("@windows"));
  VALUE *ptr = RARRAY_PTR(sprites);
  int i, len = RARRAY_LEN(sprites);
  RgssBitmapData *bmpdata, *passed_bmpdata = 0;

  pD3DDevice->lpVtbl->Clear(pD3DDevice, 0, NULL, (D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0,0,0,0), 1.0f, 0);
  pD3DDevice->lpVtbl->SetRenderState(pD3DDevice, D3DRS_ALPHABLENDENABLE,TRUE);
  pD3DDevice->lpVtbl->SetRenderState(pD3DDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  pD3DDevice->lpVtbl->SetRenderState(pD3DDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3DDevice->lpVtbl->SetFVF(pD3DDevice, FVF_VERTEX);
  pEffect->lpVtbl->SetMatrix(pEffect,  "matWVP", &mat);
  pEffect->lpVtbl->SetTechnique(pEffect, "ExtRgssTec");
  if(SUCCEEDED(pD3DDevice->lpVtbl->BeginScene(pD3DDevice))) {
    UINT numPass;
    pEffect->lpVtbl->Begin(pEffect, &numPass, 0 );

    pEffect->lpVtbl->BeginPass(pEffect, 0);
    for(i = 0; i < len; i++) {
      Sprite *sprite = EXT_SPRITE(ptr[i]);
      VALUE bitmap = sprite->bitmap;
      if(sprite->disposed || !sprite->visible || !RTEST(bitmap)) {
        continue;
      }
      bmpdata = RGSS_BITMAPDATA(bitmap);
      if(passed_bmpdata != bmpdata) {
        passed_bmpdata = bmpdata;
        BitmapData__update(bmpdata);
        pEffect->lpVtbl->SetTexture(pEffect, "Tex", (LPDIRECT3DBASETEXTURE9)BITMAP_EXTDATA(bmpdata)->texture);
        pEffect->lpVtbl->CommitChanges(pEffect);
      }
      pD3DDevice->lpVtbl->DrawPrimitiveUP(pD3DDevice, D3DPT_TRIANGLESTRIP, 2, sprite->vertex_data, sizeof(VERTEX));
    }
    pEffect->lpVtbl->EndPass(pEffect);

    ptr = RARRAY_PTR(windows);
    len = RARRAY_LEN(windows);
    pEffect->lpVtbl->BeginPass(pEffect, 1);
    for(i = 0; i < len; i++) {
      Window *window = EXT_WINDOW(ptr[i]);
      VALUE contents = window->contents;
      if(window->disposed || !window->visible || !RTEST(contents)) {
        continue;
      }
      if(window->openness != 255) {
        continue;
      }
      bmpdata = RGSS_BITMAPDATA(contents);
      if(passed_bmpdata != bmpdata) {
        passed_bmpdata = bmpdata;
        BitmapData__update(bmpdata);
        pEffect->lpVtbl->SetTexture(pEffect, "Tex", (LPDIRECT3DBASETEXTURE9)BITMAP_EXTDATA(bmpdata)->texture);
        pEffect->lpVtbl->CommitChanges(pEffect);
      }
      pD3DDevice->lpVtbl->DrawPrimitiveUP(pD3DDevice, D3DPT_TRIANGLESTRIP, 2, window->vertex_data, sizeof(VERTEX));
    }
    pEffect->lpVtbl->EndPass(pEffect);

    pEffect->lpVtbl->End(pEffect);
    pD3DDevice->lpVtbl->EndScene(pD3DDevice);
  }
  pD3DDevice->lpVtbl->Present(pD3DDevice, NULL,NULL,NULL,NULL);
  return Qnil;
}

static VALUE Graphics_s_dummy() {
  return Qnil;
}

void Graphics__create_texture(LPDIRECT3DTEXTURE9 *ptr_texture, LONG w, LONG h) {
  pD3DDevice->lpVtbl->CreateTexture(pD3DDevice, w, h, 1, 0, D3DFMT_A8R8G8B8,
                                    D3DPOOL_MANAGED, ptr_texture, NULL);
}

void Graphics__update_texture(LPDIRECT3DTEXTURE9 texture, DWORD *src, LONG w, LONG h) {
  D3DLOCKED_RECT lockRect;
  char *dst;
  int y;

  texture->lpVtbl->LockRect(texture, 0, &lockRect, NULL, D3DLOCK_DISCARD);
  dst = lockRect.pBits + h * lockRect.Pitch;
  for(y = 0; y < h; y++) {
    dst -= lockRect.Pitch;
    memcpy(dst, src, sizeof(DWORD) * w);
    src += w;
  }
  texture->lpVtbl->UnlockRect(texture, 0);
}

void Graphics__update_vertex(VERTEX *v, LONG x, LONG y, LONG w, LONG h, RgssRect *rect) {
  v[0].z = v[1].z = v[2].z = v[3].z = 0;
  v[0].x = v[2].x = x;
  v[0].y = v[1].y = y;
  v[1].x = v[3].x = x + rect->w;
  v[2].y = v[3].y = y + rect->h;
  v[0].u = v[2].u = (double)rect->x / w;
  v[0].v = v[1].v = (double)rect->y / h;
  v[1].u = v[3].u = (double)(rect->x + rect->w) / w;
  v[2].v = v[3].v = (double)(rect->y + rect->h) / h;
}

static VALUE Graphics_snap_to_bitmap(VALUE self) {
  VALUE bmp = rb_funcall(mOldGraphics, rb_intern("snap_to_bitmap"), 0);

  Bitmap__init_extdata(bmp);

  return bmp;
}

void Init_ExtGraphics() {
  mOldGraphics = rb_const_get(rb_cObject, rb_intern("Graphics"));
  mGraphics = rb_define_module_under(mExtRgss, "Graphics");
  VALUE sprites = rb_ary_new();
  rb_ivar_set(mGraphics, rb_intern("@sprites"), sprites);
  VALUE windows = rb_ary_new();
  rb_ivar_set(mGraphics, rb_intern("@windows"), windows);
  rb_const_set(rb_cObject, rb_intern("OldGraphics"), mOldGraphics);
  rb_const_set(rb_cObject, rb_intern("Graphics"), mGraphics);
  rb_define_singleton_method(mGraphics, "init", Graphics_s_init, 0);
  rb_define_singleton_method(mGraphics, "update", Graphics_s_update, 0);
  rb_define_singleton_method(mGraphics, "freeze", Graphics_s_dummy, 0);
  rb_define_singleton_method(mGraphics, "transition", Graphics_s_dummy, -1);
  rb_define_singleton_method(mGraphics, "fadein", Graphics_s_dummy, 1);
  rb_define_singleton_method(mGraphics, "fadeout", Graphics_s_dummy, 1);
  rb_define_singleton_method(mGraphics, "snap_to_bitmap", Graphics_snap_to_bitmap, 0);
}
