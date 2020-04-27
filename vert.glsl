#version 460

layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in float cat_color;
layout(location = 2) in float barAattr;
layout(location = 3) in float barBattr;
layout(location = 4) in float barCattr;
layout(location = 5) in float barDattr;
layout(location = 6) in float barEattr;
layout(location = 7) in float barFattr;
out float barA;
out float barB;
out float barC;
out float barD;
out float barE;
out float barF;
out float color;

void main() {
    gl_Position = vec4(vertex_pos, 0.0f, 1.0f);
    barA = barAattr;
    barB = barBattr;
    barC = barCattr;
    barD = barDattr;
    barE = barEattr;
    barF = barFattr;
    color = cat_color;
}