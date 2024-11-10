uniform sampler2D texture;
//
//const vec4 emptyColor = vec4(0.0, 0.0, 0.0, 0.0);
//
//const int searchSizeMax = 4;
//const int neighborsCountMax = 128;
//
void main() {
	

//	// get the size of the texture
//	vec2 rayTexSize = textureSize(texture, 0);
//	// get the current texCoord
//	ivec2 fragCoord = gl_TexCoord[0].xy * rayTexSize;
//
//	vec4 pixelColor = texelFetch(texture, fragCoord, 0);
//
//	// skip if we are blank
//	if (pixelColor == emptyColor) {
//		gl_FragColor = pixelColor;
//		return;
//	}
//
//	vec4 neighborsColor;
//
//	int searchSize = 1;
//
//	// amount of colors that are tallied up in the blur
//	int neighborsCount = 0;
//	
//	bool neighborEmptyFound = false;
//	while ((!neighborEmptyFound) && (searchSize <= searchSizeMax)) {
//	
//		for (int x = -searchSize; x <= +searchSize; x++) {
//			for (int y = -searchSize; y <= +searchSize; y++) {
//
//				if (abs(float(x)) < float(searchSize) && abs(float(y)) < float(searchSize)) continue;
//
//				// the current neighboring texel's encoded ray position
//				vec4 color = texelFetchOffset(texture, fragCoord, 0, ivec2(x, y));
//				// if the neighbor is empty, go to the next iteration
//				if (color == emptyColor) {
//					neighborEmptyFound = true;
//					continue;
//				}
//
//				// add the current color to the end color. 
//				neighborsColor += color;
//				// increment the counter, as we are a non-empty
//				neighborsCount++;
//			}
//		}
//
//		searchSize++;
//	
//	}
//
//
//	if (neighborEmptyFound) {
//
//		// if the count is higher than 0, average the endColor by the count
//		if (neighborsCount > 0) {
//			neighborsColor /= neighborsCount;
//
//			float neighborCountInvertedScaled = 1.0 - (float(neighborsCount) / float(neighborsCountMax));
//
//			pixelColor.a -= neighborsColor.a;
//
//		}
//
//		// set the fragColor to the endColor.
//		gl_FragColor = pixelColor;
//	}
//	else {
//		gl_FragColor = pixelColor;
//	}
}

