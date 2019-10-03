#version 110

uniform float time;

uniform vec2 player_position;
uniform vec2 player_size;
uniform vec2 view_position;

varying float light_level;

float process_viewcone(float llev, vec2 pos)
{
    return dot(pos - view_position, player_position - pos);
}

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    //light_level = distance(gl_Vertex.xy, player_position + player_size / 2.) / 90.;
    light_level = distance(gl_Vertex.xy, player_position + player_size / 2.);
    light_level = (light_level * light_level) / 80./ 80.;
    light_level -= .1;
    
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}
