uniform sampler2D rayPositions;
uniform sampler2D worldColors;
uniform vec2 worldSize;

void main() {

	vec2 texPos = gl_TexCoord[0].xy;

	vec4 rayPosEncoded = texture2D(rayPositions, texPos);

	vec2 rayPos = vec2((rayPosEncoded.r * 255) * (rayPosEncoded.g * 255), (rayPosEncoded.b * 255) * (rayPosEncoded.a * 255));

	vec2 rayTexPos = vec2(rayPos.x / worldSize.x, rayPos.y / worldSize.y);

	vec4 worldColor = texture2D(worldColors, rayTexPos);

	gl_FragColor = worldColor;
}