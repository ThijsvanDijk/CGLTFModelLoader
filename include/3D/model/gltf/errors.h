#ifndef GLTF_ERRORS_H
#define GLTF_ERRORS_H

#define GLTF_NOERRORMSG 3
#define GLTF_MAXLENERRORMSG 50

#include <3D/model/gltf/common.h>
#include <stdio.h>

i8 gltf_error(u16 code);

#endif