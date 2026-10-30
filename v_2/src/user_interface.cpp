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
#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

void User_interface::init(SDL_Window *window,Resource_manager *resource_manager,Input_state *i_state,Render_state *r_state,System_state *s_state) {
  this->window = window;
  this->resource_manager = resource_manager;
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
                              SDL_TEXTUREACCESS_STREAMING, render_state->width, render_state->height);

}

void User_interface::handle_input() {

    while(SDL_PollEvent(&events)){
        ImGui_ImplSDL3_ProcessEvent(&events);
        switch(events.type){
            case SDL_EVENT_QUIT :
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED :
                system_state->running = false;
                break;
            case SDL_EVENT_KEY_DOWN :
                if(events.key.key == SDLK_ESCAPE){
                    if(input_state->mouse_lock){
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

  SDL_GetRelativeMouseState(&input_state->rel_mousex, &input_state->rel_mousey);
  uint32_t buttons = SDL_GetMouseState(&input_state->mousex, &input_state->mousey);
  if (buttons & SDL_BUTTON_LMASK)
    input_state->mouse_left_key = true;
  if (buttons & SDL_BUTTON_RMASK)
    input_state->mouse_right_key = true;

}

void User_interface::run(const std::vector<uint32_t> &framebuffer) {
  handle_input();

  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  float temp = render_state->width/4.0f;
  float w = render_state->width - temp;
  ImGui::SetNextWindowPos(ImVec2(w, 0.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(temp, (float)render_state->height), ImGuiCond_Always);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove;
  ImGui::Begin("Menu",nullptr,flags);

  ImGui::BeginDisabled(file_selection != nullptr);
  if(ImGui::Button("File Selection")) {
      file_selection = std::make_unique<pfd::open_file>(
          "Select models",
          ".",
          std::vector<std::string>{"All Files (*)","*"},
          pfd::opt::multiselect
      );
  }
  ImGui::EndDisabled();

  if(file_selection && file_selection->ready(0)) {
      auto result = file_selection->result();
      if(!result.empty()){
          for(const auto& file:result){
              std::filesystem::path path (file);
              std::cout<<path<<std::endl;
              resource_manager->queue_model_load(path);
          }
      }
      file_selection.reset();
  }

  if(ImGui::Button("Mouse_lock")){
      if(!input_state->mouse_lock){
          SDL_SetWindowRelativeMouseMode(window,true);
          input_state->mouse_lock = true;
      }
  }
  ImGui::SliderFloat("Fov",&render_state->fov,1.0f,180.0f);
  ImGui::SliderFloat("Near_plane",&render_state->near_plane,0.001f,1.0f);
  ImGui::SliderFloat("Far_plane",&render_state->far_plane,50.0f,1000.0f);

  ImGui::Checkbox("Rasterization",&render_state->rasterization);
  ImGui::Checkbox("Wireframe",&render_state->wireframe);

  // const char* items[] = { "cmd.exe" };
  std::vector<const char*> items ;
  for(const auto& [first,_] : resource_manager->models){
      std::cout<<first<<std::endl;
      items.push_back(first.data());
  }

  static int currentItemIndex = 0; // Stores the index of the selected item

  if (ImGui::BeginCombo("Render Model", items[currentItemIndex]))
  {
      for (int i = 0; i < items.size(); i++)
      {
          const bool isSelected = (currentItemIndex == i);

          if (ImGui::Selectable(items[i], isSelected))
          {
              currentItemIndex = i; // Update selection
          }

          // Set the initial focus when opening the combo (scrollbar positioning)
          if (isSelected) {
              ImGui::SetItemDefaultFocus();
          }
      }
      ImGui::EndCombo();
  }

  static std::vector<std::string> options = { "Option A", "Option B", "Option C", "Option D" };
     static std::vector<bool> selected_states = { false, false, false, false };

     // 2. Generate a preview string showing what is currently selected
     std::string preview_text = "";
     int selected_count = 0;

     for (size_t i = 0; i < options.size(); ++i) {
         if (selected_states[i]) {
             if (!preview_text.empty()) preview_text += ", ";
             preview_text += options[i];
             selected_count++;
         }
     }
     if (selected_count == 0) {
         preview_text = "Select elements...";
     } else if (selected_count > 2) {
         preview_text = std::to_string(selected_count) + " items selected"; // Fallback for many items
     }

     // 3. Render the Combo Dropdown
     if (ImGui::BeginCombo("My Multi-Drop", preview_text.c_str()))
     {
         for (size_t i = 0; i < options.size(); ++i)
         {
             // The magic flag: ImGuiSelectableFlags_NoAutoClosePopups
             // keeps the dropdown window open when clicked.
             if (ImGui::Selectable(options[i].c_str(), selected_states[i], ImGuiSelectableFlags_NoAutoClosePopups))
             {
                 // Toggle selection state
                 selected_states[i] = !selected_states[i];
             }
         }
         ImGui::EndCombo();
     }

  if(ImGui::Button("Close")){
      system_state->running = false;
  }
  ImGui::End();

  SDL_SetRenderDrawColor(renderer,0,0,0,0);
  SDL_RenderClear(renderer);


  SDL_UpdateTexture(texture, nullptr, framebuffer.data(), render_state->width * sizeof(uint32_t));

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
