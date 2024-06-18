/*
** smarkusg: part of the code comes from mplayer 1.5 AOS
** License: GPL v2 or later
*/
#ifdef __AMIGAOS4__
#include <exec/types.h>
#include <SDL2/SDL.h>

#define AMIGA_VERSION "mGBA"
#define AMIGA_VERSION_SIGN "mGBA 0.10.3 clib4 (18.06.2024)"

int AmigaOS_Open(int argc, char *argv[]); // returns -1 if a problem
void AmigaOS_Close(void);
void AmigaOS_ParseArg(int argc, char *argv[], int *new_argc, char ***new_argv);
void VARARGS68K EasyRequester(CONST_STRPTR text, CONST_STRPTR button, ...);
void SDLAcaption(SDL_Window *window, char *filename);
void set_SDL_FULL ();
#endif //AOS4