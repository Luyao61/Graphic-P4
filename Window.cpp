#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"

int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
float spinAngle = 0.05;
float orbitAngle = 0.05;

bool drawHouse = false;
bool drawSphere = false;
bool drawCube = false;
bool drawBear = false;
bool drawBunny = false;
bool drawDragon = false;

Vector3 oldPoint;
float oldX, oldY;
bool isDraging = false;
bool rightButton = false;
bool leftButton = false;

std::vector<OBJObject*>* allOBJ;

void Window::initialize(void)
{
    
    //Setup the Directional light
    Vector4 lightDirection(0.0, 10.0, 15.0, 0.0);  //w is zero if light source is directional
    Globals::directionLight.position = lightDirection;
    
    //Setup the point light
    Vector4 pointLightPos(0.0, -10.0, 15.0, 1.0);
    Globals::pointLight.position = pointLightPos;
    Globals::pointLight.quadraticAttenuation = 0.02;
    
    
    //Setup the spot light
    Vector4 spotLightPos(0.0, 20.0, 20.0, 1.0);
    Globals::soptLight.position = spotLightPos;
    Vector3 spotLightDirection(0.0, 0.0, 0.0);
    Globals::soptLight.spot_direction = spotLightDirection;
    Globals::soptLight.cutoffParameter = 20;
    Globals::soptLight.quadraticAttenuation = 0.02;
    
    /*
    //Setup the light
    Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;
     */
    
    //Initialize cube matrix:
    Globals::cube.toWorld.identity();
    
    //Setup the cube's material properties
    Color color(0x23ff27ff);
    Globals::cube.material.color = color;
    
    //
    drawSphere = true;
    
    //Initialize sphere matrix:
    Globals::sphere.toWorld.identity();
    
    //Setup the sphere's material properties
    Globals::sphere.material.color = color;
    
    Globals::house.toWorld.identity();

     allOBJ = new std::vector<OBJObject*>();

}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    if(drawCube == true){
    //Rotate cube; if it spins too fast try smaller values and vice versa
    //Globals::cube.spin(spinAngle);
    //Call the update function on cube
    Globals::cube.update(Globals::updateData);
    }
    
    /*
    if (drawSphere == true) {
        Globals::sphere.move();
        Globals::sphere.update(Globals::updateData);
    }
    Globals::bear.spin(spinAngle);
     */
    Globals::sphere.update(Globals::updateData);
    //Call the display routine to draw the cube
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    Globals::pointLight.bind(0);

    Globals::directionLight.bind(1);

    Globals::soptLight.bind(2);
    
    //Globals::light.bind(0);
    
    
    
    //Draw the cube!
    if (drawCube == true) {
        Globals::cube.draw(Globals::drawData);
    }
    
    //Draw the sphere!
    if (drawSphere == true) {
        Globals::sphere.draw(Globals::drawData);
    }
    
    if (drawHouse == true) {
        Globals::house.draw(Globals::drawData);
    }

    for (int i =0; i<allOBJ->size(); i++) {
        allOBJ->at(i)->draw(Globals::drawData);
    }

    
    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}


//TODO: Keyboard callbacks!
void Window::processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);
    }
    else if(key == 'r'){
        if (spinAngle < 0) {
            spinAngle *= -1;
        }
        Globals::cube.reset();
        Globals::sphere.reset();
        Globals::dragon.reset();
        Globals::bear.reset();
        Globals::bunny.reset();
    }
}



//TODO: Function Key callbacks!
void Window::processSpecialKeys(int key, int x, int y) {
	Vector3 e, d, up;

	switch (key) {
	case GLUT_KEY_F1:
            drawCube = false;
            drawSphere = true;
            drawHouse = false;
            
            allOBJ->clear();

            Globals::camera.reset();
            break;
	case GLUT_KEY_F2:
            drawHouse =false;
            drawSphere = false;
            drawCube= true;
            
            e.set(0, 24.14, 24.14);
            d.set(0, 0, 0);
            up.set(0, 1, 0);
            
            allOBJ->clear();

		break;
	case GLUT_KEY_F3:
            drawHouse =true;
            drawSphere = false;
            drawCube= false;
            
		break;
	case GLUT_KEY_F4:
            drawBunny = true;
            drawHouse = false;
            drawCube = false;
            drawSphere = false;

            Globals::camera.reset();
            
            allOBJ->push_back(new OBJObject(Globals::bunny));
            break;
    case GLUT_KEY_F5:
            drawBear = true;
            drawHouse = false;
            drawCube = false;
            drawSphere = false;

            
            Globals::camera.reset();
            
            allOBJ->push_back(new OBJObject(Globals::bear));
            break;
    case GLUT_KEY_F6:
            drawDragon = true;
            drawHouse = false;
            drawCube = false;
            drawSphere = false;

            Globals::camera.reset();
            
            allOBJ->push_back(new OBJObject(Globals::dragon));
            break;
	}
}

//TODO: Mouse callbacks!
void Window::processMouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { // left mouse button pressed
            isDraging = true;
            leftButton = true;
        }
        else  { /* (state = GLUT_UP) */
            isDraging = false;
            leftButton = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN) { // left mouse button pressed
            isDraging = true;
            rightButton = true;
            
            oldX = x;
            oldY = y;
        }
        else  { /* (state = GLUT_UP) */
            isDraging = false;
            rightButton = false;
        }
    }
}
//TODO: Mouse Motion callbacks!
void Window::processMotion(int x, int y){
    Vector3 currPoint;
    Vector3 direction;
    if (isDraging) {
        if (leftButton && !rightButton) {
            currPoint = trackObjMapping(x, y);
            direction = currPoint - oldPoint;
            Vector3 rotateAxis;
            float rotateAngle;
            rotateAxis = currPoint.cross(oldPoint);
            rotateAngle = oldPoint.angle(currPoint);
            
            Globals::cube.makeRotateArbitrary(rotateAxis, rotateAngle);
        }
        else if(!leftButton && rightButton){
            Globals::cube.makeTranslate((x-oldX)/10.0, (oldY - y)/10.0, 0.0);
        }
    }
    oldPoint = currPoint;
    oldX = x;
    oldY = y;
}

//map 2D coordinate to real world 3D coordinate
Vector3 Window::trackObjMapping(int x, int y){
    Vector3 v;
    float d;
    v.set((2.0*x - width)/width, (height - 2.0*y)/height, 0.0);
    d = v.magnitude();
    d = (d<1.0) ? d : 1.0;
    
    v.set((2.0*x - width)/width, (height - 2.0*y)/height, sqrtf(1.001 - d*d));
    v = v.normalize();
    return v;
}


