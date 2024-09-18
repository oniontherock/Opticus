uniform sampler2D rayPositions;
uniform sampler2D worldTexture;
uniform vec2 worldSize;

const vec4 emptyColor = vec4(0.0, 0.0, 0.0, 0.0);

// the way a ray is encoded is as follows:
//
// we have a vec2 that stores 32-bit floats, lets call it rayPos,
// we need to turn this vec2, into a vec4, but the vec4 only stores 8-bit unsigned ints, lets call this vec4 encodedVec.
//
// so the question is, how do we turn a floating point value that can go higher than 255, into a value that only stores numbers as large as 255?
//
// we do this by having the r and b components of the encodedVec equal a chunk of 255 pixels in the x and y axis respectively.
// and the g and a components are used as  sub pixels within these 255 pixel chunks.
//
// so an x of 927, for example, would be represented as r=3, g=162, as (3*255)+162 == 927; for y, the same process is used, but with the b and a components instead.

// turn an encoded ray position into an actual ray position, then get the color at the ray's position from the world texture
vec4 colorGetFromEncodedPosition(vec4 encodedPosition) {

	// outside of the shader the color values are between 0-255, but in the shader, they are scaled to 0.0-1.0,
	// so we must scale them back up to get a correct ray position
	encodedPosition *= 255.0;
	// get the ray's position in the world.
	// a 
	vec2 rayPosWorld = vec2((encodedPosition.r * 255) + encodedPosition.g, (encodedPosition.b * 255) + encodedPosition.a);

	vec2 rayPosTex = vec2(rayPosWorld.x / worldSize.x, 1.0 - (rayPosWorld.y / worldSize.y));

	return texture2D(worldTexture, rayPosTex);
}
void main() {
	
	// get the size of the rayPositions texture
	vec2 rayTexSize = textureSize(rayPositions, 0);
	// get the size of a texel in the rayPositions texture
	vec2 texelSize = vec2(1.0, 1.0) / rayTexSize;
	// get the current texCoord
	vec2 texCoord = gl_TexCoord[0].xy;

	vec4 endColor;

	// amount of colors that are tallied up in the blur
	uint8_t count = 0;
	// iterate through the surrounding texels and add up their texel colors.
	// if the center texel (the texel at 0x0 in the loop) is NOT empty, then we just override the bluring and make the endColor equal the center texel color.
	// otherwise, we simply make the endColor the average of the surrounding texels.
	for (float x = -texelSize.x; x <= +texelSize.x; x += texelSize.x) {
		for (float y = -texelSize.y; y <= +texelSize.y; y += texelSize.y) {

			// the center texCoord, offset by x and y
			vec2 offsetTexCoord = texCoord + vec2(x, y);
			// the current neighboring texel's encoded ray position
			vec4 curNeighborRayPositionEncoded = texture2D(rayPositions, offsetTexCoord);
			// if the neighbor is empty, go to the next iteration
			if (curNeighborRayPositionEncoded == emptyColor) {
				continue;
			}
			// if the neighbor is NOT empty, AND the current texel in the loop is 0x0, then we override the averaging process, and set the endColor to the center color
			if (x == 0 && y == 0) {
				// set count to 1 so that no division occurs.
				count = 1;
				// set the endColor to the center color.
				endColor = colorGetFromEncodedPosition(curNeighborRayPositionEncoded);
				// set x and y to 9999 so that when we break the inner loop, the outer loop does not continue.
				x = 9999;
				y = 9999;
				// exit the loop.
				break;
			}

			// add the current color to the end color. 
			endColor += colorGetFromEncodedPosition(curNeighborRayPositionEncoded);
			// increment the counter, as we are a non-empty, non-center pixel. 
			count++;
		}
	}

	// if the count is higher than 0, average the endColor by the count
	if (count > 0) {
		endColor /= count;
	}

	// set the fragColor to the endColor.
	gl_FragColor = endColor;
}

