#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "stexture.cpp"


class Game{
    public:
        Game();
        ~Game();

        int startClock;
        int fps;

        __int64_t previous_time;
        __int64_t current_time;
        __int64_t delta_time;

        void init(const char* title, int width, int height);

        void setup();
        void handleEvents();
        void update(); 
        void render();
        void clean();
        bool running();


    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;

        SDL_Rect player1;
        SDL_Rect player2;

        int player_speed = 30;

        SDL_Rect ball;

        STexture* tex;
        STexture * texCopy;


        int window_width, window_height;

        float ball_x_speed = 0.0f; //0.4
        float ball_y_speed = 0.0f; //0.2


};
