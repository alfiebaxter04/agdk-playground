if(NOT TARGET game-activity::game-activity)
add_library(game-activity::game-activity STATIC IMPORTED)
set_target_properties(game-activity::game-activity PROPERTIES
    IMPORTED_LOCATION "/usr/local/google/home/alfiebaxter/.gradle/caches/8.13/transforms/d9bf1cf30f82f24ae31a89016e57483a/transformed/games-activity-4.0.0/prefab/modules/game-activity/libs/android.x86/libgame-activity.a"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/local/google/home/alfiebaxter/.gradle/caches/8.13/transforms/d9bf1cf30f82f24ae31a89016e57483a/transformed/games-activity-4.0.0/prefab/modules/game-activity/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

if(NOT TARGET game-activity::game-activity_static)
add_library(game-activity::game-activity_static STATIC IMPORTED)
set_target_properties(game-activity::game-activity_static PROPERTIES
    IMPORTED_LOCATION "/usr/local/google/home/alfiebaxter/.gradle/caches/8.13/transforms/d9bf1cf30f82f24ae31a89016e57483a/transformed/games-activity-4.0.0/prefab/modules/game-activity_static/libs/android.x86/libgame-activity_static.a"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/local/google/home/alfiebaxter/.gradle/caches/8.13/transforms/d9bf1cf30f82f24ae31a89016e57483a/transformed/games-activity-4.0.0/prefab/modules/game-activity_static/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

