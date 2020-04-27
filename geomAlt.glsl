# version 460

uniform mat4 matrix;
layout(points) in;
in float barA[];
in float barB[];
in float barC[];
in float barD[];
in float barE[];
in float barF[];
in float color[];
layout (triangle_strip, max_vertices = 200) out;
out float barColor;
out float pass;
out vec2 position;

const float PI = 3.1415926;
const float H_PI = PI/2.0f;
float glyphSize = 0.06f;
float barWidth = glyphSize/3.0;
float noOfVariables = 6.0;
float halfVar = noOfVariables/2.0;

void main() {
    vec4 center = gl_in[0].gl_Position;
    vec4 pos = matrix*center;

    //draw variable bars
    pass = 0;
    for (int i=0; i<noOfVariables;i++) {
        float barValue = 0.0f;
        switch (i) {
        case 0: barValue = barA[0]; break;
        case 1: barValue = barB[0]; break;
        case 2: barValue = barC[0]; break;
        case 3: barValue = barD[0]; break;
        case 4: barValue = barE[0]; break;
        case 5: barValue = barF[0]; break;
        default: barValue = 0.0f;  break;
        }
        float barMultiply = barValue*glyphSize + glyphSize;
        barColor = barValue;

        float angle = H_PI-i*(2*PI/noOfVariables);
        vec4 point2 = vec4(pos.x+barWidth*cos(angle+H_PI),pos.y+barWidth*sin(angle+H_PI),pos.z,1.0);
        gl_Position = point2;
        position =vec2(0.f,0.f);
        EmitVertex();
        vec4 point1 = vec4(pos.x+barWidth*cos(angle-H_PI),pos.y+barWidth*sin(angle-H_PI),pos.z,1.0);
        gl_Position = point1;
        position =vec2(1.f,0.f);
        EmitVertex();
        gl_Position = vec4(point1.x+barMultiply*cos(angle),point1.y+barMultiply*sin(angle),pos.z,1.0);
        position =vec2(1.f,1.f);
        EmitVertex();
        gl_Position = vec4(point2.x+barMultiply*cos(angle),point2.y+barMultiply*sin(angle),pos.z,1.0);
        position =vec2(0.f,1.f);
        EmitVertex();
        gl_Position = point2;
        position =vec2(0.f,0.f);
        EmitVertex();
        gl_Position = point1;
        position =vec2(1.f,0.f);
        EmitVertex();
    }

    //draw indicator bar
    pass = 2;
    barColor =  (color[0]);
    position =vec2(0.5,0.5);
    EmitVertex();
    gl_Position = vec4(pos.x-glyphSize,pos.y - glyphSize, pos.z,1.0);
    position =vec2(0.f,0.f);
    EmitVertex();
    gl_Position = vec4(pos.x-glyphSize,pos.y + glyphSize, pos.z,1.0);
    position =vec2(0.f,1.f);
    EmitVertex();
    gl_Position = vec4(pos.x+glyphSize,pos.y+glyphSize, pos.z,1.0);
    position =vec2(1.f,1.f);
    EmitVertex();
    gl_Position = vec4(pos.x+glyphSize,pos.y - glyphSize, pos.z,1.0);
    position =vec2(1.f,0.f);
    EmitVertex();
    gl_Position = vec4(pos.x-glyphSize,pos.y - glyphSize, pos.z,1.0);
    position =vec2(0.f,0.f);
    EmitVertex();
    gl_Position = pos;
    position =vec2(0.5,0.5);
    EmitVertex();
}
