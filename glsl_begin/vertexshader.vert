#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal; 
out vec4 color;

float ambientstrength=0.5f;

uniform vec3 lightcolor;
uniform vec3 materialcolor;

uniform mat4 MVP;

void main(){
 
 // gl_Position.xyz = vertexPosition_modelspace;
//  gl_Position.w = 1;
//color=vertexPosition_modelspace;
//color=vec4(lightcolor*materialcolor,1);

vec3 ambient=ambientstrength*lightcolor;

vec3 result=ambient*materialcolor;
color=vec4(result,1);


gl_Position = MVP * vec4(vertexPosition_modelspace,1);

}

