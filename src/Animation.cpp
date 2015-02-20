#include "Animation.h"
#include "Utility.h"

bool Animation::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	/////////////////////

	//FBX Loader
	m_file = new FBXFile;

	m_file->load("./models/characters/enemynormal/enemynormal.fbx");
	//loads textures using fbx information of where they are found
	m_file->initialiseOpenGLTextures();

	GenerateGLMeshes(m_file);

	Utility::LoadShader("./shaders/skinned_vertex.glsl", "./shaders/skinned_fragment.glsl", &m_program_ID);

	/////////////////////

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(3);

	//unsigned 

	return true;
}


bool Animation::ShutDown()
{
	m_file->unload();

	delete m_file;

	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool Animation::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();

	

	//Set time to 0
	glfwSetTime(0.0f);

	//Camera Update
	m_camera->Update(DeltaTime);

	Gizmos::clear();

	/////////////////////

	FBXSkeleton* skele = m_file->getSkeletonByIndex(0);

	FBXAnimation* anim = m_file->getAnimationByIndex(0);

	m_timer += DeltaTime;



	///////////////////

	EvaluateSkeleton(anim, skele, m_timer, 24.0f);

	for (unsigned int i = 0; i < skele->m_boneCount; i++)
	{
		skele->m_nodes[i]->updateGlobalTransform();
		mat4 node_global = skele->m_nodes[i]->m_globalTransform;
		vec3 node_pos = node_global[3].xyz;

		Gizmos::addAABBFilled(node_pos, vec3(0.5f), vec4(1, 0, 0, 1), &node_global);

		if (skele->m_nodes[i]->m_parent != nullptr)
		{
			vec3 parent_pos = skele->m_nodes[i]->m_parent->m_globalTransform[3].xyz;
			Gizmos::addLine(node_pos, parent_pos, vec4(0, 1, 0, 1));
		}
	}






	/////////////////////

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	//Draw Grid 20 x 20
	for (int i = 0; i <= 20; i++)
	{
		//x == -10 + i
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
			i == 10 ? white : black);
	}

	return true;
}

bool Animation::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////

	//start using the program
	glUseProgram(m_program_ID);

	//pass in the projection view (the camera)
	int proj_view_uniform =
		glGetUniformLocation(m_program_ID, "projection_view");
	glUniformMatrix4fv(proj_view_uniform, 1, GL_FALSE,
						(float*)&m_camera->GetProjectionView());

	//the texture stuff
	int diffuse_uniform =
		glGetUniformLocation(m_program_ID, "diffuse");
	glUniform1i(diffuse_uniform, 0); // point to texture location 0







	FBXSkeleton* skeleton = m_file->getSkeletonByIndex(0);

	///////////////////////
	UpdateBones(skeleton);

	int bones_uniform =
		glGetUniformLocation(m_program_ID, "bones");

	glUniformMatrix4fv(bones_uniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);










	//loop through all the meshes
	for (unsigned int i = 0; i < m_meshes.size(); i++) 
	{
		FBXMeshNode* curr_mesh = m_file->getMeshByIndex(i); //current mesh
		FBXMaterial* mesh_material = curr_mesh->m_material; //current material on the mesh

		//bind texture diffuse to texture 0
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D,
			mesh_material->textures[FBXMaterial::DiffuseTexture]->handle);
		//
		mat4 world_transform = m_file->getMeshByIndex(i)->m_globalTransform;
		//
		int world_uniform = glGetUniformLocation(m_program_ID, "world");
		//
		glUniformMatrix4fv(world_uniform, 1, GL_FALSE, (float*)&world_transform);

		glBindVertexArray(m_meshes[i].m_VAO);

		glDrawElements(GL_TRIANGLES,
			m_meshes[i].m_index_count,
			GL_UNSIGNED_INT, 0);
	}

	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}


void Animation::GenerateGLMeshes(FBXFile* fbx)
{
	//find the number of meshes
	unsigned int mesh_count = fbx->getMeshCount();
	m_meshes.resize(mesh_count);

	for (unsigned int mesh_index = 0; mesh_index < mesh_count; mesh_index++)
	{
		FBXMeshNode* curr_mesh =
			fbx->getMeshByIndex(mesh_index);

		//need to find out how many indices there are and tell the mesh
		m_meshes[mesh_index].m_index_count =
			curr_mesh->m_indices.size();

		glGenVertexArrays(1, &m_meshes[mesh_index].m_VAO);
		glGenBuffers(1, &m_meshes[mesh_index].m_VBO);
		glGenBuffers(1, &m_meshes[mesh_index].m_IBO);

		glBindVertexArray(m_meshes[mesh_index].m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[mesh_index].m_VBO);

		glBufferData(GL_ARRAY_BUFFER,
			curr_mesh->m_vertices.size() * sizeof(FBXVertex), //how many bytes there are
			curr_mesh->m_vertices.data(), GL_STATIC_DRAW); //pointer to the start of the array

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			curr_mesh->m_indices.size() * sizeof(unsigned int), //how many bytes there are
			curr_mesh->m_indices.data(), GL_STATIC_DRAW); //pointer to the start of the array
	
		glEnableVertexAttribArray(0);	//pos
		glEnableVertexAttribArray(1);	//tex co-ord
		glEnableVertexAttribArray(2);	//bone indices
		glEnableVertexAttribArray(3);	//bone weights

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::PositionOffset);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::TexCoord1Offset);
	
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::IndicesOffset);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::WeightsOffset);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}


void Animation::EvaluateSkeleton(FBXAnimation* animation, FBXSkeleton* skeleton, float timer, float fps)
{
	int current_frame = (int)(timer * fps);

	//Loop through all the tracks
	for (unsigned int track_index = 0; track_index < animation->m_trackCount; track_index++)
	{
		//wrap back to start off track if we've gone too far
		int track_frame_count = animation->m_tracks[track_index].m_keyframeCount; // mod-equal aye...
		int track_frame = current_frame % track_frame_count;

		//find what keyframes are currently affecting the bones
		FBXKeyFrame curr_frame = 
			animation->m_tracks[track_index].m_keyframes[track_frame];
		FBXKeyFrame next_frame =
			animation->m_tracks[track_index].m_keyframes[(track_frame + 1) % track_frame_count];

		//interpolate between the keyframes to generate the matrix
		//for the current pose
		float time_since_frame_flip = timer - (current_frame / fps);
		float t = time_since_frame_flip * fps;
		
		vec3 new_pos = glm::mix(curr_frame.m_translation,
										next_frame.m_translation, t);

		vec3 new_scale = glm::mix(curr_frame.m_scale,
										next_frame.m_scale, t);

		glm::quat new_rot = glm::slerp(curr_frame.m_rotation,
										next_frame.m_rotation, t);

		mat4 local_transform =
			glm::translate(new_pos) *
			glm::toMat4(new_rot) *
			glm::scale(new_scale);


		//get the right bone for the given track
		//FBXTrack* bone_track = &animation->m_tracks[track_index];

		int bone_index = animation->m_tracks[track_index].m_boneIndex;

		if (bone_index < skeleton->m_boneCount)
		{
			//set the FBX node's local transforms to match
			skeleton->m_nodes[bone_index]->m_localTransform = local_transform;
		}
	}
}


void Animation::UpdateBones(FBXSkeleton* skeleton)
{
	//loop through the nodes in the skeleton
	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; bone_index++)
	{
		//generate their global transforms
		int parent_index = skeleton->m_parentIndex[bone_index];

		//if negative one, it doesnt have a parent
		if (parent_index == -1)
		{
			skeleton->m_bones[bone_index] = skeleton->m_nodes[bone_index]->m_localTransform;
		}
		else
		{
			skeleton->m_bones[bone_index] =
				skeleton->m_bones[parent_index] * skeleton->m_nodes[bone_index]->m_localTransform;
		}
	}

	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; bone_index++)
	{
		//multiply the global transform by the inverse bind pose
		skeleton->m_bones[bone_index] = skeleton->m_bones[bone_index] * skeleton->m_bindPoses[bone_index];
	}
}