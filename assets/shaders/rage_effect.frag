#version 330

uniform float time;
uniform vec2 resolution;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    float wave1 = sin(uv.x * 10.0 + time * 2.0) * 0.5 + 0.5;
    float wave2 = cos(uv.y * 8.0 - time * 1.5) * 0.5 + 0.5;
    float wave3 = sin((uv.x + uv.y) * 6.0 + time * 3.0) * 0.5 + 0.5;
    float wave4 = cos((uv.x - uv.y) * 12.0 - time * 2.5) * 0.5 + 0.5;
    
    vec2 center = vec2(0.5, 0.5);
    float dist = length(uv - center);
    float radialWave = sin(dist * 15.0 - time * 4.0) * 0.5 + 0.5;
    
    float diagonal = sin((uv.x + uv.y) * 5.0 - time * 3.0) * 0.5 + 0.5;
    
    vec3 color1 = vec3(1.0, 0.1, 0.2); 
    vec3 color2 = vec3(1.0, 0.4, 0.0); 
    vec3 color3 = vec3(1.0, 0.8, 0.0);
    vec3 color4 = vec3(1.0, 0.0, 0.5);  
    vec3 color5 = vec3(0.8, 0.2, 1.0); 

    vec3 finalColor = mix(color1, color2, wave1);
    finalColor = mix(finalColor, color3, wave2 * diagonal);
    finalColor = mix(finalColor, color4, wave3 * radialWave);
    finalColor = mix(finalColor, color5, wave4 * 0.3);
    
    finalColor += vec3(radialWave * 0.15);
    finalColor += vec3(wave3 * diagonal * 0.2);
    
    float alpha = 0.2;
    
    gl_FragColor = vec4(finalColor * alpha, alpha);
}
