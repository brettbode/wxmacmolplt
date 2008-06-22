varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
uniform vec3 light_dir;

void main() {

   // Shade fragment with ambient, diffuse, and specular.
   vec4 color = ambient;
   vec3 n = normalize(normal);   // must renormalize after interpolation
   float n_dot_l = max(dot(n, light_dir), 0.0);
   color += diffuse * n_dot_l;
   float n_dot_h = max(dot(n, gl_LightSource[0].halfVector.xyz), 0.0);
   color += specular * pow(n_dot_h, gl_FrontMaterial.shininess);

   gl_FragColor = color;

}
