#include "fractol.h"

vec3    perturb_normal(vec3 normal, vec3 perturbation)
{
    vec3 new_up = v3(0, 0, -1);
    if (normal.z > 0.99 || normal.z < -0.99)
        new_up = v3(1, 0, 0);

    vec3    pV = v_cross(new_up, normal);
    pV = v_norm(pV);
    vec3    pU = v_cross(normal, pV);
    pU = v_norm(pU);

    vec3 output = normal;
    output = v_add(output, v_scal(pU, perturbation.y));
    output = v_add(output, v_scal(pV, -perturbation.x));
    output = v_add(output, v_scal(normal, perturbation.z));

    output = v_norm(output);

    return output;
}

double LinearInterp(double x0, double y0, double x1, double y1, double x)
{
	double output;
	
	if ((x1-x0) == 0.0)
		output = y0;
	else
		output = y0 + ((x - x0)*((y1 - y0)/(x1 - x0)));
		
	return output;
}

double BilinearInterp(	double x0, double y0, double v0,
                        double x1, double y1, double v1,
                        double x2, double y2, double v2,
                        double x3, double y3, double v3,
                        double x, double y)
{
	double p1 = LinearInterp(x0, v0, x1, v1, x);
	double p2 = LinearInterp(x2, v2, x3, v3, x);
	double p3 = LinearInterp(y0, p1, y2, p2, y);
	return p3;
}

vec3    compute_rgb_perturbation(texture *t, vec3 normal, vec3 uv)
{
    double xD = 0.0;
	double yD = 0.0;
	double zD = 0.0;
    // Apply the local transform to the (u,v) coordinates.
    //vec3 inputLoc = uvCoords;
    //vec3 newLoc = ApplyTransform(inputLoc);		
    double u = uv.x;//newLoc.GetElement(0);
    double v = uv.y;//newLoc.GetElement(1);
    
    // Modulo arithmatic to account for possible tiling.
    //u = fmod(u, 1.0);
    //v = fmod(v, 1.0);		
    
    // Convert (u,v) to image dimensions (x,y).
    double xsd = (t->image_width);
    double ysd = (t->image_height);
    double xF = ((u + 1.0) / 2.0) * xsd;
    double yF = ysd - (((v + 1.0) / 2.0) * ysd);
    int x = (round(xF));
    int y = (round(yF));
    int xMin = (floor(xF));
    int yMin = (floor(yF));
    int xMax = (ceil(xF));
    int yMax = (ceil(yF));
    
    // Perform bilinear interpolation.
    double r0, g0, b0, a0;
    double r1, g1, b1, a1;
    double r2, g2, b2, a2;
    double r3, g3, b3, a3;
    vec3 v0 = evaluate(t, xMin, yMin);//GetPixelValue(xMin, yMin, r0, g0, b0, a0);
    vec3 v1 = evaluate(t, xMax, yMin);//GetPixelValue(xMax, yMin, r1, g1, b1, a1);
    vec3 v2 = evaluate(t, xMin, yMax);//GetPixelValue(xMin, yMax, r2, g2, b2, a2);
    vec3 v3 = evaluate(t, xMax, yMax);//GetPixelValue(xMax, yMax, r3, g3, b3, a3);
    v0 = v_add(v_mult(v0, v3(2,2,1)), v3(-1,-1,0));
    v1 = v_add(v_mult(v1, v3(2,2,1)), v3(-1,-1,0));
    v2 = v_add(v_mult(v2, v3(2,2,1)), v3(-1,-1,0));
    v3 = v_add(v_mult(v3, v3(2,2,1)), v3(-1,-1,0));
    r0 = v0.x; g0 = v0.y; b0 = v0.z;
    r1 = v1.x; g1 = v1.y; b1 = v1.z;
    r2 = v2.x; g2 = v2.y; b2 = v2.z;
    r3 = v3.x; g3 = v3.y; b3 = v3.z;
    double interpR = BilinearInterp(xMin, yMin, r0, xMax, yMin, r1, xMin, yMax, r2, xMax, yMax, r3, xF, yF);
    double interpG = BilinearInterp(xMin, yMin, g0, xMax, yMin, g1, xMin, yMax, g2, xMax, yMax, g3, xF, yF);
    double interpB = BilinearInterp(xMin, yMin, b0, xMax, yMin, b1, xMin, yMax, b2, xMax, yMax, b3, xF, yF);
    
    // Use the RGB values (ignore alpha) for the perturbation.
    xD = interpR;
    yD = interpG;
    zD = interpB;
	
	/*if (m_reverseXY)
	{
		xD = -xD;
		yD = -yD;
	}*/
		
	vec3 perturbation = v3(xD, yD, zD);
	return perturb_normal(normal, perturbation);
}

vec3    texture_diff_bw(texture *t, vec3 uv)
{
    double h = 0.001;
    double uGrad = (evaluate_bw(t, uv.x + h, uv.y) - evaluate_bw(t, uv.x - h, uv.y)) / (2.0 * h);
    double vGrad = (evaluate_bw(t, uv.x, uv.y + h) - evaluate_bw(t, uv.x, uv.y - h)) / (2.0 * h);

    vec3 output = v3(uGrad, vGrad);

    return output;
}