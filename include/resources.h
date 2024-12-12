#ifndef RESOURCES_H
#define RESOURCES_H


#ifdef __DEBUG__
#define RESOURCES_PATH "../resources/"
#else
#define RESOURCES_PATH "resources/"
#endif

#define SHADERS_PATH  RESOURCES_PATH "shaders/"
#define TEXTURES_PATH  RESOURCES_PATH "textures/"
#define SOUNDS_PATH RESOURCES_PATH "audio/"

static bool file_modified(const char *filename, int filetime) {
    int file_timestamp = GetFileModTime(filename);
    return file_timestamp != filetime;
}

#endif
