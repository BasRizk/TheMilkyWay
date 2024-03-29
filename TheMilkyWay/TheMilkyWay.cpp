// TheMilkyWay.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define PRESPECTIVE_RATIO 0.0000001
#define PRESPECTIVE_SCALE_OF_DIST 20000 * PRESPECTIVE_RATIO
#define ANGULAR_ACC_RATIO 0.01
class Planet {
public:
	double rad;
	double how_far_from_sun;
	double current_rot_about_sun;
	double speed_of_rot_about_sun;
	double current_rot_about_axis;
	double speed_rot_about_axis;
	float r, g, b;
	Planet() {}
	void Set(double _rad, double _how_far, double _rot_about_sun, double _rot_about_axis, float _r, float _g, float _b) {
		rad = _rad * PRESPECTIVE_RATIO;
		how_far_from_sun = _how_far * PRESPECTIVE_SCALE_OF_DIST;
		speed_of_rot_about_sun = _rot_about_sun/360;
		speed_rot_about_axis = _rot_about_axis*1000/360;
		current_rot_about_sun = 0;
		current_rot_about_axis = 0;
		r = _r; g = _g; b = _b;
	}

};

Planet *all_planets = new Planet[10];

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60, 1080 / 720, 0.001, 100);
	gluPerspective(35, 1920 / 1080, PRESPECTIVE_SCALE_OF_DIST, 1000000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawOrbit(double rad) {
	//rad = rad * PRESPECTIVE_SCALE_OF_DIST;
	glPushMatrix();
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
	double current_x_pos;
	double current_z_pos;
	double theta_in_radians;
	for (double i = 0; i < 360; i += 0.5) {
		
		theta_in_radians = i * 3.14 / 180;

		current_z_pos = cos(theta_in_radians)*rad;
		current_x_pos = sin(theta_in_radians)*rad;

		glVertex3d(current_x_pos, 0, current_z_pos);
		
	}
	glEnd();
	glPopMatrix();
}
void drawPlanet(double rad, double how_far,
	double current_rot_about_sun, double current_rot_about_axis,
	float r, float g, float b) 
{
	drawOrbit(how_far);
	glPushMatrix();
	glColor3f(r, g, b);
	glRotated(current_rot_about_sun, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	printf("Current Rot about sun = %f, about axis = %f \n", current_rot_about_sun, current_rot_about_axis);
	glTranslatef(how_far, 0, 0);
	glRotated(current_rot_about_axis, 0, 0, 1);
	glutWireSphere(rad, 20, 20);
	//glutSolidSphere(0.1, 15, 15);
	glPopMatrix();
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Planet current_planet;
	double rad, how_far, rot_about_sun, rot_about_axis;
	float r, g, b;
	for (int i = 0; i < 10; i++) {
		current_planet = all_planets[i];
		rad = current_planet.rad;
		how_far = current_planet.how_far_from_sun;
		rot_about_sun = current_planet.current_rot_about_sun;
		rot_about_axis = current_planet.current_rot_about_axis;
		r = current_planet.r; g = current_planet.g; b = current_planet.b;
		
		drawPlanet(rad, how_far, rot_about_sun, rot_about_axis, r, g, b);
	}
	
	glFlush();
}

void Anim() {
	Planet *current_planet;
	double speed_of_rot_about_sun, speed_of_rot_about_axis;
	for (int i = 0; i < 10; i++) {
		current_planet = &all_planets[i];
		speed_of_rot_about_sun = current_planet->speed_of_rot_about_sun;
		speed_of_rot_about_axis = current_planet->speed_rot_about_axis;

		double check_value;
		current_planet->current_rot_about_sun += speed_of_rot_about_sun * ANGULAR_ACC_RATIO;
		check_value = current_planet->current_rot_about_sun;
		if (check_value > 360) {
			current_planet->current_rot_about_sun = 0;
		}
		current_planet->current_rot_about_axis += speed_of_rot_about_axis * ANGULAR_ACC_RATIO;
		check_value = current_planet->current_rot_about_axis;
		if (check_value > 360) {
			current_planet->current_rot_about_axis = 0;
		}
	
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void InitPlanetsValues() {
	// The Sun
	all_planets[0].Set(695508, 0, 40, 0, 1, 1, 9 / 255);
	// The Mercury
	all_planets[1].Set(2439.5, 57, 47.9, 35, 1, 1, 1);
	// Venus
	all_planets[2].Set(6052, 108, 35, 2, 1, 1, 1);
	// Earth
	all_planets[3].Set(6403, 149, 29.8, 465, 1, 1, 1);
	// Mars
	all_planets[4].Set(3396, 228, 24.1, 241, 1, 1, 1);
	// Jupiter
	all_planets[5].Set(71492, 780, 13.1, 12662, 1, 1, 1);
	// Saturn
	all_planets[6].Set(60268, 1437, 9.7, 10233, 1, 1, 1);
	// Uranus
	all_planets[7].Set(25559, 2871, 6.8, 4109, 1, 1, 1);
	// Neptune
	all_planets[8].Set(24764, 4530, 5.4, 2700, 1, 1, 1);
	// Pluto
	all_planets[9].Set(1185, 5906, 4.7, 34, 1, 1, 1);

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1080, 720);
	glutInitWindowPosition(50, 50);
	InitPlanetsValues();
	glutCreateWindow("Milky Way Galaxy");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.05f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	std::cout << "Animation Running!\n";

	glutMainLoop();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
