#version 330 core
precision highp float;

// ----- UNIFORMS --------------------------------------------------------------
uniform vec2 resolution;
uniform sampler2DArray texSprite;
uniform int lejer;
// uniform int PGAA;

// ----- INPUT -----------------------------------------------------------------
in vec2 vUV;
in vec2 _A;
in vec2 _B;
in vec2 _C;

// ----- OUTPUT ----------------------------------------------------------------
out vec4 fragColor;

// ----- HELPERS ---------------------------------------------------------------
#define WHITE vec3(1.0)
#define BLACK vec3(0.0)
#define RED vec3(1.0,0.0,0.0)
#define BLUE vec3(0.0,0.0,1.0)
#define GREEN vec3(0.0,1.0,0.0)
#define YELLOW vec3(1.0,1.0,0.0)
#define PURPLE vec3(1.0,0.0,1.0)
#define CYAN vec3(0.0,1.0,1.0)

// -----------------------------------------------------------------------------
// ----- SHADER ----------------------------------------------------------------
// -----------------------------------------------------------------------------
vec2 dots[7];
int totalDots = 0;


vec2 intersect(vec2 p0, vec2 p1, vec2 p2, vec2 p3)
{
    vec2 s1 = p1 - p0;
    vec2 s2 = p3 - p2;
    float t = ( s2.x * (p0.y-p2.y)-s2.y*(p0.x-p2.x)) / (-s2.x*s1.y + s1.x*s2.y);
    return p0 + t*s1;
}

bool on_side(vec2 test, vec2 v1, vec2 v2)
{
    // vec2 n = (v1-v2).yx * vec2(1.0,-1.0);
    // vec2 p1_p2 = test-v2;
    // return dot(n,p1_p2)>=0.0;

    return ((v2.x - v1.x)*(test.y-v1.y) - (v2.y-v1.y)*(test.x-v1.x))>=0.0;
}

// Sutherland-Hodgman clip (modified)
void SHclip(vec2 v1, vec2 v2)
{
    vec2 final[7];
    int f_cnt = 0;
    vec2 sp, ep;
    bool spt, ept;
    int i;
    
    sp = dots[totalDots-1];
    for(i=0; i<totalDots; i++)
    {
        ep = dots[i];
        spt = on_side(sp,v1,v2);        
        ept = on_side(ep,v1,v2);
        
        if (spt && ept)
        {
        	final[f_cnt++] = sp;
        }
        else if (spt && !ept)
        {
        	final[f_cnt++] = sp;
        	final[f_cnt++] = intersect(v1,v2,sp,ep);
        }
        else if (!spt && ept)
        {
        	final[f_cnt++] = intersect(v1,v2,sp,ep);
        }
        sp = ep;
    }

    totalDots = f_cnt;
    for(i=0; i<totalDots; i++)
    	dots[i] = final[i];
    
}

void main()
{
    vec2 aspect = vec2(resolution.x/resolution.y,1.0);
	vec2 p = 2.0*gl_FragCoord.xy/resolution-vec2(1.0);     // clip-space coordinates
    p *= aspect;

    dots[0] = _A;
    dots[1] = _B;
    dots[2] = _C;
    totalDots = 3;
    dots[3] = vec2(0);
    dots[4] = vec2(0);
    dots[5] = vec2(0);
    dots[6] = vec2(0);

	vec2 h = vec2(1.0)/resolution * aspect;                // pixel's half-size
	vec2 ul = p + vec2(-h.x,  h.y);               // upper left pixel corner
	vec2 dl = p + vec2(-h.x, -h.y);               // lower left pixel corner
	vec2 dr = p + vec2( h.x, -h.y);               // lower right pixel corner
	vec2 ur = p + vec2( h.x,  h.y);               // upper right pixel corner

    // TODO: unroll code
	SHclip( ul, dl );                              // clip triangle against pixel sides
	SHclip( dl, dr );
	SHclip( dr, ur );
	SHclip( ur, ul );

    if(totalDots<3) discard;

    float pixel_area = h.x*h.y*4.0;                 // calc pixel area

    // float clip_area = poly_area();               // calc intersected polygon's area
    float clip_area = 0.0;
    int i;
    vec2 ba = dots[0]-dots[1];
    vec2 bc = vec2(0);
    for(i=1; i<totalDots-1; i++)                    // TODO: always 7-dot array for SIMD?
    {
        bc = dots[0]-dots[i+1];
        clip_area += abs(ba.x*bc.y-ba.y*bc.x);      // double triangle area
        ba = bc;
    }
    clip_area *= 0.5;
    
    float k = clip_area / pixel_area;               // TODO: non-linear curve?
    k = clamp(clip_area / pixel_area, 0.0, 1.0);

    // vec3 col = vec3(k,k,k);
    // vec3 col = WHITE;
    vec3 col = texture(texSprite,vec3(vUV,lejer)).rgb;

// if(PGAA==1)
    fragColor = vec4(col,k);
// else
    // fragColor = vec4(col,1.0);

    //--------------------------------------------------------------------------

    // TESTING IF SHclip() IS WORKING
    // vec3 col = BLACK;
    // if     (totalDots<3)  col = RED;     // crveno <3
    // else if(totalDots==3) col = YELLOW;  // zuta =3
    // else if(totalDots==4) col = GREEN;   // zeleno =4
    // else if(totalDots==5) col = BLUE;    // plavo =5
    // else                  col = PURPLE;  // ljubicasto >5
    // fragColor = vec4(col, 1.0);
	//--------------------------------------------------------------------------

    // TESTING IF on_side() IS WORKING
	// bool isAB = on_side(p, _A, _B);
	// bool isBC = on_side(p, _B, _C);
	// bool isCA = on_side(p, _C, _A);

	// vec3 k = RED;
	// if(isCA && isBC && isAB)
	// 	k = WHITE;
	// fragColor = vec4(k,1.0);
	//--------------------------------------------------------------------------

    // TESTING IF intersect() IS WORKING
    // vec2 ba = _B-_A;
    // vec2 bc = _B-_C;
    // vec2 ca = _C-_A;
    // vec2 halfA = normalize(ca) + normalize(ba);
    // vec2 halfB = normalize(-bc) + normalize(-ba);
    // vec2 innerCircleCenter = intersect(_A,_A+halfA,_B,_B+halfB);

    // float surface = 0.5 * abs(ba.x*bc.y-ba.y*bc.x);
    // float s = length(ba) + length(bc) + length(ca);
    // float r = surface/s*2.;

    // fragColor = vec4(length(p - innerCircleCenter)<=r?RED:WHITE,1.0);
}