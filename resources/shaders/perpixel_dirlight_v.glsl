varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
varying vec3 light_dir;
varying vec3 half_vec;

void main() {

   // Transform the normal.
   normal = normalize(gl_NormalMatrix * gl_Normal);

   // We use a directional light, so we can calculate these vectors in the
   // vertex shader.
   light_dir = normalize(vec3(gl_LightSource[0].position));
   half_vec = normalize(gl_LightSource[0].halfVector.xyz);

   // Modulate the light source colors by the material parameters.
   diffuse = gl_Color * gl_LightSource[0].diffuse;
   specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
   ambient = gl_Color * gl_LightSource[0].ambient;
   ambient += gl_LightModel.ambient * gl_Color;

   // Transform vertex into screen space.
   gl_Position = ftransform();

}
