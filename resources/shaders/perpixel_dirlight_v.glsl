varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
uniform vec3 light_dir;

void main() {

   // Transform the normal.
   normal = normalize(gl_NormalMatrix * gl_Normal);

   // Modulate the light source colors by the material parameters.
   diffuse = gl_Color * gl_LightSource[0].diffuse;
   specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
   ambient = (gl_LightSource[0].ambient + gl_LightModel.ambient) * gl_Color;

   // Transform vertex into screen space.
   gl_Position = ftransform();

}
