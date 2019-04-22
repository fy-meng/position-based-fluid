#version 330

uniform vec4 u_color;
uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // Coefficient for Blinn-Phong model
  float kd = 0.7;
  float ks = 0.6;
  vec3 global_illum = 0.1 * vec3(1, 1, 1);
  int p = 48;

  // Copmute lighting using Blinn-Phong model
  vec3 wi = u_light_pos - v_position.xyz;
  vec3 illum = u_light_intensity / dot(wi, wi);
  vec3 h = normalize(u_light_pos + u_cam_pos - 2 * v_position.xyz);
  out_color.rgb = global_illum
                  + kd * illum * max(0, dot(v_normal.xyz, normalize(wi)))
                  + ks * illum * pow(max(0, dot(v_normal.xyz, h)), p);
  out_color.a = 1;
}

