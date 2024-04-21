#ifndef NSUKIT_VERSION_H
#define NSUKIT_VERSION_H

#define PROJECT_NAME                             "NSUKit"
#define NSUKIT_PROJECT_VER                       "1.2.0"
#define NSUKIT_PROJECT_VER_MAJOR                 1
#define NSUKIT_PROJECT_VER_MINOR                 2
#define NSUKIT_PROJECT_VER_PATCH                 0
#define NSUKIT_PROJECT_VER_CODE                  (((NSUKIT_PROJECT_VER_MAJOR) << 16) + \
                                                   ((NSUKIT_PROJECT_VER_MINOR) << 8) + \
                                                   (NSUKIT_PROJECT_VER_PATCH))
#define NSUKIT_VER_(major, minor, patch)         (((major) << 16) + ((minor) << 8) + (patch))

#define NSUKIT_BUILD_TIME "20240421_140401"
#define NSUKIT_GIT_INFO "master_dacd0dc"


#define NSUKIT_WITH_XDMA

#endif //NSUKIT_VERSION_H
