#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#define PI 3.14159
#define SPACEBAR 32

void init2D(float r, float g, float b)
{
	glClearColor(r, g, b, 0.0);
	glMatrixMode(GL_PROJECTION);
}
template<typename T> struct Queue{
	const int n = 105;
	T arr[105];
	int Front = 0;
	int Rear = -1;
	void push(T k){
		Rear = (Rear+1)%n;
		arr[Rear] = k;
		return ;
	}
	void pop(){
		Front = (Front+1)%n;
		return ;
	}
	T front(){
		return arr[Front];
	}
	T rear(){
		return arr[Rear];
	}
	int size(){
		return Rear - Front + 1;
	}
	int max_size(){
		return n;
	}
};
struct cac{
	int x,breadth,length;
};
Queue<struct cac>q;
int rot_x = 0;
int rot_y = 0;
int rot_sun = 0;
double trex_height = -300;
int trex_length = 80;
int trex_breadth = 40;
int flag = 0;
int sub = 5;
double t = 0;
int score = 0;
int min_distance = 300;
void draw_cactus(int x,int breadth,int length){
	glColor3f(0.1,0.6,0.2);
	int y = -300;
	int z = 0;
	glBegin(GL_POLYGON);
		glVertex3f(x/500.0,(y)/500.0,0/500.0);
		glVertex3f(x/500.0,(y+length)/500.0,0/500.0);
		glVertex3f((x+breadth)/500.0,(y+length)/500.0,0/500.0);
		glVertex3f((x+breadth)/500.0,(y)/500.0,0/500.0);
	glEnd();
}
int check_collision(){
	int cactus_x = q.front().x;
	int cactus_breadth = q.front().breadth;
	int cactus_length = q.front().length;
	int trex_x = -430;
	int fl = 0;
	if(trex_height <= (-300 + cactus_length) && trex_x < cactus_x && trex_x + trex_breadth > cactus_x){
		fl = 1;
	}
	else if(trex_height <= -300 + cactus_length && trex_x > cactus_x && trex_x < cactus_x + cactus_breadth){
		fl = 1;
	}
	return fl;
}
void draw_sun(){
	double r = 0;
	double theta = 0;
	int i;
	glPushMatrix();
	glTranslatef(300/500.0,300/500.0,0/500.0);
	glRotatef(rot_sun,0.0,0.0,1.0);
	glLineWidth(1.5);
	glBegin(GL_LINE_STRIP);
		glColor3f(0.95,0.7,0.05);
		for(i=0;i<185;i++){
			double theta_rad = (PI/180.0)*theta;
			double x = r*cos(theta_rad);
			double y = r*sin(theta_rad);
			double z = 0;
			glVertex3f(x/500.0,y/500.0,z/500.0);
			theta+=10;
			r = r+0.5;
		}
	glEnd();
	glPopMatrix();
	rot_sun++;
	glutPostRedisplay();
}
void display_obstacle(int value){
	int front = q.Front;
	int rear = q.Rear;
	int end = (rear+1)%q.max_size();
	int v = 10;
	int count = 0;
	while(front!=end){
		glPushMatrix();
		glTranslatef(-sub/500.0,0/500.0,0/500.0);
		draw_cactus(q.arr[front].x,q.arr[front].breadth,q.arr[front].length);
		glPopMatrix();
		q.arr[front].x-=sub;
		front = (front+1)%q.max_size();
	}
	int k = check_collision();
	if(k == 1){
		printf("Your Final Score is: %d\n", score);
		printf("Game Over\n");
		exit(1);
	}
	if(q.front().x + q.front().breadth < -500){
		q.pop();
		score++;
		if(score%10==0){
			printf("Checkpoint Score: %d\n",score);
		}
		if(score%10==0){
			sub++;
			min_distance+=50;
		}
		int last_x = q.rear().x;
		int random_x = rand()%400 + min_distance;
		int random_breadth = rand()%30 + 30;
		int random_length = rand()%40 + 50;
		q.push({last_x + random_x,random_breadth,random_length});
	}
	glutPostRedisplay();
	// glutTimerFunc(5,display_obstacle,0);
}
void display_road(){
	int x = 500;
	int y = -300;
	int z = 0;
	draw_sun();
	glBegin(GL_POLYGON);
		glColor3f(0.9,0.95,0.95);
		glVertex3f(-x/500.0,500.0/500.0,(z+1)/500.0);
		glVertex3f(x/500.0,500.0/500.0,(z+1)/500.0);
		glVertex3f(x/500.0,y/500.0,(z+1)/500.0);
		glVertex3f(-x/500.0,y/500.0,(z+1)/500.0);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0.2,0.2,0.2);
		glVertex3f(-x/500.0,y/500.0,z/500.0);
		glVertex3f(x/500.0,y/500.0,z/500.0);
		glVertex3f(x/500.0,(y-5)/500.0,z/500.0);
		glVertex3f(-x/500.0,(y-5)/500.0,z/500.0);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0.9,0.6,0.1);
		glVertex3f(-x/500.0,(y-5)/500.0,(z+1)/500.0);
		glVertex3f(x/500.0,(y-5)/500.0,(z+1)/500.0);
		glVertex3f(x/500.0,-500/500.0,(z+1)/500.0);
		glVertex3f(-x/500.0,-500/500.0,(z+1)/500.0);
	glEnd();
}
void animate_trex(int value){
	if(flag == 1){
		double v = 12;
		double tmp = v - 8*(2*t);
		trex_height += tmp;
		// printf("%lf\n",trex_height);
		t+=0.02;
		// printf("t => %lf\n",t);
		if(trex_height<-300){
			trex_height = -300;
			flag=0;
			return ;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(3,animate_trex,0);
}
void draw_trex(){
	glColor3f(0.5,0.5,0.9);
	int x = -430;
	int y = -300;
	int z = 0;
	glBegin(GL_POLYGON);
		glVertex3f(x/500.0,y/500.0,z/500.0);
		glVertex3f((x+trex_breadth)/500.0,y/500.0,z/500.0);
		glVertex3f((x+trex_breadth)/500.0,(y+trex_length)/500.0,z/500.0);
		glVertex3f(x/500.0,(y+trex_length)/500.0,z/500.0);
	glEnd();
}
void display_trex(){
	glPushMatrix();
	glTranslatef(0,(trex_height+300)/500.0,0);
	draw_trex();
	glPopMatrix();
	glutPostRedisplay();
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(rot_x,1.0,0.0,0.0);
	glRotatef(rot_y,0.0,1.0,0.0);
	display_road();
	display_trex();
	display_obstacle(0);
	glutSwapBuffers();
}
void key_press(unsigned char key,int x,int y){
	if(key==' '){
		if(flag == 0){
			t = 0.2;
			flag = 1;
			animate_trex(0);
		}
	}
}
int main(int argc,char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Trex-game");
	glEnable(GL_DEPTH_TEST);
	init2D(1,1,1);
	srand(time(NULL));
	q.push({300,30,50});
	q.push({700,50,70});
	q.push({1200,40,60});
	glutDisplayFunc(display);
	glutKeyboardFunc(key_press);
	glutMainLoop();
}