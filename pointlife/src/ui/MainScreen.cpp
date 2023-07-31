//
//  MainScreen.cpp
//  ClusterBotz
//
//  Created by Scott Schafer on 10/3/17.
//
//

//#define NANOGUI_USE_OPENGL

//#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>
#include <nanogui/canvas.h>

//#include <iostream>

#include "MainScreen.h"
#include "SettingsWindow.h"
#include "RenderUtils.h"
//#define NANOVG_GL2_IMPLEMENTATION  // Use GL2 implementation.
//#include "nanovg_gl.h"
#include "nanovg.h"
#include "nanovg_mtl.h"
using namespace nanogui;

static const int minSpeed = 50;

nanogui::ref<MainScreen> screen;
nanogui::ref<SettingsWindow> settingsWindow;
GLFWwindow* window;

#include "nanovg.h"

NVGcontext* ctx;
//struct NVGcontext* vg;


class MyCanvas : public Canvas {
public:
  MyCanvas(Widget *parent) : Canvas(parent, 1) {}
  
      
    virtual void draw_contents() {
      nvgBeginPath(ctx);
      nvgRect(ctx, 100,100, 120,30);
      nvgFillColor(ctx, nvgRGBA(255,192,0,255));
      nvgFill(ctx);
      
      return;
      glViewport(0, 0, 100, 100);
            glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
      return;
//      nvgBeginPath(vg);
//      nvgRect(vg, 100,100, 120,30);
//      nvgFillColor(vg, nvgRGBA(255,192,0,255));
//      nvgFill(vg);
      return;
//        using namespace nanogui;

        Matrix4f view = Matrix4f::look_at(
            Vector3f(0, -2, -10),
            Vector3f(0, 0, 0),
            Vector3f(0, 1, 0)
        );

        Matrix4f model = Matrix4f::rotate(
            Vector3f(0, 1, 0),
            (float) glfwGetTime()
        );

        Matrix4f model2 = Matrix4f::rotate(
            Vector3f(1, 0, 0),
            m_rotation
        );

        Matrix4f proj = Matrix4f::perspective(
            float(25 * M_PI / 180),
            0.1f,
            20.f,
            m_size.x() / (float) m_size.y()
        );

        Matrix4f mvp = proj * view * model * model2;

//        m_shader->set_uniform("mvp", mvp);
//
//        // Draw 12 triangles starting at index 0
//        m_shader->begin();
//        m_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 12*3, true);
//        m_shader->end();
    }

private:
    nanogui::ref<Shader> m_shader;
    float m_rotation;
};

int run(WorldData & w) {
  glfwInit();
  glfwSetTime(0);

#if defined(NANOGUI_USE_OPENGL)
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(NANOGUI_USE_GLES)
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#elif defined(NANOGUI_USE_METAL)
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  metal_init();
//  ctx = nvgCreateMTL(metalLayer, NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // Create a GLFWwindow object
  window = glfwCreateWindow(1024, 1024, "PointLife", nullptr, nullptr);
  if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);
  NVGcontext* vg = nvgCreate(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
  if (vg == NULL) {
      printf("Could not init nanovg.\n");
      return -1;
  }

  nvgBeginPath(vg);
  nvgRect(vg, 20, 20, 100, 100);
  nvgFillColor(vg, nvgRGBA(255,192,0,255));
  nvgFill(vg);
  nvgEndFrame(vg);
//  glfwSwapBuffers(window);

#if defined(NANOGUI_GLAD)
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
      throw std::runtime_error("Could not initialize GLAD!");
  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
#endif

  // Create a nanogui screen and pass the glfw pointer to initialize
  screen = new MainScreen(w);
  screen->initialize(window, true);
  void * metalLayer = metal_layer(window);
  ctx = nvgCreateMTL(metalLayer, null, NVG_ANTIALIAS | NVG_STENCIL_STROKES);

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glfwSwapInterval(0);
  glfwSwapBuffers(window);
#endif
  
//  vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

//
//  // Create nanogui gui
//  bool enabled = true;
  
  screen->initUI();

  screen->set_visible(true);
  screen->perform_layout();
  screen->clear();
  screen->draw_all();

  glfwSetCursorPosCallback(window,
          [](GLFWwindow *, double x, double y) {
          screen->cursor_pos_callback_event(x, y);
      }
  );

  glfwSetMouseButtonCallback(window,
      [](GLFWwindow *, int button, int action, int modifiers) {
          screen->mouse_button_callback_event(button, action, modifiers);
      }
  );

  glfwSetKeyCallback(window,
      [](GLFWwindow *, int key, int scancode, int action, int mods) {
          screen->key_callback_event(key, scancode, action, mods);
      }
  );

  glfwSetCharCallback(window,
      [](GLFWwindow *, unsigned int codepoint) {
          screen->char_callback_event(codepoint);
      }
  );

  glfwSetDropCallback(window,
      [](GLFWwindow *, int count, const char **filenames) {
          screen->drop_callback_event(count, filenames);
      }
  );

  glfwSetScrollCallback(window,
      [](GLFWwindow *, double x, double y) {
          screen->scroll_callback_event(x, y);
     }
  );

  glfwSetFramebufferSizeCallback(window,
      [](GLFWwindow *, int width, int height) {
          screen->resize_callback_event(width, height);
      }
  );

  // Game loop
  while (!glfwWindowShouldClose(window)) {
      // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      /* Alternatively call 'screen->draw_all();' that subsumes all of the above steps,
         including setting up an autorelease pool on macOS / Metal targets */

#if defined(NANOGUI_USE_METAL)
      // Important to periodically free memory used up by Metal command queues, etc.
      void *pool = autorelease_init();
#endif

      // Draw nanogui
      screen->draw_setup();
      screen->clear(); // glClear
      screen->draw_contents();
      screen->draw_widgets();
      screen->draw_teardown();

#if defined(NANOGUI_USE_METAL)
      autorelease_release(pool);
#endif

      glfwSwapBuffers(window);
  }

//  delete gui;
  screen.reset();

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();

#if defined(NANOGUI_USE_METAL)
  metal_shutdown();
#endif

  return 0;

}

int MainScreen :: run(WorldData &w) {
  return ::run(w);
}

void MainScreen :: initUI() {
  settingsWindow = new SettingsWindow(this, w);
  MyCanvas * canvas = new MyCanvas(this);
}

/// Default keyboard event handler
bool MainScreen :: keyboard_event(int key, int scancode, int action, int modifiers) {

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    
    switch (key) {
      case GLFW_KEY_ESCAPE:             // ESCAPE key
        delete ::settingsWindow;
        glfwSetWindowShouldClose(::window, GL_TRUE);
        Globals::exit = true;
        exit(0);
        return true;
        
      case GLFW_KEY_G:
        //Globals::gravity = ! Globals::gravity;
        return true;
        
      case 'A': {
        CellVO * pCells = (CellVO *) w.cellData.data();
        pCells[0].localData[0] += .1f;
        break; }
      
      case 'Z': {
        CellVO * pCells = (CellVO *) w.cellData.data();
        pCells[0].localData[0] -= .1f;
        break; }
        
      case '\'': {
        CellVO * pCells = (CellVO *) w.cellData.data();
        pCells[0].localData[1] += .1f;
        break; }
      
      case '/': {
        CellVO * pCells = (CellVO *) w.cellData.data();
        pCells[0].localData[1] -= .1f;
        break; }

      case 'p':
        //Globals::pause = ! Globals::pause;
        return true;
        
      case 'f':
        //Globals::inFitnessTest = ! Globals::inFitnessTest;
        return true;
        
      case GLFW_KEY_DOWN:
        Globals::magnification *= 1.1;
        return true;
        
      case GLFW_KEY_UP:
        Globals::magnification *= .9;
        return true;

      case GLFW_KEY_RIGHT:
        Globals::focusedCritter = (Globals::focusedCritter + 1) % w.totalOrganismCount;
        return true;

      case GLFW_KEY_LEFT:
        Globals::focusedCritter = (Globals::focusedCritter + w.totalOrganismCount - 1) % w.totalOrganismCount;
        return true;
        
      case GLFW_KEY_LEFT_BRACKET:
        if (Globals::minMsPerTurn == 0) {
          Globals::minMsPerTurn = minSpeed;
        }
        else {
          Globals::minMsPerTurn += 1;
        }
        return true;
        
      case GLFW_KEY_RIGHT_BRACKET:
        Globals::minMsPerTurn = std::max(0L, Globals::minMsPerTurn - 1);
//        if (Globals::minMsPerTurn <= minSpeed) {
//          Globals::minMsPerTurn = 0;
//        }
        return true;
    }
  }
}

bool MainScreen :: mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
  
  if (! down) {
    return;
  }
  
  float scale = Globals::magnification;
  float offsetX = Globals::offsetX;
  float offsetY = Globals::offsetY;

  float x = p.x();
  float y = p.y();
  
  x /= scale;
  y /= scale;

  x -= offsetX;
  y -= offsetY;
  
  
  w.cellData.copyFromDevice();
  CellVO * pCell = (CellVO *) w.cellData.data();
  int maxCells = w.cellData.size() / sizeof(CellVO);
//  OrganismVO * pOrganisms = (OrganismVO *) worldData.organismData.data();

  
  float closestDist = WORLD_DIM * WORLD_DIM;
  CellVO * pClosest = NULL;;
  
  while (maxCells-- > 0) {
    if (pCell->connectionCount >= 0) {
      float dx = pCell->x - x;
      float dy = pCell->y - y;
      float dsquared = dx * dx + dy * dy;
      if (dsquared < closestDist) {
        closestDist = dsquared;
        pClosest = pCell;
      }
      if (dsquared < CELL_SIZE * CELL_SIZE * 4) {
        Globals::focusedCritter = pCell->organismIndex;
        break;
      }
      break;
    }
  
    ++pCell;
  }

  cout << "click @:" << int(x) << ", " << int(y) << std::endl;
  if (pClosest) {
    cout << "closest:" << pClosest->x << ", " << pClosest->y << std::endl;
  }
  
  return true;
}


void DrawString(int x, int y, char *string);
void DrawString(int x, int y, char *string)
{
  int len, i;
  
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

void MainScreen :: draw_contents() {
  return;
  glEnable( GL_POINT_SMOOTH );
  glEnable( GL_BLEND );
  glEnable(GL_SCISSOR_TEST);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  int generation, entityLength = 0;
  
  float offsetX = 0, offsetY = 0, magnification = 1;
//
//  double magnification = Globals::magnification * WINDOW_SIZE / WORLD_DIM;
//
//  std::vector<Point> points;
//  std::vector<Point> connectorLines;
//  std::vector<bool> connectorOutside;
//  std::vector<Point> forceLines;
//  std::vector<GLColor> colors;
//
//  std::vector<bool> cellActive;
//
//  glScissor(0, 0, WINDOW_SIZE, WINDOW_SIZE);
  
  glClearColor( 0, 0, 0, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  glWindowPos2i(-offsetX,-offsetY);
  glPixelZoom(magnification, magnification);
  //  glDrawPixels(WORLD_DIM,WORLD_DIM,GL_RGB,GL_FLOAT,world.getWorldSpace().getFluidColors());
  glRasterPos2d(0,0);
  glPixelZoom(1,1);
  
  // save transformation
  glPushMatrix();
  
  // position rendering
  glTranslatef(-offsetX, -offsetY, 0);
  glScalef(magnification, magnification, 1);
  
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  setLineWidth(1);
  
  glVertex2f(0, 0);
  glVertex2f(WORLD_DIM, 0);
  
  glVertex2f(WORLD_DIM, 0);
  glVertex2f(WORLD_DIM, WORLD_DIM);
  
  glVertex2f(0, WORLD_DIM);
  glVertex2f(WORLD_DIM, WORLD_DIM);
  
  glVertex2f(0, 0);
  glVertex2f(0, WORLD_DIM);
  glEnd();
  
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  glBegin(GL_TRIANGLES);
  setLineWidth(1);
//
//  for (int i = 0; i < NUM_TRIANGLES; i++) {
//    TriangleVO & t = triangles[i];
//    glVertex2f(t.points[0].x, t.points[0].y);
//    glVertex2f(t.points[1].x, t.points[1].y);
//    glVertex2f(t.points[2].x, t.points[2].y);
//  }
//  glEnd();
//
//
//  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//  glBegin(GL_TRIANGLES);
//  setLineWidth(1);
//
//  for (int i = 0; i < NUM_TRIANGLES; i++) {
//    TriangleVO & t = triangles[i];
//    if (t.numManifolds > 0) {
//      glVertex2f(t.points[0].x, t.points[0].y);
//      glVertex2f(t.points[1].x, t.points[1].y);
//      glVertex2f(t.points[2].x, t.points[2].y);
//    }
//  }
//  glEnd();
  //
  //  generation = 1;
  //
  //  for (int i = 0; i < NUM_CELLS; i++) {
  //    Cell & cell = cells[i];
  //
  //    if (cell.mOnBoard) {
  //
  //      float r = 0, g = 0, b = 0, a = 1;
  //
  //      a = float(cell.mEntityHead->mEnergy / cell.mEntityHead->mInitialEnergy);
  //
  //      cellActive.push_back(cell.mActivated);
  //
  //      Point pos(cell.mPos.x, cell.mPos.y);
  //
  //      if (cells[i].mNumConnections == 0) {
  //        if (cells[i].mAction == fluid) {
  //          b = .5;
  //        }
  //        else {
  //          g = 1;
  //        }
  //      }
  //      else {
  //        ++entityLength;
  //        if (cells[i].mLastFlagellum) {
  //          b = 1;
  //          forceLines.push_back(pos);
  //          forceLines.push_back(Point(pos.x+cell.mFlagellumVector.x,
  //                                     pos.y+cell.mFlagellumVector.y));
  //
  //        }
  //        else if (cells[i].mLastAte) {
  //          r = 1;
  //        }
  //        else if (cells[i].mInactiveCount) {
  //          r = g = b = .4f;
  //        }
  //        else if (cells[i].mAction == actionBite) {
  //          r = .5f;
  //        }
  //        else if (cells[i].mAction == actionFlagellum) {
  //          b = 1;
  //        }
  //        else if (cells[i].mAction == actionLook) {
  //          r = g = 1;
  //        }
  //        //                else if (cells[i].mLastCollision) {
  //        //                    colors.push_back(GLColor(1, 0, 0));
  //        //                }
  //        else {
  //          r = g = b = 1;
  //        }
  //      }
  //      colors.push_back(GLColor(r, g, b, a));
  //
  //      points.push_back(pos);
  //
  //      for (int j = 0; j < cell.mNumConnections; j++) {
  //        if (cell.mConnections[j]) {
  //          connectorLines.push_back(pos);
  //          Point pos2 = cell.mConnections[j]->mPos;
  //          connectorLines.push_back(Point(pos2.x, pos2.y));
  //
  //          int connections = cell.mNumConnections + cell.mConnections[j]->mNumConnections;
  //          connectorOutside.push_back((cell.mNumAllConnections <= 6) ? true : false);
  //        }
  //      }
  //    }
  //  }
  //
  //
  //  if (focusIndex != -1) {
  //    setPointSize(WORLD_DIM / 20);
  //    glColor4f(1.0, 1.0, 1.0, .25);
  //    glBegin( GL_POINTS );
  //    glVertex2f(critterX, critterY);
  //    glEnd();
  //  }
  //
  //
  //  float pointSize = WINDOW_SIZE/WORLD_DIM * CELL_SIZE;
  //
  //  setLineWidth(.2);
  //
  //
  //  setPointSize(pointSize * 1.25);
  //  glBegin( GL_POINTS );
  //  for (int i = 0; i < points.size(); i++) {
  //
  //    if (!cellActive[i]) {
  //      glColor4f(1, 1, 1, .25 * colors[i].a);
  //      //double x = w2s(points[i].x, magnification);
  //      //double y = w2s(points[i].y, magnification);
  //
  //      glVertex2f(points[i].x, points[i].y);
  //    }
  //  }
  //  glEnd();
  //
  //  setPointSize(pointSize);
  //  glBegin( GL_POINTS );
  //
  //  for (int fpass = 0; fpass < 2; fpass++) {
  //    for (int i = 0; i < points.size(); i++) {
  //
  //      //double x = w2s(points[i].x, magnification);
  //      //double y = w2s(points[i].y, magnification);
  //
  //      GLColor c = colors[i];
  //
  //      if (fpass == 0 || (c.r == 0 && c.g == 0 && c.b == 1)) {
  //        glColor4f(c.r, c.g, c.b, c.a);
  //        //                glVertex2f(x, y);
  //        glVertex2f(points[i].x, points[i].y);
  //      }
  //    }
  //  }
  //  glEnd();
  //
  //  //if (false) {
  //
  //  //////
  //  glBegin(GL_LINES);
  //  setLineWidth(.1);// * (pass ? magnification : 1));
  //
  //  float r = pointSize/2.5f;
  //  for (int i = 0; i < points.size(); i++) {
  //
  //    GLColor c = colors[i];
  //
  //    glColor4f(c.r, c.g, c.b, 1);
  //
  //    DrawCircle(points[i].x, points[i].y, r, 20);
  //  }
  //  glEnd();
  //  //}
  //  ////
  //
  //  glColor4f(.3, .3, 1.0, .85);
  //  glBegin(GL_LINES);
  //  setLineWidth(.4);// * (pass ? magnification : 1));
  //  for (int i = 0; i < forceLines.size(); i += 2) {
  //    double x1 = forceLines[i].x;
  //    double y1 = forceLines[i].y;
  //    double x2 = forceLines[i+1].x;
  //    double y2 = forceLines[i+1].y;
  //
  //    glVertex2f(x1, y1);
  //    glVertex2f(x2, y2);
  //  }
  //  glEnd();
  //
  //  glColor4f(1.0, 1.0, 1.0, .75);
  //  glBegin(GL_LINES);
  //  for (int i = 0; i < connectorLines.size(); i += 2) {
  //
  //    if (connectorOutside[i/2]) {
  //      glColor4f(1, 0, 0, .85);
  //    }
  //    else{
  //      glColor4f(1, 1, 1, .85);
  //    }
  //    //        setLineWidth(40);//connectorLineWidth[i/2]);
  //
  //    double x1 = connectorLines[i].x;
  //    double y1 = connectorLines[i].y;
  //    double x2 = connectorLines[i+1].x;
  //    double y2 = connectorLines[i+1].y;
  //
  //    glVertex2f(x1, y1);
  //    glVertex2f(x2, y2);
  //  }
  glEnd();
  
  
  glPopMatrix();
  
//
//  if (true) {
//
//  int curTurnK = (Globals::numTurns / 1000);
//  if (curTurnK != turnCountK) {
//    msPerTurn = (curMicroseconds() - lastTurnMS) / 1000;
//    if (msPerTurn < 0) {
//      msPerTurn = 0;
//    }
//    lastTurnMS = curMicroseconds();
//    turnCountK = curTurnK;
//  }
//
//    char buffer[200];
//    //        sprintf(buffer, "Generation #%d, index = #%d, length = %d", generation, testIndex, entityLength);
//    //sprintf(buffer, "Turn #%d, # critters = %d", world.getTurn(), world.getNumEntities());
//    sprintf(buffer, "%dK turns, msPerTurn = %d", (Globals::numTurns / 1000), (int) msPerTurn);
//    DrawString(20, 20, buffer);
//  }
//
  
  
  
  /*
   RGBType *pixels = new RGBType[WORLD_DIM*WORLD_DIM];
   
   
   //    world.getWorldSpace()
   //    for (int x = 0; x < WORLD_DIM; y)
   for (int i=0; i< 600*600; i++) {
   
   
   
   pixels[i].r=0.5;
   
   pixels[i].g=0.5;
   
   pixels[i].b=0.5;
   
   
   
   }
   */
  
  
  
  
  //delete[] pixels;
  
  
  glFinish();
  glutSwapBuffers();
  
  glutPostRedisplay();
 
  DrawString(10, 10, "WTF");
}
