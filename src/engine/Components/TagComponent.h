#pragma once

enum class EntityTag {
    Player,
    Enemy,
    Bullet,
    Wall
};

struct TagComponent {
    EntityTag tag;
};