

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846  /* pi */

using namespace std;

class Circle {
public:
    float cx, cy, cz, r;
    float angle, angular_velocity; // for circular motion
    float orbit_radius; // radius of the orbit

    float mass;

    Circle(float orbit_radius, float r, float angular_velocity = 0, float mass = 1.0) {
        this->orbit_radius = orbit_radius;
        this->cx = orbit_radius; // Initial position at the orbit radius
        this->cy = 0.0f;
        this->cz = 0.0f;
        this->r = r;
        this->mass = mass;
        this->angle = 0.0f;
        this->angular_velocity = angular_velocity;
        cout << "Circle C" << endl;
    }

    ~Circle() {
        cout << "Circle D" << endl;
    }

    void update(float deltaTime) {
        // Update the angle for circular motion
        angle += angular_velocity * deltaTime;
        cx = (cos(angle) * orbit_radius);
        cz = (sin(angle) * orbit_radius);
    }

    void DrawCircle() {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        glutSolidSphere(r, 20, 20); // Draw a solid sphere using FreeGLUT
        glPopMatrix();
    }
};

// Global sun object
Circle sun(0.0, 0.3, 0.0); // Central sun
Circle planet1(1.0, 0.1, 0.5); // Planets with orbits and angular velocities
Circle planet2(1.5, 0.1, 0.3);
Circle planet3(2.0, 0.1, 0.65);
Circle planet4(2.5, 0.1, 0.1);
Circle planet5(3.0, 0.1, 0.15);
Circle planet6(3.5, 0.1, 0.43);
Circle planet7(4.0, 0.1, 0.13);
Circle planet8(4.5, 0.1, 0.005);
Circle planet9(5.0, 0.1, 0.13);
Circle planet10(5.5, 0.1, 0.13);

float lastTime;

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Position the light at the sun's position
    GLfloat light_position[] = { sun.cx, sun.cy, sun.cz, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Set the ambient light properties
    GLfloat ambient_light[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    // Set the diffuse light properties
    GLfloat diffuse_light[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

    // Set the specular light properties
    GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
}

void setupViewport(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


float camera_distance = 10.0f;
float camera_angle_x = 0.0f;
float camera_angle_y = 0.0f;

void display() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    /* Update positions based on velocities */
    sun.update(deltaTime); // Ensure the sun updates in case it moves
    planet1.update(deltaTime);
    planet2.update(deltaTime);
    planet3.update(deltaTime);
    planet4.update(deltaTime);
    planet5.update(deltaTime);
    planet6.update(deltaTime);
    planet7.update(deltaTime);
    planet8.update(deltaTime);
    planet9.update(deltaTime);
    planet10.update(deltaTime);

    /* Update light position */
    setupLighting();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

        float eyeX = camera_distance * sin(camera_angle_x) * cos(camera_angle_y);
        float eyeY = camera_distance * sin(camera_angle_y);
        float eyeZ = camera_distance * cos(camera_angle_x) * cos(camera_angle_y);
    
        gluLookAt(eyeX, eyeY, eyeZ,  // Eye position
            0.0, 0.0, 0.0,    // Look-at point
            0.0, 1.0, 0.0);   // Up direction

    sun.DrawCircle();
    planet1.DrawCircle();
    planet2.DrawCircle();
    planet3.DrawCircle();
    planet4.DrawCircle();
    planet5.DrawCircle();
    planet6.DrawCircle();
    planet7.DrawCircle();
    planet8.DrawCircle();
    planet9.DrawCircle();
    planet10.DrawCircle();

    glutSwapBuffers();
}



void keyboard(int key, int x, int y) {
    float angle_step = 0.1f;
    switch (key) {
    case GLUT_KEY_LEFT:
        camera_angle_x -= angle_step;
        break;
    case GLUT_KEY_RIGHT:
        camera_angle_x += angle_step;
        break;
    case GLUT_KEY_UP:
        camera_angle_y += angle_step;
        if (camera_angle_y > M_PI / 2.0f) camera_angle_y = M_PI / 2.0f; // Limit the vertical angle
        break;
    case GLUT_KEY_DOWN:
        camera_angle_y -= angle_step;
        if (camera_angle_y < -M_PI / 2.0f) camera_angle_y = -M_PI / 2.0f; // Limit the vertical angle
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == 3) { // Scroll up
            camera_distance -= 0.5f;
            if (camera_distance < 2.0f) camera_distance = 2.0f; // Limit zoom in
        }
        else if (button == 4) { // Scroll down
            camera_distance += 0.5f;
            if (camera_distance > 50.0f) camera_distance = 50.0f; // Limit zoom out
        }
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    setupViewport(w, h);
}

int main(int argc, char** argv) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System in 3D");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    setupLighting(); // Set up lighting

    cout << glGetString(GL_VERSION) << endl;

    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutSpecialFunc(keyboard); // Register keyboard function for arrow keys
    glutMouseFunc(mouse); // Register mouse function for scrolling


    glutMainLoop();




    return 0;
}




