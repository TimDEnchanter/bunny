// main.c

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include "plyparse.h"

float eye[] = {3.0,3.0,3.0};
float viewpt[] = {0.0,0.0,0.0};
float up[] = {0.0,1.0,0.0};
float light0_position[] = {3.0,3.0,0.0,1.0};
float light0_diffuse[] = {1.0,1.0,1.0,1.0};
float light0_specular[] = {1.0,1.0,1.0,1.0};
float light1_position[] = {0.0,3.0,3.0,1.0};
float light1_diffuse[] = {0.5,0.5,0.5,1.0};
float light1_specular[] = {0.5,0.5,0.5,1.0};
float light2_position[] = {0.0,3.0,-3.0,1.0};
float light2_diffuse[] = {0.5,0.5,0.5,1.0};
float light2_specular[] = {0.5,0.5,0.5,1.0};

ply p;

char *read_shader_program(char *filename) 
{
FILE *fp;
char *content = NULL;
int fd, count;
fd = open(filename,O_RDONLY);
count = lseek(fd,0,SEEK_END);
close(fd);
content = (char *)calloc(1,(count+1));
fp = fopen(filename,"r");
count = fread(content,sizeof(char),count,fp);
content[count] = '\0';
fclose(fp);
return content;
}

void set_light()
{
//enable lighting
glEnable(GL_LIGHTING);

//light 0
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
//light 1
glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
//light 2
glLightfv(GL_LIGHT2,GL_POSITION,light2_position);
glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
glLightfv(GL_LIGHT2,GL_SPECULAR,light2_specular);

//enable lights
glEnable(GL_LIGHT0);
glEnable(GL_LIGHT1);
glEnable(GL_LIGHT2);
} 

void set_material()
{
float mat_diffuse[] = {0.55,0.47,0.33,1.0};
float mat_specular[] = {0.95,0.87,0.73,1.0};
float mat_shininess[] = {20.0};

glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void view_volume()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,1.0,20.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye[0],eye[1],eye[2],viewpt[0],viewpt[1],viewpt[2],up[0],up[1],up[2]);
}

void renderScene(void)
{
glClearColor(0.5,0.4,0.3,1.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//load object here
glTranslatef(0.0, -1.0, 0.0);
glScalef(10.0, 10.0, 10.0);
glDrawArrays(GL_TRIANGLES, 0, 3*p.fsize);
glutSwapBuffers();
}

unsigned int set_shaders()
{
GLint vertCompiled, fragCompiled;
char *vs, *fs;
GLuint v, f, p;
int result = -1;

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs = read_shader_program("bunny.vert");
fs = read_shader_program("bunny.frag");
glShaderSource(v,1,(const char **)&vs,NULL);
glShaderSource(f,1,(const char **)&fs,NULL);
free(vs);
free(fs); 
glCompileShader(v);
glCompileShader(f);
glGetShaderiv(f,GL_COMPILE_STATUS,&result);
fprintf(stderr,"%d\n",result);
p = glCreateProgram();
glAttachShader(p,f);
glAttachShader(p,v);
glLinkProgram(p);
glUseProgram(p);
return(p);
}

void getout(unsigned char key, int x, int y)
{
switch(key) {
        case 'q':
                exit(1);
        default:
                break;
    }
}

int main(int argc, char **argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
glutInitWindowPosition(100, 100);
glutInitWindowSize(768,768);
glutCreateWindow("bunny");
glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE_ARB);
glEnable(GL_NORMALIZE);
view_volume();
set_light();
set_material();
set_shaders();

//setup the element buffer
FILE * f;
p = new_ply(f);
GLfloat * obj_data = malloc(2*3*3*p.fsize*sizeof(GLfloat));
int i;
for (i=0; i<(3*p.fsize); i++) {
	//add vert
	obj_data[3*i] = p.vert[3*p.face[i]];
	obj_data[3*i+1] = p.vert[3*p.face[i]+1];
	obj_data[3*i+2] = p.vert[3*p.face[i]+2];

	//add normal
	obj_data[(3*3*p.fsize)+3*i] = p.norm[3*p.face[i]];
	obj_data[(3*3*p.fsize)+3*i+1] = p.norm[3*p.face[i]+1];
	obj_data[(3*3*p.fsize)+3*i+2] = p.norm[3*p.face[i]+2];
}

//assign element buffer
int bunny_buf = 1;
glBindBuffer(GL_ARRAY_BUFFER, bunny_buf);
glBufferData(GL_ARRAY_BUFFER, 2*3*3*p.fsize*sizeof(GLfloat), obj_data, GL_STATIC_DRAW);
glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), NULL+0);
glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), NULL+(3*3*p.fsize*sizeof(GLfloat)));
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);

glutDisplayFunc(renderScene);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}
