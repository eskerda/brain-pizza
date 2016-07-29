//
// Copyright (c) 2012 by Mickael Pointier.
// This work is made available under the terms of the Creative Commons Attribution-ShareAlike 3.0 Unported license,
// http://creativecommons.org/licenses/by-sa/3.0/.
//
#version 130

uniform sampler2D tilemap;
uniform sampler2D tileGraphics;

uniform vec2 tileCount;
uniform vec2 tilemapSize;
uniform float tileSize;

void main()
{

    float index=ceil(texture2D(tilemap,gl_TexCoord[0].xy/tileSize).r*255);
    vec2 tilePos=vec2(mod(index,tileCount.x),floor(index/tileCount.x))/tileCount;
    vec2 tileOffset=mod((gl_TexCoord[0].xy*tilemapSize)/tileSize,1.0);
    //gl_FragColor=vec4(index/256.f,0,texture2D(tileGraphics,tilePos+tileOffset/tileCount).b,1);
    gl_FragColor=texture2D(tileGraphics,tilePos+tileOffset/tileCount);
}
