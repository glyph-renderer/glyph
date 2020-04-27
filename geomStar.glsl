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
const float T_PI = PI*2.0f;
float glyphSize = 0.12f;
float barWidth = glyphSize/30.f;  //line width...
float axisWidth = 0.002;//glyphSize/50.f;  //line width...
//float barWidth = glyphSize/3.0;
float noOfVariables = 6.0;
float halfVar = noOfVariables/2.0;


void main()
{
    vec4 centre = gl_in[0].gl_Position;
    vec4 pos = matrix*centre;
    position = vec2(0.25f,0.25f);

    //Draw Axes
    pass = -1;
    float barMultiply = glyphSize;
    barColor = 0.f;
    gl_Position = pos;
    EmitVertex();
    for (int i=0; i<noOfVariables;i++)
    {
        float angleA = H_PI-i*(T_PI/noOfVariables);
        vec4 pointB = vec4(pos.x+axisWidth*cos(angleA+PI/2),pos.y+axisWidth*sin(angleA+H_PI),pos.z,1.0);
        gl_Position = pointB;
        EmitVertex();
        vec4 pointA = vec4(pos.x+axisWidth*cos(angleA-PI/2),pos.y+axisWidth*sin(angleA-H_PI),pos.z,1.0);
        gl_Position = pointA;
        EmitVertex();
        gl_Position = vec4(pointA.x+barMultiply*cos(angleA),pointA.y+barMultiply*sin(angleA),pos.z,1.0);
        EmitVertex();
        gl_Position = vec4(pointB.x+barMultiply*cos(angleA),pointB.y+barMultiply*sin(angleA),pos.z,1.0);
        EmitVertex();
        gl_Position = pointB;
        EmitVertex();
        gl_Position = pointA;
        EmitVertex();
    }
    vec4 pointA = vec4(pos.x,pos.y+barMultiply,pos.z,1.0);
    gl_Position = pointA;
    EmitVertex();

    pass = 1;
    barColor =  (color[0]);
    for (int i=0; i<noOfVariables;i++)
    {
        float barValue = 0.0f;
        float barValueA = 1.0f;
        switch (i) {
        case 0: barValue = barA[0]; barValueA= barB[0]; break;
        case 1: barValue = barB[0]; barValueA= barC[0]; break;
        case 2: barValue = barC[0]; barValueA= barD[0]; break;
        case 3: barValue = barD[0]; barValueA= barE[0]; break;
        case 4: barValue = barE[0]; barValueA= barF[0]; break;
        case 5: barValue = barF[0]; barValueA = barA[0]; break;
        default: barValue = 0.0f; barValueA = 1.0f;
        }
        barMultiply = barValue*glyphSize;
        float barMultiplyB = barValueA*glyphSize;
        float angleA = H_PI-i*(T_PI/noOfVariables);
        float angleB = H_PI-(i+1)*(T_PI/noOfVariables);
        vec4 pointA = vec4(pos.x+barMultiply*cos(angleA),pos.y+barMultiply*sin(angleA),pos.z,1.0);
        vec4 pointB = vec4(pos.x+barMultiplyB*cos(angleB),pos.y+barMultiplyB*sin(angleB),pos.z,1.0);
        vec4 var = vec4(barWidth*cos(angleA),barWidth*sin(angleA),0.f,0.f);
        vec2 diff = barWidth * normalize(vec2(-(pointB.y-pointA.y),(pointB.x-pointA.x)));
        var = vec4(diff.x,diff.y,0.f,0.f);
        gl_Position = pointA + var;
        EmitVertex();
        EmitVertex();
        gl_Position = pointA-var;
        EmitVertex();
        gl_Position = pointB+var;
        EmitVertex();
        gl_Position = pointB-var;
        EmitVertex();
    }

    barMultiply = barA[0]*glyphSize;
    float angleA = H_PI*(T_PI/noOfVariables);
    pointA = vec4(pos.x,pos.y+barMultiply,pos.z,1.0);
    vec4 var = vec4(barWidth,0.f,0.f,0.f);
    gl_Position = pointA+var;
    EmitVertex();
    gl_Position = pointA-var;
    EmitVertex();
}