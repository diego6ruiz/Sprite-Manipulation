#ifndef STEXTURE
#define STEXTURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class STexture 
{
    public:
        STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow);
        ~STexture();
        void load(std::string path);

        void render(int x, int y, int w, int h, SDL_Rect* clip = nullptr, double angle=0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE); 
        void renderWithShader(int x, int y, int w, int h, Uint32 (*func)(Uint32));

        int getWidth();
        int getHeight();

        void free();

        int getPitch();
        int getPixelCount();

        Uint32 *getPixels();
        Uint32 getPixel(int x, int y);

        void executeShader(Uint32 (*func)(Uint32, double), double dT);

        void lockTexture();
        void unLockTexture();

        Uint32 color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

    private:
        SDL_Renderer* renderer;
        SDL_Window* window;

        int tWidth;
        int tHeight;

        Uint32 format;                      //window
        SDL_PixelFormat* mappingFormat;     //texture

        void* mPixels;
        int mPitch;

        SDL_Texture* mTexture;

};

STexture::STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow)
{   

    renderer = gRenderer;
    window = gWindow;

    mTexture = NULL;
    tWidth = 0;
    tHeight = 0;
    mPitch = 0;

    format = SDL_GetWindowPixelFormat(window);
    mappingFormat = SDL_AllocFormat(format);
}

STexture::~STexture()
{
    SDL_FreeFormat(mappingFormat);
    SDL_DestroyTexture(mTexture);
    mPixels = nullptr;
}

Uint32 STexture::color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
  return SDL_MapRGBA(mappingFormat, red, green, blue, 255);
}

void STexture::load(std::string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, format, 0);
    mTexture = SDL_CreateTexture(renderer, 
    format, 
    SDL_TEXTUREACCESS_STREAMING, 
    loadedSurface->w, 
    loadedSurface->h);

    tWidth = formattedSurface->w;
    tHeight = formattedSurface->h;
    mPitch = 0;

    lockTexture();

    memcpy(
        mPixels, 
        formattedSurface->pixels, 
        formattedSurface->pitch* formattedSurface->h
    );
    memcpy(mPixels, formattedSurface->pixels,(formattedSurface -> pitch* formattedSurface -> h));


    unLockTexture();


    SDL_FreeSurface(loadedSurface);
    SDL_FreeSurface(formattedSurface);
}

void STexture::lockTexture()
{
    SDL_LockTexture(mTexture, nullptr, &mPixels, &mPitch);
}
void STexture::unLockTexture()
{
    SDL_UnlockTexture(mTexture);
    mPixels = nullptr;
    mPitch = 0;
}

int STexture::getPitch()
{
    return mPitch;
}

int STexture::getWidth()
{
    return tWidth;
}

int STexture::getHeight()
{
    return tHeight;
}

void STexture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = { x, y, w, h };
    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void STexture::renderWithShader(int x, int y, int w, int h, Uint32 (*func)(Uint32))
{
    SDL_Rect renderQuad = {x, y, w, h};

    SDL_LockTexture(mTexture, nullptr, &mPixels, &mPitch);

    Uint32 *pixels = getPixels();
    SDL_Texture *copyTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, tWidth, tHeight);
    void *copyPixels;
    int copyPitch;
    SDL_LockTexture(copyTexture, NULL, &copyPixels, &copyPitch);
    Uint32 *copyPixelsInt = (Uint32 *)copyPixels;

    for (int i = 0; i < getPixelCount(); i++)
    {
        copyPixelsInt[i] = func(pixels[i]);
    }

    SDL_RenderCopy(renderer, copyTexture, nullptr, &renderQuad);

    SDL_UnlockTexture(copyTexture);
    SDL_DestroyTexture(copyTexture);
    unLockTexture();
}

int STexture::getPixelCount()
{
    return tWidth * tHeight;
}

Uint32 *STexture::getPixels()
{
    return (Uint32 *)mPixels;
}

Uint32 STexture::getPixel(int x, int y)
{
    lockTexture();
    Uint32 *pixels = getPixels();
    Uint32 pixel = pixels[y * tWidth + x];
    unLockTexture();
    return pixel;
}

void STexture::executeShader(Uint32 (*func)(Uint32, double), double dT)
{
    lockTexture();
    Uint32 *pixels = getPixels();

    for (int i = 0; i < getPixelCount(); i++)
    {
        pixels[i] = func(pixels[i], dT);
    }

    unLockTexture();
}

#endif