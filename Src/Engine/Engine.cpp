/**
 *  @file       Engine.cpp
 *  @brief	    Main engine module.
 *  @author     Sébastien Lussier.
 *  @date       25/08/02.
 */
/*
 *  Copyright (C) 2004 Gamedesk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  Gamedesk
 *  http://gamedesk.type-cast.com
 *
 */
#include "Engine.h"
#include "Module/ModuleManager.h"

#include "Application/Application.h"
#include "Application/Window.h"
#include "Game/Game.h"
#include "Manager/Manager.h"
#include "Network/NetworkSubsystem.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Font/Font.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Mesh/SkeletalMesh.h"
#include "Graphic/Mesh/StaticMesh.h"
#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Buffer/SoftwareIndexBuffer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/SoftwareVertexBuffer.h"
#include "Graphic/RenderTarget/RenderTarget.h"
#include "Graphic/RenderTarget/RenderTexture.h"
#include "Graphic/RenderTarget/RenderWindow.h"
#include "Graphic/Shader/ShaderObject.h"
#include "Graphic/Shader/ShaderProgram.h"
#include "Sound/Sound.h"
#include "Sound/SoundData.h"
#include "Sound/SoundSubsystem.h"
#include "Input/InputSubsystem.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "World/SpacePartition/SpacePartition.h"
#include "World/SpacePartition/Octree.h"
#include "World/SpacePartition/BSP.h"
#include "World/Camera.h"
#include "World/Character.h"
#include "World/CharacterCamera.h"
#include "World/Entity.h"
#include "World/FollowCamera.h"
#include "World/Level.h"
#include "World/LookCamera.h"
#include "World/Model3D.h"
#include "World/ParticleEmitter.h"
#include "World/SkyDome.h"
#include "World/Sound3D.h"
#include "World/Terrain.h"
#include "World/World.h"
#include "World/WorldTile.h"
#include "Resource/Resource.h"
#include "Resource/ResourceManager.h"
#include "Subsystem/Subsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(Engine);


void StaticInitEngineClasses()
{
	Application::StaticClass();
	Window::StaticClass();
	Game::StaticClass();
	Manager::StaticClass();
	NetworkSubsystem::StaticClass();
	GraphicSubsystem::StaticClass();
	Renderer::StaticClass();
	Font::StaticClass();
	Texture::StaticClass();
	Texture1D::StaticClass();
	Texture2D::StaticClass();
	Texture3D::StaticClass();
	Cubemap::StaticClass();
	Mesh::StaticClass();
	SkeletalMesh::StaticClass();
	StaticMesh::StaticClass();
	IndexBuffer::StaticClass();
	SoftwareIndexBuffer::StaticClass();
	SoftwareVertexBuffer::StaticClass();
	VertexBuffer::StaticClass();
	RenderTarget::StaticClass();
	RenderTexture::StaticClass();
	RenderTexture1D::StaticClass();
	RenderTexture2D::StaticClass();
	RenderCubemap::StaticClass();
	RenderWindow::StaticClass();
	ShaderObject::StaticClass();
	ShaderProgram::StaticClass();
	Sound::StaticClass();
	SoundData::StaticClass();
	SoundSubsystem::StaticClass();
	InputDevice::StaticClass();
	InputSubsystem::StaticClass();
	Keyboard::StaticClass();
	Mouse::StaticClass();
	SpacePartition::StaticClass();
	Octree::StaticClass();
	Bsp::StaticClass();
	Camera::StaticClass();
	Character::StaticClass();
	CharacterCamera::StaticClass();
	Entity::StaticClass();
	FollowCamera::StaticClass();
	Level::StaticClass();
	LookCamera::StaticClass();
	Model3D::StaticClass();
	ParticleEmitter::StaticClass();
	SkyDome::StaticClass();
	Sound3D::StaticClass();
	Terrain::StaticClass();
	World::StaticClass();
	WorldTile::StaticClass();
	Resource::StaticClass();
	ResourceImporter::StaticClass();
	ResourceExporter::StaticClass();
	ResourceManager::StaticClass();
	Subsystem::StaticClass();
}


} // namespace Gamedesk
