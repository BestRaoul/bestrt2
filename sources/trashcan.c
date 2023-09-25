
#include "fractol.h"

// item ADD/MODIFY
vec3 plane_alligned_add(vec3 base, vec3 add)
{
	return v3(base.x + add.x * (v.plane != YZ && v.plane != Y && v.plane != Z),
			  base.y + add.y * (v.plane != XZ && v.plane != X && v.plane != Z),
			  base.z + add.z * (v.plane != XY && v.plane != X && v.plane != Y));
}

// returns time elapsed from event in ms
int get_elapsed(struct timeval event)
{
	struct timeval current;
	gettimeofday(&current, 0);
	int elapsed_ms = ((current.tv_sec - event.tv_sec) * 1000000 + current.tv_usec - event.tv_usec);
	return elapsed_ms;
}


//returns array with n+1 points along a circular path offset by r
//[0] == [n]
vec3	*get_npoints(int n, double r_offset)
{
	vec3 *points = malloc((n+1) * sizeof(vec3));

	for (int i = 0; i <= n; i++)
	{
		double _a = MYPI * 2 / n * i + r_offset;
		points[i] = v3(sin(_a), cos(_a));
	}
	return points;
}

vec3	rotate(vec3 v, double o)
{
	return v3(v.x*cos(o) - v.y*sin(o),
			  v.x*sin(o) + v.y*cos(o));
}

vec3	rotate_x(vec3 v, double alpha)
{
	double cosA = cos(alpha), sinA = sin(alpha);
	
	return v3(v.x,
			  cosA * v.y -sinA * v.z,
			  sinA * v.y +cosA * v.z
			);
}

vec3	rotate_y(vec3 v, double beta)
{
	double cosB = cos(beta), sinB = sin(beta);
	
	return v3(cosB * v.x +sinB * v.z,
			  v.y,
			 -sinB * v.x +cosB * v.z
			);
}

vec3	rotate_z(vec3 v, double gamma)
{
	double cosY = cos(gamma), sinY = sin(gamma);
	
	return v3(cosY * v.x -sinY * v.y,
			  sinY * v.x +cosY * v.y,
			  v.z
			);
}

vec3	rotate3(vec3 v, vec3 r)
{
	double _a = r.x, _b = r.y, _y = r.z;
	double cosA = cos(_a), sinA = sin(_a);
	double cosB = cos(_b), sinB = sin(_b);
	double cosY = cos(_y), sinY = sin(_y);

	
	return v3(v.x*(cosB*cosY) + v.y*(sinA*sinB*cosY - cosA*sinY) + v.z*(cosA*sinB*cosY + sinA*sinY),
			  v.x*(cosB*sinY) + v.y*(sinA*sinB*sinY + cosA*cosY) + v.z*(cosA*sinB*sinY - sinA*cosY),
			  v.x*(-sinB) 	  + v.y*(sinA*cosB) 				 + v.z*(cosA*cosB));
}


vec3	rotate_around(vec3 v, vec3 p, vec3 axis, double angle)
{
	v = v_sub(v, p);
	vec3 rotated = v;
	rotated.x = v.x*cos(angle) + v_cross(axis, v).x*sin(angle) + axis.x*v_dot(axis, v)*(1-cos(angle));
    rotated.y = v.y*cos(angle) + v_cross(axis, v).y*sin(angle) + axis.y*v_dot(axis, v)*(1-cos(angle));
    rotated.z = v.z*cos(angle) + v_cross(axis, v).z*sin(angle) + axis.z*v_dot(axis, v)*(1-cos(angle));
	v = v_add(rotated, p);
	return v;
}

vec3	world_to_local(vec3 cam_pos, vec3 cam_rot, vec3 pos)
{
	return mult_point_matrix(v_sub(pos, cam_pos), v.rotation_matrix);
	(void) cam_rot;
	//return rotate3(v_sub(pos, cam_pos), v_scal(cam_rot, -1));
}

vec3	reverse_project(vec3 pos, double vfov, double aspect)
{
	double f = pos.z * tan(vfov * DEG2RAD * 0.5f);
	pos.x *= f * aspect;
	pos.y *= f;
	return pos;
}

vec3	project(vec3 pos, double vfov, double aspect)
{
	double f = 1.0 / tan(vfov * DEG2RAD * 0.5f);
	if (v.orthographic_toggle)
		f /= 10;
	else
		f /= pos.z;
	pos.x *= f / aspect;
	pos.y *= f;
	return pos;
}

vec3	clip_space_to_viewport(vec3 pos)
{
	pos.x = pos.x * 0.5f + 0.5f;
	pos.y = pos.y * 0.5f + 0.5f;
	return pos;
}

vec3	world_to_viewport(vec3 cam_pos, vec3 cam_rot, double vfov, double aspect, vec3 pos)
{
	vec3 p = world_to_local(cam_pos, cam_rot, pos);
	p = project(p, vfov, aspect);
	return clip_space_to_viewport(p);
}

vec3	world_to_screenpos(vec3 pos)
{
	vec3 p = world_to_viewport(v.camera_pos, v.camera_rot, v.vfov, (double)v.w / (double)v.h, pos);
	p.x *= v.w;
	p.y *= v.h;
	p.y = v.h - p.y;
	return p;
}

void	print_pos(vec3 pos, char *msg)
{
	printf("%s (%f, %f, %f)\n", msg, pos.x, pos.y, pos.z);
}

//you gotta guarantee pos is in bounds
//1 means failure
int	check_heat(vec3 pos)
{
	int x = pos.x;// / (double)v.upscale;
	int y = pos.y;// / (double)v.upscale;
	return (pos.z < 0
		|| (pos.z > v.dist_heatmap[x][y]));
}

void	set_heat(vec3 pos)
{
	int x = pos.x;// / (double)v.upscale;
	int y = pos.y;// / (double)v.upscale;
	v.dist_heatmap[x][y] = pos.z;
}

quat	euler_to_quaternion(vec3 euler) {
    quat q;
    double t0 = cos(euler.y * 0.5f);
    double t1 = sin(euler.y * 0.5f);
    double t2 = cos(euler.z * 0.5f);
    double t3 = sin(euler.z * 0.5f);
    double t4 = cos(euler.x * 0.5f);
    double t5 = sin(euler.z * 0.5f);

    q.w = t0 * t2 * t4 + t1 * t3 * t5;
    q.x = t0 * t3 * t4 - t1 * t2 * t5;
    q.y = t0 * t2 * t5 + t1 * t3 * t4;
    q.z = t1 * t2 * t4 - t0 * t3 * t5;
    return q;
}

vec3	quaternion_to_euler(quat q) {
    
	vec3 euler;

	//Pitch (x angle)
	euler.x = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x*q.x + q.y*q.y));
	//Yaw (y angle)
	euler.y = asin(2 * (q.w * q.y - q.x * q.z));
	//Roll (z angle)
	euler.z = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y*q.y + q.z*q.z));

	return euler;
	//---------

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (fabs(sinp) >= 1)
        euler.y = copysign(DEG2RAD, sinp); // use 90 degrees if out of range
    else
        euler.y = asin(sinp);

    return euler;
}

// ------Interval
Bool	contains(interval _t, double x)
{
	return _t.min <=x && x <= _t.max;
}

Bool	surrounds(interval _t, double x)
{
	return _t.min < x && x < _t.max;
}

double	clamp(interval _t, double x)
{
	if (x < _t.min) return _t.min;
	if (x > _t.max) return _t.max;
	return x;
}

double	clamp_(double x)
{
	if (x < 0.0) return 0.0;
	if (x > 1.0) return 1.0;
	return x;
}

double	linear_to_gamma(double linear_component)
{
	return sqrt(linear_component);
}

Bool	near_zero(vec3 e)
{
	double s = 1e-8;
	return (fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s);
}

vec3	reflect(vec3 v, vec3 n)
{
    return v_sub(v, v_scal(n, 2 * v_dot(v, n)));
}

vec3	refract(vec3 uv, vec3 n, double etai_over_etat)
{
    double cos_theta = fmin(v_dot(v_scal(uv, -1), n), 1.0);
    vec3 r_out_perp =  v_scal(v_add(uv, v_scal(n, cos_theta)), etai_over_etat);
    vec3 r_out_parallel = v_scal(n, -sqrt(fabs(1.0 - length_squared(r_out_perp))));
    return v_add(r_out_perp, r_out_parallel);
}

double	reflectance(double cosine, double ref_idx)
{
        // Use Schlick's approximation for reflectance.
        double r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
}

vec3	lookRotation(vec3 lookfrom, vec3 lookat) {
    vec3 dir = v_sub(lookat, lookfrom);
	dir = v_norm(dir);

	vec3 rot = v.camera_rot;
	rot.x = atan2(-dir.y, sqrt(dir.z*dir.z + dir.x*dir.x));
	rot.y = atan2(dir.x, dir.z);

    return rot;
}

vec3	look_at(vec3 lookfrom, vec3 lookat, vec3 up) {
    vec3 xaxis = v_norm(v_sub(lookat, lookfrom));
	vec3 zaxis = v_norm(v_cross(up, xaxis));
	vec3 yaxis = v_cross(xaxis, zaxis);


	/*
	x.x  y.x  z.x  0
	x.y  y.y  z.y  0
	x.z  y.z  z.z  0
	?    ?    ?    1
	*/
	double r10 = yaxis.x;
	double r00 = xaxis.x;
	double r01 = xaxis.y;
	double r02 = xaxis.z;

	double r20 = zaxis.x;

	double r21 = zaxis.y;
	double r22 = zaxis.z;

	double yaw = atan2(r00, r02);//r10, r00);

	double pitch = atan2(-r01, sqrt(r00*r00 + r02*r02));//(-r20, sqrt(r21*r21 + r22*r22));

	double roll = -atan2(r21, r22);

	vec3 rot = v3(pitch, yaw, roll);

    return rot;
}

// Define the constants for the ACES tone mapping function
#define A 2.51
#define B 0.03
#define C 2.43
#define D 0.59
#define E 0.14

// ACES tone mapping function
double ACES(double x) {
    return (x * (A * x + B)) / (x * (C * x + D) + E);
}
