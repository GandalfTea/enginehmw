
#include <GL/glut.h>
#include <Procedural.h>
#include <GeometricPrimitives.h>
#include <Coordinates.h>
#include <Object.h>

#define FPS 60
#define W 1920
#define H 1080

#define SHOW_VERTICES
#define SHOW_TOPOGRAPHY
//#define SHOW_COLLISION
//#define SHOW_NORMALS


using namespace MEGA;


typedef enum {
    MEGA_TRIANGLES,
    MEGA_QUADS
} ModelTopography;


// Models
ProceduralTerrain a( 6942069, 3, 3, 1500, 150, true);
Model sphere = Sphere(1, 10, 10);
Model sphere_render = Sphere(1, 10, 10);
Matrix<F32C1> sphere_translation;


// Global variables
float view_pitch = 0.0f, view_yaw = 0.0f;
float player_x = 0.0, player_y = -1.0, player_z = 0.0;
float mouse_sensitivity = 0.001;
float movement_sensitivity = 0.005;
bool first_render = true;
bool check_collision = false;
int collision_idx = 0;
bool lock_view = false;


// Movement conditionals
struct Motion { bool forward, backward, left, right, up, down; };
struct Collision { bool forward, backward, left, right, up, down; };
Motion motion = { false, false, false, false, false, false };
Collision collision = { false, false, false, false, false, false };



void init() {
   //glClearColor(0.79f, 0.91f, 0.96f, 1.0f);   // CLEAR SKY COLOR
   glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
   glClearDepth(1.0f);                 
   glEnable(GL_DEPTH_TEST); 
   glDepthFunc(GL_LEQUAL);
   glShadeModel(GL_SMOOTH); 
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

   glutWarpPointer(W/2, H/2);  // move cursor to center of screen
   glutSetCursor( GLUT_CURSOR_NONE ); // hide cursor
}


void drawModel( Model* model, ModelTopography render_method ) {

#ifdef SHOW_VERTICES
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 1.0f);
    for( size_t i{}; i < model->vertices.size(); i++ ) {
       glVertex3f( model->vertices[i].position[0], model->vertices[i].position[1]+0.001, model->vertices[i].position[2]);
    }
    glEnd();
#endif

#ifdef SHOW_TOPOGRAPHY
    if( render_method == MEGA_QUADS ) {
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
                glVertex3f( x[0], x[1], x[2]);
                glVertex3f( y[0], y[1], y[2]);
                glVertex3f( z[0], z[1], z[2]);
                glVertex3f( w[0], w[1], w[2]);
            }
        glEnd();

    } else if ( render_method == MEGA_TRIANGLES ) {


        // Because the only object in the scene that is made from triangles is the collision sphere
        // We only render it as lines in order to see outside of it.
        glBegin(GL_LINES);                
            glLineWidth(0.001);
            glColor3f(1.0f, 0.7f, 0.0f);
            for( size_t i = 0; i < model->triangles.size(); i += 3) {
                float x [3];
                float y [3];
                float z [3]; 
                x[0] = model->vertices[ model->triangles[i]].position[0];
                x[1] = model->vertices[ model->triangles[i]].position[1];
                x[2] = model->vertices[ model->triangles[i]].position[2];

                y[0] = model->vertices[ model->triangles[i + 1]].position[0];
                y[1] = model->vertices[ model->triangles[i + 1]].position[1];
                y[2] = model->vertices[ model->triangles[i + 1]].position[2];

                z[0] = model->vertices[ model->triangles[i + 2]].position[0];
                z[1] = model->vertices[ model->triangles[i + 2]].position[1];
                z[2] = model->vertices[ model->triangles[i + 2]].position[2];

                glVertex3f( x[0], x[1], x[2]);
                glVertex3f( y[0], y[1], y[2]);

                glVertex3f( y[0], y[1], y[2]);
                glVertex3f( z[0], z[1], z[2]);

                glVertex3f( z[0], z[1], z[2]);
                glVertex3f( x[0], x[1], x[2]);

            }
        glEnd();
    }
#endif

#ifdef SHOW_NORMALS
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
            glVertex3f(oX+(x/15), oY+(y/15), oZ+(z/15));
            glVertex3f(oX, oY, oZ);
        }
    glEnd();
#endif

#ifdef SHOW_COLLISION
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for( size_t i{}; i < model->collision.size(); i += 4 ) {
       glVertex3f( model->collision[i].position[0], model->collision[i].position[1]+0.01, model->collision[i].position[2]);
       glVertex3f( model->collision[i+1].position[0], model->collision[i+1].position[1]+0.01, model->collision[i+1].position[2]);

       glVertex3f( model->collision[i+1].position[0], model->collision[i+1].position[1]+0.01, model->collision[i+1].position[2]);
       glVertex3f( model->collision[i+2].position[0], model->collision[i+2].position[1]+0.01, model->collision[i+2].position[2]);

       glVertex3f( model->collision[i+2].position[0], model->collision[i+2].position[1]+0.01, model->collision[i+2].position[2]);
       glVertex3f( model->collision[i+3].position[0], model->collision[i+3].position[1]+0.01, model->collision[i+3].position[2]);

       glVertex3f( model->collision[i+3].position[0], model->collision[i+3].position[1]+0.01, model->collision[i+3].position[2]);
       glVertex3f( model->collision[i].position[0], model->collision[i].position[1]+0.01, model->collision[i].position[2]);
    }
    glEnd();
#endif
}

void detectCollision() {
    if( check_collision ) {
        // We will be checking if any quad point is closer to the circle origin than the radius
        // The collision of the terrain is wierd non-planar points.
        for( auto i : sphere_render.vertices ) {
            for( auto j : a.terrain.collision ) {
                float diff_x = std::abs( i.position[0] - j.position[0] );
                float diff_y = std::abs( i.position[1] - j.position[1] );
                float diff_z = std::abs( i.position[2] - j.position[2] );

                if( diff_x <= 1 && diff_y <= 1) {
                    if(i.position[0] < j.position[0]) collision.forward = true; 
                    if(i.position[0] > j.position[0]) collision.backward = true; 
                    std::cout << collision.forward << " " << collision.backward << " " << collision.up << " " << collision.down << " ";
                    std::cout << collision.left << " " << collision.right << std::endl;
                    return;
                } else if (diff_y <= 1) {
                    if(i.position[1] < j.position[1]) collision.up = true; 
                    if(i.position[1] > j.position[1]) collision.down = true; 
                    std::cout << collision.forward << " " << collision.backward << " " << collision.up << " " << collision.down << " ";
                    std::cout << collision.left << " " << collision.right << std::endl;
                    return;
                } else if( diff_z <= 1 && diff_y <= 1) {
                    if(i.position[3] > j.position[3]) collision.left = true; 
                    if(i.position[3] < j.position[3]) collision.right = true; 
                    std::cout << collision.forward << " " << collision.backward << " " << collision.up << " " << collision.down << " ";
                    std::cout << collision.left << " " << collision.right << std::endl;
                    return;
                }
            }
        }
        collision.forward = false;
        collision.backward = false;
        collision.up = false;
        collision.down = false;
        collision.left = false;
        collision.right = false;
        std::cout << collision.forward << " " << collision.backward << " " << collision.up << " " << collision.down << " ";
        std::cout << collision.left << " " << collision.right << std::endl;
    }
}

void moveCameraCollision() {
    try {
        for( size_t i{}; i <= sphere_render.vertices.size(); i++) {
            sphere_render.vertices[i].position[0] = sphere.vertices[i].position[0] + -player_x;
            sphere_render.vertices[i].position[1] = sphere.vertices[i].position[1] + -player_y;
            sphere_render.vertices[i].position[2] = sphere.vertices[i].position[2] + -player_z;
        }
    } catch(...) {
        std::cout << "Unexpected exception when moving camera collision" << std::endl;
    }
}


void moveCamera() {

    if( first_render ) {
        gluLookAt( 0, 0.0f, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f); 
        first_render = false;
    }

    // NOTE : All moements are inverted because the camera is upside down.
    // For the time being, I cannot find a reason for the wrong up vector.
    if( motion.forward && !collision.forward) { 
        player_x += cos(degrees_to_radians((view_yaw+90+180))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90+180))) * movement_sensitivity;
        moveCameraCollision();

    } else if ( motion.backward && !collision.backward) {
        player_x += cos(degrees_to_radians((view_yaw+90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90))) * movement_sensitivity;
        moveCameraCollision();

    } else if ( motion.left && !collision.left) {
        player_x += cos(degrees_to_radians((view_yaw+90-90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90-90))) * movement_sensitivity;
        moveCameraCollision();

    } else if ( motion.right && !collision.right) {
        player_x += cos(degrees_to_radians((view_yaw+90+90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90+90))) * movement_sensitivity;
        moveCameraCollision();

    } else if ( motion.up && !collision.up) {
        player_y -= 1.f * movement_sensitivity;
        moveCameraCollision();

    } else if ( motion.down && !collision.down) {
        player_y += 1.f * movement_sensitivity;
        moveCameraCollision();

    }

    // Move the collision sphere along with the camera

    if( view_pitch >=  89) view_pitch =  89;
    if( view_pitch <= -89) view_pitch = -89;
    glRotatef(-view_pitch, 1.0, 0.0, 0.0);
    glRotatef(-view_yaw, 0.0, 1.0, 0.0);
    glTranslatef( player_x, player_y, player_z);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();                

    // do collision
    collision_idx++;
    detectCollision();

    moveCamera();
    drawModel( &a.terrain, MEGA_QUADS );
    //drawModel( &sphere, MEGA_TRIANGLES );
    
    glutSwapBuffers();
}



//      KEYBOARD INPUT
void keyboard( unsigned char key, int x, int y ) {
    switch(key) {
        case 'W':
        case 'w':
            motion.forward = true;
            break;
        case 'A':
        case 'a':
            motion.left = true;
            break; 

        case 'S':
        case 's':
            motion.backward = true;
            break;
        case 'D':
        case 'd':
            motion.right = true;
            break;
        case ' ':
        case 'e':
            motion.up = true;
            break;
        case 'q':
            motion.down = true;
            break;
        case 'c':
            (check_collision) ? check_collision = false : check_collision = true;
            if(!check_collision) {
                collision.forward = false;
                collision.backward = false;
                collision.up = false;
                collision.down = false;
                collision.left = false;
                collision.right = false;
            }
            break;
        case 'v':
           lock_view ? lock_view = false : lock_view = true; 
            
    }
}

void keyboard_up( unsigned char key, int x, int y ) {
    switch(key) {
        case 'W':
        case 'w':
            motion.forward = false;
            break;
        case 'A':
        case 'a':
            motion.left = false;
            break; 

        case 'S':
        case 's':
            motion.backward = false;
            break;
        case 'D':
        case 'd':
            motion.right = false;
            break;
        case ' ':
        case 'e':
            motion.up = false;
            break;
        case 'q':
            motion.down = false;
            break;
    }
}


void reshape( int w, int h ) {
    if (h == 0) h = 1;  
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    glViewport(0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60, aspect, 0.001f, 100);
    //gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

void timer(int) {
    glutPostRedisplay();
    if(lock_view) glutWarpPointer(W/2, H/2);  // move cursor to center of screen
    glutTimerFunc(1000/FPS, timer, 0);
}

void mouseMotion( int x, int y ) {
    int new_x = (W/2)-x;
    int new_y = (H/2)-y;
    view_yaw += (float)new_x * mouse_sensitivity;
    view_pitch += (float)new_y * mouse_sensitivity;
}

int main(int argc, char** argv) {

//    Object sky;
//    sky.model = sphere;
    sphere_translation.translation(0, 0, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( W, H );
    glutCreateWindow( "First Person Demo" );

    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutTimerFunc(0, timer, 0);

    // Input
    glutPassiveMotionFunc( mouseMotion );
    glutKeyboardFunc( keyboard );
    glutKeyboardUpFunc( keyboard_up );

    glutMainLoop();
}
