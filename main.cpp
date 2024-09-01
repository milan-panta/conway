// conway/main.cpp
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 10;
const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

void initSDL() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void closeSDL() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int alive_neighbors(int i, int j, const std::vector<std::vector<int>> &grid) {
  int ans = 0;
  for (int di = -1; di <= 1; di++) {
    for (int dj = -1; dj <= 1; dj++) {
      if (di == 0 && dj == 0)
        continue;
      int ni = (i + di + GRID_HEIGHT) % GRID_HEIGHT;
      int nj = (j + dj + GRID_WIDTH) % GRID_WIDTH;
      if (grid[ni][nj])
        ans++;
    }
  }
  return ans;
}

void run_one_cycle(std::vector<std::vector<int>> &grid,
                   std::vector<std::vector<int>> &new_grid) {
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      int nbrs = alive_neighbors(i, j, grid);
      if (grid[i][j] == 1) {
        new_grid[i][j] = (nbrs == 2 || nbrs == 3) ? 1 : 0;
      } else {
        new_grid[i][j] = (nbrs == 3) ? 1 : 0;
      }
    }
  }
  grid = new_grid;
}

void render(const std::vector<std::vector<int>> &grid) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      if (grid[i][j]) {
        SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

int main() {
  srand(time(0));
  std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH));
  std::vector<std::vector<int>> new_grid = grid;

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      grid[i][j] = (rand() % 100 < 15) ? 1 : 0;
    }
  }

  initSDL();

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    run_one_cycle(grid, new_grid);
    render(grid);
    SDL_Delay(100);
  }

  closeSDL();
  return 0;
}
