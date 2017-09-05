#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D u_texture;
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
uniform mat4 fiterMat;
void main(void)
{
    vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord);
    gl_FragColor = fiterMat*value;
};
