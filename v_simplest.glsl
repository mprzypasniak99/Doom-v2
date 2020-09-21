#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 ls[2];

//Atrybuty
layout (location = 1) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 normal;
layout (location = 4) in vec2 texCoord0;

out vec4 iC;
out vec4 l[2];
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;


void main(void) {
    l[0] = normalize(V*ls[0]-V*M*vertex);
    l[1] = normalize(V*ls[1]-V*M*vertex);
    n = normalize(V*M*normal);
    v = normalize(vec4(0,0,0,1) - V*M*vertex);

    iC = color;

    gl_Position=P*V*M*vertex;

    iTexCoord0 = texCoord0;
}
