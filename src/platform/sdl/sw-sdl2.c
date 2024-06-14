/* Copyright (c) 2013-2015 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "main.h"

#include <mgba/core/core.h>
#include <mgba/core/thread.h>
#include <mgba/core/version.h>
#include "amigaos.h"

#ifdef __AMIGAOS4__
extern char* SDL_FULL; //tooltype from amigaos.c
#else
char* SDL_FULL = "TRUE";
#endif

static bool mSDLSWInit(struct mSDLRenderer* renderer);
static void mSDLSWRunloop(struct mSDLRenderer* renderer, void* user);
static void mSDLSWDeinit(struct mSDLRenderer* renderer);

void mSDLSWCreate(struct mSDLRenderer* renderer) {
	renderer->init = mSDLSWInit;
	renderer->deinit = mSDLSWDeinit;
	renderer->runloop = mSDLSWRunloop;
}

bool mSDLSWInit(struct mSDLRenderer* renderer) {

	unsigned width, height;

#ifdef __AMIGAOS4__
        uint32_t * _outputBuffer;
#endif

	renderer->core->baseVideoSize(renderer->core, &width, &height);
       if (SDL_FULL)	renderer->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN_DESKTOP * renderer->player.fullscreen));
	 else renderer->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN * renderer->player.fullscreen));

#ifdef __AMIGAOS4__
	SDLAcaption(renderer->window,renderer->fname);
#endif
	SDL_GetWindowSize(renderer->window, &renderer->viewportWidth, &renderer->viewportHeight);
	renderer->player.window = renderer->window;
	renderer->sdlRenderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
       SDL_RenderSetIntegerScale(renderer->sdlRenderer, SDL_FALSE);
       SDL_RenderSetLogicalSize(renderer->sdlRenderer, renderer->viewportWidth, renderer->viewportHeight);
       SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
       SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE,"overscan");

#ifdef COLOR_16_BIT
#ifdef COLOR_5_6_5
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
#else
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
#else
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
    SDL_SetWindowMinimumSize(renderer->window, 160, 144);
    if (renderer->player.fullscreen) SDL_ShowCursor(SDL_FALSE);
        else SDL_ShowCursor(SDL_TRUE);

	int stride;
#ifndef __AMIGAOS4__
	SDL_LockTexture(renderer->sdlTex, 0, (void**) &renderer->outputBuffer, &stride);
	renderer->core->setVideoBuffer(renderer->core, renderer->outputBuffer, stride / BYTES_PER_PIXEL);
#else
	SDL_LockTexture(renderer->sdlTex, 0, (void**) &_outputBuffer, &stride);
	renderer->core->setVideoBuffer(renderer->core, _outputBuffer, stride / BYTES_PER_PIXEL);
#endif

	return true;
}

void mSDLSWRunloop(struct mSDLRenderer* renderer, void* user) {
	struct mCoreThread* context = user;
	SDL_Event event;
//markus test
#ifdef __AMIGAOS4__
        uint32_t * _outputBuffer;
	bool ARender=SDL_FALSE;

	unsigned width,height,g_width,g_height;
	SDL_GetRendererOutputSize(SDL_GetRenderer(renderer->window), &width, &height);

	SDL_Rect dstRect = {0, 0, width, height};
        SDL_Rect srcRect = {0, 0, 160, 144};

	renderer->core->desiredVideoDimensions(renderer->core, &g_width, &g_height);

if ((g_width==160)&&(g_height==144)) {
	SDL_SetWindowResizable(renderer->window,SDL_FALSE);
	ARender=SDL_TRUE;
} 
#endif

//main loop
	while (mCoreThreadIsActive(context)) {
		while (SDL_PollEvent(&event)) {
			mSDLHandleEvent(context, &renderer->player, &event);
		}
		if (mCoreSyncWaitFrameStart(&context->impl->sync)) {

			SDL_UnlockTexture(renderer->sdlTex);
#ifdef __AMIGAOS4__

if (ARender==SDL_TRUE) {
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, &srcRect, &dstRect);
} else {
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, 0, 0);
}
#else
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, 0, 0);
#endif
			SDL_RenderPresent(renderer->sdlRenderer);
			int stride;

#ifndef __AMIGAOS4__
			SDL_LockTexture(renderer->sdlTex, 0, (void**) &renderer->outputBuffer, &stride);
			renderer->core->setVideoBuffer(renderer->core, renderer->outputBuffer, stride / BYTES_PER_PIXEL);
#else
			SDL_LockTexture(renderer->sdlTex, 0, (void**) &_outputBuffer, &stride);
			renderer->core->setVideoBuffer(renderer->core, _outputBuffer, stride / BYTES_PER_PIXEL);
#endif
		}
		mCoreSyncWaitFrameEnd(&context->impl->sync);
	}

}

void mSDLSWDeinit(struct mSDLRenderer* renderer) {
#ifndef __AMIGAOS4__
	if (renderer->ratio > 1) {
		free(renderer->outputBuffer);
	}
#endif
}
