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

extern char* SDL_FULL; //tooltype from amigaos.c

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


//
//	 SDLcaption(renderer->fname, &wm_title2);
//         wm_title=wm_title2;
//printf ("zzz 3 \n");
//	renderer->core->desiredVideoDimensions(renderer->core, &width, &height);
	renderer->core->baseVideoSize(renderer->core, &width, &height);
//printf ("zzz 3 end\n");

//	width=160; 
//        height=144;


//	width=320; 
//        height=288;

//markus fixme
//	renderer->window = SDL_CreateWindow(projectName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_OPENGL | (SDL_WINDOW_FULLSCREEN_DESKTOP * renderer->player.fullscreen));
//	renderer->window = SDL_CreateWindow(projectName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, (SDL_WINDOW_FULLSCREEN * renderer->player.fullscreen));
//	renderer->window = SDL_CreateWindow("mGBA 0.10.3 (6.05.2024)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN * renderer->player.fullscreen));
//	renderer->window = SDL_CreateWindow(wm_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN * renderer->player.fullscreen));
       if (SDL_FULL)	renderer->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN_DESKTOP * renderer->player.fullscreen));
	 else renderer->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->viewportWidth, renderer->viewportHeight, SDL_WINDOW_RESIZABLE | (SDL_WINDOW_FULLSCREEN * renderer->player.fullscreen));

	SDLAcaption(renderer->window,renderer->fname);
	SDL_GetWindowSize(renderer->window, &renderer->viewportWidth, &renderer->viewportHeight);
	renderer->player.window = renderer->window;
	renderer->sdlRenderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//	renderer->sdlRenderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED);

//markus
//       SDL_RenderSetIntegerScale(renderer->sdlRenderer, SDL_TRUE);
       SDL_RenderSetIntegerScale(renderer->sdlRenderer, SDL_FALSE);
       SDL_RenderSetLogicalSize(renderer->sdlRenderer, renderer->viewportWidth, renderer->viewportHeight);
//      SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_NONE);
       SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
       SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE,"overscan");
//
#ifdef COLOR_16_BIT
#ifdef COLOR_5_6_5
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
#else
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
#else
	renderer->sdlTex = SDL_CreateTexture(renderer->sdlRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
//printf ("cretext w %u h %u \n",width, height);

//markus
//    SDL_SetWindowMinimumSize(renderer->window, renderer->viewportWidth, renderer->viewportHeight);
    SDL_SetWindowMinimumSize(renderer->window, 160, 144);
//    SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_NONE);

//    SDL_RenderClear(renderer->sdlRenderer);

//       SDL_ShowCursor(SDL_FALSE);
 if (renderer->player.fullscreen)
       SDL_ShowCursor(SDL_FALSE);
 else SDL_ShowCursor(SDL_TRUE);

	int stride;
	SDL_LockTexture(renderer->sdlTex, 0, (void**) &renderer->outputBuffer, &stride);
	renderer->core->setVideoBuffer(renderer->core, renderer->outputBuffer, stride / BYTES_PER_PIXEL);

	return true;
}

void mSDLSWRunloop(struct mSDLRenderer* renderer, void* user) {
	struct mCoreThread* context = user;
	SDL_Event event;
	bool ARender=SDL_FALSE;
//markus test
#ifdef __AMIGAOS4__

//	unsigned width = renderer->width * renderer->ratio;
//	unsigned height = renderer->height * renderer->ratio;
//	const struct GB* gb = renderer->core->board;
	unsigned width,height,g_width,g_height;

//	renderer->core->desiredVideoDimensions(renderer->core, &width, &height);

	SDL_GetRendererOutputSize(SDL_GetRenderer(renderer->window), &width, &height);

	SDL_Rect dstRect = {0, 0, width, height};
        SDL_Rect srcRect = {0, 0, 160, 144};

	renderer->core->desiredVideoDimensions(renderer->core, &g_width, &g_height);

//if (gb->model == 0x00) {};
if ((g_width==160)&&(g_height==144)) {
	SDL_SetWindowResizable(renderer->window,SDL_FALSE);
	ARender=SDL_TRUE;
} 

//			    *width = 160;
//			    *height = 144;


//        		    *width = 256;
//        		    *height = 224;
//	SDL_Rect dstRect = {0, 0, 256, 224};
//        SDL_Rect srcRect = {0, 0, 160, 144};

//        SDL_Rect srcRect = {0, 0, (160 / renderer->ratio), (144 / renderer->ratio)};

//	SDL_SetWindowResizable(renderer->window,SDL_FALSE);
//	SDL_SetWindowSize(renderer->window, width,height);
//	SDL_SetWindowSize(renderer->window, renderer->viewportWidth,renderer->viewportHeight);

//	SDL_Rect dstRect = {0, 0, width, height};
//        SDL_Rect srcRect = {0, 0, 160, 144};
//        SDL_Rect srcRect = {0, 0, 160, 144};
//        SDL_SetWindowMaximumSize(renderer->window, 160, 144);

//	SDL_Rect srcRect=dstRect;
#endif

//printf ("loop viewport  %u h %u \n",renderer->viewportWidth,renderer->viewportHeight);
//printf ("loop2 zwkle high  %u h %u \n",width,height);
//lol
//static const Uint32 min_frame_time = 16;
//

//main loop
	while (mCoreThreadIsActive(context)) {
//SDL_Delay(5);

		while (SDL_PollEvent(&event)) {
			mSDLHandleEvent(context, &renderer->player, &event);
		}
//
//
		if (mCoreSyncWaitFrameStart(&context->impl->sync)) {
//lol
//Uint32 time_delta = SDL_GetTicks() - start_time;
//auto fps_sum += 1000.0 / (float)time_delta;
//auto fps_count++;

//printf ("s %d  d %d,\n", start_time, time_delta);
//if(time_delta < min_frame_time) printf ("dupa \n");
//if(fps_count >= fps_max_count)
//{
//    printf("FPS: %f\n", fps_sum / (float)fps_count);
//    fps_count = 0;
//    fps_sum = 0.0;
//}
//printf ("f %d \n",fps/10000);

			SDL_UnlockTexture(renderer->sdlTex);
#ifdef __AMIGAOS4__
//		    if ((width < 256) && (height < 224))

if (ARender==SDL_TRUE) {
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, &srcRect, &dstRect);
} else {
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, 0, 0);
}
//			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, NULL, &dstRect);

//       if(!repaint) {
//            return;
//        }
//        repaint = false;
//        std::cout<<"Repaint "<<count++<<std::endl;

//        SDL_Surface* surf = SDL_GetWindowSurface(renderer->window);
//        int w, h;
//        SDL_GetWindowSize(renderer->window, &w, &h);
//        SDL_Rect rect = {0, 0, w, h};
//        SDL_FillRect(surf, &rect, 0xff0000ff);
//        SDL_UpdateWindowSurface(renderer->window);
#else
			SDL_RenderCopy(renderer->sdlRenderer, renderer->sdlTex, 0, 0);
#endif
			SDL_RenderPresent(renderer->sdlRenderer);
			int stride;
			SDL_LockTexture(renderer->sdlTex, 0, (void**) &renderer->outputBuffer, &stride);
			renderer->core->setVideoBuffer(renderer->core, renderer->outputBuffer, stride / BYTES_PER_PIXEL);
//SDL_Delay(1);
		}
		mCoreSyncWaitFrameEnd(&context->impl->sync);
	}
}

void mSDLSWDeinit(struct mSDLRenderer* renderer) {
	if (renderer->ratio > 1) {
		free(renderer->outputBuffer);
	}
}
