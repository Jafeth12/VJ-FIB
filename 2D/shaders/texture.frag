#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int starEffect;
uniform int justTookDamage;
uniform int starFrame;

in vec2 texCoordFrag;
out vec4 outColor;

vec4 STAR_COLOR0 = vec4(1.5, 1.5, 1.5, 1.0);
vec4 STAR_COLOR1 = vec4(0.3, 0.3, 0.3, 1.0);
vec4 STAR_COLOR2 = vec4(0.0, 0.5, 0.0, 1.0);

vec4 DAMAGE_COLOR0 = vec4(0.2, 0.2, 0.2, 1.0);
vec4 DAMAGE_COLOR1 = vec4(0.5, 0.5, 0.5, 1.0);
vec4 DAMAGE_COLOR2 = vec4(0.8, 0.8, 0.8, 1.0);

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;

    vec4 colorModifier = vec4(1.0, 1.0, 1.0, 1.0);

    if (starEffect == 1) {
        switch (starFrame) {
            case 0:
                colorModifier = STAR_COLOR0;
                break;
            case 1:
                colorModifier = STAR_COLOR1;
                break;
            case 2:
                colorModifier = STAR_COLOR2;
                break;
        }
    }


    if (justTookDamage == 1) {
        texColor.a = 0.3f;
        switch (starFrame) {
            case 0:
                colorModifier = DAMAGE_COLOR0;
                break;
            case 1:
                colorModifier = DAMAGE_COLOR2;
                break;
        }
    }


	outColor = colorModifier * texColor;
}

