// actions.h

#ifndef ACTIONS_H
#define ACTIONS_H

#include "SceneObject.h"

#define MAX_ACTIONS 100

typedef enum {
    ACTION_ADD,
    ACTION_REMOVE,
    ACTION_TRANSFORM,
    ACTION_CHANGE_COLOR,
    ACTION_TOGGLE_OPTION
} ActionType;

typedef struct {
    ActionType type;
    SceneObject previousState;
    SceneObject newState;
    int objectIndex;
    char description[256];
} Action;

extern Action actionHistory[MAX_ACTIONS];
extern int historyCount;

void pushUndoAction(Action action);
Action popUndoAction();
void pushRedoAction(Action action);
Action popRedoAction();
void addToHistory(Action action);
void undo_last_action();
void redo_last_action();
void addObjectWithAction(ObjectType type, bool useTextures, int textureID, bool useColors, Model* model, PBRMaterial material, bool usePBR);
void removeObjectWithAction(int index);
void transformObjectWithAction(int index, Vector3 position, Vector3 rotation, Vector3 scale);
void changeColorWithAction(int index, Vector4 color);
void toggleOptionWithAction(const char* optionName, bool newValue);

#endif // ACTIONS_H
