uniform sampler2DShadow depth_map;
/* uniform sampler2D depth_map; */
varying vec4 tex_coords;
varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
varying vec3 light_dir;

void main() {

   vec3 n; // Local copy of normal since varyings can't be changed.
   float n_dot_l;
   float n_dot_h;

   vec4 color = ambient;
   n = normalize(normal);
   n_dot_l = max(dot(n, light_dir), 0.0);

   // If the fragment faces the light source, we light it according to
   // its degree of facing the light.
   if (n_dot_l > 0.0) {
      /* color += texture2DProj(depth_map, tex_coords) * diffuse * n_dot_l; */
      color += diffuse * n_dot_l;
      n_dot_h = max(dot(n, gl_LightSource[0].halfVector.xyz), 0.0);
      color += specular * pow(n_dot_h, gl_FrontMaterial.shininess);
   }

   gl_FragColor = color * shadow2DProj(depth_map, tex_coords).r;

}
