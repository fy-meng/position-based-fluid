#version 330


uniform vec3 u_cam_pos;

uniform samplerCube u_texture_cubemap;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;

out vec4 out_color;

void main() {
  vec3 wo = normalize(u_cam_pos - v_position.xyz);
  vec3 wi = 2 * dot(wo, v_normal.xyz) * v_normal.xyz - wo;
  out_color = texture(u_texture_cubemap, wi);
}
