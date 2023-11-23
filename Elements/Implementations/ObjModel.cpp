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
    //calculate the min and max
    //make the transformations
    glm::mat4 model = getModelMatrix();
    glm::vec3 node = glm::vec3(model * glm::vec4(vertices[0][0], vertices[0][1], vertices[0][2], 1.0f));
    GLuint CenterX, CenterY, CenterZ ;
    CenterX = CenterY = CenterZ = 0;
    max_x = node.x;
    max_y = node.y;
    max_z = node.z;
    for (int i = 0; i < vertices.size(); i++) {
        node = glm::vec3(model * glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f));
        CenterX += node.x;
        CenterY += node.y;
        CenterZ += node.z;
    }

    this->centerMass = glm::vec3(
        (CenterX)/vertices.size(),
        (CenterY) / vertices.size(),
        (CenterZ) / vertices.size()
    );
    if (max_x < node.x) max_x = node.x;
    if (max_y < node.y) max_y = node.y;
    if (max_z < node.z) max_z = node.z;
    if (min_x < node.x) min_x = node.x;
    if (min_y < node.y) min_y = node.y;
    if (min_z < node.z) min_z = node.z;
    this->maxV = glm::vec3(max_x, max_y, max_z);
    this->minV = glm::vec3(min_x, min_y, min_z);
    this ->boundingR = glm::length( maxV - this->centerMass);
}

void ObjModel::assemble(
    std::vector<std::vector<float>> textcoords,
    std::vector<std::vector<float>> vertices,
    std::vector<std::vector<float>> normals,
    std::vector<std::vector<std::vector<float>>> faces
) {
    std::vector<unsigned int> indices;
    std::vector <Vertex> vertx;
    //create the vertices
    for (int face = 0; face < faces.size(); face++) {

   
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
            vn0 = glm::vec3(
                normals[faces[face][0][2] - 1][0],
                normals[faces[face][0][2] - 1][1],
                normals[faces[face][0][2] - 1][2]
            );
            //normal v1
            vn1 = glm::vec3(
                normals[faces[face][1][2] - 1][0],
                normals[faces[face][1][2] - 1][1],
                normals[faces[face][1][2] - 1][2]
            );
            //normal v2
            vn2 = glm::vec3(
                normals[faces[face][2][2] - 1][0],
                normals[faces[face][2][2] - 1][1],
                normals[faces[face][2][2] - 1][2]
            );
        }
        //create the verts
        // Generate random floating-point values between 0 and 1
        float randomFloat1 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float randomFloat2 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float randomFloat3 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        Vertex vx0 = Vertex{//v0
            v0,
            glm::vec3(randomFloat1,randomFloat3,randomFloat1),
            vn0,
            vt0,
        };
        Vertex vx2 = Vertex{//v2
           v2,
           glm::vec3(randomFloat3,randomFloat1,randomFloat3),
           vn2,
           vt2,
        };
        vertx.push_back(vx0);
        vertx.push_back(Vertex{//v1
           v1,
           glm::vec3(randomFloat1,randomFloat2,randomFloat3),
           vn1,
           vt1,
            });
        vertx.push_back(vx2);

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
                vn3 = glm::vec3(
                    normals[faces[face][3][2] - 1][0],
                    normals[faces[face][3][2] - 1][1],
                    normals[faces[face][3][2] - 1][2]
                );
            }

            //push the indices of v0,v2,v3
            //create the verts
            vertx.push_back(vx0); //v0

            vertx.push_back(vx2); //v2

            vertx.push_back(Vertex{//v3
               v3,
               glm::vec3(randomFloat2,randomFloat3,randomFloat1),
               vn3,
               vt3,
                });
        }
    }
    //create the indices (groups of 3)
    for (int vtx = 0; vtx < vertx.size(); vtx += 3) {
        indices.push_back(vtx);
        indices.push_back(vtx + 1);
        indices.push_back(vtx + 2);
    }

    //create and push the mesh
    this->meshes.push_back(Mesh(vertx, indices, this->textures));
}

glm::mat4 ObjModel::getModelMatrix() {
    // Create a 4x4 identity matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Apply translation
    modelMatrix = glm::translate(modelMatrix, this->position);

    // Apply rotation

    modelMatrix = glm::rotate(modelMatrix,glm::radians(rotation.x),glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

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

