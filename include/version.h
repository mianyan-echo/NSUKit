#ifndef NSUKIT_VERSION_H
#define NSUKIT_VERSION_H

#define PROJECT_NAME                             "NSUKit"
#define NSUKIT_PROJECT_VER                       "0.1.3"
#define NSUKIT_PROJECT_VER_MAJOR                 0
#define NSUKIT_PROJECT_VER_MINOR                 1
#define NSUKIT_PROJECT_VER_PATCH                 3
#define NSUKIT_PROJECT_VER_CODE                  (((NSUKIT_PROJECT_VER_MAJOR) << 16) + \
                                                   ((NSUKIT_PROJECT_VER_MINOR) << 8) + \
                                                   (NSUKIT_PROJECT_VER_PATCH))
#define NSUKIT_VER_(major, minor, patch)         (((major) << 16) + ((minor) << 8) + (patch))

#define NSUKIT_BUILD_TIME "20231011_160429"
#define NSUKIT_GIT_INFO "master_0637955"


#define NSUKIT_WITH_XDMA

#endif //NSUKIT_VERSION_H
