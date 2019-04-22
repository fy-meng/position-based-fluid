#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform vec4 u_color;

uniform sampler2D u_texture_3;
uniform vec2 u_texture_3_size;

uniform float u_normal_scaling;
uniform float u_height_scaling;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;

float h(vec2 uv) {
  // Returns the height given texture coordinate
  return texture(u_texture_3, uv).r;
}

void main() {
  // Compute displaced model space normal
  mat3 tbn = mat3(v_tangent.xyz, cross(v_normal.xyz, v_tangent.xyz), v_normal.xyz);
  float du = (h(vec2(v_uv.x + 1 / u_texture_3_size.x, v_uv.y)) - h(v_uv)) * u_normal_scaling * u_height_scaling;
  float dv = (h(vec2(v_uv.x, v_uv.y + 1 / u_texture_3_size.y)) - h(v_uv)) * u_normal_scaling * u_height_scaling;
  vec3 nd = tbn * vec3(-du, -dv, 1);

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
                  + kd * illum * max(0, dot(nd, normalize(wi)))
                  + ks * illum * pow(max(0, dot(nd, h)), p);
  out_color.a = 1;
}

