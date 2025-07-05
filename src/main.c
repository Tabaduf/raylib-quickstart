#include "raylib.h"
#include "resource_dir.h" 

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 20
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)

typedef struct {
	int x, y;
} Vector2Int;

typedef struct {
	Vector2Int position;
	Vector2Int direction;
	int length;
	Vector2Int body[GRID_WIDTH * GRID_HEIGHT];
} Snake;

typedef struct {
	Vector2Int position;
} Food;

void InitSnake(Snake *snake) {
	snake->position = (Vector2Int){GRID_WIDTH / 2, GRID_HEIGHT / 2};
	snake->direction = (Vector2Int){1, 0};
	snake->length = 1;
	snake->body[0] = snake->position;
}

void InitFood(Food *food) {
	food->position = (Vector2Int){GetRandomValue(0, GRID_WIDTH - 1), GetRandomValue(0, GRID_HEIGHT - 1)};
}

void UpdateSnake(Snake *snake) {
	for (int i = snake->length - 1; i > 0; i--) {
		snake->body[i] = snake->body[i - 1];
	}
	snake->position.x += snake->direction.x;
	snake->position.y += snake->direction.y;
	snake->body[0] = snake->position;
}

bool CheckCollisionWithFood(Snake *snake, Food *food) {
	return snake->position.x == food->position.x && snake->position.y == food->position.y;
}

bool CheckCollisionWithSelf(Snake *snake) {
	for (int i = 1; i < snake->length; i++) {
		if (snake->position.x == snake->body[i].x && snake->position.y == snake->body[i].y) {
			return true;
		}
	}
	return false;
}

bool CheckCollisionWithWalls(Snake *snake) {
	return snake->position.x < 0 || snake->position.y < 0 || snake->position.x >= GRID_WIDTH || snake->position.y >= GRID_HEIGHT;
}

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
	SetTargetFPS(10);

	Snake snake;
	Food food;

	InitSnake(&snake);
	InitFood(&food);

	bool gameOver = false;

	while (!WindowShouldClose()) {
		if (!gameOver) {
			// Handle input
			if (IsKeyPressed(KEY_UP) && snake.direction.y == 0) snake.direction = (Vector2Int){0, -1};
			if (IsKeyPressed(KEY_DOWN) && snake.direction.y == 0) snake.direction = (Vector2Int){0, 1};
			if (IsKeyPressed(KEY_LEFT) && snake.direction.x == 0) snake.direction = (Vector2Int){-1, 0};
			if (IsKeyPressed(KEY_RIGHT) && snake.direction.x == 0) snake.direction = (Vector2Int){1, 0};

			// Update game state
			UpdateSnake(&snake);

			if (CheckCollisionWithFood(&snake, &food)) {
				snake.length++;
				InitFood(&food);
			}

			if (CheckCollisionWithSelf(&snake) || CheckCollisionWithWalls(&snake)) {
				gameOver = true;
			}
		}

		// Draw
		BeginDrawing();
		ClearBackground(BLACK);

		if (!gameOver) {
			// Draw snake
			for (int i = 0; i < snake.length; i++) {
				DrawRectangle(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, GREEN);
			}

			// Draw food
			DrawRectangle(food.position.x * GRID_SIZE, food.position.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
		} else {
			DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 20, 40, RED);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}