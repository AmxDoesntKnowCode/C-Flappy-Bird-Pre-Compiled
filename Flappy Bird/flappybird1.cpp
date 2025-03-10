#include "src/include/SDL2/SDL.h"
#include "src/include/SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include <random>
using namespace std; 

bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return (rect1.x < rect2.x + rect2.w &&
            rect1.x + rect1.w > rect2.x &&
            rect1.y < rect2.y + rect2.h &&
            rect1.y + rect1.h > rect2.y);
}


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    
    SDL_Window* window = SDL_CreateWindow("FlappyBird-UserEdition-PipeTexture", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Surface* BGsurface = IMG_Load("assets\\background.png");
    SDL_Texture* BGtexture = SDL_CreateTextureFromSurface(renderer, BGsurface);
    SDL_FreeSurface(BGsurface);

    SDL_Surface* bird = IMG_Load("assets\\bird.png");
    SDL_Texture* birdTexture = SDL_CreateTextureFromSurface(renderer, bird);
    SDL_FreeSurface(bird);

    SDL_Surface* pipe = IMG_Load("assets\\pipe.png");
    SDL_Texture* pipeT = SDL_CreateTextureFromSurface(renderer, pipe);
    SDL_FreeSurface(pipe);

    SDL_Surface* pipeI = IMG_Load("assets\\pipe_inverted.png");
    SDL_Texture* pipeTI = SDL_CreateTextureFromSurface(renderer, pipeI);
    SDL_FreeSurface(pipeI);


    // making a random number 
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(1, 10); 

        int randomNumber; 

     //     X Position, Y Position, Width, Height           

    SDL_Rect player {100, 150, 40, 40};

    // Top and Bottom pipes 
    SDL_Rect TopPipe {400, 0, 40, 200};
    SDL_Rect BotPipe {400, 300 ,40 ,200}; 
  
    SDL_Rect groundObst {0, 470, 650, 10}; // Floor 
    SDL_Rect roofObst {0, 0, 650, 10}; // Roof

    float gravity = 0.7f;    
    float flapStrength = -10.0f; 
    float velocityY = 0.0f;      

    bool ShowHB = false; // Show Bird Hitbox
    bool ShowPHB = false; // Show Pipes Hitboxes

    int ObstSpeed = 6; // Pipes Speed

    bool moving = true; 
    SDL_Event e;

    // this tracks whether ur moving or not 
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    while (moving) {
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) {
                moving = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            
            if (e.button.button == SDL_BUTTON_LEFT) { // Left Click 
            velocityY = flapStrength; // applies The flapstrengh so the bird Jumps
        }
    }
            if (e.type == SDL_KEYDOWN) {  
                if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    velocityY = flapStrength;
                }
               
               if(e.key.keysym.scancode == SDL_SCANCODE_P){
                ShowHB = !ShowHB; 
               }
               if(e.key.keysym.scancode == SDL_SCANCODE_L){
                ShowPHB = !ShowPHB; 
               }
         }

        }

       
        velocityY += gravity;
        player.y += static_cast<int>(velocityY);

        // Q for Quitting cuz why not 
        if(currentKeyStates[SDL_SCANCODE_Q]) { 
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        
            SDL_Quit();
            return 0;
        }

     
        // moves the pipes 
        TopPipe.x -= ObstSpeed; 

        BotPipe.x -= ObstSpeed; 

        // pipe loop. 

        // it works by chekcing if the box is outside of the window.
        // if it is outside, it will reset its x pos to 640 which basically puts it into a loop :D 
        
        if (TopPipe.x < -TopPipe.w) {      

            TopPipe.x = 640;
            TopPipe.y = 0;
            TopPipe.h = 200; 

            BotPipe.x = 640;
            BotPipe.y = 300;
            BotPipe.h = 200;

            randomNumber = distrib(gen); //generate a random value when the obst loop resets 
        }
                // Top Pipe Collision
        if (checkCollision(player, TopPipe)) {
            int overlapX = min(player.x + player.w, TopPipe.x + TopPipe.w) - max(player.x, TopPipe.x);
            int overlapY = min(player.y + player.h, TopPipe.y + TopPipe.h) - max(player.y, TopPipe.y);

            if (overlapX < overlapY) {
                if (player.x < TopPipe.x) {
                     player.x -= overlapX;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0;
                } else {
                     player.x += overlapX;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0;
                }
            } else {
                if (player.y < TopPipe.y) {
                     player.y -= overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0;
                } else {
                    player.y += overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            }
        }
        

            // Bottom Pipe Collision
        if (checkCollision(player, BotPipe)) {
            int overlapX = min(player.x + player.w, BotPipe.x + BotPipe.w) - max(player.x, BotPipe.x);
            int overlapY = min(player.y + player.h, BotPipe.y + BotPipe.h) - max(player.y, BotPipe.y);

            if (overlapX < overlapY) {
                if (player.x < BotPipe.x) {
                     player.x -= overlapX;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                } else {
                     player.x += overlapX;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            } else {
                if (player.y < BotPipe.y) {
                    player.y -= overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                } else {
                     player.y += overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            }
        }
                        // Ground Collision
        if (checkCollision(player, groundObst)) {
            int overlapX = min(player.x + player.w, groundObst.x + groundObst.w) - max(player.x, groundObst.x);
            int overlapY = min(player.y + player.h, groundObst.y + groundObst.h) - max(player.y, groundObst.y);

            if (overlapX < overlapY) {
                if (player.x < groundObst.x) {
                     player.x -= overlapX;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                } else {
                     player.x += overlapX;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            } else {
                if (player.y < groundObst.y) {
                     player.y -= overlapY;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                } else {
                     player.y += overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            }
        }
                    // Roof Collision
        if (checkCollision(player, roofObst)) {
            int overlapX = min(player.x + player.w, roofObst.x + roofObst.w) - max(player.x, roofObst.x);
            int overlapY = min(player.y + player.h, roofObst.y + roofObst.h) - max(player.y, roofObst.y);

            if (overlapX < overlapY) {
                if (player.x < roofObst.x) {
                     player.x -= overlapX;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                    } else {
                     player.x += overlapX;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                    }
                } else {
                if (player.y < roofObst.y) {
                     player.y -= overlapY;
                     ObstSpeed = 0; 
                     flapStrength = 0.0f;
                     velocityY = 0; 
                } else {
                     player.y += overlapY;
                     ObstSpeed = 0;  
                     flapStrength = 0.0f;
                     velocityY = 0; 
                }
            }
        }

        
        


         
    

    // this mini algorithm randomizes the h values of the obstacles and yes i know its shit but idc 
        if(randomNumber == 7){
            TopPipe.h = 250;
            TopPipe.y = 0;  
            BotPipe.h = 230; 
            BotPipe.y = 350;
        }
        if(randomNumber == 6){
            TopPipe.h = 250; 
            TopPipe.y = 0; 
            BotPipe.h = 250; 
            BotPipe.y = 390;
        }
        if(randomNumber == 10){
            TopPipe.h = 190;
            TopPipe.y = -80;  
            BotPipe.h = 300; 
            BotPipe.y = 190;
        }
        if(randomNumber == 3){
            TopPipe.h = 190;
            TopPipe.y = -90;  
            BotPipe.h = 280; 
            BotPipe.y = 200;
        }
        if(randomNumber == 1){
            TopPipe.h = 190; 
            TopPipe.y = -110;
            BotPipe.h = 290; 
            BotPipe.y = 180;
        }
        if(randomNumber == 4){
            TopPipe.h = 190;
            TopPipe.y = 0; 
            BotPipe.h = 300; 
            BotPipe.y = 300;
        }
        if(randomNumber == 8){
            TopPipe.h = 190; 
            TopPipe.y = -50; 
            BotPipe.h = 230; 
            BotPipe.y = 250;
        }
        if(randomNumber == 9){
            TopPipe.h = 190; 
            TopPipe.y = -140; 
            BotPipe.h = 330; 
            BotPipe.y = 140;
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // this enables  the transparency thinggy 
      
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, BGtexture, nullptr, nullptr);

        if(ShowHB){
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 140);
            SDL_RenderFillRect(renderer, &player);
        } 
        if(ShowPHB){ 
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 140);
            SDL_RenderFillRect(renderer, &TopPipe);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 140);
            SDL_RenderFillRect(renderer, &BotPipe);
        }

         SDL_RenderCopy(renderer, birdTexture, nullptr, &player); // applies the bird texture 

         SDL_RenderCopy(renderer, pipeTI, nullptr, &TopPipe); // also applies the top pipe texture 
         SDL_RenderCopy(renderer, pipeT, nullptr, &BotPipe); // bottom pipe texture

        SDL_SetRenderDrawColor(renderer, 210, 203, 141, 255); // this is the floor
        SDL_RenderFillRect(renderer, &groundObst); // yes this is also the floor 

        SDL_SetRenderDrawColor(renderer, 210, 203, 141, 255);
        SDL_RenderFillRect(renderer, &roofObst);

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit(); 

    return 0; 
}

/* 

    
***************************************************************||-IDEAS-||****************************************************************\\
 ---------------------------------------------------------------------------------------------------------------------------------------- \\
                                                                                                                                          \\  
  |1|  soo might make some sort of gravity although im too stupid to handle gravity and velocity and this other shit.   // DONE           \\
                                                                                                                                          \\
  |2|  so what im planning to do IS.. maybe install SDL.image and maybe try to make sprite sheets. i can get a map overlay and add zum    \\
       of that SWEET SWEET collision. i aint sure ive got school tmr and I. WANNA. WORK. ON. SDL!!........ :D    // DONE                  \\
                                                                                                                                          \\       
******************************************************************************************************************************************\\
*/