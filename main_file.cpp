/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "camera.hpp"
#include "model.h"
#include "wall.h"
#include "Prop.h"
#include "Foe.h"

float speed_x=0;
float speed_y=0;
float aspectRatio=1;
Model wall = Model(myWallVertices, myWallColors, myWallNormals, myWallTexCoords, 6);

Model* eye;

Foe* foe;

ShaderProgram *sp;

GLuint tex0;

bool key_dir[4] = { false }; //zmienne używane do poruszania kamery

Camera cam;
glm::vec3 cPos = glm::vec3(0.f, 0.f, -10.f);
glm::vec3 cDir = glm::vec3(0.f, 0.f, 10.f);

float lastX = 250.0f, lastY = 250.0f, xoffset = 0.0f, yoffset = 0.0f, yaw = -90.0f, pitch = 0.0f, camSpeed = 0.1f;
bool first = true;


//Odkomentuj, żeby rysować kostkę
//float* vertices = myCubeVertices;
//float* normals = myCubeNormals;
//float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
//int vertexCount = myCubeVertexCount;


//Odkomentuj, żeby rysować czajnik
float* vertices = myTeapotVertices;
float* normals = myTeapotVertexNormals;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
int vertexCount = myTeapotVertexCount;


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
		if (key == GLFW_KEY_W) key_dir[up] = true;
		if (key == GLFW_KEY_S) key_dir[down] = true;
		if (key == GLFW_KEY_A) key_dir[left] = true;
		if (key == GLFW_KEY_D) key_dir[right] = true;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
		if (key == GLFW_KEY_W) key_dir[up] = false;
		if (key == GLFW_KEY_S) key_dir[down] = false;
		if (key == GLFW_KEY_A) key_dir[left] = false;
		if (key == GLFW_KEY_D) key_dir[right] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first)
	{
		lastX = xpos;
		lastY = ypos;
		first = false;
	}
	xoffset = xpos - lastX;
	yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cam.MouseMov(yaw, pitch);
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	tex0 = readTexture("metal.png");
	wall.setTex(tex0);
	eye = new Model("models/virus/virus.obj", "models/virus/virus.png");
	eye->scale(3.f);
	foe = new Foe(eye, glm::mat4(1.f), 1.f);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
	glDeleteTextures(1, &tex0);
}




//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.UpdateCam(key_dir);

	glm::mat4 V = cam.GetViewMatrix();

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu

	glm::mat4 Mw = glm::mat4(1.0f);
	Mw = glm::rotate(Mw, PI / 2, glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 t;

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	glUniform4f(sp->u("ls"), 0, 3, 0, 1);
	glUniform1i(sp->u("textureMap0"), 0); //przypisanie zerowej jednostki teksturującej do zmiennej jednorodnej

    glEnableVertexAttribArray(1);  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(2);  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(3);  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(4);  //Włącz przesyłanie danych do atrybutu vertex
    
	glVertexAttribPointer(1,4,GL_FLOAT,false,0,vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	
	glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, colors);

	glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, normals);

	glVertexAttribPointer(4, 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

    glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt

	for (int i = 0; i < 3; i++)
	{
		t = glm::translate(Mw, glm::vec3(0.f, -2.f, 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(t));
		wall.draw();
		Mw = glm::rotate(Mw, -PI / 2, glm::vec3(0.f, 0.f, 1.f));
	}

	foe->updatePos(V);
	t = foe->getPos();
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(t));
	foe->draw();

    glDisableVertexAttribArray(1);  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        angle_x+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
