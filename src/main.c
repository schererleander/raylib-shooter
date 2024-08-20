#include <raylib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_BULLET 100
#define MAX_ENEMY 100

typedef struct {
  Vector2 position;
  Vector2 velocity;
  bool alive;
} Bullet;

typedef struct {
  Vector2 position;
  Vector2 movement;
} Enemy;

void updatePlayerMovement(Vector2* position, Vector2* movement, float speed, int playerWidth, int playerHeight) {
    movement->x = 0.0f;
    movement->y = 0.0f;

    if (IsKeyDown(KEY_A))
        movement->x -= 1.0f;
    if (IsKeyDown(KEY_D))
        movement->x += 1.0f;
    if (IsKeyDown(KEY_W))
        movement->y -= 1.0f;
    if (IsKeyDown(KEY_S))
        movement->y += 1.0f;

    // Normalize the movement Vector
    float magnitude = sqrt(movement->x * movement->x + movement->y * movement->y);
    if (magnitude > 0)
    {
        movement->x /= magnitude;
        movement->y /= magnitude;
    }

    //Keepl player in player area
    if(position->x + (float)playerWidth >= GetScreenWidth())
    {
      position->x = GetScreenWidth()-(float)playerWidth;
    }
    if(position->x <= 0)
    {
      position->x = 0;
    }
    if(position->y + (float)playerWidth >= GetScreenHeight())
    {
      position->y = GetScreenHeight()-(float)playerHeight;
    }
    if(position->y <= 0)
    {
      position->y = 0;
    }

    // Update player position based on movement
    float deltaTime = GetFrameTime();
    position->x += movement->x * speed * deltaTime;
    position->y += movement->y * speed * deltaTime;

}

void updateBullet(Bullet bullets[]) {
  for(int i = 0; i < MAX_BULLET; i++)
  {
    bullets[i].position.x += bullets[i].velocity.x;
    bullets[i].position.y += bullets[i].velocity.y;

    int margin = 10;

    if(bullets[i].position.x > GetScreenWidth() + margin)
    {
      bullets[i].alive = false;
    }
    if(bullets[i].position.x < 0 - margin)
    {
      bullets[i].alive = false;
    }
    if(bullets[i].position.y > GetScreenHeight() + margin)
    {
      bullets[i].alive = false;
    }
    if(bullets[i].position.y < 0 - margin)
    {
      bullets[i].alive = false;
    }
  }
}

void moveEnemy(Enemy *enemies, Vector2 playerPosition) {

}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Shooter");

    Vector2 playerPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    Vector2 playerMovementVec = { 0.0f, 0.0f };
    int playerHeight = 20;
    int playerWidth = 20;
    float playerSpeed = 200.0f;
    int playerHP = 100;

    int currentBullet = 0;
    int bulletSpeed = 5;
    Bullet bullets[MAX_BULLET] = {0};

    int currentEnemy = 0;
    int numEnemyAlive = 0;
    Enemy enemies[MAX_ENEMY] = {0};

    SetTargetFPS(240);

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        updatePlayerMovement(&playerPosition, &playerMovementVec, playerSpeed, playerWidth, playerHeight);


        Vector2 mousePosition = GetMousePosition();
        float rotationAngle = atan2f(mousePosition.y - (playerPosition.y + (float)playerHeight/2), mousePosition.x - (playerPosition.x + (float)playerWidth/2)) * RAD2DEG;

        int length = 50;
        Vector2 endPoint = { playerPosition.x + (float)playerWidth/2 + length * cosf(rotationAngle * DEG2RAD), playerPosition.y + (float)playerHeight/2 + length * sinf(rotationAngle * DEG2RAD) };

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          Vector2 direction = {mousePosition.x - (playerPosition.x + (float)playerWidth/2), mousePosition.y - (playerPosition.y + (float)playerHeight/2)};
          float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
          if (length != 0) {
            direction.x /= length;
            direction.y /= length;
          }

          Vector2 bulletVelocity = {direction.x * bulletSpeed, direction.y * bulletSpeed};
          bullets[currentBullet].position = endPoint;
          bullets[currentBullet].velocity = bulletVelocity;
          bullets[currentBullet].alive = true;
          currentBullet = (currentBullet + 1) % MAX_BULLET;
        }

        updateBullet(bullets);


        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            //Draw background lines
            for(int i=0; i < GetScreenWidth(); i += 50) {
              DrawLineEx((Vector2){i, 0}, (Vector2){i, GetScreenHeight()}, 1, BLACK);
            }

            for(int i=0; i < GetScreenHeight(); i += 50) {
              DrawLineEx((Vector2){0, i}, (Vector2){GetScreenWidth(), i}, 1, BLACK);
            }

            //Draw player and "weapon"
            DrawLineEx((Vector2){playerPosition.x+(float)playerWidth/2, playerPosition.y+(float)playerHeight/2}, endPoint, 5, RED);
            DrawRectangle(playerPosition.x, playerPosition.y, playerWidth, playerHeight, GREEN);

            //Draw debug info
            DrawText(TextFormat("Current FPS: %i", (int)(1.0f / GetFrameTime())), 20, 20, 10, GREEN);
            DrawText(TextFormat("Input: (%.2f, %.2f)", playerMovementVec.x, playerMovementVec.y), 20, 30, 10, RED);
            DrawText(TextFormat("Player Position: (%.0f, %.0f)", playerPosition.x, playerPosition.y), 20, 40, 10, RED);
            DrawText(TextFormat("Bullet Count: %i", currentBullet), 20, 50, 10, GREEN);

            DrawText(TextFormat("HP: %i", playerHP), 20, 60, 10, RED);

            //Draw bullets
            for(int i=0; i < MAX_BULLET; i++) {
              DrawCircleV(bullets[i].position, 2, RED);
            }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return 0;
}

