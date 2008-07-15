varying vec4 tex_coords;
varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
varying vec3 light_dir;
uniform mat4 model2light;
uniform mat4 mv_mat;

void main() {

   // Transform the normal.
   normal = normalize(gl_NormalMatrix * gl_Normal);

   // This should really be a uniform since directional lights don't need
   // to be interpolated.
   light_dir = normalize(gl_LightSource[0].position.xyz);

   // Modulate the light source colors by the material parameters.
   diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
   ambient = (gl_LightSource[0].ambient + gl_LightModel.ambient) *
             gl_FrontMaterial.ambient;

   // Compute vertex's coordinates in light space.
   tex_coords = gl_TextureMatrix[0] * gl_Vertex;
   /* tex_coords = model2light * gl_Vertex; */
   /* tex_coords = model2light * mv_mat * gl_Vertex; */

   // Transform vertex into screen space.
   gl_Position = ftransform();

}
