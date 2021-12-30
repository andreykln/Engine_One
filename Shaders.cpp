#include "Shaders.h"

Shaders::Shaders(Graphics& in_gfx)
	: pSgfx(&in_gfx)
{
	VS_IL_Init(&pLightAndTextureVS, IL.posNormalTexture, &pLightAndTextureIL,
				IL.nPosNormalTexture, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	PS_Init(&pLightAndTexturePS, L"Shaders\\Pixel\\LightAndTexturePS.cso");

	VS_IL_Init(&pLightVS, IL.light, &pLightIL, IL.nLightElements, L"Shaders\\Vertex\\LightVS.cso");
	PS_Init(&pLightPS, L"Shaders\\Pixel\\LightPS.cso");
	
	//skull sm
	VS_IL_Init(&pSkullSMGenVS, IL.skull, &pSkullSMIL, IL.nSkullElements, L"Shaders\\Vertex\\ShadowMapGenSkullVS.cso");
	VS_IL_Init(&pSkullSMDrawVS, IL.skull, &pSkullSMIL, IL.nSkullElements, L"Shaders\\Vertex\\ShadowMapDrawSkullVS.cso");
	PS_Init(&pSkullSMPS, L"Shaders\\Pixel\\ShadowMapDrawSkullPS.cso");
	PS_Init(&pSkullSMGenPS, L"Shaders\\Pixel\\ShadowMapSkullGenPS.cso");

	PS_Init(&pLightAndTextureArrayPS, L"Shaders\\Pixel\\LightAndTextureArrayPS.cso");

	VS_IL_Init(&pDepthComplexityVS, IL.positionColorIL, &pDepthCoplexityIL, IL.nPosition_Color, L"Shaders\\Vertex\\DepthCompVS.cso");
	PS_Init(&pDepthComplexityPS, L"Shaders\\Pixel\\DepthCompPS.cso");
	CS_Init(&pHorizontalBlurCS, L"Shaders\\Compute\\HorizontalGaussianBlurCS.cso");
	CS_Init(&pVerticalBlurCS, L"Shaders\\Compute\\VerticalGaussianBlurCS.cso");
	PS_Init(&pBlurTexturePS, L"Shaders\\Pixel\\BlurTexturePS.cso");

	CS_Init(&pHorizontalBilateralBlur, L"Shaders\\Compute\\HorizontalBilateralBlur.cso");
	CS_Init(&pVerticalBilateralBlur, L"Shaders\\Compute\\VerticalBilateralBlur.cso");

	CS_Init(&pDisturbWaves, L"Shaders\\Compute\\DisturbWavesCS.cso");
	CS_Init(&pUpdateWaves, L"Shaders\\Compute\\UpdateWavesCS.cso");
	VS_IL_Init(&pGPUWavesVS, IL.posNormalTexture, &pLightAndTextureIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\GPUWavesVS.cso");

	VS_IL_Init(&pQuadTessellationVS, IL.positonIL, &pPositonIL, IL.nPositon, L"Shaders\\Vertex\\QuadTessellationPassVS.cso");
	PS_Init(&pQuadTessellationPS, L"Shaders\\Pixel\\QuadTessPS.cso");
	HS_Init(&pQuadTesselationHS, L"Shaders\\Hull\\QuadTessHS.cso");
	DS_Init(&pQuadTesselationDS, L"Shaders\\Domain\\QuadTessDS.cso");

	VS_IL_Init(&pInstancedSkullVS, IL.instancedSkull, &pInstancedSkullIL, IL.nInstancedSkull, L"Shaders\\Vertex\\InstancedSkullVS.cso");
	PS_Init(&pInstancedSkullPS, L"Shaders\\Pixel\\InstancedSkullPS.cso");

	VS_IL_Init(&pSkyVS, IL.positonIL, &pSkyIL, IL.nPositon, L"Shaders\\Vertex\\SkyVS.cso");
	PS_Init(&pSkyPS, L"Shaders\\Pixel\\SkyPS.cso");

	PS_Init(&pCubeMapsPS, L"Shaders\\Pixel\\SphereCubeMap.cso");

	PS_Init(&pNormalMappingPS, L"Shaders\\Pixel\\MainLightPS.cso");

	VS_IL_Init(&pDisplacementMappingVS, IL.posNormalTexCoordTangent, &pPosNormalTexCTangentIL,
		IL.nPosNormalTexCoordTangent, L"Shaders\\Vertex\\DisplacementMappingVS.cso");
	HS_Init(&pDisplacementMappingHS, L"Shaders\\Hull\\DisplacementMappingHS.cso");
	DS_Init(&pDisplacementMappingDS, L"Shaders\\Domain\\DisplacementMappingDS.cso");

	VS_IL_Init(&pDisplacementWavesVS, IL.posNormalTexCoordTangent, &pPosNormalTexCTangentIL,
		IL.nPosNormalTexCoordTangent, L"Shaders\\Vertex\\DisplacementWavesVS.cso");
	PS_Init(&pDisplacementWavesPS, L"Shaders\\Pixel\\DisplacementWavesPS.cso");
	HS_Init(&pDisplacementWavesHS, L"Shaders\\Hull\\DisplacementWavesHS.cso");
	DS_Init(&pDisplacementWavesDS, L"Shaders\\Domain\\DisplacementWavesDS.cso");

	//Terrain
	VS_IL_Init(&pTerrainVS, IL.terrainHeightMap, &pTerrainIL, IL.nTerrainHeightMap, L"Shaders\\Vertex\\TerrainVS.cso");
	HS_Init(&pTerrainHS, L"Shaders\\Hull\\TerrainHS.cso");
	DS_Init(&pTerrainDS, L"Shaders\\Domain\\TerrainDS.cso");
	PS_Init(&pTerrainPS, L"Shaders\\Pixel\\TerrainPS.cso");

	//Particles fire
	GS_SO_Init(&pSOFireGS, L"Shaders\\Geometry\\ParticleFireSOGS.cso");
	VS_IL_Init(&pSOVS, IL.particleSO, &pSOIL, IL.nParticleStreamOut, L"Shaders\\Vertex\\ParticleSOVS.cso");
	VS_IL_Init(&pParticleFireVS, IL.particle, &pParticleDrawIL, IL.nParticle, L"Shaders\\Vertex\\ParticleFireVS.cso");
	GS_Init(&pParticleFireGS, L"Shaders\\Geometry\\ParticleFireGS.cso");
	PS_Init(&pParticleFirePS, L"Shaders\\Pixel\\ParticlePS.cso");

	//particles rain
	GS_SO_Init(&pParticleRainGSSO, L"Shaders\\Geometry\\ParticleRainSOGS.cso");
	GS_Init(&pParticleRainGS, L"Shaders\\Geometry\\ParticleRainGS.cso");
	VS_IL_Init(&pParticleRainVS, IL.particle, &pParticleDrawIL, IL.nParticle, L"Shaders\\Vertex\\ParticleRainVS.cso");
	PS_Init(&pParticleRainPS, L"Shaders\\Pixel\\ParticleRainPS.cso");

	//particles explosion
	GS_SO_Init(&pParticleExplosionSOGS, L"Shaders\\Geometry\\ParticleExplosionSOGS.cso");
	GS_Init(&pParticleExplosionGS, L"Shaders\\Geometry\\ParticleExplosionGS.cso");
	VS_IL_Init(&pParticleExplosionVS, IL.particle, &pParticleDrawIL, IL.nParticle, L"Shaders\\Vertex\\ParticleExplosionVS.cso");

	//fountain
	GS_SO_Init(&pParticleFountainSOGS, L"Shaders\\Geometry\\ParticleFountainSOGS.cso");
	GS_Init(&pParticleFountainGS, L"Shaders\\Geometry\\ParticleFountainGS.cso");
	VS_IL_Init(&pParticleFountainVS, IL.particle, &pParticleDrawIL, IL.nParticle, L"Shaders\\Vertex\\ParticleFountainVS.cso");

	//shadow map
	VS_IL_Init(&pShadowMapVS, IL.posNormalTexCoordTangent,
		&pPosNormalTexCTangentIL, IL.nPosNormalTexCoordTangent, L"Shaders\\Vertex\\NormalMappingShadowsVS.cso");
	PS_Init(&pShadowMapPS, L"Shaders\\Pixel\\NormalMappingShadowPS.cso");

	//shadow map generation
	VS_Init(&pShadowMapGenVS, L"Shaders\\Vertex\\ShadowMapGenVS.cso");
	PS_Init(&pShadowMapGenPS, L"Shaders\\Pixel\\ShadowMapGenPS.cso");

	//main light shader for textured surfaced
	PS_Init(&pDefaultInstancedPS, L"Shaders\\Pixel\\DefaultInstanced_PS.cso");

	//SM generation instanced
	VS_IL_Init(&pShadowMapInstancedVS, IL.smInstancedGen, &pShadowMapInstancedIL,
		 IL.nInstancedSMGen, L"Shaders\\Vertex\\ShadowMapGenInstancedVS.cso");
	VS_Init(&pShadowMapDrawInstancedVS, L"Shaders\\Vertex\\ShadowMapDrawInstancedVS.cso");

	//SSAO
	//NormalMap generation  
	VS_IL_Init(&pCreateNormalMapVS, IL.posNormalTexCoordTangent, &pPosNormalTexCTangentIL,
		IL.nPosNormalTexCoordTangent, L"Shaders\\Vertex\\CreateNormalMapVS.cso");
	PS_Init(&pCreateNormalMapPS, L"Shaders\\Pixel\\CreateNormalMapPS.cso");
	VS_Init(&pCreateNormalMapSkullVS, L"Shaders\\Vertex\\CreateNormalMapSkullVS.cso");
	VS_Init(&pCreateNormalMapInstancedVS, L"Shaders\\Vertex\\CreateNormalMapInstancedVS.cso");
	//compute ssao
	VS_IL_Init(&pSSAOFullScreenQuadVS, IL.posNormalTexture, &pLightAndTextureIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\ComputeSSAOVS.cso");
	PS_Init(&pSSAOFullScreenQuadPS, L"Shaders\\Pixel\\ComputeSSAOPS.cso");
	
	//debug quad
	VS_IL_Init(&pDebugQuadVS, IL.posNormalTexture, &pLightAndTextureIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\DebugScreenQuadVS.cso");
	PS_Init(&pDebugQuadPS, L"Shaders\\Pixel\\DebugScreenQuadPS.cso");


}

void Shaders::BindVSandIA(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pLightAndTextureVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Light_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pLightVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DepthComplexityVS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pDepthCoplexityIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pDepthComplexityVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::GPUWaves_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pGPUWavesVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::QuadTessellation_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPositonIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pQuadTessellationVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::InstancedSkull_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pInstancedSkullIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pInstancedSkullVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Sky_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pSkyIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pSkyVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementMapping_VS_DS_HS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPosNormalTexCTangentIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pDisplacementMappingVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPosNormalTexCTangentIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pDisplacementWavesVS, nullptr, 0u);
		break;

	}
	case ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pTerrainIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pTerrainVS, nullptr, 0u);
		break;

	}
	case ShaderPicker::Particles_FireStreamOut_VS_GS:
	case ShaderPicker::Particles_RainStreamOut_VS_GS:
	case ShaderPicker::Particles_ExplosionStreamOut_VS_GS:
	case ShaderPicker::Particle_FountainStreamOut_VS_GS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pSOIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pSOVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pParticleDrawIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pParticleFireVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pParticleDrawIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pParticleRainVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pParticleDrawIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pParticleExplosionVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pParticleDrawIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pParticleFountainVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMap_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPosNormalTexCTangentIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pShadowMapVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapGen_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPosNormalTexCTangentIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pShadowMapGenVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapGenSkull_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pSkullSMIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pSkullSMGenVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapDrawSkull_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pSkullSMIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pSkullSMDrawVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapInstancedGen_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pShadowMapInstancedIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pShadowMapInstancedVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapInstancedDraw_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pShadowMapInstancedIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pShadowMapDrawInstancedVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CreateNormalMap_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPosNormalTexCTangentIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pCreateNormalMapVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CreateNormalMapSkullVS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pSkullSMIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pCreateNormalMapSkullVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CreateNormalMapInstancedVS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pShadowMapInstancedIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pCreateNormalMapInstancedVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeSSAO_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pSSAOFullScreenQuadVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DrawDebugTexQuad_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pDebugQuadVS, nullptr, 0u);
		break;
	}
	default:
	break;
	}
}

void Shaders::BindPS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
		{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTexturePS, nullptr, 0u);
		break;
		}
	case ShaderPicker::Light_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::LightAndTextureArrayPS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTextureArrayPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DepthComplexityVS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pDepthComplexityPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::BlurTexture_PS :
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pBlurTexturePS, nullptr, 0u);
		break;
	}
	case ShaderPicker::QuadTessellation_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pQuadTessellationPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::InstancedSkull_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pInstancedSkullPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Sky_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pSkyPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CubeMap_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pCubeMapsPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementMapping_VS_DS_HS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pNormalMappingPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pDisplacementWavesPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pTerrainPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pParticleFirePS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pParticleRainPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMap_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pShadowMapPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapGen_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pShadowMapGenPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapDrawSkull_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pSkullSMPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMapGenSkull_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pSkullSMGenPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DefaultInstanced_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pDefaultInstancedPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CreateNormalMap_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pCreateNormalMapPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeSSAO_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pSSAOFullScreenQuadPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DrawDebugTexQuad_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pDebugQuadPS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::BindGS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::Particles_FireStreamOut_VS_GS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pSOFireGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleFireGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainStreamOut_VS_GS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleRainGSSO, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleRainGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleExplosionGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_ExplosionStreamOut_VS_GS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleExplosionSOGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainStreamOut_VS_GS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleFountainSOGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pParticleFountainGS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::BindCS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::HorizontalBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pHorizontalBlurCS, nullptr, 0u);
		break;
	}
	case ShaderPicker::VerticalBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pVerticalBlurCS, nullptr, 0u);
		break;
	}
	case ShaderPicker::HorizontalBilateralBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pHorizontalBilateralBlur, nullptr, 0u);
		break;
	}
	case ShaderPicker::VerticalBilateralBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pVerticalBilateralBlur, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisturbWaves_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pDisturbWaves, nullptr, 0u);
		break;
	}
	case ShaderPicker::UpdateWaves_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pUpdateWaves, nullptr, 0u);
		break;
	}
	}
}

void Shaders::BindHS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::QuadTessellation_HS:
	{
		pSgfx->pgfx_pDeviceContext->HSSetShader(pQuadTesselationHS, 0u, 0u);
		break;
	}
	case ShaderPicker::DisplacementMapping_VS_DS_HS:
	{
		pSgfx->pgfx_pDeviceContext->HSSetShader(pDisplacementMappingHS, 0u, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pSgfx->pgfx_pDeviceContext->HSSetShader(pDisplacementWavesHS, 0u, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS:
	{
		pSgfx->pgfx_pDeviceContext->HSSetShader(pTerrainHS, 0u, 0u);
		break;
	}
	}
}

void Shaders::BindDS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::QuadTessellation_DS:
	{
		pSgfx->pgfx_pDeviceContext->DSSetShader(pQuadTesselationDS, 0u, 0u);
		break;
	}
	case ShaderPicker::DisplacementMapping_VS_DS_HS:
	{
		pSgfx->pgfx_pDeviceContext->DSSetShader(pDisplacementMappingDS, 0u, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pSgfx->pgfx_pDeviceContext->DSSetShader(pDisplacementWavesDS, 0u, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS:
	{
		pSgfx->pgfx_pDeviceContext->DSSetShader(pTerrainDS, 0u, 0u);
		break;
	}
	}
}

void Shaders::UnbindCS()
{
	pSgfx->pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindGS()
{
	pSgfx->pgfx_pDeviceContext->GSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindPS()
{
	pSgfx->pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindVS()
{
	pSgfx->pgfx_pDeviceContext->VSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindHS()
{
	pSgfx->pgfx_pDeviceContext->HSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindDS()
{
	pSgfx->pgfx_pDeviceContext->DSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindAll()
{
	UnbindCS();
	UnbindDS();
	UnbindGS();
	UnbindHS();
	UnbindPS();
	UnbindVS();
}

void Shaders::VS_IL_Init(ID3D11VertexShader** pVShader,
						const D3D11_INPUT_ELEMENT_DESC* inputLayout,
						ID3D11InputLayout** pIL,
						UINT nElements, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateVertexShader(
										pBlob->GetBufferPointer(),
										pBlob->GetBufferSize(),
										nullptr,
										pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		pSgfx->SetDebugName(*pVShader, path.c_str());
	}
#endif
	InitializeInputLayout(inputLayout,pIL ,nElements, pBlob, L"VertexShader_");

	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::VS_Init(ID3D11VertexShader** pVShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		pSgfx->SetDebugName(*pVShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pPSShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pPSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout,
									ID3D11InputLayout** pIL, 
									UINT nElements,
									ID3DBlob* pBlob, const std::wstring& name)
{
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateInputLayout(
		inputLayout,
		nElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		pIL));
#ifdef MY_DEBUG
	if (name != std::wstring())
	{
		pSgfx->SetDebugName(pLightAndTextureIL, name.c_str());
	}
#endif
}

void Shaders::GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pGSShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pGSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::GS_SO_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateGeometryShaderWithStreamOutput(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		SO.fire,
		SO.fireSize, 
		NULL, 0, D3D11_SO_NO_RASTERIZED_STREAM,	nullptr,
		pGSShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pGSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateComputeShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pCShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pCShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::HS_Init(ID3D11HullShader** pHShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateHullShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pHShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pHShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateDomainShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pDshader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pDshader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::Bind(Graphics& gfx) noexcept
{

}
