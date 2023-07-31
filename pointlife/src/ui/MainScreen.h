//
//  MainScreen.h
//  ClusterBotz
//
//  Created by Scott Schafer on 10/3/17.
//
//

#ifndef MainScreen_h
#define MainScreen_h

#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <nanogui/nanogui.h>
#include "../Globals.h"
#include "../simulation/WorldData.h"
#include <GLFW/glfw3.h>

using namespace nanogui;

class MainScreen : public Screen {
  private:
    WorldData & w;
  
  public:
  static int run(WorldData & w);
  
//  static MainScreen * initScreen(WorldData & w, const Vector2i &size, const std::string &caption,
//                    bool resizable = true, bool fullscreen = false);
//
    MainScreen(WorldData & w) : w(w) {}
//    MainScreen(WorldData & w, const Vector2i &size, const std::string &caption,
//           bool resizable = true, bool fullscreen = false)
//
//           : Screen(
//                    size,
//                    caption,
//                    resizable,
//                    fullscreen,
//                    true,
//                    true), w(w) {
//
//
////             Screen(
////                 const Vector2i &size,
////                 const std::string &caption = "Unnamed",
////                 bool resizable = true,
////                 bool fullscreen = false,
////                 bool depth_buffer = true,
////                 bool stencil_buffer = true,
////                 bool float_buffer = false,
////                 unsigned int gl_major = 3,
////                 unsigned int gl_minor = 2
////             );
//
//            set_background(nanogui::Color(0,0,0,255));
//           }
  
  void initUI();
  virtual bool keyboard_event(int key, int scancode, int action, int modifiers);
  virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers);
  virtual void draw_contents();
};


#endif /* MainScreen_h */
