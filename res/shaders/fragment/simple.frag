#version 110

uniform sampler2D texture;

uniform vec2 player_position;
uniform vec2 texture_size;
uniform vec2 tile_position;
uniform float time;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    
    // multiply it by the color
    //float modulate = abs(sin(time/300.)) * .02;
    float dist = .003 * distance(player_position + texture_size * 0.8 / 2., tile_position + texture_size / 2.);
    vec2 tile_player = player_position - tile_position;
    gl_FragColor = pixel * 1. - (dist);// + modulate*0.1;
}
