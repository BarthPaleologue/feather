layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;

out vec2 vUV;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);

    vUV = uv;
}