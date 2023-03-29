#ifndef ABRIRVENTANA_H
#define ABRIRVENTANA_H
//#define SDL_MAIN_HADLED
#define SDL_MAIN_HANDLED
#include <SDL.h>  //si no se incluye es por que hay que ver si el proyecto esta en 64bits y el release tambien

void ventanuco() {
     
        // Inicializar SDL
        SDL_Init(SDL_INIT_VIDEO);

        // Crear una ventana de 640x480 píxeles
        SDL_Window* window = SDL_CreateWindow("Mi ventana", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

        // Crear un renderer para dibujar en la ventana
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

        // Pintar la ventana de color rojo
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Actualizar la ventana para que se muestre en pantalla
        SDL_RenderPresent(renderer);

        // Esperar 3 segundos antes de cerrar la ventana
        SDL_Delay(3000);

        // Liberar los recursos de SDL
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }





#endif
