// --------------------------------------------
//  __  __      _ _ ___ ___ 
// |  \/  |__ _| (_) __| _ )
// | |\/| / _` | | | _|| _ \
// |_|  |_\__,_|_|_|_| |___/
//
// The simple Mali Frame Buffer Surface toolkit
//
// (C) COPYRIGHT 2020 Retro Games Ltd, and portions
// (C) COPYRIGHT 2015 ARM Limited
//
// ALL RIGHTS RESERVED
// --------------------------------------------
#ifndef MALIFB_CL_H
#define MALIFB_CL_H

#define EGL_FBDEV 1

#if defined(__arm__)
# define EGL_EGLEXT_PROTOTYPES
# define GL_GLEXT_PROTOTYPES
#endif

#if defined(__arm__) && !defined( FORCE_OPENGL )
# define USE_MALI
#endif

#if defined PLATFORM_AMD64 || defined __x86_64__
# define TIDLEN 4
# define SIDLEN 3
#else
# define TIDLEN 2
# define SIDLEN 3
#endif

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#if defined(__arm__)
# include <EGL/eglext.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Data values ----------------------------------------------------------------

typedef enum {
    MFB_Surface_None        = 0,
    MFB_Surface_Transparent = 1<<0,
    MFB_Surface_Opaque      = 1<<2, // use to make sure even RGBA images are rendered opaque
    MFB_Surface_Hidden      = 1<<3,
} _MFB_Surface_Flags;

typedef int MFB_Surface_Flags;

typedef enum {
    MFB_Display_Unknown    = -1,
    MFB_Display_50Hz       = 1<<0,
    MFB_Display_60Hz       = 1<<1,
} MFB_Display_Flags;

typedef enum {
    MFB_LUMINANCE       = 1,
    MFB_RGB             = 2,
    MFB_RGB565          = 3,

    MFB_ALPHA           = 256, // Bit field
    MFB_LUMINANCE_ALPHA = MFB_LUMINANCE | MFB_ALPHA,
    MFB_RGBA            = MFB_RGB       | MFB_ALPHA,
} MFB_Image_Type;

typedef enum {
    MFB_Texture_Flag_None      = 0,
    MFB_Texture_Flag_Linear    = 1<<0,
    MFB_Texture_Flag_Nearest   = 1<<1,
    MFB_Texture_Flag_CRT       = 1<<2,
    MFB_Texture_Flag_CRT_Blend = MFB_Texture_Flag_Linear | MFB_Texture_Flag_CRT,
} MFB_Texture_Flag;

// Data structures ------------------------------------------------------------

typedef void MFB_Screen;
typedef struct _mfb_surface MFB_Surface;

typedef struct {
    int _id[TIDLEN];
    int width;
    int height;
} MFB_Texture;

struct _mfb_surface {
    int _id[SIDLEN];
    int width;
    int height;
};

typedef void (*MFB_VSyncCallbackHandler)( MFB_Screen *s );

// Prototypes -----------------------------------------------------------------

MFB_Display_Flags MFB_GetDisplayMode();

MFB_Screen  * MFB_ScreenCreate ( const char *name, MFB_Display_Flags dflags );
MFB_Screen  * MFB_ScreenCreateFromExisting( NativeWindowType native_window, MFB_Display_Flags dflags );
void          MFB_ScreenDestroy( MFB_Screen **s );

MFB_Surface * MFB_SurfaceCreate( MFB_Screen *s, int x, int y, int w, int h, int layer, MFB_Surface_Flags f );

//            Coords and dimenstions define rectangle on screen that surface occupies
int           MFB_SurfaceSize( MFB_Surface *, int x, int y, int w, int h );
int           MFB_SurfaceHide( MFB_Surface * );
int           MFB_SurfaceReveal( MFB_Surface * );

//            Coords and dimensions define rectangle within texture to map onto surface
int           MFB_SurfaceUpdate ( MFB_Surface *, int x, int y, int w, int h, MFB_Texture * );
void          MFB_SurfaceAlpha  ( MFB_Surface *, unsigned int );
void          MFB_SurfaceDestroy( MFB_Surface ** );

int           MFB_ScreenRender( MFB_Screen * );
void          MFB_Display( MFB_Screen *s );
#if defined( PLATFORM_SUN50IW6 ) || defined (__x86_64__) || defined (__i386__)
int           MFB_VSyncCallback( MFB_Screen *s, MFB_VSyncCallbackHandler cb );
#endif

MFB_Texture * MFB_TextureCreate( int w, int h, MFB_Image_Type color_format );
int           MFB_TextureRegister( MFB_Screen *s, MFB_Texture *t );
int           MFB_TextureDeregister( MFB_Screen *s, MFB_Texture *t );
int           MFB_TextureUpdate( MFB_Texture *t, int x, int y, int w, int h, uint8_t *src_data, MFB_Texture_Flag flags ); // x and w not used
void          MFB_TextureDestroy( MFB_Texture **t );

#ifdef __cplusplus
}
#endif

#endif
