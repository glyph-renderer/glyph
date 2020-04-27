#version 460

in float barColor;
in float pass;
in vec2 position;
uniform sampler1D attr_texture; //data map
uniform sampler1D cat_texture; //cat map
uniform mat4 matrix;
layout (location = 0) out vec4 color;

float ambient = 0.5;
float opacity = 1.0f;
float no_cat = 10.0f;
float lineColor = 0.5f;

void main() {
    vec3 Normal;
    Normal =vec3(position,0.f);
    vec4 colors;
    float barC =float(barColor/(no_cat + 1.0f));

    if (pass < 0.0) {
        color = vec4(lineColor,lineColor,lineColor,lineColor);
        return;
    } else if (pass < 1.0) {
        colors = texture( attr_texture, barColor);
        Normal.z= 1.f-2*(abs(position.x-0.5));
    } else {
        colors = texture( cat_texture, barC);
        Normal.xy = position * 2.0 - vec2(1.0);
        float mag = dot( Normal.xy, Normal.xy );
	Normal.z = 1.f-2*(abs(position.x-0.5));
        if (pass > 1.0) {
            if (mag > 1.0) {
                discard;
            }
            Normal.z = sqrt(1.0-mag);
        }
    }
    colors.w = 1.0f;

    vec4 temp = matrix * vec4(0.f,0.f,1.f,0.f);
    vec3 lightDir = normalize(temp.xyz);
    float diffuse = clamp(dot(lightDir, Normal),0,1);
    color = colors*(vec4(vec3(ambient),1.f) + vec4(vec3(diffuse),1.f));
}
