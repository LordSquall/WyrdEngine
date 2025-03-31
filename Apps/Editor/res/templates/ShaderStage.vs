// Name: <<NAME>>
// Stage: Vertex
#version 330 core

<<INPUT_ATTRIBUTES>>
<<INPUT_UNIFORMS>>
<<OUTPUT_ATTRIBUTES>>
void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_Position, 1.0);	
}