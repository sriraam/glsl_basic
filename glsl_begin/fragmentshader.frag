#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;
  float ambientStrength=0.5;

  uniform vec3 materialcolor;
  uniform vec3 lightPos; 
  uniform vec3 lightColor;
  uniform vec3 viewPos;

  uniform sampler2D mytexture;

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
	

	  // Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-l, n);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

	 vec3 result = (diffuse+specular) * materialcolor;
   
	// color=  texture(mytexture,TexCoord);
	 color=  texture(mytexture,TexCoord) * vec4(result,1);
	//  color=    vec4(result,1);
	 //color= vec4(0,0,0,1);
	 //color=vec4(TexCoord.x,TexCoord.y,0,1);
}




