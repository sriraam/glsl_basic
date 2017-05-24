#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location =1) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

 uniform vec3 lightcolor;
uniform vec3 lightpos;

out vec3 result;
float ambientstrength;

void main(){

vec3 FragPos = vec3(view * model * vec4(vertexPosition,1.0f) );
vec3 Light_in_cameraspace = vec3(view * vec4(lightpos,1.0f));

vec3 normal_cameraspace = vec3(view * vec4(normals,1.0f));

//ambient Lighting
ambientstrength = 0.3f;
vec3 ambient = ambientstrength * lightcolor;

//Diffuse Lighting
vec3 n = normalize(normal_cameraspace);
vec3 lightDir = normalize(Light_in_cameraspace - FragPos);
float diff_intensity = max(dot(lightDir,n),0.0f);
vec3 diffuse = diff_intensity * vec3(2.0f,1.0f,0.3f);

result = (ambient + diffuse);

gl_Position=projection *view* model*vec4(vertexPosition,1);

}