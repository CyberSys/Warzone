uniform vec4		u_Local9;
uniform vec4		u_Local10;

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
uniform sampler2D					u_DiffuseMap;
#endif //defined(USE_BINDLESS_TEXTURES)

varying vec2						var_TexCoords;
varying vec3						var_vertPos;
varying vec3						var_Normal;
flat varying float					var_IsWater;

#define material					u_Local10.b

out vec4 out_Glow;
out vec4 out_Normal;
out vec4 out_Position;
#ifdef USE_REAL_NORMALMAPS
out vec4 out_NormalDetail;
#endif //USE_REAL_NORMALMAPS

// Maximum waves amplitude
#define maxAmplitude u_Local10.g

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

void main()
{
	out_Glow = vec4(0.0);
	out_Normal = vec4(EncodeNormal(var_Normal), 0.0, 1.0);
#ifdef USE_REAL_NORMALMAPS
	out_NormalDetail = vec4(0.0);
#endif //USE_REAL_NORMALMAPS
	//if (var_IsWater >= 2.0 && var_IsWater <= 5.0)
	//	out_Color = vec4(texture(u_DiffuseMap, var_TexCoords).rgb, 1.0);
	//else
	//	out_Color = vec4(1.0);

	if (material == 2.0 || material == 5.0)
	{
		out_Position = vec4(texture(u_DiffuseMap, var_TexCoords).rgb, material);
		out_Color = vec4(1.0);
	}
	else if (material == 6.0)
	{
		out_Position = vec4(var_vertPos.xyz, material);
		out_Color = vec4(1.0);
	}
	else if (material == 3.0 || material == 4.0)
	{
		float dist = (0.5 - distance(var_TexCoords, vec2(0.5))) * 2.0;
		dist = pow(dist, 4.0);

		if (dist > 0.0)
		{
			out_Position = vec4(dist, var_TexCoords.x - 0.5, var_TexCoords.y - 0.5, material);
			out_Color = vec4(1.0);
		}
		else
		{
			out_Position = vec4(0.0);
			out_Color = vec4(0.0);
		}
	}
	else
	{
		out_Position = vec4(var_vertPos.xyz, var_IsWater);
		out_Color = vec4(1.0);
	}

	out_Glow = vec4(0.0);
	out_Normal = vec4(EncodeNormal(normalize(var_Normal)), 0.0, 1.0);
#ifdef USE_REAL_NORMALMAPS
	out_NormalDetail = vec4(0.0);
#endif //USE_REAL_NORMALMAPS
}
