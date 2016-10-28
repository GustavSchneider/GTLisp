#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "../data/types.h"
#include "../scheme_utils.h"


GLFWwindow* GLOBAL_WINDOW;





Scheme_object* g_init(Scheme_object* _){
  if(!glfwInit()){
    return Scheme_false;
  } else {
    return Scheme_true;
  }
}

Scheme_object* g_create_window(Scheme_object* args){
  if(GLOBAL_WINDOW)
    return Scheme_false;
  
  int width = su_obj_at(args, 0)->data.integer;
  int height = su_obj_at(args, 1)->data.integer;
  char* name = su_obj_at(args, 2)->data.string.str;

  GLOBAL_WINDOW = glfwCreateWindow(width, height, name, NULL, NULL);

  if(!GLOBAL_WINDOW)
    return Scheme_false;


  glfwMakeContextCurrent(GLOBAL_WINDOW);
  
  return Scheme_true;
  
}





Scheme_object* g_swap_buffers(Scheme_object* _){
  glfwSwapBuffers(GLOBAL_WINDOW);
  return Scheme_void;
}


Scheme_object* g_terminate(Scheme_object* _){
  glfwTerminate();
  return Scheme_void;
}

Scheme_object* g_poll_evnets(Scheme_object* _){
  glfwPollEvents();
  return Scheme_void;
}

Scheme_object* key_is_down(int btn){
 int state = glfwGetKey(GLOBAL_WINDOW, btn);
 if( state == GLFW_PRESS)
   return Scheme_true;
 return Scheme_false;
}

Scheme_object* g_key_is_down_UP(Scheme_object* _){
  return key_is_down(GLFW_KEY_UP);
}


Scheme_object* g_key_is_down_DOWN(Scheme_object* _){
  return key_is_down(GLFW_KEY_DOWN);
}


Scheme_object* g_key_is_down_W(Scheme_object* _){
  return key_is_down(GLFW_KEY_W);
}

Scheme_object* g_key_is_down_S(Scheme_object* _){
  return key_is_down(GLFW_KEY_S);
}


Scheme_object* g_draw_rect(Scheme_object* args){
  Scheme_object* coord = su_car(args);
  Scheme_object* dims  = su_car(su_cdr(args));

  float x_pos = (float)su_car(coord)->data.real;
  float y_pos = (float)su_cdr(coord)->data.real;

  float width = (float)su_car(dims)->data.real;
  float height = (float)su_cdr(dims)->data.real;

  

  glBegin(GL_QUADS);
  glColor3f(1.0f,0.0f,0.0f); 
  glVertex3f(x_pos, y_pos, 0.0);
  glVertex3f(x_pos + width, y_pos, 0.0);
  glVertex3f(x_pos+ width, y_pos - height, 0.0);
  glVertex3f(x_pos, y_pos - height, 0.0);

  glEnd();

  return Scheme_void;
}

Scheme_object* g_clear(Scheme_object* args){
  float red = su_obj_at(args, 0)->data.real;
  float green = su_obj_at(args, 1)->data.real;
  float blue = su_obj_at(args, 2)->data.real;
  glClearColor(red,green,blue,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  return Scheme_void;
}


