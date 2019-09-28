#version 330

layout(origin_upper_left) in vec4 gl_FragCoord;
in vec4 gl_TexCoord[1];

uniform sampler2D texture;
uniform vec2 player_position;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    
    gl_FragColor = vec4(distance(gl_FragCoord.xy, player_position) / 1000);
}
