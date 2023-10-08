#ifndef NSUKIT_VERSION_H
#define NSUKIT_VERSION_H

#define PROJECT_NAME                             "NSUKit"
#define NSUKIT_PROJECT_VER                       "0.1.1"
#define NSUKIT_PROJECT_VER_MAJOR                 0
#define NSUKIT_PROJECT_VER_MINOR                 1
#define NSUKIT_PROJECT_VER_PATCH                 1
#define NSUKIT_PROJECT_VER_CODE                  (((NSUKIT_PROJECT_VER_MAJOR) << 16) + \
                                                   ((NSUKIT_PROJECT_VER_MINOR) << 8) + \
                                                   (NSUKIT_PROJECT_VER_PATCH))
#define NSUKIT_VER_(major, minor, patch)         (((major) << 16) + ((minor) << 8) + (patch))

#define NSUKIT_BUILD_TIME "20231008_221022"
#define NSUKIT_GIT_INFO "master_1e0b20d"


#define NSUKIT_WITH_XDMA

#endif //NSUKIT_VERSION_H
