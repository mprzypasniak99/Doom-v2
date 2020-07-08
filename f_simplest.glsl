#version 330

uniform sampler2D textureMap0;

in vec4 iC;
in vec4 l;
in vec4 n;
in vec4 v;
in vec2 iTexCoord0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela


float toon(float val, float degrees) {
	return round(val * degrees)/degrees;
}

void main(void) {
	vec4 norm_l = normalize(l);
	vec4 norm_n = normalize(n);
	vec4 norm_v = normalize(v);

	vec4 r = reflect(-norm_l, norm_n);
	float ln = clamp(dot(norm_l, norm_n), 0, 1);
	float rv = clamp(pow(dot(r, norm_v), 20), 0, 1);

	vec4 texColor = texture(textureMap0, iTexCoord0);

	pixelColor=vec4(texColor.rgb * ln, texColor.a) + vec4(1, 1, 1, 0) * rv;
}
