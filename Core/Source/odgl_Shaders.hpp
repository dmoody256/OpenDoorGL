#ifndef ODGL_SHADERS_H
#define ODGL_SHADERS_H

#include "odgl_Include.hpp"

#include <string>

namespace OpenDoorGL
{
    static const std::string button_frag = "#version 330 core\n"
"\n"
"// Interpolated values from the vertex shaders\n"
"in vec3 fragmentColor;\n"
"\n"
"// Ouput data\n"
"out vec3 color;\n"
"in vec2 UV;\n"
"\n"
"void main(){\n"
"\n"
"// Output color = color specified in the vertex shader,\n"
"// interpolated between all 3 surrounding vertices\n"
"if(UV.x > 0.9)\n"
"{\n"
"color = vec3(UV.x, UV.y, 1.0);\n"
"}\n"
"else\n"
"{\n"
"color = fragmentColor;\n"
"}\n"
"//color = fragmentColor;\n"
"\n"
"}\n"
;

static const std::string button_vert = "#version 330 core\n"
"\n"
"// Input vertex data, different for all executions of this shader.\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec2 vertexUV;\n"
"layout(location = 2) in vec3 vertexColor;\n"
"\n"
"// Output data ; will be interpolated for each fragment.\n"
"out vec3 fragmentColor;\n"
"out vec2 UV;\n"
"// Values that stay constant for the whole mesh.\n"
"uniform mat4 MVP;\n"
"\n"
"void main(){\n"
"// Output position of the vertex, in clip space : MVP * position\n"
"gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"
"\n"
"// The color of each vertex will be interpolated\n"
"// to produce the color of each fragment\n"
"fragmentColor = vertexColor;\n"
"UV = vertexUV;\n"
"}\n"
;

static const std::string color_frag = "#version 330 core\n"
"\n"
"// Interpolated values from the vertex shaders\n"
"in vec3 fragmentColor;\n"
"\n"
"// Ouput data\n"
"out vec3 color;\n"
"\n"
"void main(){\n"
"\n"
"// Output color = color specified in the vertex shader,\n"
"// interpolated between all 3 surrounding vertices\n"
"color = fragmentColor;\n"
"\n"
"}\n"
;

static const std::string color_vert = "#version 330 core\n"
"\n"
"// Input vertex data, different for all executions of this shader.\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec3 vertexColor;\n"
"\n"
"// Output data ; will be interpolated for each fragment.\n"
"out vec3 fragmentColor;\n"
"\n"
"// Values that stay constant for the whole mesh.\n"
"uniform mat4 MVP;\n"
"\n"
"void main(){\n"
"// Output position of the vertex, in clip space : MVP * position\n"
"gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"
"\n"
"// The color of each vertex will be interpolated\n"
"// to produce the color of each fragment\n"
"fragmentColor = vertexColor;\n"
"}\n"
;

static const std::string texture_frag = "#version 330 core\n"
"\n"
"uniform sampler2D myTextureSampler;\n"
"\n"
"in vec2 UV;\n"
"// Ouput data\n"
"out vec3 color;\n"
"\n"
"void main(){\n"
"\n"
"// Output color = color specified in the vertex shader,\n"
"// interpolated between all 3 surrounding vertices\n"
"color = texture( myTextureSampler, UV ).rgb;\n"
"\n"
"}\n"
;

static const std::string texture_vert = "#version 330 core\n"
"\n"
"// Input vertex data, different for all executions of this shader.\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec2 vertexUV;\n"
"\n"
"out vec2 UV;\n"
"// Values that stay constant for the whole mesh.\n"
"uniform mat4 MVP;\n"
"\n"
"void main(){\n"
"// Output position of the vertex, in clip space : MVP * position\n"
"gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"
"UV = vertexUV;\n"
"}\n"
;

static const std::string textureModel_frag = "#version 330 core\n"
"\n"
"// Interpolated values from the vertex shaders\n"
"in vec2 UV;\n"
"in vec3 Position_worldspace;\n"
"in vec3 Normal_cameraspace;\n"
"in vec3 EyeDirection_cameraspace;\n"
"in vec3 LightDirection_cameraspace;\n"
"\n"
"// Ouput data\n"
"out vec3 color;\n"
"\n"
"// Values that stay constant for the whole mesh.\n"
"uniform sampler2D myTextureSampler;\n"
"uniform mat4 MV;\n"
"uniform vec3 LightPosition_worldspace;\n"
"\n"
"void main(){\n"
"// Light emission properties\n"
"// You probably want to put them as uniforms\n"
"vec3 LightColor = vec3(1,1,1);\n"
"float LightPower = 50.0f;\n"
"\n"
"// Material properties\n"
"vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;\n"
"vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;\n"
"vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);\n"
"\n"
"// Distance to the light\n"
"float distance = length( LightPosition_worldspace - Position_worldspace );\n"
"\n"
"// Normal of the computed fragment, in camera space\n"
"vec3 n = normalize( Normal_cameraspace );\n"
"// Direction of the light (from the fragment to the light)\n"
"vec3 l = normalize( LightDirection_cameraspace );\n"
"// Cosine of the angle between the normal and the light direction,\n"
"// clamped above 0\n"
"//  - light is at the vertical of the triangle -> 1\n"
"//  - light is perpendicular to the triangle -> 0\n"
"//  - light is behind the triangle -> 0\n"
"float cosTheta = clamp( dot( n,l ), 0,1 );\n"
"\n"
"// Eye vector (towards the camera)\n"
"vec3 E = normalize(EyeDirection_cameraspace);\n"
"// Direction in which the triangle reflects the light\n"
"vec3 R = reflect(-l,n);\n"
"// Cosine of the angle between the Eye vector and the Reflect vector,\n"
"// clamped to 0\n"
"//  - Looking into the reflection -> 1\n"
"//  - Looking elsewhere -> < 1\n"
"float cosAlpha = clamp( dot( E,R ), 0,1 );\n"
"\n"
"color =\n"
"// Ambient : simulates indirect lighting\n"
"MaterialAmbientColor +\n"
"// Diffuse : \"color\" of the object\n"
"MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +\n"
"// Specular : reflective highlight, like a mirror\n"
"MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);\n"
"}\n"
;

static const std::string textureModel_vert = "#version 330 core\n"
"\n"
"// Input vertex data, different for all executions of this shader.\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec2 vertexUV;\n"
"layout(location = 2) in vec3 vertexNormal_modelspace;\n"
"\n"
"// Output data ; will be interpolated for each fragment.\n"
"out vec2 UV;\n"
"out vec3 Position_worldspace;\n"
"out vec3 Normal_cameraspace;\n"
"out vec3 EyeDirection_cameraspace;\n"
"out vec3 LightDirection_cameraspace;\n"
"\n"
"// Values that stay constant for the whole mesh.\n"
"uniform mat4 MVP;\n"
"uniform mat4 V;\n"
"uniform mat4 M;\n"
"uniform vec3 LightPosition_worldspace;\n"
"\n"
"void main(){\n"
"\n"
"// Output position of the vertex, in clip space : MVP * position\n"
"gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"
"\n"
"// Position of the vertex, in worldspace : M * position\n"
"Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;\n"
"\n"
"// Vector that goes from the vertex to the camera, in camera space.\n"
"// In camera space, the camera is at the origin (0,0,0).\n"
"vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;\n"
"EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n"
"\n"
"// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.\n"
"vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n"
"LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n"
"\n"
"// Normal of the the vertex, in camera space\n"
"// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.\n"
"Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz;\n"
"\n"
"// UV of the vertex. No special space for this one.\n"
"UV = vertexUV;\n"
"}\n"
;

}
#endif
