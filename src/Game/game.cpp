#include <iostream>
#include "game.h"
#include <string>
#include <SDL2/SDL_image.h>
#include "stexture.cpp"

Game::Game ()
{
    
    
}

Game::~Game ()
{

}

void Game::init(const char* title, int width, int height)
{
    std::cout << "Game Setting Up..." << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0,0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1,0);
    isRunning = true;
}



void Game::setup()
{
    std::cout << "Game Setting Up..." << std::endl;

    
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    ball.w = 20;
    ball.h = 20;
    ball.x = 50;
    ball.y = 250;

    player1.w = 30;
    player1.h = window_height / 4;
    player1.x = 0;
    player1.y = 250;
    
    player2.w = 30;
    player2.h = window_height / 4;
    player2.x = window_width - player2.w;
    player2.y = 250;

    tex = new STexture(renderer, window);
    tex->load("./assets/1.png");

    texCopy = new STexture(renderer, window);
    texCopy->load("./assets/1.png");
    
}

void Game::handleEvents()
{
    std::cout << "Game Handling Event..." << std::endl;

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            isRunning = false;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                //Player 1
                case SDLK_w:
                    if(player1.y >= 0){
                        player1.y -= player_speed;
                    }
                    
                break;
                case SDLK_s:
                    if(player1.y <= (window_height - player1.h)){
                        player1.y += player_speed;
                    }
                    
                break;

                //Player 2
                case SDLK_UP:
                    if(player2.y >= 0){
                        player2.y -= player_speed;
                    }
                    
                break;
                case SDLK_DOWN:
                    if(player2.y <= (window_height - player2.h)){
                        player2.y += player_speed;
                    }
                    
                break;

                
            }
        }
    }
}

void Game::update()
{
    std::cout << "Game Updating..." << std::endl;

    ball.y += ball_y_speed * delta_time;
    if((ball.y > (window_height - ball.h)) || (ball.y < 0)){
        ball_y_speed *= -1.1f;
    }

    ball.x += ball_x_speed * delta_time;
    if(ball_x_speed < 0)
    {
        if((ball.y + ball.h >= player1.y) && (ball.y <= player1.y + player1.h))
        {
            if(ball.x <= 0 + player1.w)
            {
                ball_x_speed *= -1.1f;
            }
        }
        if(ball.x <= 0)
        {
            std::cout << "Player 2 Won" << std::endl;
            isRunning = false;
        }

    }
    
    else if(ball_x_speed > 0)
    {
        if((ball.y + ball.h >= player2.y) && (ball.y <= player2.y + player2.h))
        {
            if(ball.x >= (window_width - ball.w - player2.w))
            {
                ball_x_speed *= -1.1f;
            }
        }
        if(ball.x >= window_width - ball.w)
        {
            std::cout << "Player 1 Won" << std::endl;
            isRunning = false;
        }
    }
}

Uint32 fragment(Uint32 currentColor)
{
    //12001808 = #b72210, color de pañuelo
    if (currentColor == 12001808) {
        currentColor = 12366612; //#bcb314 dorado
        return currentColor;
    }
}

void Game::render()
{
    std::cout << "Game Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    tex->render(250, 100, 300, 300);
    texCopy->renderWithShader(550, 100, 300, 300, fragment);
    

    SDL_SetRenderDrawColor(renderer, 255,255,255,0);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);

    SDL_RenderPresent(renderer);

    /*
    SDL_Surface* surface = IMG_Load("./assets/1.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &ball);
    SDL_DestroyTexture(texture);
    */

    //SDL_RenderFillRect(renderer, &ball);

    


}

void Game::clean()
{
    std::cout << "Game Cleaning Up..." << std::endl;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Over..." << std::endl;
}

bool Game::running()
{
    return isRunning;
}