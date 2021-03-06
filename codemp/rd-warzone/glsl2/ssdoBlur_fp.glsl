#if defined(USE_BINDLESS_TEXTURES)
layout(std140) uniform u_bindlessTexturesBlock
{
uniform sampler2D					u_DiffuseMap;
uniform sampler2D					u_LightMap;
uniform sampler2D					u_NormalMap;
uniform sampler2D					u_DeluxeMap;
uniform sampler2D					u_SpecularMap;
uniform sampler2D					u_PositionMap;
uniform sampler2D					u_WaterPositionMap;
uniform sampler2D					u_WaterHeightMap;
uniform sampler2D					u_HeightMap;
uniform sampler2D					u_GlowMap;
uniform sampler2D					u_EnvironmentMap;
uniform sampler2D					u_TextureMap;
uniform sampler2D					u_LevelsMap;
uniform sampler2D					u_CubeMap;
uniform sampler2D					u_SkyCubeMap;
uniform sampler2D					u_SkyCubeMapNight;
uniform sampler2D					u_EmissiveCubeMap;
uniform sampler2D					u_OverlayMap;
uniform sampler2D					u_SteepMap;
uniform sampler2D					u_SteepMap1;
uniform sampler2D					u_SteepMap2;
uniform sampler2D					u_SteepMap3;
uniform sampler2D					u_WaterEdgeMap;
uniform sampler2D					u_SplatControlMap;
uniform sampler2D					u_SplatMap1;
uniform sampler2D					u_SplatMap2;
uniform sampler2D					u_SplatMap3;
uniform sampler2D					u_RoadsControlMap;
uniform sampler2D					u_RoadMap;
uniform sampler2D					u_DetailMap;
uniform sampler2D					u_ScreenImageMap;
uniform sampler2D					u_ScreenDepthMap;
uniform sampler2D					u_ShadowMap;
uniform sampler2D					u_ShadowMap2;
uniform sampler2D					u_ShadowMap3;
uniform sampler2D					u_ShadowMap4;
uniform sampler2D					u_ShadowMap5;
uniform sampler3D					u_VolumeMap;
uniform sampler2D					u_MoonMaps[4];
};
#else //!defined(USE_BINDLESS_TEXTURES)
uniform sampler2D			u_DiffuseMap;			// Screen
uniform sampler2D			u_PositionMap;			// Positions
uniform sampler2D			u_NormalMap;			// Normals
uniform sampler2D			u_DeluxeMap;			// Occlusion
#endif //defined(USE_BINDLESS_TEXTURES)

uniform vec2				u_Dimensions;
uniform vec3				u_ViewOrigin;
uniform vec4				u_PrimaryLightOrigin;

uniform vec4				u_Local0;

#define dir					u_Local0.rg

varying vec2   				var_TexCoords;

#define znear				u_ViewInfo.r			//camera clipping start
#define zfar				u_ViewInfo.g			//camera clipping end

vec2 EncodeNormal(vec3 n)
{
	float scale = 1.7777;
	vec2 enc = n.xy / (n.z + 1.0);
	enc /= scale;
	enc = enc * 0.5 + 0.5;
	return enc;
}
vec3 DecodeNormal(vec2 enc)
{
	vec3 enc2 = vec3(enc.xy, 0.0);
	float scale = 1.7777;
	vec3 nn =
		enc2.xyz*vec3(2.0 * scale, 2.0 * scale, 0.0) +
		vec3(-scale, -scale, 1.0);
	float g = 2.0 / dot(nn.xyz, nn.xyz);
	return vec3(g * nn.xy, g - 1.0);
}

vec4 dssdo_blur(vec2 tex)
{
	float weights[9] = float[]
	(
		0.013519569015984728,
		0.047662179108871855,
		0.11723004402070096,
		0.20116755999375591,
		0.240841295721373,
		0.20116755999375591,
		0.11723004402070096,
		0.047662179108871855,
		0.013519569015984728
	);

	vec4 pMap  = texture(u_PositionMap, var_TexCoords);

	if (pMap.a-1.0 == 1024.0 || pMap.a-1.0 == 1025.0)
	{// Skybox... Skip...
		return vec4(0.0, 0.0, 0.0, 1.0);
	}

	const float indices[9] = float[](-4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0);

	vec2 step = dir/u_Dimensions.xy;

	int i;

	vec3 normal[9];

//#pragma unroll 9
	for (i = 0; i < 9; i++)
	{
		normal[i] = texture(u_NormalMap, tex + indices[i]*step).xyz;
		normal[i] = DecodeNormal(normal[i].xy);
	}

	float total_weight = 1.0;
	float discard_threshold = 0.85;

//#pragma unroll 9
	for (i = 0; i < 9; i++)
	{
		if( dot(normal[i], normal[4]) < discard_threshold )
		{
			total_weight -= weights[i];
			weights[i] = 0;
		}
	}

	//

	vec4 res = vec4(0.0);

//#pragma unroll 9
	for (i = 0; i < 9; i++)
	{
		res += texture(u_DeluxeMap, tex + indices[i]*step) * weights[i];
	}

	res /= total_weight;

	return vec4(res.rgb, 1.0);
}

void main() 
{
	gl_FragColor = dssdo_blur(var_TexCoords);
}
