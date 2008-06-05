varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec3 normal;
varying vec3 light_dir;
varying vec3 half_vec;

void main() {

   vec3 n; // Local copies of normal and half_vec since varying
   vec3 h; //    variables are read-only and we normalize them.
   float n_dot_l;
   float n_dot_h;

   vec4 color = ambient;
   n = normalize(normal);
   n_dot_l = max(dot(n, light_dir), 0.0);

   // If the fragment faces the light source, we light it according to
   // its degree of facing the light source.
   if (n_dot_l > 0.0) {
      color += diffuse * n_dot_l;
      h = normalize(half_vec);
      n_dot_h = max(dot(n, half_vec), 0.0);
      color += specular * pow(n_dot_h, gl_FrontMaterial.shininess);
   }

   gl_FragColor = color;

}
