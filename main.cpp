#include <SDL.h>
#include <cstdio>
#include <cstring>

int coincidence(int mat[3][3]);

SDL_Texture* SDL_LoadTexture(SDL_Renderer* renderer, const char* file)
{
    SDL_Surface* surface = SDL_LoadBMP(file);
    if (!surface) return 0;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

#define SDL_WIN_CENTERXY SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
    SDL_Window* window = SDL_CreateWindow("Window", SDL_WIN_CENTERXY, 600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture[3];
    texture[0] = SDL_LoadTexture(renderer, "background.bmp");
    texture[1] = SDL_LoadTexture(renderer, "1.bmp");
    texture[2] = SDL_LoadTexture(renderer, "2.bmp");

    SDL_RenderCopy(renderer, texture[0], NULL, NULL); // копируем текстуру в рендер
    SDL_RenderPresent(renderer); // обнавляем

    SDL_Rect rect;
    int count = 0, mat[3][3], team = 1; // создаем матрицу 3 на 3 и команду

    // обнуляем всю матрицу
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            mat[i][j] = 0;

    SDL_Event event;
    while (true)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) break;
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.clicks == 1) // только один клик
                {
                    if (event.button.button == SDL_BUTTON_LEFT) // левый клик
                    {
                        rect.x = (int)(event.button.x / 200); // 200 = 600 / 3
                        rect.y = (int)(event.button.y / 200);

                        if (mat[rect.x][rect.y] == 0) // если ячейка пуста
                        {
                            team = (team != 2) ? 2 : 1; // переключатель команды
                            mat[rect.x][rect.y] = team; // где крестик где нолик
                            rect.x = rect.x * 200;
                            rect.y = rect.y * 200;
                            rect.w = rect.h = 200;

                            SDL_SetTextureBlendMode(texture[team], SDL_BLENDMODE_MOD);
                            SDL_RenderCopy(renderer, texture[team], NULL, &rect);
                            SDL_RenderPresent(renderer);

                            if (count++ == 8 || coincidence(mat) > 0)
                            {
                                count = 0;
                                SDL_RenderClear(renderer);
                                SDL_RenderCopy(renderer, texture[0], NULL, NULL);
                                SDL_RenderPresent(renderer);

                                for (int i = 0; i < 3; i++)
                                    for (int j = 0; j < 3; j++)
                                        mat[i][j] = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < 3; i++)
        SDL_DestroyTexture(texture[i]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

int coincidence(int mat[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        if ((mat[0][i] == 2 && mat[1][i] == 2 && mat[2][i] == 2)
            || (mat[i][0] == 2 && mat[i][1] == 2 && mat[i][2] == 2)) return 2;

        if ((mat[0][i] == 1 && mat[1][i] == 1 && mat[2][i] == 1)
            || (mat[i][0] == 1 && mat[i][1] == 1 && mat[i][2] == 1)) return 1;
    }
    if ((mat[0][0] == 2 && mat[1][1] == 2 && mat[2][2] == 2)
        || (mat[2][0] == 2 && mat[1][1] == 2 && mat[0][2] == 2)) return 2;

    if ((mat[0][0] == 1 && mat[1][1] == 1 && mat[2][2] == 1)
        || (mat[2][0] == 1 && mat[1][1] == 1 && mat[0][2] == 1)) return 1;

    return 0;
}