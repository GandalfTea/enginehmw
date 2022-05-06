
#include <GL/glut.h>
#include <Procedural.h>
#include <Object.h>

#define FPS 60
#define W 1920
#define H 1080

#define SHOW_VERTICES
#define SHOW_QUADS
//#define SHOW_COLLISION

using namespace MEGA;

ProceduralTerrain a( 6942069, 3, 3, 600, 50, true);
Model* model = &a.terrain;
float view_pitch = 0.0f, view_yaw = 0.0f;
float player_x = 0.0, player_y = -1.0, player_z = 0.0;
float mouse_sensitivity = 0.001;
float movement_sensitivity = 0.01;
bool first_render = true;

struct Motion { bool forward, backward, left, right, up, down; };
Motion motion = { false, false, false, false, false, false };

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

void drawTerrain() {

    //glTranslatef(0, 0.0f, -2.0f);  

#ifdef SHOW_VERTICES
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 1.0f);
    for( size_t i{}; i < model->vertices.size(); i++ ) {
       glVertex3f( model->vertices[i].position[0], model->vertices[i].position[1]+0.01, model->vertices[i].position[2]);
    }
    glEnd();
#endif

#ifdef SHOW_QUADS
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


//      MOUSE POSITION
void moveCamera() {

    if( first_render ) {
        gluLookAt( 0, 0.0f, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f); 
        first_render = false;
    }

    // NOTE : All moements are inverted because the camera is upside down.
    // For the time being, I cannot find a reason for the wron up vector.
    if( motion.forward ) { 
        player_x += cos(degrees_to_radians((view_yaw+90+180))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90+180))) * movement_sensitivity;
    } else if ( motion.backward ) {
        player_x += cos(degrees_to_radians((view_yaw+90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90))) * movement_sensitivity;
    } else if ( motion.left ) {
        player_x += cos(degrees_to_radians((view_yaw+90-90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90-90))) * movement_sensitivity;
    } else if ( motion.right ) {
        player_x += cos(degrees_to_radians((view_yaw+90+90))) * movement_sensitivity;
        player_z -= sin(degrees_to_radians((view_yaw+90+90))) * movement_sensitivity;
    } else if ( motion.up ) {
        player_y -= 1.f * movement_sensitivity;
    } else if ( motion.down ) {
        player_y += 1.f * movement_sensitivity;
    }

    if( view_pitch >=  89) view_pitch =  89;
    if( view_pitch <= -89) view_pitch = -89;
    // gluLookAt( 1, 1.0f, 2, 0, 0, 0, 0.0f, 1.0f, 0.0f); TODO: IMPLEMENT THIS IN OBJECT VIEWER
    glRotatef(-view_pitch, 1.0, 0.0, 0.0);
    glRotatef(-view_yaw, 0.0, 1.0, 0.0);
    glTranslatef( player_x, player_y, player_z);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();                

    moveCamera();
    drawTerrain();
    
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
    gluPerspective(60, aspect, 0.1f, 100);
    //gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

void timer(int) {
    glutPostRedisplay();
    glutWarpPointer(W/2, H/2);  // move cursor to center of screen
    glutTimerFunc(1000/FPS, timer, 0);
}

void mouseMotion( int x, int y ) {
    int new_x = (W/2)-x;
    int new_y = (H/2)-y;
    view_yaw += (float)new_x * mouse_sensitivity;
    view_pitch += (float)new_y * mouse_sensitivity;
}

int main(int argc, char** argv) {
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
