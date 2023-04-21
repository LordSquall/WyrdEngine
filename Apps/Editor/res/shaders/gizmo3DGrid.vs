#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 nearPoint;
out vec3 farPoint;
out mat4 viewMat;
out mat4 projectionMat;

uniform mat4 u_view;
uniform mat4 u_projection;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(u_view);
    mat4 projInv = inverse(u_projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    viewMat = u_view;
    projectionMat = u_projection;

	gl_Position =  vec4(a_Position, 1.0);	

	nearPoint = UnprojectPoint(a_Position.x, a_Position.y, 0.0, u_view, u_projection); // unprojecting on the near plane
	farPoint = UnprojectPoint(a_Position.x, a_Position.y, 1.0, u_view, u_projection); // unprojecting on the far plane
}