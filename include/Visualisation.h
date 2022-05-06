
#ifndef MEGA_VISUALISATION_CORE
#define MEGA_VISUALISATION_CORE

#include <GL/glut.h>
#include <chrono> // for spinning around object

using namespace std::chrono; // very bad, only used for auto rotation TODO: get rid of it

namespace MEGA {

/*
    WARNING: This is not only some of the worst code I have seen but also the absolute worst
             part of this project.

*/

// For now this works
#define SHOW_VERTICES
//#define SHOW_NORMALS
#define SHOW_QUADS
#define SHOW_COLLISION

// TODO: Implement Options for display
enum {
    MEGA_VERTICES,
    MEGA_NO_QUADS,
    MEGA_NORMALS,
    MEGA_SPARSE_NORMALS
} DisplayOptions;


void InitGL() {
   glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
   glClearDepth(1.0f);                 
   glEnable(GL_DEPTH_TEST); 
   glDepthFunc(GL_LEQUAL);
   glShadeModel(GL_SMOOTH); 
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
}
Model* model;


// GLOBAL VARIABLES
// TODO: FIND A WAY TO NOT HAVE THEM GLOBAL
static float last_x = 400, last_y = 300;
const float sensitivity = 0.0001f;
bool first_move = true;
bool moved = false;
float yaw = 0, pitch = 0;
std::vector<float> direction{0, 0, 0};

const float radius = 1.0f;
const float rotation_speed = 0.0000003;
auto start = std::chrono::high_resolution_clock::now();


void mouseMotion(int x, int y) {

    if( false ) {// MOUSE BUTTON PRESSED
        if( first_move ) {
            last_x = x;
            last_y = y;
            first_move = false;
        }

        moved = true;

        float xoffset = x - last_x;
        float yoffset = y - last_y;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if(pitch >  89.0f) pitch =  89.0f;
        if(pitch < -89.0f) pitch = -89.0f;

        // TODO: replace with own vector class
        direction.push_back( cos( degrees_to_radians(yaw)) * cos( degrees_to_radians(pitch)));
        direction.push_back( degrees_to_radians(pitch));
        direction.push_back( sin( degrees_to_radians(yaw)) * cos( degrees_to_radians(pitch)));

        // normalize direction
        // this will be done in the class after it is fixed
        float len = sqrt( pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2], 2));
        direction[0] /= len;
        direction[1] /= len;
        direction[2] /= len;
    }
}


void viewModel__display_func() { 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();                


    glTranslatef(0, 0.0f, -2.0f);  

    if(moved) {
        gluLookAt( 1, 1.0f, 2, direction[0], direction[1], direction[2], 0.0f, 1.0f, 0.0f);
        moved = false;
    }

    // calculate delta time for automatic rotation around origin
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<microseconds>(stop - start);

    float rotcam_x = sin(duration.count()*rotation_speed) * radius;
    float rotcam_y = cos(duration.count()*rotation_speed) * radius;

    //std::cout << rotcam_x << " " << rotcam_y << std::endl;
    gluLookAt(rotcam_x, 0.9, rotcam_y, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

#ifdef SHOW_VERTICES
    // DRAW VERTICES
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 1.0f);
    for( auto i : model->vertices ) {
       glVertex3f( i.position[0], i.position[1], i.position[2]);
    }
    glEnd();
#endif

#ifdef SHOW_QUADS
    // DRAW QUADS
    glBegin(GL_QUADS);                

        glColor3f(0.4f, 0.4f, 0.4f);
        for( size_t i = 0; i < model->triangles.size(); i += 4) {
            float x [3];
            float y [3];
            float z [3]; 
            float w [3];
            x[0] = model->vertices[ model->triangles[i]].position[0];
            x[1] = model->vertices[ model->triangles[i]].position[1];
            x[2] = model->vertices[ model->triangles[i]].position[2];

            y[0] = model->vertices[ model->triangles[i + 1]].position[0];
            y[1] = model->vertices[ model->triangles[i + 1]].position[1];
            y[2] = model->vertices[ model->triangles[i + 1]].position[2];

            z[0] = model->vertices[ model->triangles[i + 2]].position[0];
            z[1] = model->vertices[ model->triangles[i + 2]].position[1];
            z[2] = model->vertices[ model->triangles[i + 2]].position[2];

            w[0] = model->vertices[ model->triangles[i + 3]].position[0];
            w[1] = model->vertices[ model->triangles[i + 3]].position[1];
            w[2] = model->vertices[ model->triangles[i + 3]].position[2];
/*
            std::cout << "X : ";
            for( auto i : x ) { std::cout << i << " ";}
            std::cout << std::endl;
            std::cout << "Y : ";
            for( auto i : y ) { std::cout << i << " ";}
            std::cout << std::endl;
            std::cout << "Z : ";
            for( auto i : z ) { std::cout << i << " ";}
            std::cout << std::endl;
            std::cout << "W : ";
            for( auto i : w ) { std::cout << i << " ";}
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << model->triangles.size()/4 << std::endl;
*/
            glVertex3f( x[0], x[1], x[2]);
            glVertex3f( y[0], y[1], y[2]);
            glVertex3f( z[0], z[1], z[2]);
            glVertex3f( w[0], w[1], w[2]);
        }
    glEnd();
#endif

#ifdef SHOW_NORMALS
    // DRAW NORMALS
    glColor3f(1.0f, 0.5f, 0.2f);
    glBegin(GL_LINES);
    glLineWidth(0.001);
        size_t k = 0;
            for( size_t i{}; i < model->vertices.size(); i++ ) {
            float x = model->normals[i].x;
            float y = model->normals[i].y;
            float z = model->normals[i].z;
            float oX = model->vertices[i].position[0];
            float oY = model->vertices[i].position[1];
            float oZ = model->vertices[i].position[2];
            //if(i%4==0) k++;

            glVertex3f(oX+(x/15), oY+(y/15), oZ+(z/15));
            glVertex3f(oX, oY, oZ);
        }
    glEnd();
#endif

#ifdef SHOW_COLLISION
    // DRAW VERTICES
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for( size_t i{}; i < model->collision.size(); i += 4 ) {
       glVertex3f( model->collision[i].position[0], model->collision[i].position[1]+0.05, model->collision[i].position[2]);
       glVertex3f( model->collision[i+1].position[0], model->collision[i+1].position[1]+0.05, model->collision[i+1].position[2]);

       glVertex3f( model->collision[i+1].position[0], model->collision[i+1].position[1]+0.05, model->collision[i+1].position[2]);
       glVertex3f( model->collision[i+2].position[0], model->collision[i+2].position[1]+0.05, model->collision[i+2].position[2]);

       glVertex3f( model->collision[i+2].position[0], model->collision[i+2].position[1]+0.05, model->collision[i+2].position[2]);
       glVertex3f( model->collision[i+3].position[0], model->collision[i+3].position[1]+0.05, model->collision[i+3].position[2]);

       glVertex3f( model->collision[i+3].position[0], model->collision[i+3].position[1]+0.05, model->collision[i+3].position[2]);
       glVertex3f( model->collision[i].position[0], model->collision[i].position[1]+0.05, model->collision[i].position[2]);
    }
    glEnd();
#endif

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
   if (height == 0) height = 1;  
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();          
   gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

void viewModel( Model mesh , int argc, char** argv ) {

   model = &mesh;
   glutInit(&argc, argv);           
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(800, 800);  
   glutInitWindowPosition(50, 50);
   glutCreateWindow("Model Visualisation");       

   glutDisplayFunc(viewModel__display_func);      
   glutReshapeFunc(reshape);      
   glutIdleFunc(viewModel__display_func);

   glutPassiveMotionFunc( mouseMotion );

   InitGL();                      
   glutMainLoop();                 
   return;
}

} // namespace
#endif
