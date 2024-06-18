
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
    Circle* parent; // Pointer to parent object (planet)

    Circle(float orbit_radius, float r, float angular_velocity = 0, float mass = 1.0, float cy = 0.0f, float cz = 0.0f, Circle* parent = nullptr) {
        this->orbit_radius = orbit_radius;
        this->cx = orbit_radius; // Initial position at the orbit radius
        this->cy = cy;
        this->cz = cz;
        this->r = r;
        this->mass = mass;
        this->angle = 0.0f;
        this->angular_velocity = angular_velocity;
        this->parent = parent;
        cout << "Circle C" << endl;
    }

    ~Circle() {
        cout << "Circle D" << endl;
    }

    void update(float deltaTime) {
        // Update the angle for circular motion
        angle += angular_velocity * deltaTime;
        cx = cos(angle) * orbit_radius;
        cz = sin(angle) * orbit_radius;

        // If the circle has a parent (e.g., a moon orbiting a planet), update its position relative to the parent
        if (parent) {
            cx += parent->cx;
            cz += parent->cz;
        }
    }

    void DrawCircle() {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        glutSolidSphere(r, 20, 20); // Draw a solid sphere using FreeGLUT
        glPopMatrix();
    }
};
// Global sun object
Circle sun(0.0, 1.0, 0.0); // Central sun
Circle mercury(0.39, 0.005, 0.5); // Planets with orbits and angular velocities
Circle venus(0.72, 0.01, 0.3);

Circle moon2(0.2, 0.05, 0.5, 1.0, 0.0f, 0.0f, &venus);

Circle earth(1.00, 0.01, 0.65);

Circle moon3(0.2, 0.05, 0.6, 1.0, 0.0f, 0.0f, &earth);

Circle mars(1.52, 0.005, 0.1);

Circle jupiter(5.20, 0.1, 0.15);
Circle saturn(9.54, 0.1, 0.43);
Circle uranus(19.22, 0.05, 0.13);
Circle neptune(30.06, 0.05, 0.005);


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

float X = 0.0, Y = 0.0, Z = 0.0;

void display() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;



    /* Update positions based on velocities */
    sun.update(deltaTime); // Ensure the sun updates in case it moves
    mercury.update(deltaTime);
    venus.update(deltaTime);
    moon2.update(deltaTime);
    earth.update(deltaTime);
    moon3.update(deltaTime);
    mars.update(deltaTime);
    jupiter.update(deltaTime);
    saturn.update(deltaTime);
    uranus.update(deltaTime);
    neptune.update(deltaTime);


    /* Update light position */
    setupLighting();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float eyeX = camera_distance * sin(camera_angle_x) * cos(camera_angle_y);
    float eyeY = camera_distance * sin(camera_angle_y);
    float eyeZ = camera_distance * cos(camera_angle_x) * cos(camera_angle_y);

    gluLookAt(eyeX, eyeY, eyeZ,  // Eye position
        X, Y, Z,    // Look-at point
        0.0, 1.0, 0.0);   // Up direction

    sun.DrawCircle();
    mercury.DrawCircle();
    venus.DrawCircle();
    moon2.DrawCircle();
    earth.DrawCircle();
    moon3.DrawCircle();
    mars.DrawCircle();
    jupiter.DrawCircle();
    saturn.DrawCircle();
    uranus.DrawCircle();
    neptune.DrawCircle();


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

        

    case GLUT_KEY_F1:
        X = sun.cx; Y = sun.cy; Z = sun.cz;
        break;
    case GLUT_KEY_F2:
        X = mercury.cx; Y = mercury.cy; Z = mercury.cz;
        break;
    case GLUT_KEY_F3:
        X = venus.cx; Y = venus.cy; Z = venus.cz;
        break;

    case GLUT_KEY_F4:
        X = earth.cx; Y = earth.cy; Z = earth.cz;
        break;
    case GLUT_KEY_F5:
        X = mars.cx; Y = mars.cy; Z = mars.cz;
        break;
    case GLUT_KEY_F6:
        X = jupiter.cx; Y = jupiter.cy; Z = jupiter.cz;
        break;
    case GLUT_KEY_F7:
        X = saturn.cx; Y = saturn.cy; Z = saturn.cz;
        break;
    case GLUT_KEY_F8:
        X = uranus.cx; Y = uranus.cy; Z = uranus.cz;
        break;
    case GLUT_KEY_F9:
        X = neptune.cx; Y = neptune.cy; Z = neptune.cz;
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



