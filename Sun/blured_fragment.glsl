layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 fColor;
uniform sampler2D renderedTexture;

float normpdf(in float x, in float sigma)
{
	
	return 0.03*x;
}
void main(){
		float sigma = 7.0;
		float Z = 0.0;
		int mSize = 11;
		int kSize = 5;
		float kernel[11]={0.1,0.2,0.3,0.4,0.5,0.5,0.5,0.4,0.3,0.2,0.1};
vec3 c=vec3(0.0);

		for (int j = 0; j < mSize; ++j)
		{
			Z += kernel[j];
		}
		
		for (int i=-kSize; i <= kSize; ++i)
		{
			for (int j=-kSize; j <= kSize; ++j)
			{
				c += kernel[kSize+j]*kernel[kSize+i]*texture(renderedTexture, (gl_FragCoord.xy+vec2(float(i),float(j)))/512).rgb;	
			}
		}


fColor=vec4(c, 1.0);
}