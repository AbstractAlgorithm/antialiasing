// #version 330 core

// uniform float time;

// layout(location = 0) in vec2 vPosition;
// layout(location = 1) in vec3 vColor;

// out vec3 fColor;

// void main()
// {
// 	fColor = vColor;
// 	vec2 pos = vPosition + vec2(0.,0.3)*sin(time);
// 	gl_Position = vec4(pos, 0.5, 1.0);
// }


#version 330 core
layout(location = 0) in vec2 aPosition;
uniform vec2 cameraT;
uniform vec2 size;
uniform vec2 pos;
uniform float angle;
out vec2 uv;

void main()
{
    uv = (aPosition+1.0)/2.0;
    vec2 rlp = aPosition*size;
    vec2 rpos = vec2(rlp.x*cos(angle)-rlp.y*sin(angle),rlp.y*cos(angle)+rlp.x*sin(angle));
    vec2 spos = (rpos + 2.0*pos)*cameraT;
    gl_Position = vec4(spos, 0.5, 1.0);
}