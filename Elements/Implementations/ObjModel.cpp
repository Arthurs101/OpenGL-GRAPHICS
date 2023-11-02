#include "../ObjModel.h"

ObjModel::ObjModel(
	const char* filename, 
	glm::vec3 _position, 
	glm::vec3 _scale, 
	glm::vec3 _rotation, 
	std::vector<Texture> _textures
)
{
    position = _position;
    scale = _scale;
    rotation = _rotation;
    textures = _textures;
    this->extract_data(filename);

}
//get the object data
void ObjModel::extract_data(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    std::string line;

    std::vector<std::vector<float>> textcoords;
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<std::vector<float>>> faces;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;

        

        iss >> prefix;
        //if (prefix == "o") { //is another mesh 
        //    if (vertices.size() > 0) { //check if it's not the first declaraton
        //        assemble(
        //            textcoords,
        //            vertices,
        //            normals,
        //            faces
        //        );
        //        // Reset vectors
        //        std::vector<std::vector<float>>().swap(textcoords);
        //        std::vector<std::vector<float>>().swap(vertices);
        //        std::vector<std::vector<float>>().swap(normals);
        //        std::vector<std::vector<std::vector<float>>>().swap(faces);
        //    }
        //}
        if (prefix == "v") { // Vertices
            std::vector<float> vertex;
            float value;
            while (iss >> value) {
                vertex.push_back(value);
            }
            vertices.push_back(vertex);
        }
        else if (prefix == "vt") { // Texture Coordinates
            std::vector<float> texcoord;
            float value;
            while (iss >> value) {
                texcoord.push_back(value);
            }
            textcoords.push_back(texcoord);
        }
        else if (prefix == "vn") { // Normals
            std::vector<float> normal;
            float value;
            while (iss >> value) {
                normal.push_back(value);
            }
            normals.push_back(normal);
        }
        else if (prefix == "f") { // Faces
            std::vector<std::vector<float>> face;
            std::string vert;
            while (iss >> vert) {
                std::istringstream viss(vert);
                std::string v;
                std::vector<float> vertex;
                while (std::getline(viss, v, '/')) {
                    vertex.push_back(std::stoi(v));
                }
                face.push_back(vertex);
            }
            faces.push_back(face);
        }
    }
    assemble(
        textcoords,
        vertices,
        normals,
        faces
    );
}

void ObjModel::assemble(
    std::vector<std::vector<float>> textcoords,
    std::vector<std::vector<float>> vertices,
    std::vector<std::vector<float>> normals,
    std::vector<std::vector<std::vector<float>>> faces
) {
    std::vector< GLuint> indices;
    std::vector <Vertex> vertx;
    for (int face = 0; face < faces.size(); face++) {

        //push the vertex index
        indices.push_back( //v0
            static_cast<GLuint>(faces[face][0][0] - 1)
        );
        indices.push_back( //v1
            static_cast<GLuint>(faces[face][1][0] - 1)
        );
        indices.push_back( //2
            static_cast<GLuint>(faces[face][2][0] - 1)
        );

        //create the coordinates

        glm::vec3 v0(
            vertices[faces[face][0][0] - 1][0],
            vertices[faces[face][0][0] - 1][1],
            vertices[faces[face][0][0] - 1][2]
        );
        glm::vec3 v1(
            vertices[faces[face][1][0] - 1][0],
            vertices[faces[face][1][0] - 1][1],
            vertices[faces[face][1][0] - 1][2]
        );
        glm::vec3 v2(
            vertices[faces[face][2][0] - 1][0],
            vertices[faces[face][2][0] - 1][1],
            vertices[faces[face][2][0] - 1][2]
        );
        //texture coords

        glm::vec2 vt0(
            textcoords[faces[face][0][1] - 1][0],
            textcoords[faces[face][0][1] - 1][1]
        );
        glm::vec2 vt1(
            textcoords[faces[face][1][1] - 1][0],
            textcoords[faces[face][1][1] - 1][1]
        );
        glm::vec2 vt2(
            textcoords[faces[face][2][1] - 1][0],
            textcoords[faces[face][2][1] - 1][1]
        );
        //face normals
        glm::vec3 vn0, vn1, vn2, vn3;
        if (normals.size() == 0) {
            vn0 = glm::normalize(v0);
            vn1 = glm::normalize(v1);
            vn2 = glm::normalize(v2);
        }
        else {
            //normal v0
            vn0.x = normals[faces[face][0][2] - 1][0];
            vn0.y = normals[faces[face][0][2] - 1][1];
            vn0.z = normals[faces[face][0][2] - 1][2];
            //normal v1
            vn1.x = normals[faces[face][1][2] - 1][0];
            vn1.y = normals[faces[face][1][2] - 1][1];
            vn1.z = normals[faces[face][1][2] - 1][2];
            //normal v2
            vn2.x = normals[faces[face][2][2] - 1][0];
            vn2.y = normals[faces[face][2][2] - 1][1];
            vn2.z = normals[faces[face][2][2] - 1][2];
        }
        //create the verts

        //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        vertx.push_back(Vertex{//v0
            v0,
            glm::vec3(1,1,1),
            vn0,
            vt0,
            });
        vertx.push_back(Vertex{//v1
           v1,
           glm::vec3(1,1,1),
           vn1,
           vt1,
            });
        vertx.push_back(Vertex{//v2
           v2,
           glm::vec3(1,1,1),
           vn2,
           vt2,
            });

        if (faces[face].size() == 4) { //if it has 4 verts
            glm::vec3 v3(
                vertices[faces[face][3][0] - 1][0],
                vertices[faces[face][3][0] - 1][1],
                vertices[faces[face][3][0] - 1][2]
            );

            glm::vec2 vt3(
                textcoords[faces[face][3][1] - 1][0],
                textcoords[faces[face][3][1] - 1][1]
            );
            if (normals.size() == 0) {
                vn3 = glm::normalize(v3);
            }
            else {
                vn3.x = normals[faces[face][3][2] - 1][0];
                vn3.y = normals[faces[face][3][2] - 1][1];
                vn3.z = normals[faces[face][3][2] - 1][2];
            }

            //push the indices of v0,v2,v3
            //create the verts
            vertx.push_back(Vertex{//v0
            v0,
            glm::vec3(1,1,1),
            vn0,
            vt0,
                });

            vertx.push_back(Vertex{//v2
               v2,
               glm::vec3(1,1,1),
               vn2,
               vt2,
                });

            vertx.push_back(Vertex{//v3
               v3,
               glm::vec3(1,1,1),
               vn3,
               vt3,
                });

            //push the vertex index
            indices.push_back( //v0
                static_cast<GLuint>(faces[face][0][0] - 1)
            );
            indices.push_back( //2
                static_cast<GLuint>(faces[face][2][0] - 1)
            );
            indices.push_back( //2
                static_cast<GLuint>(faces[face][3][0] - 1)
            );
        }
    }
    
    //create and push the mesh
    this->meshes.push_back(Mesh(vertx, indices, this->textures));
}

glm::mat4 ObjModel::getModelMatrix() {
    // Create a 4x4 identity matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Apply translation
    modelMatrix = glm::translate(modelMatrix, this->position);

    //// Apply rotation
    //glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(this->rotation.x, this->rotation.y, this->rotation.z, 0));
    //modelMatrix = modelMatrix * rotationMatrix;

    // Apply scale
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}
void ObjModel::Draw(Shader& shader, Camera& camera)
{
   

    for (Mesh mesh : this->meshes) {
        mesh.Draw(shader, camera);
    }
}

