/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#version 330 core

uniform vec4 color;
uniform vec2 innerShift;
uniform vec2 outerShift;
uniform mat2 innerTransform;
uniform mat2 outerTransform;
uniform float aafactor;

float localAlpha(in vec2 shift, in mat2 transform) {
    vec2 local = transform * (gl_FragCoord.xy - shift);
    float radius = distance(local, vec2(0.5));
    float aarad = aafactor * fwidth(radius);
    return smoothstep(0.5 + aarad, 0.5 - aarad, radius);
}

void main() {
    float outerAlpha = localAlpha(outerShift, outerTransform);
    float innerAlpha = 1. - localAlpha(innerShift, innerTransform);
    gl_FragColor = color * vec4(vec3(1.), outerAlpha * innerAlpha);
}
