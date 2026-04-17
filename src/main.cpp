#include "raylib.h"
#include <vector>
#include <string>
// 控制台
const int window_width = 1440;
const int window_height = 900;
const int bubble_size_limit[2] = {40,100};
const int bubble_speed_limit[2] = {60,150};
const int bubble_spawn_rate = 35;
const int FPS = 90;
//


class bubble{
    private:
        Vector2 bubble_position; // vector2 = {x,y}
        float bubble_size;
        Color bubble_color;
        float bubble_speed;
        bool bubble_is_alive;
    public:
        bubble();
        ~bubble() = default;
        bool is_alive();
        bool is_clicked(float mouse_x, float mouse_y);
        void set_is_alive(bool is_alive);
        void draw();
        void update();
        
};

void bubble::set_is_alive(bool is_alive){
    bubble_is_alive = is_alive;
}
bubble::bubble(){
    bubble_size = GetRandomValue(bubble_size_limit[0],bubble_size_limit[1]);
    bubble_position.x = GetRandomValue(0,window_width);
    bubble_position.y = window_height+bubble_size;
    bubble_color = { // 4 bytes, {red,green,blue,transparency}  
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        255 
    };
    bubble_speed = GetRandomValue(bubble_speed_limit[0],bubble_speed_limit[1]); 
    bubble_is_alive = true;
}
void bubble::update(){ // 位置更新
    if(bubble_is_alive == true){
        float frame_time  = GetFrameTime();
        bubble_position.y -= bubble_speed * frame_time;
        if(bubble_position.y < -bubble_size){
            bubble_is_alive = false;
        }
    }
}
bool bubble::is_clicked(float mouse_x, float mouse_y){ // 是否點擊
    if(bubble_is_alive == false) return false;
    Vector2 mouse_position = {mouse_x,mouse_y}; 
    return CheckCollisionPointCircle(mouse_position,bubble_position,bubble_size);
}
void bubble::draw(){ // 畫面更新
    if(bubble_is_alive){
        DrawCircle(bubble_position.x,bubble_position.y,bubble_size,bubble_color);
    }
}


int main() {
    InitWindow(window_width, window_height, "Raylib OOP");
    SetTargetFPS(FPS);
    std::vector<bubble> bubble_list;
    int score = 0;
    while (!WindowShouldClose()) {
        // 生成ㄆㄆ
        if(GetRandomValue(1,bubble_spawn_rate) == 1){
            bubble new_bubble;
            bubble_list.push_back(new_bubble);
        }
        // 更新ㄆㄆ
        for(int i = 0; i < bubble_list.size();i++){
            bubble_list[i].update();
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(bubble_list[i].is_clicked(GetMouseX(),GetMouseY())){
                    bubble_list[i].set_is_alive(false);
                    score++;   
                }
            }
        }
        // 畫面構造
        BeginDrawing(); // Setup canvas (framebuffer) to start drawing
        ClearBackground(RAYWHITE); // Set background color (framebuffer clear color)
        
        for(int i = 0;i<bubble_list.size();i++){
            bubble_list[i].draw();
        }
        DrawText(TextFormat("Score: %d", score),20,20,40,BLACK); // text,x,y,size,color
        DrawText(TextFormat("FPS: %d",GetFPS()),20,60,20,BLACK);
        
        
        EndDrawing(); // End canvas drawing and swap buffers (double buffering)
    }

    CloseWindow();
    return 0;
}
