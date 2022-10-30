#include <zzzjson/zzzjson.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#ifndef _WIN32
    #include <unistd.h>
#endif
#include <fcntl.h>

//========== Definitions ==========//
#define GLB_IDENTIFICATION_CODE 0x46546C67
#define GLB_CHUNK_STRUCTURED_JSON 0x4E4F534A
#define GLB_CHUNK_BINARY_BUFFER 0x004E4942
#define GLB_CHUNK_BYTE_ALIGNMENT 4
#define GLTF_VERSION 2
#define GLTF_IMAGE_START_PATTERN_PNG (uint8_t[]){0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a}
#define GLTF_IMAGE_START_PATTERN_JPEG (uint8_t[]){0xff, 0xd8, 0xff}

//========== ENUMS ==========//
typedef enum{
    GLTF_ACCESSOR_COMPONENTTYPE_BYTE = 0,
    GLTF_ACCESSOR_COMPONENTTYPE_UNSIGNED_BYTE = 1,
    GLTF_ACCESSOR_COMPONENTTYPE_SHORT = 2,
    GLTF_ACCESSOR_COMPONENTTYPE_UNSIGNED_SHORT = 3,
    GLTF_ACCESSOR_COMPONENTTYPE_UNSIGNED_INT = 5,
    GLTF_ACCESSOR_COMPONENTTYPE_FLOAT = 6
} GLTFAccessorComponentType;

typedef enum{
    GLTF_ACCESSOR_TYPE_SCALAR,
    GLTF_ACCESSOR_TYPE_VEC2,
    GLTF_ACCESSOR_TYPE_VEC3,
    GLTF_ACCESSOR_TYPE_VEC4,
    GLTF_ACCESSOR_TYPE_MAT2,
    GLTF_ACCESSOR_TYPE_MAT3,
    GLTF_ACCESSOR_TYPE_MAT4
} GLTFAccessorType;

typedef enum{
    GLTF_ANIMATION_CHANNEL_PATH_TRANSLATION,
    GLTF_ANIMATION_CHANNEL_PATH_ROTATION,
    GLTF_ANIMATION_CHANNEL_PATH_SCALE,
    GLTF_ANIMATION_CHANNEL_PATH_WEIGHTS
} GLTFAnimationChannelTargetPath;

typedef enum{
    GLTF_ANIMATION_SAMPLER_INTERPOLATION_LINEAR,
    GLTF_ANIMATION_SAMPLER_INTERPOLATION_STEP,
    GLTF_ANIMATION_SAMPLER_INTERPOLATION_CUBICSPLINE
} GLTFAnimationSamplerInterpolation;

typedef enum{
    GLTF_BUFFERVIEW_TARGET_ARRAY_BUFFER = 34962,
    GLTF_BUFFERVIEW_TARGET_ELEMENT_ARRAY_BUFFER = 34963
} GLTFBufferViewTarget;

typedef enum{
    GLTF_CAMERA_TYPE_ORTHOGRAPHIC,
    GLTF_CAMERA_TYPE_PERSPECTIVE
} GLTFCameraType;

typedef enum{
    GLTF_IMAGE_MIMETYPE_JPEG,
    GLTF_IMAGE_MIMETYPE_PNG
} GLTFImageMimeType;

typedef enum{
    GLTF_MATERIAL_ALPHAMODE_OPAQUE,
    GLTF_MATERIAL_ALPHAMODE_MASK,
    GLTF_MATERIAL_ALPHAMODE_BLEND
} GLTFMaterialAlphaMode;

typedef enum{
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_POSITION,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_NORMAL,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_TANGENT,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_TEXCOORD,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_COLOR,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_JOINTS,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_WEIGHTS
} GLTFMeshPrimitiveAttributeType;

typedef enum{
    GLTF_MESH_PRIMITIVE_MODE_POINTS,
    GLTF_MESH_PRIMITIVE_MODE_LINES,
    GLTF_MESH_PRIMITIVE_MODE_LINE_LOOP,
    GLTF_MESH_PRIMITIVE_MODE_LINE_STRIP,
    GLTF_MESH_PRIMITIVE_MODE_TRIANGLES,
    GLTF_MESH_PRIMITIVE_MODE_TRIANGLE_STRIP,
    GLTF_MESH_PRIMITIVE_MODE_TRIANGLE_FAN
} GLTFMeshPrimitiveMode;

typedef enum{
    GLTF_SAMPLER_MAGFILTER_NEAREST = 9728,
    GLTF_SAMPLER_MAGFILTER_LINEAR = 9729
} GLTFSamplerMagFilter;

typedef enum{
    GLTF_SAMPLER_MINFILTER_NEAREST = 9728,
    GLTF_SAMPLER_MINFILTER_LINEAR = 9729,
    GLTF_SAMPLER_MINFILTER_NEAREST_MIPMAP_NEAREST = 9984,
    GLTF_SAMPLER_MINFILTER_LINEAR_MIPMAP_NEAREST = 9985,
    GLTF_SAMPLER_MINFILTER_NEAREST_MIPMAP_LINEAR = 9986,
    GLTF_SAMPLER_MINFILTER_LINEAR_MIPMAP_LINEAR = 9987
} GLTFSamplerMinFilter;

typedef enum{
    GLTF_SAMPLER_WRAP_CLAMP_TO_EDGE = 33071,
    GLTF_SAMPLER_WRAP_MIRRORED_REPEAT = 33648,
    GLTF_SAMPLER_WRAP_REPEAT = 10497
} GLTFSamplerWrapMode;


//========== STRUCTS ==========//
/**
 * @brief Vertices are made of:
 * position vectors (float[3]),
 * normal vectors (float[3]) 
 */
typedef struct {
    vec3 position;
    vec3 normal;
    // vec2 tex_coords;
    // vec3 tangent;
    // vec3 bitangent;
} Vertex;

/**
 * @brief Meshes are used to store vertex and index data loaded from 3d models 
 */
typedef struct {
    Vertex * vertices;
    unsigned int * indices;
} Mesh;

//---------- Headers ----------//
/**
 * @brief Every GLTF file has a 12 bit header with some basic info:
 * an identifier code, the version number and the length of the file (including header) 
 */
typedef struct {
    unsigned int magic, version, length;
} GLBHeader;

/**
 * @brief Two chunks can exist in a GLB file, the JSON and the buffer. Both sections have
 * a short header which specifies which chunk this section is and what the length of the section is. 
 */
typedef struct {
    unsigned int chunkLength, chunkType;
} GLBChunkHeader;

//---------- Accessor ----------//
// typedef struct{
//     // Required
//     uint32_t bufferView;
//     GLTFAccessorComponentType componentType;
//     // Optional
//     uint64_t byteOffset;
// } GLTFAccessorSparseIndices;

// typedef struct{
//     // Required
//     uint32_t bufferView;
//     // Optional
//     uint64_t byteOffset;
// } GLTFAccessorSparseValues;

// typedef struct{
//     // Required
//     uint64_t count;
//     GLTFAccessorSparseIndices indices;
//     GLTFAccessorSparseValues values;
// } GLTFAccessorSparse;

typedef struct{
    // Required
    GLTFAccessorComponentType componentType;
    uint64_t count;
    GLTFAccessorType type;
    // Optional
    uint32_t bufferView;
    uint64_t byteOffset; // Default: 0
    bool normalized; // Default: false
    // float* max;
    // float* min;
    // GLTFAccessorSparse sparse;
    const char* name;
} GLTFAccessor;

//---------- Animation ----------//
// typedef struct{
//     // Required
//     GLTFAnimationChannelTargetPath path;
//     // Optional
//     uint32_t node;
// } GLTFAnimationChannelTarget;

// typedef struct{
//     // Required
//     uint32_t sampler;
//     GLTFAnimationChannelTarget target;
// } GLTFAnimationChannel;

// typedef struct{
//     // Required
//     uint32_t input;
//     uint32_t output;
//     // Optional
//     GLTFAnimationSamplerInterpolation interpolation;
// } GLTFAnimationSampler;

// typedef struct{
//     // Required
//     GLTFAnimationChannel* channels;
//     GLTFAnimationSampler* samplers;
//     // Optional
//     const char* name;
//     uint32_t channelCount, samplerCount;
// } GLTFAnimation;

//---------- Asset ----------//
typedef struct {
    // Required
    uint8_t versionMajor;
    uint8_t versionMinor;
    bool hasMinVersion : 1;
    uint32_t copyrightLength;
    uint32_t generatorLength;
    // Optional
    uint8_t minVersionMajor;
    uint8_t minVersionMinor;
    const char* copyright;
    const char* generator;
} GLTFAsset;

//---------- Buffer ----------//
typedef struct{
    // Required
    uint64_t byteLength;
    // Optionals
    const char* name;
    const char* uri;
} GLTFBuffer;

//---------- Buffer View ----------//
typedef struct{
    // Required
    uint32_t buffer;
    uint64_t byteLength;
    // Optional
    uint64_t byteOffset; // Default : 0
    uint8_t byteStride;
    GLTFBufferViewTarget target;
    const char* name;
} GLTFBufferView;

//---------- Camera ----------//
// typedef struct{
//     // Required
//     float xmag;
//     float ymag;
//     float zfar;
//     float znear;
// } GLTFCameraOrthographic;

// typedef struct{
//     // Required
//     float yfov;
//     float znear;
//     // Optional
//     float aspectRatio;
//     float zfar;
// } GLTFCameraPerspective;

// typedef struct{
//     // Required
//     GLTFCameraType type;
//     // Optional
//     GLTFCameraOrthographic orthographic;
//     GLTFCameraPerspective perspective;
//     const char* name;
// } GLTFCamera;

//---------- Image ----------//
// typedef struct{
//     // Optional
//     const char* uri;
//     GLTFImageMimeType mimeType;
//     uint32_t bufferView;
//     const char* name;
// } GLTFImage;

//---------- Material ----------//
// typedef struct{
//     // Required
//     uint32_t index;
//     // Optional
//     uint32_t texCoord;
// } GLTFTextureInfo;

// typedef struct{
//     // Required
//     uint32_t index;
//     // Optional
//     uint32_t texCoord;
//     float scale;
// } GLTFMaterialNormalTextureInfo;

// typedef struct{
//     // Required
//     uint32_t index;
//     // Optional
//     uint32_t texCoord;
//     float strength;
// } GLTFMaterialOcclusionTextureInfo;

// typedef struct{
//     // Optional
//     vec4 baseColorFactor;
//     GLTFTextureInfo baseColorTexture;
//     float metallicFactor;
//     float roughnessFactor;
//     GLTFTextureInfo metallicRoughnessTexture;
// } GLTFMaterialPBRMetallicRoughness;

// typedef struct{
//     // Optional
//     const char* name;
//     GLTFMaterialPBRMetallicRoughness pbrMetallicRoughness;
//     GLTFMaterialNormalTextureInfo normalTexture;
//     GLTFMaterialOcclusionTextureInfo occlusionTexture;
//     GLTFTextureInfo emissiveTexture;
//     vec3 emissiveFactor;
//     GLTFMaterialAlphaMode alphaMode;
//     float alphaCutoff;
//     bool doubleSided;
// } GLTFMaterial;

//---------- Mesh ----------//
typedef struct{
    GLTFMeshPrimitiveAttributeType type;
    uint32_t accessor;
} GLTFMeshPrimitiveAttribute;

typedef GLTFMeshPrimitiveAttribute* GLTFMorphTarget;

typedef struct{
    // Required
    GLTFMeshPrimitiveAttribute* attributes;
    // Optional
    uint32_t indices;
    uint32_t material;
    GLTFMeshPrimitiveMode mode;
    // GLTFMorphTarget* targets;
} GLTFMeshPrimitive;

typedef struct{
    // Required
    GLTFMeshPrimitive* primitives;
    // Optional
    const char* name;
    float* weights;
} GLTFMesh;

//---------- Node ----------//
typedef struct{
    // Optional
    // uint32_t camera;    
    uint32_t* children;
    // uint32_t skin;
    mat4 matrix; // Default : [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
    uint32_t mesh;    
    vec4 rotation; // Default : [0,0,0,1]
    vec3 scale; // Default : [1,1,1]
    vec3 translation; // Default : [0,0,0]
    // float* weights;
    const char* name; 
    uint32_t childrenCount;  
} GLTFNode;

//---------- Sampler ----------//
// typedef struct{
//     // Optional
//     GLTFSamplerMagFilter magFilter;
//     GLTFSamplerMinFilter minFilter;
//     GLTFSamplerWrapMode wrapS;
//     GLTFSamplerWrapMode wrapT;
//     const char* name;
// } GLTFSampler;

//---------- Scene ----------//
typedef struct{
    // Optional
    uint32_t* nodes;
    const char* name;    
} GLTFScene;

//---------- Skin ----------//
// typedef struct{
//     // Required
//     uint32_t* joints;
//     // Optional
//     uint32_t inverseBindMatrices;
//     uint32_t skeleton;
//     const char* name;
// } GLTFSkin;

//---------- Texture ----------//
// typedef struct{
//     // Optional
//     uint32_t sampler;
//     uint32_t source;
//     const char* name;
// } GLTFTexture;

//---------- Root Object ----------//
typedef struct{
    // Required
    GLTFAsset asset;
    // Optional
    // GLTFAccessor* accessors;
    // GLTFAnimation* animations;
    // GLTFBuffer* buffers;
    // GLTFBufferView* bufferViews;
    // GLTFCamera* cameras;
    // GLTFImage* images;
    // GLTFMaterial* materials;
    // GLTFMesh* meshes;
    // GLTFNode* nodes;
    // GLTFSampler* samplers;
    uint32_t scene; 
    GLTFScene* scenes;
    // GLTFSkin* skins;
    // GLTFTexture* textures; 
    // Json Allocator
    zj_Allocator* allocator;
    uint8_t data[];
} GLBModel;

/**
 * @brief Is used to store information about a GLTF File. This is done so the reading of the data can be done
 * as uninterrupted as possible. 
 */
typedef struct {
    zj_Allocator * json_allocator;
    char bin_buffer_uri[FILENAME_MAX];
    unsigned int json_data_size;
    unsigned int vertex_count, vertex_position_data_offset, vertex_normal_data_offset, vertex_texcoord_data_offset;
    unsigned int index_count, index_data_offset, index_type;
    unsigned int vertex_position_type, vertex_normal_type, vertex_texcoord_type;
    unsigned int glb_size;
} GLTFModelInfo;

#ifndef _WIN32
/**
 * @brief Convert a GLTF file into a optimised GLB format where data is stored exactly as OpenGL works with it,
 * allowing possible future extension with DirectStorage like technologies, drastically reducing load times.
 * 
 * @param info The Bufferinfo that contains info about the model that needs to be converted
 * @param mesh The mesh where the model data is stored
 * @param filename The created GLB file is stored here
 * @return char Returns 0 if succesful, if 0 is not returned: error
 */
char convertGLTFToGLB(GLTFModelInfo * info, Mesh * mesh, const char * filename);
#endif

/**
 * @brief Gets info about a model before loading occurs
 * 
 * @param file File to get info from
 * @param data Info struct to fill in the model info from file
 * @return char 0 if succesful, < 0 if unsuccesful
 */
char getGLTFModelInfo(const char * file, GLTFModelInfo * model_info);

/**
 * @brief Reads the vertices and indices from the buffer and stores these in result
 * 
 * @param buffer_data Info struct with info about the 3D model
 * @param result The Mesh where the data is stored
 * @return char 0 if succesful, < 0 if unsuccesful
 */
char loadGLTFBinaryData(GLTFModelInfo * model_info, Mesh * result);

char prepareGLBRead(const char* file_name, GLBModel* model_info, char* error_msg);

void releaseModelInfo(GLBModel* model);

/**
 * @brief UNDER CONSTRUCTION! Will eventually read the optimised GLB file and store data in result
 * 
 * @param file File to read the data from
 */
void readGLB(const char * file);