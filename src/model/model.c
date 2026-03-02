#include "model.h"

Model get_model(void) {
    Model model;
    model.world = get_world();
    return model;
}

/** TODO: Should not need index for level once we have internal management */
void model_update(Model *model) {
    world_update(&model->world, 0);
}
