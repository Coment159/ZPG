#version 450

in vec2 vt_out;

uniform sampler2D textureUnitID;

out vec4 frag_colour;

void main () {
    vec4 color  = texture(textureUnitID, vt_out);
    frag_colour = vec4(color.rgb, 1);
}

