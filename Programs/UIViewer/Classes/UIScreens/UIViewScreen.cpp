#include "UIScreens/UIViewScreen.h"

#include <Base/ScopedPtr.h>
#include <Base/ObjectFactory.h>
#include <CommandLine/ProgramOptions.h>
#include <Engine/Engine.h>
#include <FileSystem/LocalizationSystem.h>
#include <FileSystem/FileSystem.h>
#include <Render/2D/TextBlock.h>
#include <Render/2D/Systems/VirtualCoordinatesSystem.h>
#include <Sound/SoundSystem.h>
#include <UI/Layouts/UILayoutSystem.h>
#include <UI/Styles/UIStyleSheetSystem.h>
#include <UI/Update/UIUpdateComponent.h>
#include <UI/DefaultUIPackageBuilder.h>
#include <UI/UIControlSystem.h>
#include <UI/UIPackageLoader.h>
#include <UI/UIStaticText.h>
#include <UI/UIYamlLoader.h>
#include <Utils/UTF8Utils.h>

namespace UIViewScreenDetails
{
class PreviewPackageBuilder : public DAVA::DefaultUIPackageBuilder
{
public:
    PreviewPackageBuilder(DAVA::UIPackagesCache* packagesCache_ = nullptr)
        : DAVA::DefaultUIPackageBuilder(packagesCache_)
    {
    }

protected:
    DAVA::RefPtr<DAVA::UIControl> CreateControlByName(const DAVA::String& customClassName, const DAVA::String& className) override
    {
        using namespace DAVA;

        if (ObjectFactory::Instance()->IsTypeRegistered(customClassName))
        {
            return RefPtr<UIControl>(ObjectFactory::Instance()->New<UIControl>(customClassName));
        }

        return RefPtr<UIControl>(ObjectFactory::Instance()->New<UIControl>(className));
    }

    std::unique_ptr<DAVA::DefaultUIPackageBuilder> CreateBuilder(DAVA::UIPackagesCache* packagesCache) override
    {
        return std::make_unique<PreviewPackageBuilder>(packagesCache);
    }
};

DAVA::RefPtr<DAVA::UIControl> LoadControl(const DAVA::FilePath& yamlPath, const DAVA::String& controlName)
{
    using namespace DAVA;
    PreviewPackageBuilder packageBuilder;
    if (UIPackageLoader().LoadPackage(yamlPath, &packageBuilder))
    {
        UIControl* ctrl = packageBuilder.GetPackage()->GetControl<UIControl*>(controlName);
        return RefPtr<UIControl>(SafeRetain(ctrl));
    }

    return RefPtr<UIControl>();
}
}

UIViewScreen::UIViewScreen(DAVA::Window* window_, DAVA::ProgramOptions* options_)
    : BaseScreen()
    , window(window_)
    , options(options_)
{
    GetOrCreateComponent<DAVA::UIUpdateComponent>();

    if (options != nullptr)
    {
        projectPath = options->GetOption("-project").AsString();
    }
    else
    {
        projectPath = "~doc:/UIViewer/Project/";
    }
}

void UIViewScreen::LoadResources()
{
    BaseScreen::LoadResources();

    DAVA::FilePath::AddResourcesFolder(projectPath + "/Data/");
    DAVA::FilePath::AddResourcesFolder(projectPath + "/DataSource/");

    SetupUI();
}

void UIViewScreen::UnloadResources()
{
    DAVA::FilePath::RemoveResourcesFolder(projectPath + "/DataSource/");
    DAVA::FilePath::RemoveResourcesFolder(projectPath + "/Data/");

    BaseScreen::UnloadResources();
}

void UIViewScreen::SetupUI()
{
    using namespace DAVA;

    if (options == nullptr)
    {
        PrintError("Options were not parsed");
        return;
    }

    { //  localization
        String locale = options->GetOption("-locale").AsString();
        FilePath stringsFolder("~res:/Strings/");
        LocalizationSystem* localizationSystem = GetEngineContext()->localizationSystem;
        localizationSystem->SetDirectory(stringsFolder);
        localizationSystem->SetCurrentLocale(locale);
        localizationSystem->Init();

        { //  sound
            SoundSystem* soundSystem = GetEngineContext()->soundSystem;
            soundSystem->SetCurrentLocale(locale);
        }

        { //  Fonts
            FilePath fontsConfigsDirectory = options->GetOption("-fontsDir").AsString();
            FilePath localizedFontsPath(fontsConfigsDirectory + locale + "/fonts.yaml");
            if (GetEngineContext()->fileSystem->Exists(localizedFontsPath) == false)
            {
                localizedFontsPath = fontsConfigsDirectory + "/fonts.yaml";
            }
            UIYamlLoader::LoadFonts(localizedFontsPath);
        }

        window->GetUIControlSystem()->GetStyleSheetSystem()->AddGlobalClass(FastName(locale));
    }

    { //  rtl
        bool isRtl = options->GetOption("-isRtl").AsBool();
        TextBlock::SetBiDiSupportEnabled(isRtl);
        window->GetUIControlSystem()->GetLayoutSystem()->SetRtl(isRtl);
    }

    FilePath placeHolderYaml = options->GetOption("-blankYaml").AsString();
    String placeHolderRootControl = options->GetOption("-blankRoot").AsString();

    FilePath testedYaml = options->GetOption("-testedYaml").AsString();
    String testedControlName = options->GetOption("-testedCtrl").AsString();

    RefPtr<UIControl> placeHolderRoot = UIViewScreenDetails::LoadControl(placeHolderYaml, placeHolderRootControl);
    if (placeHolderRoot)
    {
        RefPtr<UIControl> testedControl = UIViewScreenDetails::LoadControl(testedYaml, testedControlName);
        if (testedControl)
        {
            placeHolderRoot->AddControl(testedControl.Get());
        }
        else
        {
            PrintError(Format("Cannot find %s in %s", testedControlName.c_str(), testedYaml.GetStringValue().c_str()));
        }

        AddControl(placeHolderRoot.Get());
    }
    else
    {
        PrintError(Format("Cannot find %s in %s", placeHolderRootControl.c_str(), placeHolderYaml.GetStringValue().c_str()));
    }
}

void UIViewScreen::PrintError(const DAVA::String& errorMessage)
{
    using namespace DAVA;

    RemoveAllControls();

    ScopedPtr<UIStaticText> errorText(new UIStaticText(GetRect()));
    errorText->SetFont(font);
    errorText->SetTextColorInheritType(UIControlBackground::COLOR_IGNORE_PARENT);
    errorText->SetTextColor(Color(1.f, 0.f, 0.f, 1.f));
    errorText->SetText(UTF8Utils::EncodeToWideString(errorMessage));
    AddControl(errorText);
}
