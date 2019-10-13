#version 110

uniform sampler2D texture;
uniform vec2 texture_size;
uniform vec2 tile_position;
uniform float time;

in float light_level;
in vec4 gl_TexCoord[1];

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    /*)
    
    // multiply it by the color
    //float modulate = abs(sin(time/300.)) * .02;
    float dist = .003 * distance(player_position + player_size / 2., tile_position + texture_size / 2.);
    vec2 tile_player = player_position - tile_position;
    gl_FragColor = pixel * 1. - (dist);// +dsw modulate*0.1;
    gl_FragColor = pixel;
    */
    gl_FragColor = pixel * (1.- light_level);
}
