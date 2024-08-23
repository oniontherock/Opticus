#version 450

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// the texture of the world
layout(rgba32f, binding = 0) uniform image2D worldTex;
// the vision texture that is written to in the shader and read from outside the shader
// doesn't necessarily reflect the worldTex
layout(rgba32f, binding = 1) uniform image2D visionTex;

// list of starting transforms for the rays, the r and g value of each pixel is the x and y coordinate of each ray, and the b value is the rotation of each ray
layout(rgba32f, binding = 2) uniform image2D rayStartingTransforms;

void main() {
//
	// get id to read/write data from
    const ivec2 id = ivec2(gl_GlobalInvocationID.xy);

	// the starting rotation of this ray, only used for calculating the ray's heading
    const float rayRot = imageLoad(rayStartingTransforms, id).b;
	// the ray's starting position
    const vec2 rayPosOrig = imageLoad(rayStartingTransforms, id).rg;
	// the ray's starting heading
	const vec2 rayHeadingOrig = vec2(cos(rayRot), sin(rayRot));

	// the ray's position, this changes as the ray moves
    vec2 rayPos = rayPosOrig;
	// the ray's heading, this can be changed by world distortions
	vec2 rayHeading = rayHeadingOrig;

	// the distance the ray must travel, doesn't necessarily reflect the actual amount the ray has to move in the world
    const float desiredDistance = 500;
	// the distance the ray has traveled so far, doesn't necessarily reflect the actual amount the ray has moved in the world
	float movedAmount = 0;

	while (movedAmount <= desiredDistance) {

		//	WorldDistortionGrid::headingApplyDistortionSafe(heading, sf::Vector2i(position));
		rayPos += rayHeading;
		movedAmount += 1.f;

		// the pixel position of the ray in the visionTex, this isn't necessarily the ray's actual world position
		ivec2 visionPos = ivec2(rayPosOrig + (rayHeadingOrig*movedAmount));
		// the pixel position of the ray in the world, this is the ray's actual world position
		vec4 worldTexel = imageLoad(worldTex, ivec2(rayPos));

		// set the vision pixel to the world texel
		imageStore(visionTex, visionPos, worldTexel);
	}
}

