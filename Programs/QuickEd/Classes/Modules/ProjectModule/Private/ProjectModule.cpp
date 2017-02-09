#include "Modules/ProjectModule/ProjectModule.h"
#include "Modules/ProjectModule/ProjectData.h"

#include "Application/QEGlobal.h"
#include "UI/mainwindow.h"

#include <TArc/WindowSubSystem/QtAction.h>
#include <TArc/WindowSubSystem/ActionUtils.h>
#include <TArc/Utils/ModuleCollection.h>

#include <Engine/PlatformApi.h>
#include <FileSystem/YamlNode.h>
#include <FileSystem/YamlEmitter.h>
#include <Base/Result.h>

#include <QApplication>

DAVA_VIRTUAL_REFLECTION_IMPL(ProjectModule)
{
    DAVA::ReflectionRegistrator<ProjectModule>::Begin()
    .ConstructorByPointer()
    .End();
}

namespace ProjectModuleDetails
{
const DAVA::String propertiesKey = "ProjectModuleProperties";
const DAVA::String lastProjectKey = "Last project";
const DAVA::String projectsHistoryKey = "Projects history";
const DAVA::uint32 projectsHistoryMaxSize = 5;

DAVA::Result CreateNewProjectInfrastructure(const QString& projectPath);
bool SerializeProjectDataToFile(const ProjectData* data);
}

ProjectModule::ProjectModule() = default;
ProjectModule::~ProjectModule() = default;

void ProjectModule::PostInit()
{
    CreateActions();
    RegisterOperation(QEGlobal::OpenLastProject.ID, this, &ProjectModule::OpenLastProject);
    RegisterOperation(ProjectModuleTesting::CreateProjectOperation.ID, this, &ProjectModule::CreateProject);
}

void ProjectModule::OnWindowClosed(const DAVA::TArc::WindowKey& key)
{
    CloseProject();
    DVASSERT(GetAccessor()->GetContextCount() == 0);
}

void ProjectModule::CreateActions()
{
    const QString toolBarName("mainToolbar");
    const QString fileMenuName("File");

    const QString newProjectActionName("New project");
    const QString openProjectActionName("Open project");
    const QString closeProjectActionName("Close project");
    const QString recentProjectsActionName("Recent");

    using namespace DAVA::TArc;
    ContextAccessor* accessor = GetAccessor();
    UI* ui = GetUI();
    //action new project
    {
        QAction* action = new QAction(QIcon(":/Icons/newscene.png"), newProjectActionName, nullptr);
        connections.AddConnection(action, &QAction::triggered, DAVA::Bind(&ProjectModule::OnNewProject, this));
        ActionPlacementInfo placementInfo;
        placementInfo.AddPlacementPoint(CreateMenuPoint(fileMenuName, { InsertionParams::eInsertionMethod::BeforeItem }));
        placementInfo.AddPlacementPoint(CreateToolbarPoint(toolBarName, { InsertionParams::eInsertionMethod::BeforeItem }));

        ui->AddAction(QEGlobal::windowKey, placementInfo, action);
    }

    //action open project
    {
        QAction* action = new QAction(QIcon(":/Icons/openproject.png"), openProjectActionName, nullptr);
        action->setShortcut(QKeySequence("Ctrl+O"));
        connections.AddConnection(action, &QAction::triggered, DAVA::Bind(&ProjectModule::OnOpenProject, this));
        ActionPlacementInfo placementInfo;
        placementInfo.AddPlacementPoint(CreateMenuPoint(fileMenuName, { InsertionParams::eInsertionMethod::AfterItem, newProjectActionName }));
        placementInfo.AddPlacementPoint(CreateToolbarPoint(toolBarName, { InsertionParams::eInsertionMethod::AfterItem, newProjectActionName }));

        ui->AddAction(QEGlobal::windowKey, placementInfo, action);
    }

    //action close project
    {
        QtAction* action = new QtAction(accessor, QIcon(":/QtTools/Icons/close-16.png"), closeProjectActionName);

        FieldDescriptor fieldDescr;
        fieldDescr.type = DAVA::ReflectedTypeDB::Get<ProjectData>();
        fieldDescr.fieldName = DAVA::FastName(ProjectData::projectPathPropertyName);
        action->SetStateUpdationFunction(QtAction::Enabled, fieldDescr, [](const DAVA::Any& fieldValue) -> DAVA::Any {
            return fieldValue.CanCast<DAVA::FilePath>() && !fieldValue.Cast<DAVA::FilePath>().IsEmpty();
        });

        connections.AddConnection(action, &QAction::triggered, DAVA::Bind(&ProjectModule::CloseProject, this));
        ActionPlacementInfo placementInfo;
        placementInfo.AddPlacementPoint(CreateMenuPoint(fileMenuName, { InsertionParams::eInsertionMethod::AfterItem, openProjectActionName }));
        placementInfo.AddPlacementPoint(CreateToolbarPoint(toolBarName, { InsertionParams::eInsertionMethod::AfterItem, openProjectActionName }));

        ui->AddAction(QEGlobal::windowKey, placementInfo, action);
    }

    // RecentProjects
    {
        QAction* recentProjects = new QAction(recentProjectsActionName, nullptr);
        DAVA::TArc::ActionPlacementInfo placementInfo(DAVA::TArc::CreateMenuPoint(fileMenuName, DAVA::TArc::InsertionParams(InsertionParams::eInsertionMethod::AfterItem, closeProjectActionName)));
        ui->AddAction(QEGlobal::windowKey, placementInfo, recentProjects);
    }

    // Separator
    {
        QAction* separator = new QAction(nullptr);
        separator->setObjectName("project actions separator");
        separator->setSeparator(true);
        DAVA::TArc::ActionPlacementInfo placementInfo(DAVA::TArc::CreateMenuPoint("File", DAVA::TArc::InsertionParams(InsertionParams::eInsertionMethod::AfterItem, recentProjectsActionName)));
        ui->AddAction(QEGlobal::windowKey, placementInfo, separator);
    }

    //Recent content
    {
        RecentMenuItems::Params params(QEGlobal::windowKey, accessor, ProjectModuleDetails::projectsHistoryKey);
        params.ui = GetUI();
        params.getMaximumCount = [this]() {
            return ProjectModuleDetails::projectsHistoryMaxSize;
        };
        params.menuSubPath << fileMenuName << recentProjectsActionName;
        params.insertionParams.method = InsertionParams::eInsertionMethod::BeforeItem;
        recentProjects.reset(new RecentMenuItems(std::move(params)));
        recentProjects->actionTriggered.Connect([this](const DAVA::String& projectPath) {
            OpenProject(projectPath);
        });
    }
}

void ProjectModule::OnOpenProject()
{
    using namespace DAVA::TArc;

    ContextAccessor* accessor = GetAccessor();
    DataContext* globalContext = accessor->GetGlobalContext();
    ProjectData* projectData = globalContext->GetData<ProjectData>();
    QString defaultPath;
    if (projectData != nullptr)
    {
        DAVA::String absProjectPath = projectData->GetProjectFile().GetAbsolutePathname();
        defaultPath = QString::fromStdString(absProjectPath);
    }

    FileDialogParams params;
    params.dir = defaultPath;
    params.filters = QObject::tr("Project files(*.quicked *.uieditor)");
    params.title = QObject::tr("Select a project file");
    QString projectPath = GetUI()->GetOpenFileName(QEGlobal::windowKey, params);

    if (projectPath.isEmpty())
    {
        return;
    }
    projectPath = QDir::toNativeSeparators(projectPath);

    OpenProject(projectPath.toStdString());
}

void ProjectModule::OnNewProject()
{
    using namespace DAVA;
    using namespace TArc;

    DirectoryDialogParams params;
    params.title = QObject::tr("Select directory for new project");
    QString projectDirPath = GetUI()->GetExistingDirectory(QEGlobal::windowKey, params);
    if (projectDirPath.isEmpty())
    {
        return;
    }

    CreateProject(projectDirPath);
}

void ProjectModule::CreateProject(const QString& projectDirPath)
{
    using namespace DAVA;
    using namespace TArc;

    ResultList resultList;

    QDir projectDir(projectDirPath);
    const QString projectFileName = QString::fromStdString(ProjectData::GetProjectFileName());
    QString fullProjectFilePath = projectDir.absoluteFilePath(projectFileName);
    if (QFile::exists(fullProjectFilePath))
    {
        ResultList resultList(Result(Result::RESULT_WARNING, QObject::tr("Project file exists!").toStdString()));
        ShowResultList(QObject::tr("Error while creating project"), resultList);
        return;
    }

    Result result = ProjectModuleDetails::CreateNewProjectInfrastructure(fullProjectFilePath);
    if (result.type != Result::RESULT_SUCCESS)
    {
        ResultList resultList(result);
        ShowResultList(QObject::tr("Error while creating project"), resultList);
        return;
    }
    DVASSERT(fullProjectFilePath.isEmpty() == false);
    OpenProject(fullProjectFilePath.toStdString());
}

void ProjectModule::OpenProject(const DAVA::String& path)
{
    using namespace DAVA::TArc;

    if (CloseProject() == false)
    {
        return;
    }
    ContextAccessor* accessor = GetAccessor();

    DAVA::ResultList resultList;
    std::unique_ptr<ProjectData> newProjectData = std::make_unique<ProjectData>();

    resultList = newProjectData->LoadProject(QString::fromStdString(path));
    if (resultList.HasErrors() == false)
    {
        DAVA::String lastProjectPath = newProjectData->GetProjectFile().GetAbsolutePathname();
        recentProjects->Add(lastProjectPath);

        PropertiesItem propsItem = accessor->CreatePropertiesNode(ProjectModuleDetails::propertiesKey);
        propsItem.Set(ProjectModuleDetails::lastProjectKey.c_str(), DAVA::Any(lastProjectPath));

        DataContext* globalContext = accessor->GetGlobalContext();
        globalContext->CreateData(std::move(newProjectData));
    }
    ShowResultList(QObject::tr("Error while loading project"), resultList);
}

bool ProjectModule::CloseProject()
{
    using namespace DAVA::TArc;
    ContextAccessor* accessor = GetAccessor();
    DataContext* globalContext = accessor->GetGlobalContext();
    InvokeOperation(QEGlobal::CloseAllDocuments.ID);
    //project was not closed
    if (accessor->GetContextCount() != 0)
    {
        return false;
    }
    globalContext->DeleteData<ProjectData>();
    return true;
}

void ProjectModule::OpenLastProject()
{
    using namespace DAVA;
    using namespace TArc;
    ContextAccessor* accessor = GetAccessor();
    String projectPath;
    {
        PropertiesItem propsItem = accessor->CreatePropertiesNode(ProjectModuleDetails::propertiesKey);
        projectPath = propsItem.Get<DAVA::String>(ProjectModuleDetails::lastProjectKey);
    }
    if (projectPath.empty() == false)
    {
        OpenProject(projectPath);
    }
}

void ProjectModule::ShowResultList(const QString& title, const DAVA::ResultList& resultList)
{
    using namespace DAVA;
    using namespace DAVA::TArc;
    ModalMessageParams params;
    params.title = title;

    QStringList errors;
    for (const Result& result : resultList.GetResults())
    {
        if (result.type == Result::RESULT_ERROR)
        {
            errors << QString::fromStdString(result.message);
        }
    }
    if (errors.empty())
    {
        return;
    }
    params.message = errors.join('\n');
    params.buttons = ModalMessageParams::Ok;
    UI* ui = GetUI();
    ui->ShowModalMessage(QEGlobal::windowKey, params);
}

DAVA::Result ProjectModuleDetails::CreateNewProjectInfrastructure(const QString& projectFilePath)
{
    using namespace DAVA;
    QDir projectDir(QFileInfo(projectFilePath).absolutePath());

    std::unique_ptr<ProjectData> defaultSettings(new ProjectData());

    QString resourceDirectory = QString::fromStdString(defaultSettings->GetResourceDirectory().relative);
    if (!projectDir.mkpath(resourceDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create resource directory %1.").arg(resourceDirectory).toStdString());
    }

    QString intermediateResourceDirectory = QString::fromStdString(defaultSettings->GetConvertedResourceDirectory().relative);
    if (intermediateResourceDirectory != resourceDirectory && !projectDir.mkpath(intermediateResourceDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create intermediate resource directory %1.").arg(intermediateResourceDirectory).toStdString());
    }

    QDir resourceDir(projectDir.absolutePath() + "/" + resourceDirectory);

    QString gfxDirectory = QString::fromStdString(defaultSettings->GetGfxDirectories().front().directory.relative);
    if (!resourceDir.mkpath(gfxDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create gfx directory %1.").arg(gfxDirectory).toStdString());
    }

    QString uiDirectory = QString::fromStdString(defaultSettings->GetUiDirectory().relative);
    if (!resourceDir.mkpath(uiDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create UI directory %1.").arg(uiDirectory).toStdString());
    }

    QString textsDirectory = QString::fromStdString(defaultSettings->GetTextsDirectory().relative);
    if (!resourceDir.mkpath(textsDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create UI directory %1.").arg(textsDirectory).toStdString());
    }

    QString fontsDirectory = QString::fromStdString(defaultSettings->GetFontsDirectory().relative);
    if (!resourceDir.mkpath(fontsDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create fonts directory %1.").arg(fontsDirectory).toStdString());
    }

    QString fontsConfigDirectory = QString::fromStdString(defaultSettings->GetFontsConfigsDirectory().relative);
    if (!resourceDir.mkpath(fontsConfigDirectory))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create fonts config directory %1.").arg(fontsConfigDirectory).toStdString());
    }

    QDir textsDir(resourceDir.absolutePath() + "/" + textsDirectory);
    QFile defaultLanguageTextFile(textsDir.absoluteFilePath(QString::fromStdString(defaultSettings->GetDefaultLanguage()) + ".yaml"));
    if (!defaultLanguageTextFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create localization file %1.").arg(defaultLanguageTextFile.fileName()).toStdString());
    }
    defaultLanguageTextFile.close();

    QDir fontsConfigDir(resourceDir.absolutePath() + "/" + fontsConfigDirectory);
    QFile defaultFontsConfigFile(fontsConfigDir.absoluteFilePath(QString::fromStdString(ProjectData::GetFontsConfigFileName())));
    if (!defaultFontsConfigFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create fonts config file %1.").arg(defaultFontsConfigFile.fileName()).toStdString());
    }
    defaultFontsConfigFile.close();

    defaultSettings->SetProjectFile(projectFilePath.toStdString());
    if (!ProjectModuleDetails::SerializeProjectDataToFile(defaultSettings.get()))
    {
        return Result(Result::RESULT_ERROR, QObject::tr("Can not create project file %1.").arg(projectFilePath).toStdString());
    }

    return Result();
}

bool ProjectModuleDetails::SerializeProjectDataToFile(const ProjectData* data)
{
    using namespace DAVA;

    RefPtr<YamlNode> node = data->SerializeToYamlNode();

    return YamlEmitter::SaveToYamlFile(data->GetProjectFile(), node.Get());
}

namespace ProjectModuleTesting
{
IMPL_OPERATION_ID(CreateProjectOperation);
}

DECL_GUI_MODULE(ProjectModule);
