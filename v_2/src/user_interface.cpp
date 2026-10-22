#include "../include/user_interface.hpp"
#include "../dependency/imgui/imgui.h"
#include "../dependency/imgui/imgui_impl_sdl3.h"
#include "../dependency/imgui/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <vector>

void User_interface::init(SDL_Window *window, int w, int h,Input_state *i_state,Render_state *r_state,System_state *s_state) {
  this->window = window;
  width = w;
  height = h;
  input_state = i_state;
  render_state = r_state;
  system_state = s_state;

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer)
    std::cout << "sdl_renderer not created for imgui" << std::endl;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, width, height);
}

void User_interface::handle_input() {

    while(SDL_PollEvent(&events)){
        ImGui_ImplSDL3_ProcessEvent(&events);
        switch(events.type){
            case SDL_EVENT_QUIT :
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED :
                system_state->running = false;
                break;
            // case SDL_EVENT_MOUSE_BUTTON_DOWN :
            //     if(!input_state->mouse_lock){
            //         SDL_SetWindowRelativeMouseMode(window,true);
            //         input_state->mouse_lock = true;
            //     }
            //     break;
            case SDL_EVENT_KEY_DOWN :
                if(events.key.key == SDLK_ESCAPE){
                    if(input_state->mouse_lock){
                        std::cout<<"hello";
                        SDL_SetWindowRelativeMouseMode(window,false);
                        input_state->mouse_lock = false;
                    }
                }
                break;
            case SDL_EVENT_WINDOW_FOCUS_LOST :
                if(input_state->mouse_lock){
                    SDL_SetWindowRelativeMouseMode(window,false);
                    input_state->mouse_lock = false;
                }
                break;
        }
    }

  const bool *keys = SDL_GetKeyboardState(nullptr);

  if (keys[SDL_SCANCODE_W])
    input_state->key_w = true;
  if (keys[SDL_SCANCODE_A])
    input_state->key_a = true;
  if (keys[SDL_SCANCODE_S])
    input_state->key_s = true;
  if (keys[SDL_SCANCODE_D])
    input_state->key_d = true;
  if (keys[SDL_SCANCODE_SPACE])
    input_state->key_space = true;
  if (keys[SDL_SCANCODE_LSHIFT])
    input_state->key_shift = true;


//   if (keys[SDL_SCANCODE_ESCAPE]){
//
//       // if(!input_state->mouse_lock){
//       //     SDL_SetWindowRelativeMouseMode(window,true);
//       //     input_state->mouse_lock = true;
//       // }
//
//       if(input_state->mouse_lock){
//           SDL_SetWindowRelativeMouseMode(window,false);
//           input_state->mouse_lock = false;
//       }
//   }

  SDL_GetRelativeMouseState(&input_state->rel_mousex, &input_state->rel_mousey);
  uint32_t buttons = SDL_GetMouseState(&input_state->mousex, &input_state->mousey);
  if (buttons & SDL_BUTTON_LMASK)
    input_state->mouse_left_key = true;
  if (buttons & SDL_BUTTON_RMASK)
    input_state->mouse_left_key = true;

}

void User_interface::run(const std::vector<uint32_t> &framebuffer) {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  float temp = width/6.0f;
  float w = width - temp;
  ImGui::SetNextWindowPos(ImVec2(w, 0.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(temp, (float)height), ImGuiCond_Always);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove;
  ImGui::Begin("Menu",nullptr,flags);

  handle_input();

  if(ImGui::Button("Close")){
      system_state->running = false;
  }
  if(ImGui::Button("Mouse_lock")){
      if(!input_state->mouse_lock){
          SDL_SetWindowRelativeMouseMode(window,true);
          input_state->mouse_lock = true;
      }
  }
  if(ImGui::SliderFloat("Fov",&render_state->fov,1.0f,180.0f)) render_state->changed = true;
  if(ImGui::SliderFloat("Near_plane",&render_state->near_plane,0.01f,10.0f)) render_state->changed = true;
  if(ImGui::SliderFloat("Far_plane",&render_state->far_plane,50.0f,1000.0f)) render_state->changed = true;

  ImGui::Checkbox("Rasterization",&render_state->rasterization);
  ImGui::Checkbox("Wireframe",&render_state->wireframe);

  ImGui::End();

  SDL_SetRenderDrawColor(renderer,0,0,0,0);
  SDL_RenderClear(renderer);


  SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t));

  SDL_RenderTexture(renderer, texture, nullptr, nullptr);

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

  SDL_RenderPresent(renderer);
}


void User_interface::close() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}
