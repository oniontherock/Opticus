uniform sampler2D rayPositions;
uniform sampler2D worldTexture;
uniform vec2 worldSize;

const vec4 emptyColor = vec4(0.0, 0.0, 0.0, 0.0);


vec4 getColorFromEncoded(vec4 encoded) {

	encoded *= 255.0;

	vec2 rayPosWorld = vec2((encoded.r * 255) + encoded.g, (encoded.b * 255) + encoded.a);

	vec2 rayPosTex = vec2(rayPosWorld.x / worldSize.x, 1.0 - (rayPosWorld.y / worldSize.y));

	return texture2D(worldTexture, rayPosTex);
}
void main() {
	

	vec2 rayTexSize = textureSize(rayPositions, 0);
	vec2 pixelSize = vec2(1.0, 1.0) / rayTexSize;
	vec2 texCoord = gl_TexCoord[0].xy;

	vec4 color;

	int count = 0;

	for (float x = -pixelSize.x; x <= +pixelSize.x; x += pixelSize.x) {
		for (float y = -pixelSize.y; y <= +pixelSize.y; y += pixelSize.y) {

			vec2 offsetTexCoord = texCoord + vec2(x, y);

			vec4 curNeighborColorEncoded = texture2D(rayPositions, offsetTexCoord);
	
			if (curNeighborColorEncoded == emptyColor) {
				continue;
			}

			if (x == 0 && y == 0) {
				count = 1;
				color = getColorFromEncoded(curNeighborColorEncoded);
				x = 9999;
				y = 9999;
				break;
			}

			count++;
			color += getColorFromEncoded(curNeighborColorEncoded);
		}
	}


	if (count > 0) {
		color /= count;
	}

	gl_FragColor = color;
}

