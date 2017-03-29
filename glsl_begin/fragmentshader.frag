#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  float ambientStrength=0.5;

  uniform vec3 materialcolor;
  uniform vec3 lightPos; 
  uniform vec3 lightColor;

  in vec3 l;
  in vec3 n;

void main(){

 // Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(n, l), 0.0);
    vec3 diffuse = diff * lightColor;
	 vec3 result = (diffuse) * materialcolor;
   // color = vec4(1,.5,1,1.0f);
	  color= vec4(result,1);
}




