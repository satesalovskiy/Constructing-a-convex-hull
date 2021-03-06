
#include "pch.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <ctime>
using namespace std;
struct Point
{
	int x;
	int y;
};
vector<Point>result;
vector <Point> p;


void reshape(int w, int h);
void display();



int main(int argc, char * argv[])
{
	setlocale(LC_ALL, "rus");



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Оболочка");



	
	int n;
	//Создаем и заполняем вектор точек
	cout << "Введите количество точек: " << endl;
	cin >> n;

	srand(time(0));

	for (int i = 0; i < n; i++)
	{
		int x, y;
		//cin >> x >> y;
		x = rand()%401+(-200);
		y = rand()%401+(-200);
		p.push_back(Point{ x,y });
		cout << "x=" << x << "y=" << y << endl;;
	}  
	cout << "===----------------"<<endl;
	//================================
	//Сортируем
	sort(p.begin(), p.end(), [](const Point &left, const Point &right)
	{
		if (left.y != right.y)
		{
			return left.y < right.y;
		}
		return left.x < right.x;
	});
	//================================
	//Удаление повторов
	p.erase(unique(p.begin(), p.end(), [](const Point &left, const Point &right)
	{
		return left.x == right.x && left.y == right.y;
	}),p.end());
	//================================
	//Если осталась одна точка то выходим
	n = (int)p.size();
	if (n == 1)
	{
		cout << "Одна точка!" << endl;
		_getwch();
		return 0;
	}
	//================================

	double dx = -1; //Направление вектора
	double dy = 0;
	int cur = 0; 
	
	while (true)
	{
		result.push_back(p[cur]);
		int next = -1; //След точка
		double bestCos = -2; 
		for (int i = 0; i < n; i++)
		{
			if (i != cur) // Если точка другая 
			{
				int ndx = p[i].x - p[cur].x; //Новый вектор для скалярного произведения
				int ndy = p[i].y - p[cur].y;
				double cos = (dx*ndx + dy * ndy) / sqrt(ndx*ndx + ndy * ndy);
				if (cos > bestCos) // Если кос оказался больше чем наибольший то след мы рассматриваем точка i
				{
					next = i;
					bestCos = cos;
				}
			}
		}

		//assert(bestCos != -2);
		dx = p[next].x - p[cur].x;
		dy = p[next].y - p[cur].y;
		double len = sqrt(dx*dx + dy * dy); // Длина нового вектора 
		dx /= len;
		dy /= len;
		
		cur = next;		
		if (cur == 0)
		{
			break;
		}
	}

	//Вывод точек
	int m = (int)result.size();
	cout << "Точки, которые составляют выпуклую оболочку: " << endl;
	for (int i = 0; i < m; i++)
	{
		cout <<"x="<< result[i].x <<"y="<<result[i].y << endl;
	}



	glutReshapeFunc(reshape);
	glutDisplayFunc(display);



	glutMainLoop();


	_getwch();
	return 0;
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	//glutInitWindowPosition(300, 300);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void display()
	
{
	glClearColor(0.8, 0.8, 0.8, 1); //Цвет фона
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1);
	int h = (int)result.size();
	glColor3d(0, 0, 0);            //Цвет оси Y
	    glBegin(GL_LINES);
	    glVertex2i(300, 0);
	    glVertex2i(300, 600);
		glEnd();
	glColor3d(0, 0, 0);            //Цвет оси X
		glBegin(GL_LINES);
		glVertex2i(0, 300);
		glVertex2i(600, 300);
		glEnd();

    
 
	glColor3d(0.5, 0.5, 0.5);      //Цвет Фигуры

	glBegin(GL_POLYGON);
	
	for (int i = 0; i < h; i++)
	{
		glVertex2i(result[i].x+300, result[i].y+300);
	}
	glEnd();

	glColor3d(0, 0, 0);           //Цвет обводки
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	
	for (int i = 0; i < h; i++)
	{
		glVertex2i(result[i].x + 300, result[i].y + 300);
	}
	glEnd();

	int k = (int)p.size();
	glColor3d(1, 0, 0);             //Точки все 
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < k; i++)
	{
		glVertex2i(p[i].x + 300, p[i].y + 300);
	}
	glEnd();

	glutSwapBuffers();
}