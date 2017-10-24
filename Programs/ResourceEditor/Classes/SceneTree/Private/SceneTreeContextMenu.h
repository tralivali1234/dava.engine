#pragma once

#include "Classes/Selection/Selectable.h"

#include <TArc/Core/OperationRegistrator.h>
#include <TArc/Utils/QtConnections.h>
#include <TArc/Qt/QtString.h>

#include <Base/Any.h>
#include <Base/BaseTypes.h>
#include <Base/FastName.h>
#include <Command/Command.h>
#include <FileSystem/FilePath.h>
#include <Functional/Signal.h>
#include <Reflection/ReflectedType.h>

class SceneEditor2;
class SceneTreeModelV2;

namespace DAVA
{
class ParticleEffectComponent;
class ParticleEmitter;

namespace TArc
{
class ContextAccessor;
class UI;
class OperationInvoker;
} // namespace TArc
} // namespace DAVA

class QMenu;
class QPoint;
class QAction;

class BaseContextMenu
{
public:
    BaseContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);

    void Init(DAVA::TArc::ContextAccessor* accessor, DAVA::TArc::UI* ui, DAVA::TArc::OperationInvoker* invoker);
    void Show(const QPoint& pos);

protected:
    virtual void FillActions(QMenu& menu) = 0;

    void RemoveCommandsHelper(const DAVA::String& text, const DAVA::ReflectedType* type, const DAVA::Function<std::unique_ptr<DAVA::Command>(const Selectable&)>& callback);
    void ForEachSelectedByType(const DAVA::ReflectedType* type, const DAVA::Function<void(const Selectable&)>& callback);

    DAVA::ParticleEffectComponent* GetParticleEffectComponent(const Selectable& object);
    void SaveEmitter(DAVA::ParticleEffectComponent* component, DAVA::ParticleEmitter* emitter,
                     bool askFileName, const QString& defaultName,
                     const DAVA::Function<std::unique_ptr<DAVA::Command>(const DAVA::FilePath&)>& commandCreator);

    void Connect(QAction* action, const DAVA::Function<void()>& fn);
    QString GetParticlesConfigPath();
    DAVA::FilePath GetDataSourcePath() const;

protected:
    SceneEditor2* scene = nullptr;
    const SceneTreeModelV2* model = nullptr;
    DAVA::Vector<Selectable> selectedObjects;
    Selectable currentObject;

    DAVA::TArc::ContextAccessor* accessor = nullptr;
    DAVA::TArc::UI* ui = nullptr;
    DAVA::TArc::OperationInvoker* invoker = nullptr;

    DAVA::TArc::QtConnections connections;
};

class EntityContextMenu : public BaseContextMenu
{
    using TBase = BaseContextMenu;

public:
    EntityContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);

protected:
    void FillActions(QMenu& menu) override;

private:
    void FillCameraActions(QMenu& menu);

    void SaveEntityAs();
    void EditModel();
    void ReloadModel();
    void ReloadModelAs();

    void AddEmitter();
    void SaveEffectEmitters();
    void SaveEffectEmittersAs();
    void GrabImage();

    void PerformSaveEffectEmitters(bool forceAskFileName);

    template <typename CMD, typename... Arg>
    void ExecuteCommandForEffect(Arg&&... args);

    void StartEffect();
    void StopEffect();
    void RestartEffect();

    void SetEntityNameAsFilter();

    void SetCurrentCamera();
    void SetCustomDrawCamera();

    void SaveSlotsPreset();
    void LoadSlotsPreset();
};

class ParticleLayerContextMenu : public BaseContextMenu
{
    using TBase = BaseContextMenu;

public:
    ParticleLayerContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);

protected:
    void FillActions(QMenu& menu) override;

private:
    void CloneLayer();
    void RemoveLayer();
    void AddForce();
};

class ParticleForceContextMenu : public BaseContextMenu
{
    using TBase = BaseContextMenu;

public:
    ParticleForceContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);

protected:
    void FillActions(QMenu& menu) override;

private:
    void RemoveForce();
};

class ParticleEmitterContextMenu : public BaseContextMenu
{
    using TBase = BaseContextMenu;

public:
    ParticleEmitterContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);

protected:
    void FillActions(QMenu& menu) override;

    void RemoveEmitter();
    void AddLayer();
    void LoadEmitterFromYaml();
    void SaveEmitterToYaml();
    void SaveEmitterToYamlAs();
    void SaveEmitterLocal(bool forceAskFileName);

    std::unique_ptr<DAVA::Command> CreateLoadCommand(const DAVA::FilePath& path);
    std::unique_ptr<DAVA::Command> CreateSaveCommand(const DAVA::FilePath& path);
};

DECLARE_OPERATION_ID(SetSceneTreeFilter);

std::unique_ptr<BaseContextMenu> CreateSceneTreeContextMenu(SceneEditor2* scene, const SceneTreeModelV2* model, const DAVA::Vector<Selectable>& selectedObjects, const Selectable& currentObject);
