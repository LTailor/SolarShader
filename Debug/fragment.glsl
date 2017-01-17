layout(origin_upper_left) in vec4 gl_FragCoord;
in vec3 ttt;
out vec4 fColor;
void main() { float angle=4.14;  vec2 fg = gl_FragCoord.xy*mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
float f=step(0.5,fract(fg.x/20));
float d=step(0.5,fract(fg.y/20));
fColor = vec4(f*d,f*d,-ttt.z, 1.0);
}