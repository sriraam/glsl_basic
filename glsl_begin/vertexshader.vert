#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal; 


//out vec4 color;
out vec3 FragPos;
out vec3 Normal;



//float ambientstrength=.3;

out vec3 n;
out vec3 l;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 lightcolor;
//uniform vec3 materialcolor;
uniform vec3 lightpos;

 vec3 norm;
//uniform mat4 MVP;

void main(){

 gl_Position = projection * view *  model * vec4(vertexPosition_modelspace,1);

  FragPos = vec3(view*model * vec4(vertexPosition_modelspace, 1.0f));

 vec3 vertexPosition_cameraspace = vec3( view*model * vec4(vertexPosition_modelspace,1));
vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

vec3 LightPosition_cameraspace = vec3( model* vec4(lightpos,1));
vec3 LightDirection_cameraspace =    EyeDirection_cameraspace + LightPosition_cameraspace;

 //vec3 Normal = mat3(transpose(inverse(model))) * normal;  
//below code works fine but not completly..
   vec3 normalEyeSpace = vec3(view*model*vec4(normal,0.0));
	 Normal = mat3(transpose(inverse(model))) * normalEyeSpace;  

//	vec3 Normal = mat3(transpose(inverse(model))) * normal;  
     norm = normalize(Normal);
	
	  n = normalize( normalEyeSpace);
 
  l = normalize( LightDirection_cameraspace );

  //**For Lghting per vertex
//vec3 lightDir = normalize(lightpos-FragPos); 
//float diff = max(dot(lightDir,norm), 0.0);
//vec3 diffuse = diff * lightcolor;
//vec3 ambient=ambientstrength*lightcolor;
//vec3 result=(diffuse)*materialcolor;
//color=vec4(result,1);


}

