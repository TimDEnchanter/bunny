// bunny.frag

varying vec3 ec_vnormal, ec_vposition;

void main()
{
vec3 P, N, L, V, H;
float shininess = gl_FrontMaterial.shininess;
float pi = 3.14159265;

P = ec_vposition;
N = normalize(ec_vnormal);

vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
int i;
for(i=0; i<3; i++) {
	L = normalize(gl_LightSource[i].position - P);
	V = normalize(-P);				// eye position is (0,0,0)!
	H = normalize(L+V);
		
	diffuse_color = glFrontLightProduct[i].diffuse * max(dot(N,L),0.0);
	//diffuse_color = clamp(diffuse_color, 0.0, 1.0);
	specular_color = glFrontLightProduct[i].specular * (shininess+2.0)/(8.0*pi))*pow(max(dot(H,N),0.0),shininess;
	//specular_color = clamp(specular_color, 0.0, 1.0);
	finalColor += diffuse_color + specular_color;
}
gl_FragColor = finalColor;
}
