#ifndef GLTF_TYPES_H
#define GLTF_TYPES_H

#include <3D/model/gltf/common.h>

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
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_TEXCOORD_0,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_TEXCOORD_1,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_COLOR_0,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_JOINTS_0,
    GLTF_MESH_PRIMITIVE_ATTRIBUTE_TYPE_WEIGHTS_0
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

//---------- Accessors ----------//

typedef struct{
    // Required
    u32 bufferView;
    GLTFAccessorComponentType componentType;
    // Optional
    u64 byteOffset;
} GLTFAccessorSparseIndices;

typedef struct{
    // Required
    u32 bufferView;
    // Optional
    u64 byteOffset;
} GLTFAccessorSparseValues;

typedef struct{
    // Required
    u64 count;
    GLTFAccessorSparseIndices indices;
    GLTFAccessorSparseValues values;
} GLTFAccessorSparse;

typedef struct{
    // Required
    GLTFAccessorComponentType componentType;
    u64 count;
    GLTFAccessorType type;
    // Optional
    u32 bufferView;
    u64 byteOffset; // Default: 0
    bool normalized; // Default: false
    GLTFAccessorSparse sparse;
    const string name;
    u32 nameLength;
    bool hasBufferView : 1;
    bool hasSparse : 1;
} GLTFAccessor;

//---------- Animations ----------//

typedef struct{
    // Required
    GLTFAnimationChannelTargetPath path;
    // Optional
    u32 node;
} GLTFAnimationChannelTarget;

typedef struct{
    // Required
    u32 sampler;
    GLTFAnimationChannelTarget target;
} GLTFAnimationChannel;

typedef struct{
    // Required
    u32 input;
    u32 output;
    // Optional
    GLTFAnimationSamplerInterpolation interpolation;
} GLTFAnimationSampler;

typedef struct{
    // Required
    GLTFAnimationChannel* channels;
    GLTFAnimationSampler* samplers;
    // Optional
    const string name;
    u32 channelCount, samplerCount;
} GLTFAnimation;

//---------- Asset ----------//

typedef struct {
    // Required
    u8 versionMajor;
    u8 versionMinor;
    u8 hasMinVersion;
    u32 copyrightLength;
    u32 generatorLength;
    // Optional
    u8 minVersionMajor;
    u8 minVersionMinor;
    const string copyright;
    const string generator;
} GLTFAsset;

//---------- Buffers ----------//

typedef struct{
    // Required
    u64 byteLength;
    // Optionals
    const string name;
    const string uri;
    u16 nameLength;
    u16 uriLength;
} GLTFBuffer;

//---------- BufferViews ----------//

typedef struct{
    // Required
    u32 buffer;
    u64 byteLength;
    // Optional
    u64 byteOffset; // Default : 0
    u8 byteStride; // Default : 0
    GLTFBufferViewTarget target;
    const string name;
    bool hasTarget;
    u16 nameLength;
} GLTFBufferView;

//---------- Cameras ----------//

typedef struct{
    // Required
    float xmag;
    float ymag;
    float zfar;
    float znear;
} GLTFCameraOrthographic;

typedef struct{
    // Required
    float yfov;
    float znear;
    // Optional
    float aspectRatio;
    float zfar;
} GLTFCameraPerspective;

typedef struct{
    // Required
    GLTFCameraType type;
    // Optional
    GLTFCameraOrthographic orthographic;
    GLTFCameraPerspective perspective;
    const string name;
} GLTFCamera;

//---------- Images ----------//

typedef struct{
    // Optional
    u32 bufferView;
    GLTFImageMimeType mimeType;    
    const string name;
    const string uri;    
} GLTFImage;

//---------- Materials ----------//

typedef struct{
    // Required
    u32 index;
    // Optional
    u32 texCoord;
} GLTFTextureInfo;

typedef struct{
    // Required
    u32 index;
    // Optional
    u32 texCoord;
    float scale;
} GLTFMaterialNormalTextureInfo;

typedef struct{
    // Required
    u32 index;
    // Optional
    u32 texCoord;
    float strength;
} GLTFMaterialOcclusionTextureInfo;

typedef struct{
    // Optional
    vec4 baseColorFactor;
    GLTFTextureInfo baseColorTexture;
    float metallicFactor;
    float roughnessFactor;
    GLTFTextureInfo metallicRoughnessTexture;
} GLTFMaterialPBRMetallicRoughness;

typedef struct{
    // Optional
    const string name;
    GLTFMaterialPBRMetallicRoughness pbrMetallicRoughness;
    GLTFMaterialNormalTextureInfo normalTexture;
    GLTFMaterialOcclusionTextureInfo occlusionTexture;
    GLTFTextureInfo emissiveTexture;
    vec3 emissiveFactor;
    GLTFMaterialAlphaMode alphaMode;
    float alphaCutoff;
    bool doubleSided;
} GLTFMaterial;

//---------- Meshes ----------//

typedef struct{
    u32 position;
    u32 normal;
    u32 tangent;
    u32 texcoord0;
    u32 texcoord1;
    u32 color;
    u32 joints;
    u32 weights;
    bool hasPosition : 1;
    bool hasNormal : 1;
    bool hasTangent : 1;
    bool hasTexcoord0 : 1;
    bool hasTexcoord1 : 1;
    bool hasColor : 1;
    bool hasJoints : 1;
    bool hasWeights : 1;
} GLTFMeshPrimitiveAttributes;

typedef struct{
    // Required
    GLTFMeshPrimitiveAttributes attributes;
    // Optional
    u32 indices;
    u32 material;
    GLTFMeshPrimitiveMode mode;
    bool hasIndices : 1;
    bool hasMaterial : 1;
    // GLTFMorphTarget* targets;
} GLTFMeshPrimitive;

typedef struct{
    // Required
    GLTFMeshPrimitive* primitives;
    // Optional
    const string name;
    float* weights;
    u32 primitivesLength;
    u32 weightsLength;
    u32 nameLength;
} GLTFMesh;

//---------- Nodes ----------//

typedef struct{
    // Optional
    u32 camera;    
    u32* children;
    u32 skin;
    mat4 matrix; // Default : [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
    u32 mesh;    
    vec4 rotation; // Default : [0,0,0,1]
    vec3 scale; // Default : [1,1,1]
    vec3 translation; // Default : [0,0,0]
    float* weights;
    const char* name; 
    u32 childrenLength;
    u32 weightsLength;
    u32 nameLength;  
    bool hasMesh : 1;
    bool hasCamera : 1;
    bool hasSkin : 1;
    bool hasMatrix : 1;
    bool hasRotation : 1;
    bool hasScale : 1;
    bool hasTranslation : 1;
} GLTFNode;

//---------- Samplers ----------//

typedef struct{
    // Optional
    GLTFSamplerMagFilter magFilter;
    GLTFSamplerMinFilter minFilter;
    GLTFSamplerWrapMode wrapS;
    GLTFSamplerWrapMode wrapT;
    const string name;
} GLTFSampler;

//---------- Scenes ----------//

typedef struct{
    // Optional
    u32* nodes;
    const string name; 
    u32 nodesLength;
    u32 nameLength;   
} GLTFScene;

//---------- Skins ----------//

typedef struct{
    // Required
    u32* joints;
    // Optional
    u32 inverseBindMatrices;
    u32 skeleton;
    const string name;
} GLTFSkin;

//---------- Textures ----------//

typedef struct{
    // Optional
    u32 sampler;
    u32 source;
    const string name;
} GLTFTexture;

//---------- Root Model ----------//

typedef struct{
    // Required
    GLTFAsset asset;
    // Optional
    GLTFAccessor* accessors;
    GLTFAnimation* animations;
    GLTFBuffer* buffers;
    GLTFBufferView* bufferViews;
    GLTFCamera* cameras;
    GLTFImage* images;
    GLTFMaterial* materials;
    GLTFMesh* meshes;
    GLTFNode* nodes;
    GLTFSampler* samplers;
    u32 scene; 
    GLTFScene* scenes;
    GLTFSkin* skins;
    GLTFTexture* textures; 
    u32 accessorsCount;
    u32 animationsCount;
    u32 buffersCount;
    u32 bufferViewsCount;
    u32 camerasCount;
    u32 imagesCount;
    u32 materialsCount;
    u32 meshesCount;
    u32 nodesCount;
    u32 samplersCount;
    u32 scenesCount;
    u32 skinsCount;
    u32 texturesCount;
    u64 dataLength;
    byte* data;
    byte* binDataStart;
} GLTFModel;

#endif