#include "../include/user_interface.hpp"
#include "../dependency/imgui/imgui.h"
#include "../dependency/imgui/imgui_impl_sdl3.h"
#include "../dependency/imgui/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <vector>

void User_interface::init(SDL_Window *window,int w, int h){
    this->window = window;
    width = w;
    height = h;

    renderer = SDL_CreateRenderer(window,nullptr);
    if(renderer==nullptr) std::cout<<"sdl_renderer not created for imgui"<<std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();


    ImGui_ImplSDL3_InitForSDLRenderer(window,renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,width,height);
}
Input_state User_interface::get_input_state(){
    return state;
}
void User_interface::handle_input(){
    const bool* keys = SDL_GetKeyboardState(nullptr);

    if(keys[SDL_SCANCODE_W]) state.key_w = true;
    if(keys[SDL_SCANCODE_A]) state.key_a = true;
    if(keys[SDL_SCANCODE_S]) state.key_s = true;
    if(keys[SDL_SCANCODE_D]) state.key_d = true;
    if(keys[SDL_SCANCODE_SPACE]) state.key_space = true;
    if(keys[SDL_SCANCODE_LSHIFT]) state.key_shift = true;
    if(keys[SDL_SCANCODE_TAB]) state.key_tab = true;


    uint32_t buttons = SDL_GetMouseState(&state.mousex,&state.mousey);
    if(buttons & SDL_BUTTON_LMASK) state.mouse_left_key = true;
    if(buttons & SDL_BUTTON_RMASK) state.mouse_left_key = true;

    SDL_GetRelativeMouseState(&state.rel_mousex,&state.rel_mousey);
}
void User_interface::run(const std::vector<uint32_t> &framebuffer){
    handle_input();

    SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t));

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("hello, world!");
    ImGui::Text("this is a brare minimum ImGui + SDL3 setup.");
    static float color[4] = {0.1f,0.2f,0.3f,1.0f};
    ImGui::ColorEdit4("background Color",color);

    ImTextureID texture_id = (ImTextureID)(intptr_t)texture;

    ImGui::Image(texture_id, ImVec2(width, height));

    ImGui::End();

    ImGui::Render();

    SDL_SetRenderDrawColor(renderer,
              (Uint8)(color[0] * 255),
              (Uint8)(color[1] * 255),
              (Uint8)(color[2] * 255),
              (Uint8)(color[3] * 255));
          SDL_RenderClear(renderer);

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),renderer);
    SDL_RenderPresent(renderer);
}
void User_interface::close(){
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}
