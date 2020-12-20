#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
int obs_x=0;
int obs_y=0;
int obs_z=0;
double cameraHeight;
double cameraAngle;
int bullet=0;
int drawgrid;
int drawaxes;
int loop1=0;
int v1=0;
int v2=0;
int randx,randy,randz,ra=0;
int obx[50];
int oby[50];
int obz[50];
double road_lim=5000;
double moveground=0;
int keypressflag=1;
double angle;
double angle_c_z=0;
double angle_c_x=0, angle_t_x=0, angle_t_z=0;
double red=.8, green=.8, blue=1;
double limit=0.0;
bool hit=false;

struct point
{
	double x,y,z;
};

struct point pos,l,u,r;
double X=0,Z=0;
int Y=-1;
struct point arra[100];
void resetpos(){
    //while(angle_c_x>=0||angle_c_x<=0||angle_c_x>=0||angle_c_x<=0){
                //if(angle_c_x<=0){
                    angle_c_x=0;
                //}
                //if(angle_c_x>=0){
                    angle_c_x=0;
                //}
                //if(angle_c_z<=0){
                    angle_c_z=0;
                //}
                //if(angle_c_z>=0){
                    angle_c_z=0;
                //}
    //}
    printf("key up");
}
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(0.0f,1.0f,0.0f);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

            glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

            glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void draw_circle_line(double radius,int segments)
{
  int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_LINES);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_POLYGON);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}

void draw_cylinder(double radius,double height,int segments)
{
    int i;
     struct point points[2][100];
     for(i=0;i<=segments;i++)
    {
        points[0][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[0][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[0][i].z=0;
        points[1][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[1][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[1][i].z=height;
    }

    for (i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f((i)%2,.5,(i)%2);
				glVertex3f(points[0][i].x,points[0][i].y,points[0][i].z);
				glVertex3f(points[0][i+1].x,points[0][i+1].y,points[0][i+1].z);
				glVertex3f(points[1][i+1].x,points[1][i+1].y,points[1][i+1].z);
				glVertex3f(points[1][i].x,points[1][i].y,points[1][i].z);

			}glEnd();


    }
}
void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<int(segments-limit);i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,0,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,1,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void quad()
{
  // Draw A Quad
        glBegin(GL_QUADS);

            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Top front Of The Quad (right)
            glVertex3f(1.0f,1.0f,1.0f);					// Top back Of The Quad (right)
            glVertex3f(1.0f,-1.0f,1.0f);					// Bottom back Of The Quad (right)
            glVertex3f(1.0f,-1.0f,8.0f);					// Bottom front Of The Quad (right)

            glColor3f(1.0f,1.0f,0.0f);
            glVertex3f(-1.0f,1.0f,8.0f);					// Top front Of The Quad (left)
            glVertex3f(-1.0f,1.0f,1.0f);					// Top back Of The Quad (left)
            glVertex3f(-1.0f,-1.0f,1.0f);					// Bottom back Of The Quad (left)
            glVertex3f(-1.0f,-1.0f,8.0f);					// Bottom front Of The Quad (left)


            glColor3f(1.0f,0.0f,1.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Right front Of The Quad (top)
            glVertex3f(1.0f,1.0f,1.0f);					    // Right back Of The Quad (top)
            glVertex3f(-1.0f,1.0f,1.0f);				    // Left back Of The Quad (top)
            glVertex3f(-1.0f,1.0f,8.0f);				    // Left front Of The Quad (top)


            glColor3f(0.0f,1.0f,1.0f);
            glVertex3f(1.0f,-1.0f,8.0f);					// Right front Of The Quad (bottom)
            glVertex3f(1.0f,-1.0f,1.0f);					// Right back Of The Quad (bottom)
            glVertex3f(-1.0f,-1.0f,1.0f);				    // Left back Of The Quad (bottom)
            glVertex3f(-1.0f,-1.0f,8.0f);				    // Left front Of The Quad (bottom)


            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(1.0f,1.0f,1.0f);					 // Top Right Of The Quad (Back)
            glVertex3f(-1.0f,1.0f,1.0f);					// Top Left Of The Quad (Back)
            glVertex3f(-1.0f,-1.0f,1.0f);					// Bottom Left Of The Quad (Back)
            glVertex3f(1.0f,-1.0f,1.0f);					// Bottom Right Of The Quad (Back)


            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Top Right Of The Quad (Front)
            glVertex3f(-1.0f,1.0f,8.0f);					// Top Left Of The Quad (Front)
            glVertex3f(-1.0f,-1.0f,8.0f);					// Bottom Left Of The Quad (Front)
            glVertex3f(1.0f,-1.0f,8.0f);			// Bottom Right Of The Quad (Front)

        glEnd();
}

void movement3D()
{
    draw_circle_line(110,40);
    glColor3f(1,0,0);
    drawSphere(20,30,30);

    glPushMatrix();
    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSphere(15,30,30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle,0,1,0);
    //draw_circle_line(110,40);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSphere(15,30,30);

    glPopMatrix();

}
void drawSS()
{
    draw_circle_line(110,40);
    glColor3f(1,0,0);
    drawSphere(20,30,30);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSphere(15,30,30);;

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSphere(10,30,30);;
    }
    glPopMatrix();
    glColor3f(1,1,1);

    /*glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);*/
}
void keyboardListenerup(unsigned char key, int x, int y){
    //double x,y,z;
    double rate = 0.01;


	switch(key){
		case 'w':
            resetpos();
			break;
        case 's':

			resetpos();
			break;
        case 'a':
            resetpos();
			break;
        case 'd':
            resetpos();
            break;
        default:
		    	if(angle_c_x<=0){
                    angle_c_x+=.5;
                }
                if(angle_c_x>=0){
                    angle_c_x-=.5;
                }
                if(angle_c_z<=0){
                    angle_c_z+=.5;
                }
                if(angle_c_z>=0){
                    angle_c_z-=.5;
                }
        break;
	}
}
void keyboardListener(unsigned char key, int xx,int yy){
    double x,y,z;
    double rate = 0.01;


	switch(key){
		case '1':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+1*r.x*sin(rate);
			l.y = l.y*cos(rate)+r.y*sin(rate);
			l.z = l.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-x*sin(rate);
			r.y = r.y*cos(rate)-y*sin(rate);
			r.z = r.z*cos(rate)-z*sin(rate);}
			break;
        case '2':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+r.x*sin(-rate);
			l.y = l.y*cos(-rate)+r.y*sin(-rate);
			l.z = l.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-x*sin(-rate);
			r.y = r.y*cos(-rate)-y*sin(-rate);
			r.z = r.z*cos(-rate)-z*sin(-rate);
			}
			break;
        case '3':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '4':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+1*u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case '6':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(rate)+u.x*sin(rate);
			r.y = r.y*cos(rate)+u.y*sin(rate);
			r.z = r.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '5':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(-rate)+u.x*sin(-rate);
			r.y = r.y*cos(-rate)+u.y*sin(-rate);
			r.z = r.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case 'w':

            if(angle_c_z<45){
                angle_c_z+=1.5;
                keypressflag=0;
            }
            if ( angle_t_z<45){
                angle_t_z+=1.5;
            }
            break;
        case 's':

            if(angle_c_z>-45){
                angle_c_z-=1.5;
                keypressflag=0;
            }
            if ( angle_t_z>-45){
                angle_t_z-=1.5;
            }
            break;
        case 'd':
            if(angle_c_x<45){
                angle_c_x+=1.5;
                keypressflag=0;
            }
            if ( angle_t_x<45){
                    angle_t_x+=1.5;
            }
            break;
        case 'a':

            if(angle_c_x>-45){
                angle_c_x-=1.5;
                keypressflag=0;
            }
            if (angle_t_x>-45){
                    angle_t_x-=1.5;
            }

            break;
        case 'e':
            if ( angle_t_x<45){
                    angle_t_x+=1.5;
            }
            break;
        case 'q':
            if ( angle_t_x>-45){
                    angle_t_x-=1.5;
            }
            break;
         case 'r':
            if ( angle_t_z<45){
                    angle_t_z+=1.5;
            }
            break;
        case 'f':
            if(angle_c_x<=0){
                    angle_c_x+=3.5;
                }
                if(angle_c_x>=0){
                    angle_c_x-=3.5;
                }
                if(angle_c_z<=0){
                    angle_c_z+=3.5;
                }
                if(angle_c_z>=0){
                    angle_c_z-=3.5;
                }
            break;
		default:
		    	if(angle_c_x<=0){
                    angle_c_x+=.5;
                }
                if(angle_c_x>=0){
                    angle_c_x-=.5;
                }
                if(angle_c_z<=0){
                    angle_c_z+=.5;
                }
                if(angle_c_z>=0){
                    angle_c_z-=.5;
                }
        break;

        }

}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_LEFT :
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_RIGHT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:

			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				/*bullet =1;
				double length = 649;
				//double length = (649/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
                printf("---> %lf\n",length);
                //X = length* sin(angle_c_z*pi/180);
                X= length* tan(angle_c_z*pi/180);

                Z=-100*sin(angle_c_x*pi/180)-(649-100*cos(angle_c_x*pi/180))*tan((angle_t_x+angle_c_x)*pi/180);*/



			}
			break;


		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
				//(600/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
			}
			break;
			//........

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
                if(angle_c_x<=0){
                    angle_c_x+=.5;
                }
                if(angle_c_x>=0){
                    angle_c_x-=.5;
                }
                if(angle_c_z<=0){
                    angle_c_z+=.5;
                }
                if(angle_c_z>=0){
                    angle_c_z-=.5;
                }
			break;
	}
}

void aeroplane(){
    glTranslated(0,-angle_t_x,0);
    glTranslated(angle_t_z,0,0);
    glRotated(angle_c_x,0,0,1);
    glRotated(angle_c_z,0,1,0);


    glTranslatef(0.0f,-3.0f,0.0f);
     glRotated(-90,0,0,1);
     glRotated(180,0,1,0);
    glRotated(90,1,0,0);
    glBegin(GL_QUADS);
            //plane body
            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(3.0f,3.0f,5.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,20.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(0.0f,20.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,3.0f,0.0f);

            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(3.0f,3.0f,5.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,20.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(6.0f,20.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(6.0f,3.0f,0.0f);
            //plane body end


             //plane front
            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(3.0f,-3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,3.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(6.0f,3.0f,0.0f);					// Bottom back Of The Quad (right)

            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(3.0f,-3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,3.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(0.0f,3.0f,0.0f);

            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(3.0f,-3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,3.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(0.0f,3.0f,0.0f);

            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(3.0f,-3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,3.0f,5.0f);					// Top back Of The Quad (right)
            glVertex3f(6.0f,3.0f,0.0f);
            //plane front end

            //plane rare
            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(3.0f,20.0f,5.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,28.0f,10.0f);					// Top back Of The Quad (right)
            glVertex3f(3.0f,23.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(6.0f,20.0f,0.0f);

            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(3.0f,20.0f,5.0f);					    // Top front Of The Quad (right)
            glVertex3f(3.0f,28.0f,10.0f);					// Top back Of The Quad (right)
            glVertex3f(3.0f,23.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,20.0f,0.0f);
            //plane rare end

            //plane bottom
            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(3.0f,-3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(6.0f,3.0f,0.0f);					// Top back Of The Quad (right)
            glVertex3f(6.0f,20.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(3.0f,23.0f,0.0f);


            glVertex3f(3.0f,-3.0f,0.0f);
            glVertex3f(0.0f,3.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(0.0f,20.0f,0.0f);
            glVertex3f(3.0f,23.0f,0.0f);
            //plane bottom end

    glEnd();

    glTranslatef(3.0f,3.0f,2.0f);
            //plane main wing
    glBegin(GL_QUADS);
            glColor3f(.5f,0.5f,0.5f);
            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(18.0f,8.0f,0.0f);					// Top back Of The Quad (right)
            glVertex3f(18.0f,12.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,12.0f,0.0f);

            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(-18.0f,8.0f,0.0f);					// Top back Of The Quad (right)
            glVertex3f(-18.0f,12.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,12.0f,0.0f);


            glColor3f(.5f,0.5f,0.5f);
            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(18.0f,8.0f,.8f);					// Top back Of The Quad (right)
            glVertex3f(18.0f,12.0f,.8f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,12.0f,.8f);

            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(-18.0f,8.0f,.8f);					// Top back Of The Quad (right)
            glVertex3f(-18.0f,12.0f,.8f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,12.0f,.8f);

            glVertex3f(18.0f,12.0f,0.0f);
            glVertex3f(18.0f,12.0f,.8f);					// Top back Of The Quad (right)
            glVertex3f(-18.0f,12.0f,.8f);					// Bottom back Of The Quad (right)
            glVertex3f(-18.0f,12.0f,0.0f);
            ////plane main wing end
    glEnd();


            //rear wing
            glTranslatef(0.0f,19.5f,4.0f);
            glBegin(GL_QUADS);
            glColor3f(.5f,0.5f,0.5f);
            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(8.0f,2.0f,0.0f);					// Top back Of The Quad (right)
            glVertex3f(8.0f,3.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,3.0f,0.0f);

            glVertex3f(0.0f,0.0f,0.0f);					    // Top front Of The Quad (right)
            glVertex3f(-8.0f,2.0f,0.0f);					// Top back Of The Quad (right)
            glVertex3f(-8.0f,3.0f,0.0f);					// Bottom back Of The Quad (right)
            glVertex3f(0.0f,3.0f,0.0f);

            //rear wing end
    glEnd();
}
void buildings(int y,int z){

        glPushMatrix();
        //glColor3f(.15,.15,5);
        glTranslated(-25,y,z);//220 200
        //printf("%d\n",&v2);
        glutSolidCube(45);
        //v1 = rand() % 1;
        //v2 = (rand()%(5000 - 1 + 1))+1;
        glPopMatrix();
}
void obstacles(){
    glTranslatef(-15,-5,0);
    glRotatef(90,0,1,0);
    glBegin(GL_QUADS);
            glColor3f(1.0f,0.0f,0.5f);
            glVertex3f(0,0,0);					    // Top front Of The Quad (right)
            glVertex3f(0,10,0);					// Top back Of The Quad (right)
            glVertex3f(0,10,15);					// Bottom back Of The Quad (right)
            glVertex3f(0,0,15);

            glVertex3f(1,0,0);					    // Top front Of The Quad (right)
            glVertex3f(1,10,0);					// Top back Of The Quad (right)
            glVertex3f(1,10,15);					// Bottom back Of The Quad (right)
            glVertex3f(1,0,15);

            glVertex3f(0,0,0);					    // Top front Of The Quad (right)
            glVertex3f(1,0,0);					// Top back Of The Quad (right)
            glVertex3f(1,0,15);					// Bottom back Of The Quad (right)
            glVertex3f(0,0,15);

            glVertex3f(0,10,0);					    // Top front Of The Quad (right)
            glVertex3f(1,10,0);					// Top back Of The Quad (right)
            glVertex3f(1,10,15);					// Bottom back Of The Quad (right)
            glVertex3f(0,10,15);

            glVertex3f(0,0,0);					    // Top front Of The Quad (right)
            glVertex3f(1,0,0);					// Top back Of The Quad (right)
            glVertex3f(1,10,0);					// Bottom back Of The Quad (right)
            glVertex3f(0,10,0);

            glVertex3f(0,0,15);					    // Top front Of The Quad (right)
            glVertex3f(1,0,15);					// Top back Of The Quad (right)
            glVertex3f(1,10,15);					// Bottom back Of The Quad (right)
            glVertex3f(0,10,15);
     glEnd();
}
void Ground(){

   //for (int c = 1; c <= 1; c++) {

//obstacles
  /*glPushMatrix();
    if(ra<50){


     randx=rand() % 50 + 1;
    randy=rand() % 50 + 1;
    randz=rand() % 100 + 1;

    printf("%d",randx);
    printf("%d",randy);
    printf("%d",randz);


        ra++;
   }
   if(loop1<50){
   	  glTranslatef(obx[loop1],oby[loop1],obz[loop1]);

      loop1++;
   }
   if(loop1==50){
     loop1=0;
   }
    glPopMatrix();*/



    glPushMatrix();
    glTranslated(0,0,moveground);

    //obstacles
    glPushMatrix();
        glTranslatef(0,0,100);
        obstacles();
    //printf("x:%lf y:%lf\n",angle_t_x,angle_t_z);
        if(angle_t_x>0-5 && angle_t_x<0+5 && angle_t_z>0-5 && angle_t_z<0+5 && -moveground>100 && -moveground<105 ){
            printf("HIT");

        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(15,40,180);
        obstacles();
        //printf("x:%lf y:%lf\n",angle_t_x,angle_t_z);
        if(angle_t_x>-40-5 && angle_t_x<-40+5 && angle_t_z>15-10 && angle_t_z<15+10 && -moveground>180 && -moveground<185  ){
            printf("HIT");
        }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-15,25,300);
        obstacles();
        if(angle_t_x>-25-5 && angle_t_x<-25+5 && angle_t_z>-15-10 && angle_t_z<-15+10 && -moveground>300 && -moveground<305  ){
            printf("HIT");
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,-25,450);
        obstacles();
        if(angle_t_x>25-5 && angle_t_x<25+5 && angle_t_z>-0-10 && angle_t_z<-0+10 && -moveground>450 && -moveground<455  ){
            printf("HIT");
        }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(45,0,600);
        obstacles();
         if(angle_t_x>0-5 && angle_t_x<0+5 && angle_t_z>45-10 && angle_t_z<45+10 && -moveground>600 && -moveground<605  ){
            printf("HIT");
        }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15,15,800);
        obstacles();
         if(angle_t_x>-15-5 && angle_t_x<-15+5 && angle_t_z>15-10 && angle_t_z<15+10 && -moveground>800 && -moveground<805  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15,50,900);
        obstacles();
         if(angle_t_x>-50-5 && angle_t_x<-50+5 && angle_t_z>-15-10 && angle_t_z<-15+10 && -moveground>900 && -moveground<905  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(50,50,1000);
        obstacles();
         if(angle_t_x>50-5 && angle_t_x<50+5 && angle_t_z>-50-10 && angle_t_z<-50+10 && -moveground>1000 && -moveground<1005  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(45,0,1100);
        obstacles();
         if(angle_t_x>0-5 && angle_t_x<0+5 && angle_t_z>45-10 && angle_t_z<-45+10 && -moveground>1100 && -moveground<1105  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,45,1200);
        obstacles();
         if(angle_t_x>-45-5 && angle_t_x<-45+5 && angle_t_z>0-10 && angle_t_z<0+10 && -moveground>1200 && -moveground<1205  ){
            printf("HIT");
         }
    glPopMatrix();
        glPushMatrix();
        glTranslatef(0,0,2100);
        obstacles();
         if(angle_t_x>-0-5 && angle_t_x<-0+5 && angle_t_z>-0-10 && angle_t_z<-0+10 && -moveground>2100 && -moveground<2105  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15,40,2180);
        obstacles();
         if(angle_t_x>40-5 && angle_t_x<40+5 && angle_t_z>-15-10 && angle_t_z<-15+10 && -moveground>2180 && -moveground<2185 ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-15,25,2300);
        obstacles();
         if(angle_t_x>25-5 && angle_t_x<25+5 && angle_t_z>15-10 && angle_t_z<15+10 && -moveground>2300 && -moveground<2305  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,-25,2450);
        obstacles();
         if(angle_t_x>25-5 && angle_t_x<25+5 && angle_t_z>-0-10 && angle_t_z<-0+10 && -moveground>2450 && -moveground<2455  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(45,0,2600);
        obstacles();
         if(angle_t_x>0-5 && angle_t_x<0+5 && angle_t_z>45-10 && angle_t_z<45+10 && -moveground>2600 && -moveground<2605  ){
            printf("HIT");}
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15,15,2800);
        obstacles();
         if(angle_t_x>-15-5 && angle_t_x<-15+5 && angle_t_z>15-10 && angle_t_z<15+10 && -moveground>2800 && -moveground<2805  ){
            printf("HIT");}
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15,50,2900);
        obstacles();
         if(angle_t_x>-50-5 && angle_t_x<-50+5 && angle_t_z>-15-10 && angle_t_z<-15+10 && -moveground>2900 && -moveground<2905  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(50,50,2000);
        obstacles();
         if(angle_t_x>50-5 && angle_t_x<50+5 && angle_t_z>-50-10 && angle_t_z<-50+10 && -moveground>2000 && -moveground<2005 ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(45,0,3100);
        obstacles();
         if(angle_t_x>0-5 && angle_t_x<0+5 && angle_t_z>45-10 && angle_t_z<45+10 && -moveground>3100 && -moveground<3105  ){
            printf("HIT");
         }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,45,3200);
        obstacles();
         if(angle_t_x>-45-5 && angle_t_x<-45+5 && angle_t_z>0-10 && angle_t_z<0+10 && -moveground>3200 && -moveground<3205  ){
            printf("HIT");
         }
    glPopMatrix();
    //obstacles end

    glTranslated(50,0,0);//lower than the plane
    glTranslated(0,-125,0);//set in middle position

    for(int i=0;i<50;i++){
        v1=i*100;
        glPushMatrix();
        glColor3f(.8,.5,0);
        buildings(25,v1);
        buildings(220,v1);

        glPushMatrix();
        glColor3f(.15,.15,5);
        buildings(25,v1+49);
        buildings(220,v1+49);
        glPopMatrix();

        glPopMatrix();


    }



    glBegin(GL_QUADS);
        glColor3f(.4,.5,.3);
        glVertex3f(0,0,0);
        glVertex3f(0,250,0);
        glVertex3f(0,250,road_lim);
        glVertex3f(0,0,road_lim);
    glEnd();
    glPopMatrix();
}

void drawsun(){
    glPushMatrix();
        glTranslatef(-200,-500,5000);
        glColor3f(0,0,0);
        drawCircle(500,24);
    glPopMatrix();
}
void drawEnd()
{
    glPushMatrix();
    glTranslatef(100,100,0);
    glRotated(90,1,0,0);
    glRotated(90,0,0,1);
    glBegin(GL_QUADS);
        glVertex3f(0,0,0);
        glVertex3f(0,200,0);
        glVertex3f(0,200,200);
        glVertex3f(0,0,200);
    glEnd();
    glPopMatrix();
}
void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(red,green,blue,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(angle_t_z-25,-angle_t_x,pos.z-40,	angle_t_z,-angle_t_x,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	//drawEnd();
    drawsun();
	//drawAxes();
	drawGrid();

    Ground();
    aeroplane();


    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    //movement3D();
    //drawSphere(50,20,10);

    //drawCone(5,10,50);
    //draw_cylinder(10,10,30);

    //draw_cylinder(2,500,segment);
    //100*sin(angle_c_x)+(500-100*cos(angle_c_x))*sin(angle_t_x);




    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){



	//display();
    if(moveground > -road_lim){
        moveground-=.03;
        //resetpos();
    }
    if(red>0.0){   //////for changing day to night
        red-=0.0001;
    }
    if(green>0){
        green-=0.0001;
    }
    if(blue>0){
        blue-=0.0001;
    }
	angle+=.1;
	//codes for any changes in Models, Camera
	limit+=0.01;
	if(limit>=50.0)
    {
        limit=0;
    }
	glutPostRedisplay();

}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	//cameraHeight=150.0;
	//cameraAngle=1.0;
	//angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	5000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance


}

int main(int argc, char **argv){
    pos.x=0;
    pos.y=0;
    pos.z=-20;
    l.x=0;u.x=-1;r.x=0;
    l.y=0;u.y=0;r.y=1;
    l.z=1;u.z=0;r.z=0;
	glutInit(&argc,argv);
	glutInitWindowSize(1366,768);
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Space Crush");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutKeyboardUpFunc(keyboardListenerup);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}





















/*
void drawSphere(double radius,int slices,int stacks,int up)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		if(up==3){
                r= 2*radius- radius*cos(((double)i/(double)stacks)*(pi/2));
		}else{
		     r=  radius*cos(((double)i/(double)stacks)*(pi/2));
		}
		for(j=0;j<=slices;j++)
		{
		    if(up!=3){
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		    }
		    else{
            points[i][j].x= r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=(r*sin(((double)j/(double)slices)*2*pi));
			points[i][j].z=h;

		    }
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f(j%2,j%2,j%2);
			    if(up==1 or up==-1 or up==3)
                {
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                }
                //lower hemisphere
                if(up==0 or up==-1)
                {

                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }


			}glEnd();
		}
	}
}


void offline()
{

    glBegin(GL_QUADS);{
        //upper hemisphere
        glColor3f(.1,.1,.1);
        glVertex3f(200,-650,200);
        glVertex3f(200,-650,-200);
        glVertex3f(-200,-650,-200);
        glVertex3f(-200,-650,200);

    }glEnd();


    if(bullet==1 && abs(X)<200 && abs(Z)<200){
    Y++;
    arra[Y].x=X;
    arra[Y].z = Z;
    printf("%lf %lf %lf %lf\n",X,Z,angle_c_x,angle_c_z);

	bullet =0;

    }
    int j;
    for(j=0;j<=Y;j++){
        glColor3f(1.0,0.0,0.0);

            glBegin(GL_QUADS);{
            glVertex3f(arra[j].x+5 ,-649, arra[j].z+5);
            glVertex3f(arra[j].x+5,-649, arra[j].z-5);
            glVertex3f(arra[j].x-5,-649,arra[j].z-5 );
            glVertex3f(arra[j].x-5, -649, arra[j].z+5);
        }glEnd();
    }

    int segment =30;
    glRotatef(angle_c_z,0,0,1);
    glRotatef(angle_c_x,1,0,0);
    glRotatef(90,1,0,0);
    glTranslatef(0,0,20);
    drawSphere(20,segment,segment,0);
    draw_cylinder(20,100,segment);
    glTranslatef(0,0,100);
    drawSphere(20,segment,segment,1);
    double r,l;
    r=15;
    l=170;
    glTranslatef(0,0,20);

    glRotatef(angle_t_x,1,0,0);
    glRotatef(angle_t_z,0,0,1);

    glTranslatef(0,0,r);
    drawSphere(r,segment,segment,0);
    draw_cylinder(r,l,segment);
    glTranslatef(0,0,l);
    drawSphere(r,segment,segment,3);
}
*/
