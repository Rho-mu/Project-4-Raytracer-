#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

///// STRUCTS /////
typedef struct v3 v3;
struct v3 {
  double x;
  double y;
  double z;
};

typedef struct Object Object;
struct Object {
  v3 position;
  v3 normal;
  v3 color;
  v3 specular_color;
  v3 diffuse_color;
  double radius;
  double reflectivity;
  double refractivity;
  double ior;
  char* kind;
};
Object objects[3]; // Array to store Objects. I know it's in the golbal, but I haven't found a good place for it yet.

typedef struct Light Light;
struct Light {
  v3 position;
  int is_on;
  char* kind;
  double theta;
  double radial_a0;
  double radial_a1;
  double radial_a2;

};
Light lights[3];  // Array to store lights. I know it's in the golbal, but I haven't found a good place for it yet.

typedef struct Pixel Pixel;
struct Pixel {
  unsigned char r, g, b;
};

///// VECTOR FUNCTIONS /////
double v3_dot(v3 a, v3 b) {   // Takes the dot product of 2 vectors, and returns it as a double. output = a (dot) b
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
} // END: v3_dot()
double v3_mag(v3 a) {         // Takes the maginitude of a vector, and returns it as a double. output = ||a||
  return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
} // END: v3_mag()
v3 v3_scale(v3 a, double s) { // Scales a vector by s, and returns the resulting vector. output = a * s
  v3 c;
  c.x = a.x * s;
  c.y = a.y * s;
  c.z = a.z * s;
  return c;
} // END: v3_scale()
v3 v3_sub(v3 a, v3 b) {       // Subtracts vector b from vector a, and returns the resulting vector. output = a - b
  v3 c;
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
  return c;
} // END: v3_sub()
v3 v3_add(v3 a, v3 b) {       // Adds vector a and vector b, and returns the resulting vector. output = a + b
  v3 c;
  c.x = a.x + b.x;
  c.y = a.y + b.y;
  c.z = a.z + b.z;
  return c;
} // END: v3_add()
v3 v3_mult(v3 a, v3 b) {      // Multiplies 2 vectors, and returns the resulting vector. output = a * b
  v3 c;
  c.x = a.x * b.x;
  c.y = a.y * b.y;
  c.z = a.z * b.z;
  return c;
} // END: v3_mult()
v3 v3_make_unit(v3 a) {       // Normalizes a vector into its unit vector form, and returns that unit vector. output = a / ||a||
  double mag = v3_mag(a);
  v3 c;
  c.x = a.x / mag;
  c.y = a.y / mag;
  c.z = a.z / mag;
  return c;
} // END: v3_make_unit()

///// OBJECT FUNCTIONS /////
Object new_sphere(double px, double py, double pz, double cx, double cy, double cz, double radius, double scx, double scy, double scz, double dcx, double dcy, double dcz, double refl, double refr, double ior) {                  // Creates a new sphere object.
  Object o;
  o.position.x = px;
  o.position.y = py;
  o.position.z = pz;
  o.color.x = cx;
  o.color.y = cy;
  o.color.z = cz;
  o.radius = radius;
  o.kind = "sphere";

  o.specular_color.x = scx;
  o.specular_color.y = scy;
  o.specular_color.z = scz;
  o.diffuse_color.x = dcx;
  o.diffuse_color.y = dcy;
  o.diffuse_color.z = dcz;
  o.reflectivity = refl;
  o.refractivity = refr;
  o.ior = ior;
  return o;
} // END: new_sphere()
Object new_plane(double px, double py, double pz, double cx, double cy, double cz, double nx, double ny, double nz, double scx, double scy, double scz, double dcx, double dcy, double dcz, double refl, double refr, double ior) { // Creates a new plane object.
  Object o;
  o.position.x = px;
  o.position.y = py;
  o.position.z = pz;
  o.color.x = cx;
  o.color.y = cy;
  o.color.z = cz;
  o.normal.x = nx;
  o.normal.y = ny;
  o.normal.z = nz;
  o.kind = "plane";

  o.specular_color.x = scx;
  o.specular_color.y = scy;
  o.specular_color.z = scz;
  o.diffuse_color.x = dcx;
  o.diffuse_color.y = dcy;
  o.diffuse_color.z = dcz;
  o.reflectivity = refl;
  o.refractivity = refr;
  o.ior = ior;
  return o;
} // END: new_plane()

///// LIGHT FUNCTIONS /////
Light new_light(double px, double py, double pz, double is_on, char* kind, double theta, double a0, double a1, double a2) {  // Creates a new Light.
  Light l;
  l.position.x = px;
  l.position.y = py;
  l.position.z = pz;
  l.is_on = is_on;
  l.kind = kind;
  l.theta = theta;
  l.radial_a0 = a0;
  l.radial_a1 = a1;
  l.radial_a2 = a2;
  return l;
} // END: new_light()
void turn_off(Light light) {  // Turns the light off.
  light.is_on = 0;  // 0 is off.
} // END: turn_off()
void turn_on(Light light) {   // Turns the light on.
  light.is_on = 1;  // 1 is on.
} // END: turn_on()

///// PARSE FUNCTIONS /////
// NOTE: I am planning to add a function to parse other necessary elements.
//        I've been having trouble with parsing, so I decided to hard-code
//        values for stuff to at least get the rest of the program working.
void parse_camera(FILE* file, char str, Object obj) { // Parses the camera data.
  /*
  // Iterates over elements of camera in text file.
  for(int i = 0; i < 2; i++) {
    fscanf(file, "%[^:]", str);         // Scan to : to get next value type.
    // If width.
    if(strcmp(str, "width") == 0) {
      fscanf(file, "%[^,]", str);       // Scan to , to get value.
      obj.width = atoi(str);            // Saves value into object width.
      // Now get height.
      fscanf(file, "%[^:]", str);       // Scan to : to get next value type.
      if(strcmp(str, "height") == 0) {
        fscanf(file, "%[^,]", str);     // Scan to , to get value.
        obj.height = atoi(str);         // Saves value into object height.
      }
    }
    // If height.
    if(strcmp(str, "height") == 0) {
      fscanf(file, "%[^,]", str);       // Scan to , to get value.
      obj.height = atoi(str);           // Saves value into object height.
      // Now get width.
      fscanf(file, "%[^:]", str);       // Scan to : to get next value type.
      if(strcmp(str, "width") == 0) {
        fscanf(file, "%[^,]", str);     // Scan to , to get value.
        obj.width = atoi(str);          // Saves value into object width.
      }
    }
  } // END: for-loop*/
} // END: parse_camera()
int read(FILE* file) { // Reads the input file for data on the scene, and returns the number of objects.
  /* --MINIMIZE--
  // Iterates over each object in text file.
  for(int i = 0; i < 4; i++) {
    char str;
    fscanf(file, "%[^,]", str);
    if(strcmp(str, "camera") == 0) {
      //objects[i].kind = CAMERA;
      parse_camera(file, str, objects[i]);
    }
  } // END: for-loop
  fclose(file);
  */
  int object_count = 1; // Hard-coded number of objects.
  for(int i = 0; i < object_count; i++) {
    char* str = "sphere";
    // If the read data is for a sphere...
    if(strcmp(str, "sphere") == 0) {
      Object obj = new_sphere(2,2,0,0,255,255,2,1,0,0,1,1,1,0.2,0.3,1.33);     // Create new sphere object.
      objects[i] = obj;
    } // END: If sphere
    // If the read data is for a plane...
    if(strcmp(str, "plane") == 0) {
      Object obj = new_plane(1,2,3,255,0,255,1,2,3,0,1,0,1,1,1,0,0,0);  // Create new plane object.
    } // END: If plane
  } // END: for-loop
  return object_count;
} // END: read()

///// INTERSECTION FUNCTIONS /////
Object has_intersection(v3 ray) { // Finds the nearest intersection (if there is one), and returns the object that is intersected.
  double best_t = INFINITY;
  double current_t = INFINITY;

  return objects[0]; // Hard coded to return my sphere.
} // END: has_intersection()
int sphere_intersection_test(Object obj, v3 ray) { // Finds where the ray intersects the sphere, and returns 0, 1, or 2 depending on the number of intersections.
  v3 vzero;   // Zero vector.
  vzero.x = 0;
  vzero.y = 0;
  vzero.z = 0;

  v3 neg_pos = v3_sub(vzero, obj.position); // Get the negative center of sphere for calculations.
  double a = v3_dot(ray, neg_pos);          // V3 ray dot -center
  double b = v3_dot(neg_pos, neg_pos);      // -center dot -center
  double t = ((a * a) - (b * b) + (obj.radius * obj.radius)); // t = [(ray dot -center)^2 - (-center dot -center)^2 +r^2]

  if(t < 0) { return 0; } // No intersections.
  if(t = 0) { return 1; } // One intersection.
  if(t > 0) { return 2; } // Two intersections.
  return 0; // Return 0 just in case.
} // END: sphere_intersection_test
int plane_intersection_test(Object obj, v3 ray) {  // Finds where the ray intersects the plane.
  // TODO: Intersetion test for a ray and plane.
  return 0;
} // END: plane_intersection_test()

void refract(v3 ray, int object_count, int light_count, int k, Object obj) {
  v3 refr_ray;
  v3 n = obj.normal;
  v3 b;
  double pr = 0;       // IOR of external region.
  double pt = obj.ior; // IOR of internal region.
  double sin_phi;
  double cos_phi;

  sin_phi = v3_dot(ray, b) * (pr / pt);     // Calculates sin(phi).
  cos_phi = sqrt(1 - (sin_phi * sin_phi));  // Calculates cos(phi).

  v3 minus_n = v3_scale(n, -1);                       // Makes n negative for final equation.
  v3 minus_n_x_cos_phi =  v3_scale(minus_n, cos_phi); // Calculates (-n)(cos_phi) for final equation.
  v3 b_x_sin_phi = v3_scale(b, sin_phi);              // Calculates (b)(sin_phi) for final equation.

  refr_ray = v3_add(minus_n_x_cos_phi, b_x_sin_phi);  // Final equation. Based on Ut = (-n)(cos_phi) + (b)(sin_phi).

  //shoot(refl_ray, object_count, light_count, k);
} // END: refract()
void reflect(v3 ray, int object_count, int light_count, int k, Object obj) {
  v3 refl_ray;
  v3 n = obj.normal;

  double ray_dot_n = v3_dot(ray, n);                 // Calcualtes (ray dot n) for final equation.
  v3 scaled_ray_dot_n = v3_scale(n, (2* ray_dot_n)); // Calculates 2(ray dot n)*n for final equation.

  refl_ray = v3_sub(ray, scaled_ray_dot_n);          // Final equation. Based on Um = Ur - 2(Ur dot n)*n.
  //shoot(refr_ray, object_count, light_count, k);
} // END: refract()

v3 shoot(v3 ray, int object_count, int light_count, int k) { // Shoots a ray in the direction of the unit vector, and returns a color vector to color a pxiel.
  v3 color;
  v3 bkgd_color;      // Set background color.
  bkgd_color.x = 25;  // <0,0,0> for black.
  bkgd_color.y = 25;
  bkgd_color.z = 100;
  int hit = 0;        // Initialized to 0 for no intersection (miss).

  Object obj = has_intersection(ray);         // Tests for any intersection, and returns the object.
  if(strcmp(obj.kind, "sphere") == 0) {       // If object is a sphere, do sphere intersection test.
    hit = sphere_intersection_test(obj, ray);   // Set hit to the return value of sphere intersection test.
    obj.normal = v3_sub(ray, obj.position);     // Set normal of sphere to intersection - center.
  }
  if(strcmp(obj.kind, "plane") == 0) {        // If object is a plane, do plane intersection test.
    hit = plane_intersection_test(obj, ray);    // Set hit to the return value of plane intersection test.
  }

  // Reflection and Refraction
  if(k <= 5) {  // K counts how many times shoot() is called recursively. 5 is the limit of calls.
      if(obj.reflectivity == 0 && obj.refractivity == 0) {  // If there is no refraction or reflection, just return the color.
    return color;
  }
      if(obj.reflectivity > 0) {                            // If there is reflection, shoot a new reflection ray from reflect function.
        k = k + 1;  // Increase limit counter by 1;
        reflect(ray, object_count, light_count, k, obj);
      }
      if(obj.refractivity > 0) {                            // If there is refraction, shoot a new refractionray from refract function.
        k = k + 1;  // Increase limit counter by 1;
        refract(ray, object_count, light_count, k, obj);
      }
  } // END: Reflect and Refract cases.

  if(ray.x < 0.0000000001 || ray.y < 0.0000000001) {  // Tests if there is an intersection with printf. I limited it, so it will only print a portion of the grid.
    if(hit > 0) { printf("1"); }
    if(hit <= 0) { printf("0"); }
  }

  if(hit > 0) { // Set color to the color of the object of intersection.
    double f_rad;           // Radial attenuation.
    double f_ang;           // Angular attenuation.
    double ns;
    double alpha = 3.14/2;
    double dl;              // Distance from ray to light.

    Light light = new_light(1,1,1,1,"radial",3.14,0,0,0);        // Hard-codeed light;
    lights[0] = light;                                           // Put light into light array.

    //dl = sqrt((light.position.x - ray.x)(light.position.x - ray.x) + (light.position.y - ray.y)(light.position.y - ray.y) + (light.position.z - ray.z)(light.position.z - ray.z));
    dl = 5; // Hard-coded dl, becasue ^this^ wasn't working.

    for(int i = 0; i < light_count; i++) {
      // Radial
      if(lights[i].kind = "radial") {
        if(dl > 99999999) { f_rad = 1.0; }                                                                           // Case 1 for radial. dl is inf.
        else { f_rad = (1 / ((dl * dl * lights[i].radial_a2) + (dl * lights[i].radial_a1)) + lights[i].radial_a0); } // Case 2 for radial. dl is not inf.
      } // END: If radial
      // Angular
      if(lights[i].kind = "angular") {                                                    // Case 1 would be "not a spotlight."
        if(alpha > lights[i].theta) { f_ang = 0; }                                        // Case 2 for angular if alpha is > theta.
        if(alpha <= lights[i].theta) { f_ang = pow(v3_dot(light.position, ray), alpha); } // Case 3 for angular if alpha is <= theta.
      } // END: If angular
    } // END: for-loop

    color.x = obj.color.x;
    color.y = obj.color.y;
    color.z = obj.color.z;
  } // END: if hit
  else {        // Set color to background color if no intersection is made.
    color.x = bkgd_color.x;
    color.y = bkgd_color.y;
    color.z = bkgd_color.z;
  }
  return color;
} // END: shoot()

Pixel* draw(int width, int height, int object_count, int light_count) { // Creates an array of data to be displayed in an output file, and return it as a pixmap.
  int row, col; // Row and Column trackers for for-loops.
  Pixel* pixmap = malloc(sizeof(Pixel) * width * height); // Allocate memory for a pixmap with the size of any grid.
  v3 color;     // Create color vector for each ray.
  v3 ray;       // Create ray vector.
  ray.z = -20; // Initialize z part of ray to -0.5 for camera location. (-focal length)

  for(row = 0; row < height; row++) {
    for(col = 0; col < width; col++) {
      ray.x = row;              // Set new ray x to the current row.
      ray.y = col;              // Set new ray y to the current column.
      ray = v3_make_unit(ray);  // Turns the ray into a unit vector.
      color = shoot(ray, object_count, light_count, 0);  // Shoots the ray at the scene.
      // Colors the current pixel depending on what the ray hit when shot.
      pixmap[(width * row) + col].r = color.x;
      pixmap[(width * row) + col].g = color.y;
      pixmap[(width * row) + col].b = color.z;
    } // END: col for-loop.
  } // END: row for-loop.
  return pixmap;  // Returns the pixmap to be writen to a file.
} // END: draw()

int main(int argc, char* argv[]) {

  // Makes sure you have 4 arguments.
  if(argc != 5) {
    printf("ERROR: You need 4 arguments!\n");
    printf("ERROR: Please enter ./raycast (width) (height) (input file) (output file)\n");
    printf("ERROR: Example: ./raycast 30 15 mySceneData.txt myImage.data\n");
    return 1;
  } // END CHECK: Incorrect argument count

  FILE* inputFile = fopen(argv[3], "r");  // Opens the input file.
  FILE* outputFile = fopen(argv[4], "w"); // Opens the output file.

  int w_height = atoi(argv[2]); // Stores height into M in MxN grid.
  int w_width = atoi(argv[1]); // Stores width into N in MxN grid.

  // CHECK: Makes sure you don't enter negative (or 0) values for the width and height.
  if(w_width < 1 || w_height < 1) {
    printf("ERROR: Width or height is out of bounds!\n");
    printf("ERROR: Please enter values greater than 0!\n");
    printf("ERROR: Example: ./raycast3 30 15\n");
    return 1;
  } // END CHECK: Argument out of bounds
  /* --MINIMIZE--
  // Code that I might still need.
  double p_height = w_height / M; // Height of a pixel.
  double p_width = w_width / N;   // Width of a pixel.

  v3 ray;
  v3 unit;
  ray.z = -0.5; // Initialize camera at -0.5 on the z axis.

  for(int i = 0; i < M; i++) {
    ray.y = 0 - (worldHeight / 2) + (pixHeight * (i + 0.5)); // Y coord of pixel.

    for(int j = 0; j < N; j++) {
      ray.x = 0 - (worldWidth / 2) + (pixWidth * (j + 0.5)); // X coord of pixel.

      printf(" \n");
      printf("Ray: <%f,%f,%f>\n", ray.x, ray.y, ray.z);
      unit = v3_make_unit(ray);
      printf("Unit: <%f,%f,%f>\n", unit.x, unit.y, unit.z);

      v3 x;
      x = shoot(unit);
      printf("<%f,%f,%f>", x.x, x.y, x.z);
    } // END: j for-loop
  } // END: i for-loop
  */
  int object_count = read(inputFile);     // Gets the number of objects in the scene from reading the file.
  int light_count = 1;                    // Hard-coded light count for testing.
  for(int i = 0; i < object_count; i++) { // Testing loop that prints out each object and it's position vector.
    printf("Object%d - %s: <%4.2f,%4.2f,%4.2f>\n", i, objects[i].kind, objects[i].position.x,objects[i].position.y,objects[i].position.z);
  }
  for(int i = 0; i < object_count; i++) { // Testing loop that prints out each light and it's kind.
    printf("Light%d - %s\n", i, lights[i].kind);
  }
  Pixel* pixmap = draw(w_width, w_height, object_count, light_count); // Generates pixmap through draw() function.
  fwrite(pixmap, sizeof(Pixel), w_width * w_height, outputFile);  // Writes the pixmap data to an output file.
  fclose(inputFile);  // Closes the input file.
  fclose(outputFile); // Closes the output file.
  return 0;
} // END: main()
