attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCoord;

//uniform mat4 rotateMatrix;

varying lowp vec2 varyTextCoord;

void main()
{
    varyTextCoord = aTexCoord;
    
    vec4 vPos = vec4(aPos, 1.0);

    //vPos = rotateMatrix * vPos;
    gl_Position = vPos;
}

