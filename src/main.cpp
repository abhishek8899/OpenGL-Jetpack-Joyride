#include "main.h"
#include "timer.h"
#include "ball.h"
#include "bg.h"
#include <bits/stdc++.h>

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Jet jet1;
// Ball ball2;
Floor bg1[11],bg2[11];

vector <Circle> c1;
Circle c1_;

vector <int> flg;

bounding_box_t b1,b2;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;

int vu;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);

    if(vu)
    {
    	jet1.position.x=ball1.position.x;
    	jet1.position.y=ball1.position.y;
    	jet1.draw(VP);
    }

    // ball2.draw(VP);
    for(int i=0;i<10;i++)
    	bg1[i].draw(VP),
    	bg2[i].draw(VP);
    for(int i=0;i<c1.size();i++)
    	if(flg[i])
    		c1[i].draw(VP);
    if(bg1[0].position.x<=-5.5)
    	for(int i=0;i<10;i++)
    		bg1[i].set_position(i),
    		bg2[i].set_position(i);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (left) {
        // Do something
    }
}

void tick_elements() {
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    float upthrust=0.0025,grv=0.002;
    if(left)
    {
    	if(up)
	   	{
	   		vu=1;
	    	if(ball1.position.y==-2.53)
	    		b1.ver=0.45;
	    	else
 	   			b1.ver-=upthrust;
 	   	}
	 	else
 	    {	
 	    	vu=0;
 	   		if(ball1.position.y>-2.53)
 	   			b1.ver+=grv;
 	    }
 	   	if(ball1.position.y>=3.45)
 	   		b1.ver=0,ball1.position.y=3.44;
    	if(ball1.position.x>=-3.5)
    		b1.x=ball1.tick(-1, 1, b1.ver);
    	else
    		b1.x=ball1.tick(0, 1, b1.ver);
    }
    else if(right)
    {
    	if(up)
	    {
	    	vu=1;
	    	// cout << "right up" << endl;
	    	if(ball1.position.y==-2.53)
	    		b1.ver=0.45;
	    	else
 	   			b1.ver-=upthrust;
 	   }
 	   else
 	   {	
 	   		vu=0;
 	   		if(ball1.position.y>-2.53)
 	   			b1.ver+=grv;
 	   }
 	   if(ball1.position.y>=3.45)
 	   		b1.ver=0,ball1.position.y=3.44;
       if(ball1.position.x<=3.5)
    		b1.x=ball1.tick(1, 1, b1.ver);
    	else
    		b1.x=ball1.tick(0, 1, b1.ver);
    }
    else if(up)
    {
    	vu=1;
    	if(ball1.position.y==-2.53)
    		b1.ver=0.45;
    	else
    		b1.ver-=upthrust;
    	if(ball1.position.y>=3.45)
 	   		b1.ver=0,ball1.position.y=3.44;
    	b1.x=ball1.tick(0, 0, b1.ver);
    }
    // cout << b1.ver << endl;
    if(!up)
    	vu=0;
    for(int i=0;i<10;i++)
    	bg1[i].tick(),
    	bg2[i].tick();
    for(int i=0;i<c1.size();i++)
    	c1[i].position.x-=0.025;
    // cout << ball1.position.y << endl;
    float tmp=-2.53;
    // cout << "y " << ball1.position.y << endl;
    if(ball1.position.y==tmp)
    {
    	b1.ver=0;
    	// cout << "ver" << endl;
    }
    else if(!left && !right && !up)
    {
    	if(ball1.position.y>=3.45)
 	   		b1.ver=0,ball1.position.y=3.44;
    	b1.ver+=grv;
    	b1.x=ball1.tick(0, 1, b1.ver);
    }

    for(int i=0;i<c1.size();i++)
    {
    	b2.x=c1[i].position.x;
    	b2.y=c1[i].position.y;
    	if(detect_collision(b1,b2))
    		flg[i]=0;
    }
    	// cout << b1.ver << " b" << endl;
    // b2.x=ball2.tick(b2.direction, b2.fall);
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
	b1.x=-2,b1.y=0;
	b1.ver=0;
	b1.width=b1.height=1;
	b2.width=b2.height=0.3;
	// b2.x=2,b2.y=0;
    ball1 = Ball(b1.x, b1.y, COLOR_DGREY);
    jet1 = Jet(b1.x, b1.y, COLOR_YELLOW);
    for(int i=0;i<10;i++)
    {
    	double rh = double((rand()%200 - 100))/50;
    	int rn = rand()%7 + 4;
		for(int j=0;j<rn;j++)
		{
			c1_ = Circle(i*8+(double)(j)/2, rh , COLOR_ORANGE);
    		c1.push_back(c1_);
    		flg.push_back(1);
    		c1_ = Circle(i*8+(double)(j)/2, rh+0.5 , COLOR_ORANGE);
    		c1.push_back(c1_);
    		flg.push_back(1);
    	}
    }
    // ball2       = Ball(b2.x, b2.y, COLOR_GREEN);

    for(int i=0;i<10;i++)
    	if(i%2)
    		bg1[i] = Floor(i-3.5,-3.28,COLOR_LBROWN),
    		bg2[i] = Floor(i-3.5,-3.78,COLOR_BROWN);
    	else
    		bg1[i] = Floor(i-3.5,-3.28,COLOR_BROWN),
    		bg2[i] = Floor(i-3.5,-3.78,COLOR_LBROWN);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    b1.direction=1;
    // b2.direction=-1;
    b1.fall=0;
    b1.width=1;

    window = initGLFW(width, height);

    initGL (window, width, height);
    bool eee=true;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }
        // if(eee)
        // {
        // 	cout << b1.x << " " << b2.x << endl;
        // 	eee=false;
        // }


        //make b1.fall==0 else below condition is always false
        if(b1.fall==1 && detect_collision(b1,b2))
        {
        	// cout << "collision detected" << b1.x << " " << b2.x << endl;
        	b1.fall=b2.fall=1;
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) && (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
