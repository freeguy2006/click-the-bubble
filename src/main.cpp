#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>
// 控制台
const int window_width = 1440;
const int window_height = 900;
const float bubble_size_limit[2] = {40,100};
const float bubble_speed_limit[2] = {20,300};
const float gravity = 0.1; // speed_y increasing per frame  
const float x_decline = 0.995; // speed_x decline per frame
const int bubble_spawn_rate = 60;
const int FPS = 90;
//


class bubble{
    private:
        Vector2 bubble_position; // vector2 = {x,y}
        float bubble_size;
        Color bubble_color;
        Vector2 bubble_speed;
        bool bubble_is_alive;
    public:
        bubble();
        ~bubble() = default;
        
        // getter
        float get_size();
        Vector2 get_position();
        Vector2 get_speed();
        // setter
        void set_speed(Vector2 speed);
        void set_position(Vector2 position);
        //
        bool is_alive();
        bool is_clicked(float mouse_x, float mouse_y);
        void set_is_alive(bool is_alive);
        void draw();
        void update();
        
};

bubble::bubble(){
    bubble_size = GetRandomValue(bubble_size_limit[0],bubble_size_limit[1]);
    bubble_position.x = GetRandomValue(0,window_width);
    bubble_position.y = window_height+bubble_size+300;
    bubble_color = { // 4 bytes, {red,green,blue,transparency}  
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        255 
    };
    bubble_speed = {0,-GetRandomValue(bubble_speed_limit[0],bubble_speed_limit[1])}; 
    bubble_is_alive = true;
}
// get
float bubble::get_size(){
    return bubble_size;
}
Vector2 bubble::get_position(){
    return bubble_position;
}
Vector2 bubble::get_speed(){
    return bubble_speed;
}


// set
void bubble::set_speed(Vector2 speed){
    bubble_speed.x = speed.x;
    bubble_speed.y = speed.y;
}
void bubble::set_position(Vector2 position){
    bubble_position.x = position.x;
    bubble_position.y = position.y;
}

void bubble::set_is_alive(bool alive){
    bubble_is_alive = alive;
}
//
void bubble::update(){ // 位置更新
    if(bubble_is_alive == true){
        float frame_time = GetFrameTime();

        if(bubble_position.x-bubble_size > 0 && bubble_position.x+bubble_size < window_width){
            bubble_position.x += bubble_speed.x * frame_time;
            bubble_speed.x *= x_decline;
        }else{
           bubble_position.x -= bubble_speed.x * frame_time;
           bubble_speed.x = -bubble_speed.x*x_decline;
        }
        bubble_position.y += bubble_speed.y * frame_time;
        bubble_speed.y-=gravity*frame_time;
        
        if(bubble_position.y < -bubble_size){
            bubble_is_alive = false;
        }
    }
}
bool bubble::is_alive(){
    return bubble_is_alive;
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
            if(bubble_list[i].is_alive() == false) continue;
            bubble_list[i].update();
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ // 滑鼠左鍵
                if(bubble_list[i].is_clicked(GetMouseX(),GetMouseY())){
                    bubble_list[i].set_is_alive(false);
                    score++;   
                }
            }
            for(int j = i+1;j<bubble_list.size();j++){
                float x_diff = bubble_list[i].get_position().x-bubble_list[j].get_position().x;
                float y_diff = bubble_list[i].get_position().y-bubble_list[j].get_position().y;
                float distance = sqrt(x_diff*x_diff + y_diff*y_diff);
                float the_two_size = bubble_list[i].get_size()+bubble_list[j].get_size();
                if(distance < the_two_size){
                    // 碰撞
                    float over_distance = the_two_size - distance;
                    
                    float nx = x_diff/distance; // 法向量 
                    float ny = y_diff/distance;
                    // 修正位置
                    Vector2 pos1 = bubble_list[i].get_position();
                    Vector2 pos2 = bubble_list[j].get_position();
                    
                    pos1.x += nx * over_distance/2;
                    pos1.y += ny * over_distance/2;
                    pos2.x -= nx * over_distance/2;
                    pos2.y -= ny * over_distance/2;

                    bubble_list[i].set_position(pos1);
                    bubble_list[j].set_position(pos2);
                    // obj 速度
                    Vector2 speed1 = bubble_list[i].get_speed();
                    Vector2 speed2 = bubble_list[j].get_speed();
                    float relative_speed_x = speed1.x - speed2.x;
                    float relative_speed_y = speed1.y - speed2.y;
                    float vel_along_normal = relative_speed_x * nx + relative_speed_y * ny;
                    if(vel_along_normal < 0){
                        float impulse = -1.5f * vel_along_normal; 
                        speed1.x += nx * impulse * 0.5;
                        speed1.y += ny * impulse * 0.5;
                        speed2.x -= nx * impulse * 0.5;
                        speed2.y -= ny * impulse * 0.5;
                        bubble_list[i].set_speed(speed1);
                        bubble_list[j].set_speed(speed2);

                    }
                    
                    
                    
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
